
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "imaging.h"
#include "menu.h"
#include "utils.h"
#include "options.h"

#include "sys/sys.h"

#include "hw.h"
#include "loader.h"
#include "emu.h"
#include "rc.h"
#include "rtc.h"

#include "defs.h"

#ifdef SUPERGB
#include "supergb.h"
#endif

// time in milliseconds of the about image on screen
#define ABOUT_TIME 2000000

extern struct fb fb;

extern char *rom_name; // name of the loaded rom, or NULL
extern char *rom_path; // complete_path of the loaded rom, or NULL

int * get_available_options(void); // options that are available for this system. Defined in the sys files.

static image background;
static image selector;
static image preview;
static int there_is_preview=0;
static int menu_xmax=0;
static font_image font;
static int menu_savestate = 0; // the currently selected save state
static int options_modified = 0; // wether or not the options were modified
static int menu_last_selected = 0;

static int advance_clock = 0;

// internal funstions
static int roll_number(int,unsigned int,int);

char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}

/** Frees the menu, ready for exit */
void menu_free(void){
	DPRINT("Freeing menu memory...");
	unload_png(&background);
	unload_png(&selector);
	unload_font(&font);
	options_modified = 0;
}


/** Rolls a number from 0 to max adding or substracting an amount */
static int roll_number(int number, unsigned int max, int amount){
	if(number+amount<0) return max-number+amount;
	return (number+amount)%max;
}

static char *menu_msg = NULL;
static int menu_msg_x, menu_msg_y;

/** Shows a message on the screen during the next frame */
void show_message(char *msg, int x, int y){
	menu_msg = strdup(msg);
	menu_msg_x = x;
	menu_msg_y = y;
}

/** Draws an array of chars as a menu with selection
selected is the currently selected item
menu is char** with the entries of the menu
max is the number of entries in the menu
msg is an optional information message to show
xo, yo is the position of the menu */
static void menu_draw(unsigned int selected, char** menu, int max, int xo, int yo){
	int first_item = 0;
	int last_item = 0;
	int scroll_y = 0;
	DPRINT("Drawing menu");
	if(background.data) draw_sprite(&background, 0, 0, 0, 0, fb.w, fb.h);
	first_item = (selected<MENU_FOCUS?0:selected - MENU_FOCUS);
	last_item = first_item + MENU_LENGTH_LIST;
	if(last_item > max){
		last_item = max;
		first_item = last_item - MENU_LENGTH_LIST;
	}
	if(max < MENU_FOCUS || first_item < 0) first_item = 0;
	for(;first_item < last_item; first_item++){
		if(first_item==selected){
			if(selector.data) draw_sprite_trans(&selector, 0, 0, 0, yo+scroll_y, selector.w, selector.h);
			draw_string(&font, menu[first_item], xo+12, yo + scroll_y, MENU_SELECTED, menu_xmax);
		}else{
			draw_string(&font, menu[first_item], xo, yo + scroll_y, MENU_COLOR, menu_xmax);
		}
		scroll_y += MENU_FONT_HEIGHT;
	}
	// draw the information message, if any
	if(menu_msg){
		draw_string(&font, menu_msg, menu_msg_x, menu_msg_y, MENU_MSG_COLOR, fb.w);
		FREE(menu_msg);
	}
	if(there_is_preview){
		draw_sprite(&preview, 0, 0, PREVIEW_X, PREVIEW_Y, preview.w, preview.h);
		unload_png(&preview);
		there_is_preview=0;
	}
	
	vid_redraw();
}

/** Initializes the menu */
static void menu_init(void){
	DPRINT("Init menu");
	// prepare the screen
	//clear_screen();
	//gp2x_video_RGB_setscaling(320, 240);
	
	menu_xmax=fb.w;

	// load images
	if(load_png(&background, MENU_BACKGROUND)!=TRUE) unload_png(&background);
	if(load_png(&selector, MENU_SELECTOR)!=TRUE) unload_png(&selector);
	if(load_font(&font, MENU_FONT, MENU_FONT_HEIGHT)!=TRUE){
		unload_font(&font);
		DPRINT("Error loading font");
		die(__FILE__ ": can't load font");
	}
	//menu_savestate = 0;
	options_modified=0;
	DPRINT("Menu initiated");
}

/** Draws the preview of a game, if saved.
If there is no preview image, does nothing */
static void menu_draw_preview(char *selected, char* previews){
	char prev_name[256];
	char *prev_file_name=NULL;
	prev_file_name=only_rom_name(selected);
	sprintf(prev_name, "%s/%s.png", previews, prev_file_name);
	FREE(prev_file_name);
	there_is_preview=load_png(&preview, prev_name);
}

/** Shows a menu to choose a file from a directory.
Returns a pointer with the complete path of the selected file,
or NULL if there are no files in the directory or the user cancels the selection.
If subdirs == INCLUDE_DIRS, the menu includes subdirs
*/
static char* menu_select_file(char *dir, char *previews, int subdirs){
	int running = 1;
	int cancel = 0;
	unsigned int selected = menu_last_selected;
	int button = 0;
	char **readable_names;
	char *complete_path;
	struct dirinfo *files_dir = NULL;
	int previews_enabled=(previews!=NULL);
	int previews_shown = 0;
	int i;
	
	selected=0;
	// if previews are enabled, see options.previews to start or not the preview box at the beginning
	previews_shown=(previews_enabled && options.previews);

	if(previews_shown){
		unload_png(&background);
		unload_png(&selector);
		// load the sprite for the previews
		if(!load_png(&background, MENU_BACKGROUND_SEL)) unload_png(&background);
		if(!load_png(&selector, MENU_SELECTOR_SEL)) unload_png(&selector);
		menu_xmax=109;
		previews_shown = 1;
	}

	// read the contents of the dir
	files_dir = list_dir(dir, LISTINFO_MAX_FILES, subdirs);
	if (files_dir==NULL || files_dir->num_files==0){
		cancel = 1;
		goto menu_select_file_end;
	}
	// calculate readable names for files
	if((readable_names = malloc((sizeof(char**) * files_dir->num_files)))==NULL){
		cancel = 1;
		goto menu_select_file_end;
	}	
	for(i=0; i<files_dir->num_files; i++) readable_names[i] = human_readable(files_dir, i);
	// show previews, if enabled
	if(previews_shown){
		if(files_dir->type[selected] == TYPE_FILE)
			menu_draw_preview(files_dir->file[selected], previews);
		else
			there_is_preview = 0;
		// show large name on screen
		show_message(readable_names[selected], MENU_FILES_X, MENU_MSG_Y);
	}
	
	menu_draw(selected, readable_names, files_dir->num_files, MENU_FILES_X, MENU_FILES_Y);
	
	while(running){
		button=sys_waitbutton();
		DPRINT1("Button: %d", button);
		switch(button){
		case BUTTON_UP:
			if(selected==0) selected=files_dir->num_files-1; else selected--;
			break;
		case BUTTON_DOWN:
			selected = (selected + 1) % files_dir->num_files;
			break;
		case BUTTON_PAGEUP:
			if(selected > MENU_LENGTH_LIST)
				selected = selected - MENU_LENGTH_LIST;
			else
				selected = 0;
			break;
		case BUTTON_PAGEDOWN:
			selected = selected + MENU_LENGTH_LIST;
			if (selected >= files_dir->num_files - 1) selected = files_dir->num_files - 1;
			break;
		case BUTTON_OK:
			running = 0;
			menu_last_selected = selected;
			break;
		case BUTTON_PREVS:
			if(previews_enabled){
				if(previews_shown){
					// load the normal sprites
					unload_png(&background);
					unload_png(&selector);
					if(!load_png(&background, MENU_BACKGROUND)) unload_png(&background);
					if(!load_png(&selector, MENU_SELECTOR)) unload_png(&selector);
					menu_xmax=fb.w;
					//clear_screen();
					previews_shown=0;
				}else{
					// load the sprite for the previews
					unload_png(&background);
					unload_png(&selector);
					if(!load_png(&background, MENU_BACKGROUND_SEL)) unload_png(&background);
					if(!load_png(&selector, MENU_SELECTOR_SEL)) unload_png(&selector);
					menu_xmax=109;
					previews_shown=1;
				}
			}
			break;
		case BUTTON_CANCEL:
			running = 0;
			cancel = 1;
		}
		// load and show the preview image
		if(previews_shown){
			// if previews enabled, show the name of the rom as message
			show_message(readable_names[selected], MENU_FILES_X, MENU_MSG_Y);
			if(files_dir->type[selected] == TYPE_FILE)
				menu_draw_preview(files_dir->file[selected], previews);
			else
				there_is_preview = 0;
			menu_draw(selected, readable_names, files_dir->num_files,
				MENU_FILES_X, MENU_FILES_Y);
		}else{
			menu_draw(selected, readable_names, files_dir->num_files, MENU_FILES_X, MENU_FILES_Y);
		}
	}
menu_select_file_end:
	sys_waitnobutton();
	// clean some things
	if(previews_shown){
		// load the normal sprites
		unload_png(&background);
		unload_png(&selector);
		if(!load_png(&background, MENU_BACKGROUND)) unload_png(&background);
		if(!load_png(&selector, MENU_SELECTOR)) unload_png(&selector);
		menu_xmax=fb.w;
		//clear_screen();
	}
	// clean readable names
	for(i=0; i<files_dir->num_files; i++) free(readable_names[i]);
	FREE(readable_names);
	
	// return the selected file
	if(cancel){
		complete_path = NULL;
	}else{
		if(files_dir->type[selected] == TYPE_DIR){
			// if a directory was selected, call again menu_select_file with the new directory
			char *tmppath;
			if((tmppath = strdup(files_dir->file[selected]))!=NULL){
				DPRINT1("Entering dir %s", tmppath);
				// just to improve the memory use, we free files_dir before calling other menu_select_file
				free_dirinfo(files_dir);
				FREE(files_dir);
				complete_path = menu_select_file(tmppath, previews, subdirs);
				FREE(tmppath);
			}else{
				complete_path = NULL;
			}
		}else{
			// if a file was selected, returns the complete_path and clean structures
			complete_path = strdup(files_dir->file[selected]);
		}
	}
	
	free_dirinfo(files_dir);
	FREE(files_dir);	
#ifdef DEBUG
	if(complete_path) printf("Selected file %s\n", complete_path); else printf("Selection canceled\n");
#endif	
	return complete_path;
}

/** Simple menu to advance the clock.
 * This menu cannot be canceled, since a game is already loaded when showed.
 * Returns the number of SECONDS to advance the clock.
 * */
static int menu_advance_clock(void){
	unsigned int selected;
	int running=1;
	int button=0, button2=0;
	int hours = 0, days = 0;
#define MENU_CLOCK_ITEMS 3
#define MENU_CLOCK_OK 0
#define MENU_CLOCK_HOURS 1
#define MENU_CLOCK_DAYS 2
	char *menu_options[MENU_CLOCK_ITEMS]={"Play", NULL, NULL};

	if((menu_options[MENU_CLOCK_HOURS]=malloc(10*sizeof(char)))==NULL){
		return 0;
	}
	sprintf(menu_options[MENU_CLOCK_HOURS], "Hours: %2d", hours);
	if((menu_options[MENU_CLOCK_DAYS]=malloc(10*sizeof(char)))==NULL){
		FREE(menu_options[MENU_CLOCK_HOURS]);
		return 0;
	}
	sprintf(menu_options[MENU_CLOCK_DAYS], "Days:  %2d", days);

	selected = 0;
	while(running){
		show_message("Advance clock?", MENU_MSG_X, MENU_MSG_Y);
		menu_draw(selected, menu_options, MENU_CLOCK_ITEMS, MENU_FILES_X, MENU_FILES_Y);
		button=sys_waitbutton();
		DPRINT1("Button: %d", button);
		if(button!=BUTTON_UP && button!=BUTTON_DOWN && button!=BUTTON_NONE)
			sys_waitnobutton();
		switch(button){
		case BUTTON_UP:
			selected = roll_number(selected, MENU_CLOCK_ITEMS, -1);
			break;
		case BUTTON_DOWN:
			selected = roll_number(selected, MENU_CLOCK_ITEMS, +1);
			break;
		case BUTTON_RIGHT: case BUTTON_LEFT:
			sys_waitnobutton();
			button2=(button==BUTTON_RIGHT?1:-1);
			switch(selected){
			case MENU_CLOCK_HOURS:
				hours=roll_number(hours, 24, button2);
				sprintf(menu_options[MENU_CLOCK_HOURS], "Hours: %2d", hours);
				break;
			case MENU_CLOCK_DAYS:
				days=roll_number(days, 30, button2);
				sprintf(menu_options[MENU_CLOCK_DAYS], "Days:  %2d", hours);
				break;
			}
			break;
		case BUTTON_OK:
			sys_waitnobutton();
			if(selected == MENU_CLOCK_OK){
				running=0;
			}
			break;
		}//switch(button)
	}//while running
	FREE(menu_options[MENU_CLOCK_DAYS]);
	FREE(menu_options[MENU_CLOCK_HOURS]);

	return hours * 3600 + days * 86400;
}

/** Shows the options menu */
static void menu_show_options(void){
	// FIXME: add options for system behaviour: previews, rom dir...
	unsigned int selected, cpu_opt=2;
	int running=1;
	int button=0, button2=0;
	char *menu_options[MENU_OPTIONS_ITEMS];
	
#define MACHINE_ITEMS 4
	char *machine_opts[MACHINE_ITEMS]={"Color Gameboy", "Gameboy Advance", "Super Gameboy", "Super Gameboy 2"};	
	char *sound_opts[2]={"Sound OFF", "Sound ON"};
	char *fps_opts[2]={"Do not show FPS", "Show FPS"};
	char *autofs_opts[2]={"No frameskip", "Frameskip"};	
	
#ifdef WIZLIB
	#define MAX_VIDEO_MODES 6
	
	#define CPU_OPTS_ITEMS 8
	char *cpu_opts[CPU_OPTS_ITEMS]={
		"Clock 200Mhz", "Clock 250MHz", "Clock 300MHz",
		"Clock 350MHz", "Clock 400MHz", "Clock 450MHz",
		"Clock 500MHz", "Clock 550MHz"};
	int cpu_values[CPU_OPTS_ITEMS]={
		200, 250, 300, 350, 400, 450, 500, 550};
#else
	#define MAX_VIDEO_MODES 8

	#define CPU_OPTS_ITEMS 8
	char *cpu_opts[CPU_OPTS_ITEMS]={
		"Clock 100Mhz", "Clock 125MHz", "Clock 133MHz",
		"Clock 150MHz", "Clock 166MHz", "Clock 175MHz",
		"Clock 200MHz", "Clock 225MHz"};
	int cpu_values[CPU_OPTS_ITEMS]={
		100, 125, 133, 150, 166, 175, 200, 225};
#endif

	char *video_opts[MAX_VIDEO_MODES]={"Video normal",
#ifndef WIZLIB	    
					   "Fullscreen",
					   "Fullscreen aspect ratio"
#endif
					   "Double size",
					   "Deformed double size",
					   "Interpolation",
					   "Scale2x",
					   "Scale2x Deformed"};

	char *palette=NULL;
	char *frame=NULL;
	int * available_options = NULL;
	
	options_modified = 1;
	available_options = get_available_options();
	
	// find put the selected cpu
	for(cpu_opt=0; cpu_opt<CPU_OPTS_ITEMS; cpu_opt++) if(options.cpuclock==cpu_values[cpu_opt]) break;
	if(cpu_opt==CPU_OPTS_ITEMS) cpu_opt=5;

	// fill the menu with the available options
	for(button=0; button<MENU_OPTIONS_ITEMS; button++){
		switch(available_options[button]){
		case OPTION_PALETTE: menu_options[button]=options.palette; break;
		case OPTION_SCALING: menu_options[button]=video_opts[options.scaling]; break;
		case OPTION_FRAME: menu_options[button]=options.frame; break;
		case OPTION_SOUND: menu_options[button]=sound_opts[options.sound]; break;
		case OPTION_SHOWFPS: menu_options[button]=fps_opts[options.showfps]; break;
		case OPTION_AUTOSKIP: menu_options[button]=autofs_opts[options.autofskip]; break;
		case OPTION_CPU: menu_options[button]=cpu_opts[cpu_opt]; break;
		case OPTION_SAVE_GLOBAL: menu_options[button]="Save global options"; break;
		case OPTION_SAVE_GAME: menu_options[button]="Save for game"; break;
		case OPTION_QUIT: menu_options[button]="Back to menu"; break;
		case OPTION_MACHINE: menu_options[button]=machine_opts[options.machine]; break;
		}
	}

	selected = 0;
	menu_draw(selected, menu_options, MENU_OPTIONS_ITEMS, MENU_FILES_X, MENU_FILES_Y);
	while(running){
		menu_draw(selected, menu_options, MENU_OPTIONS_ITEMS, MENU_FILES_X, MENU_FILES_Y);
		button=sys_waitbutton();
		DPRINT1("Button: %d", button);
		if(button!=BUTTON_UP && button!=BUTTON_DOWN && button!=BUTTON_NONE)
			sys_waitnobutton();
		switch(button){
		case BUTTON_UP:
			selected = roll_number(selected, MENU_OPTIONS_ITEMS, -1);
			break;
		case BUTTON_DOWN:
			selected = roll_number(selected, MENU_OPTIONS_ITEMS, +1);
			break;
		case BUTTON_RIGHT: case BUTTON_LEFT:
			sys_waitnobutton();
			button2=(button==BUTTON_RIGHT?1:-1);
			switch(available_options[selected]){
			case OPTION_SCALING: options.scaling=roll_number(options.scaling, MAX_VIDEO_MODES, button2); menu_options[selected]=video_opts[options.scaling]; break;
			case OPTION_SOUND: options.sound=(options.sound?0:1); menu_options[selected]=sound_opts[options.sound]; break;
			case OPTION_SHOWFPS: options.showfps=(options.showfps?0:1); menu_options[selected]=fps_opts[options.showfps]; break;
			case OPTION_AUTOSKIP: options.autofskip=(options.autofskip?0:1); menu_options[selected]=autofs_opts[options.autofskip]; break;
			case OPTION_CPU:
				cpu_opt=roll_number(cpu_opt, CPU_OPTS_ITEMS, button2); menu_options[selected]=cpu_opts[cpu_opt];
				options.cpuclock=cpu_values[cpu_opt]; break;
			case OPTION_MACHINE:
				options.machine=roll_number(options.machine, MACHINE_ITEMS, button2);
				menu_options[selected]=machine_opts[options.machine];
				break;
			}
			break;
		case BUTTON_OK:
			sys_waitnobutton();
			switch(available_options[selected]){
			case OPTION_PALETTE: // select palette
				button2 = menu_last_selected;
				menu_last_selected = 0;
				palette = menu_select_file(PALETTES_DIR, NULL, NO_DIRS);
				menu_last_selected = button2;
				if(palette){
					FREE(options.palette);
					menu_options[selected]=strdup(palette);
					options.palette=menu_options[selected];
					FREE(palette);
				}
				break;
			case OPTION_FRAME: // select frame
				button2 = menu_last_selected;
				menu_last_selected = 0;
				frame = menu_select_file(FRAMES_DIR, NULL, NO_DIRS);
				menu_last_selected = button2;
				if(frame){
					FREE(options.frame);
					menu_options[selected]=strdup(frame);
					options.frame=menu_options[selected];
					FREE(frame);
				}
				break;
			case OPTION_SAVE_GLOBAL: // save global options
				frame = malloc(256*sizeof(char));
				if(frame){
					sprintf(frame, "%s/%s", OPTIONS_DIR, "default.rc");
					if(options_save(frame)==0)
						show_message("Global options saved", MENU_MSG_X, MENU_MSG_Y);
					else
						show_message("Error saving options", MENU_FILES_X, MENU_FILES_Y);
				}
				FREE(frame);
				break;
			case OPTION_SAVE_GAME: // save options
				if(!rom_name){
					show_message("Load rom first", MENU_MSG_X, MENU_MSG_Y);
				}else{
					frame = malloc(256*sizeof(char));
					if(frame){
						sprintf(frame, "%s/%s.rc", OPTIONS_DIR, rom_name);
						if(options_save(frame)==0)
							show_message("Options saved", MENU_MSG_X, MENU_MSG_Y);
						else
							show_message("Error saving options", MENU_MSG_X, MENU_MSG_Y);
					}
					FREE(frame);
				}
				break;
			case OPTION_QUIT: // quit menu
				running=0;
				break;
			}
			break;
		case BUTTON_CANCEL:
			sys_waitnobutton();
			running = 0;
			break;
		}//switch(button)
	}
}

/** Shows the main menu */
void menu_show(void){
	int running = 1;
	int quit = 0;
	unsigned int selected = 0;
	int button = 0;
	byte *tmpscreen;
#define MENU_GENERAL_ITEMS 10
	char *menu_general[MENU_GENERAL_ITEMS] = {"Resume game", "Load new ROM", 0, 0, "Save SRAM", "Options", "Save preview", "Save SGB border", "About Lemonboy2x", "Exit to menu"};
	char *cmd=NULL, *tmp_rom = NULL;

	DPRINT("Starting menu");
	// save the current screen
	tmpscreen = malloc(fb.w * fb.h * fb.pelsize);
	// note that is the malloc fails, tmpscreen==NULL. We will check that before using tmpscreen
	if(tmpscreen) memcpy(tmpscreen, fb.ptr, fb.w * fb.h * fb.pelsize);
	
	menu_init();
	//dynamic menu entries
	if((menu_general[2] = calloc(16, sizeof(char)))==NULL) die(__FILE__ ": can't allocate memory");
	if((menu_general[3] = calloc(32, sizeof(char)))==NULL) die(__FILE__ ": can't allocate memory");
	sprintf(menu_general[2], "Save in slot %d", menu_savestate);
	sprintf(menu_general[3], "Load from slot %d", menu_savestate);
	// show menu
	selected = 0;
	while(running){
		menu_draw(selected, menu_general, MENU_GENERAL_ITEMS, MENU_FILES_X, MENU_FILES_Y);
		button=sys_waitbutton();
		DPRINT1("Button: %d", button);
		switch(button){
		case BUTTON_UP:
			selected=roll_number(selected, MENU_GENERAL_ITEMS, -1);
			break;
		case BUTTON_DOWN:
			selected=roll_number(selected, MENU_GENERAL_ITEMS, +1);
			break;
		case BUTTON_LEFT: case BUTTON_RIGHT:
			 if(selected==2||selected==3){
				menu_savestate=roll_number(menu_savestate, 9, button==BUTTON_LEFT?-1:1);
				sprintf(menu_general[2], "Save in slot %d", menu_savestate);
				sprintf(menu_general[3], "Load from slot %d", menu_savestate);
				DPRINT1("Menu savestate %d", menu_savestate);
			}
			sys_waitnobutton();
			break;
		case BUTTON_OK:
			sys_waitnobutton();
			switch(selected){
			case 0: // CONTINUE
				if(rom_name){
					running = 0;
				}else{
					show_message("Load a game first", MENU_MSG_X, MENU_MSG_Y);
				}
				break;
			case 1: // LOAD ROM. There is not a RESET ROM option any morez
				tmp_rom = menu_select_file(options.romsdir, PREVIEWS_DIR, (options.subdirectories?INCLUDE_DIRS:NO_DIRS));
				//tmp_rom = (selected==1?menu_select_file(options.romsdir, PREVIEWS_DIR, INCLUDE_DIRS):strdup(rom_path));
				if(tmp_rom){
					DPRINT1("Running %s\n", tmp_rom);
					// save sram
					if(rom_name) { sram_save(); sync(); }
					// point to the new game
					FREE(rom_name);
					FREE(rom_path);
					rom_path = strdup(tmp_rom);
					rom_name = only_rom_name(rom_path);
					FREE(tmp_rom);
					// load global configuracion
					if(options_modified == 0){
						cmd = malloc(256*sizeof(char));
						if(cmd){
							sprintf(cmd, "source %s/%s", OPTIONS_DIR, "default.rc");
							rc_command(cmd);
						}
						FREE(cmd);
						// load specific configuration
						cmd = malloc((strlen(rom_name) + 256)*sizeof(char));
						if(cmd){
							sprintf(cmd, "source \"%s/%s\"", OPTIONS_DIR, rom_name);
							strcat(cmd, ".rc");
							rc_command(cmd);
						}
						FREE(cmd);
					}
					options_modified == 0;
					// load and run the rom
					loader_unload();
					vid_init();
					options_apply();
					loader_init(rom_path);
					// Some carriages have an internal clock (such as Pokemon Gold)
					// Advance the clock, if supported
					if(rtc.batt){
						int advance = 0;
						advance = 60 * menu_advance_clock();
						if(advance>0){
							DPRINT1("Advance %d ticks", advance);
							while(advance--) rtc_tick();
						}
					}
					// reset the emulator state
					emu_reset();
					running = 0;
				}
				break;
			case 2: //SAVE STATE
				if(rom_name){
					state_save(menu_savestate);
					show_message("State saved", MENU_MSG_X, MENU_MSG_Y);
					DPRINT1("State saved in %d", menu_savestate);
				}else{
					show_message("Load a game first", MENU_MSG_X, MENU_MSG_Y);
				}
				break;
			case 3: //LOAD STATE
				if(rom_name){
					state_load(menu_savestate);
					DPRINT1("State loaded from %d", menu_savestate);
					running = 0; // return to the emulation
				}else{
					show_message("Load a game first", MENU_MSG_X, MENU_MSG_Y);
				}
				break;
			case 4: // SAVE SRAM
				if(rom_name){
					sram_save(); sync();
					show_message("SRAM saved", MENU_MSG_X, MENU_MSG_Y);
				}else{
					show_message("Load a game first", MENU_MSG_X, MENU_MSG_Y);
				}
				break;
			case 5: // OPTIONS MENU
				menu_show_options();
#ifdef DEBUG
				options_print();
#endif
				break;
			case 6: //SAVE PREVIEW
				if(!rom_name)
					show_message("Load a game first", MENU_MSG_X, MENU_MSG_Y);
				else{
					cmd=malloc(256);
					if(cmd&&tmpscreen){
						sprintf(cmd, "%s/%s.png", PREVIEWS_DIR, rom_name);
						if(vid_savepreview(tmpscreen, cmd))
							show_message("Previews saved", MENU_MSG_X, MENU_MSG_Y);
						else
							show_message("Error saving preview", MENU_MSG_X, MENU_MSG_Y);
					}else{
						show_message("I can't save previews", MENU_MSG_X, MENU_MSG_Y);
					}
					FREE(cmd);
				}
				break;
			case 7: // SAVE SGB BORDER
#ifndef SUPERGB
				show_message("No Super GB support!", MENU_MSG_X, MENU_MSG_Y);
#else
				if(hw.sgb == 0)
					show_message("No Super GB support!", MENU_MSG_X, MENU_MSG_Y);
				else if(!rom_name)
					show_message("Load a game first", MENU_MSG_X, MENU_MSG_Y);
				else if(sgb_border_image_ready == FALSE)
					show_message("No frame detected", MENU_MSG_X, MENU_MSG_Y);
				else{
					cmd=malloc(256);
					if(cmd){
						sprintf(cmd, "%s/%s.png", FRAMES_DIR, rom_name);
						if(save_png(cmd, sgb_border_image.data, sgb_border_image.w, sgb_border_image.h, 0) == TRUE){
							show_message("Frame saved", MENU_MSG_X, MENU_MSG_Y);
						} else {
							show_message("Error saving frame", MENU_MSG_X, MENU_MSG_Y);
						}
					}else{
						show_message("Error saving frame", MENU_MSG_X, MENU_MSG_Y);
					}
				}
				break;
#endif
			case 8: // ABOUT box
				show_about();
				break;
			case 9: quit = 1; running = 0; break; // QUIT
			}//switch(selected) with button==OK
			sys_waitnobutton();
			break;
		case BUTTON_CANCEL:
			if(rom_name){
				running = 0;
			}else{
				show_message("Load a game first", MENU_MSG_X, MENU_MSG_Y);
			}
			sys_waitnobutton();
#ifdef DEBUG
			break;
		default:
			printf("Unrecognized button: %d\n", button);
#endif
		} //switch(button)
	}//while(running)
	if(quit){
		DPRINT("Quit emulator");
		menu_free();
		if(rom_name) { sram_save(); sync(); }
		exit(0);
	}
#ifdef DEBUG
	//options_print();
#endif

	// apply the current game options
	sys_options_apply();

	// free dynamic memory
	FREE(menu_general[2]);
	FREE(menu_general[3]);
	FREE(tmpscreen);
	menu_free();
}

/** Shows an about box */
void show_about(){
	int i;

	image about;

	if (load_png(&about, "etc/viejusplash.png")){
		DPRINT("Drawing sprite...");
		draw_sprite(&about, 0, 0, 0, 0, fb.w, fb.h);
		vid_redraw();


		// wait two seconds or a button
#ifdef DEBUG
		sys_waitbutton();
#else
		sys_sleep(ABOUT_TIME*2);
#endif



		unload_png(&about);

	}



	// load the about screen
	if(load_png(&about, ABOUT_IMAGE)){
		//clear_screen();
		// shows the image on the screen
		DPRINT("Drawing sprite...");
		draw_sprite(&about, 0, 0, 0, 0, fb.w, fb.h);
		vid_redraw();
		// wait two seconds or a button
#ifdef DEBUG
		sys_waitbutton();
#else
		sys_sleep(ABOUT_TIME);
#endif
		unload_png(&about);
	}
}

