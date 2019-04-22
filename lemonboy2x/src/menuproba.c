
#include <stdlib.h>
#include <unistd.h>

#include "gui/imaging.h"

#include <SDL.h>

#include "input.h"
#include "fb.h"

#include "sys/sdl/syssdl.h"
#include "gui/utils.h"

static int use_yuv = -1;
static int fullscreen = 0;
static int use_altenter = 1;
static int use_joy = 1, sdl_joy_num;
static SDL_Joystick * sdl_joy = NULL;
static const int joy_commit_range = 3276;
static char Xstatus, Ystatus;

static SDL_Surface *screen;
static SDL_Overlay *overlay;

SDL_Rect origen,destino;

static int vmode[3] = { 0, 0, 16 };

void sys_sleep(int us)
{
	if (us <= 0) return;
	usleep(us);
}

/* keymap - mappings of the form { scancode, localcode } - from sdl/keymap.c */
extern int keymap[][2];

int die(char* msg, char* e){
	printf("%s: %s", msg, e);
	exit(1);
}

int ev_postevent(event_t *e){
	
}

static int mapscancode(SDLKey sym)
{
	/* this could be faster:  */
	/*  build keymap as int keymap[256], then ``return keymap[sym]'' */
	
	int i;
	for (i = 0; keymap[i][0]; i++)
		if (keymap[i][0] == sym)
			return keymap[i][1];
	if (sym >= '0' && sym <= '9')
		return sym;
	if (sym >= 'a' && sym <= 'z')
		return sym;
	return 0;
}


static void joy_init()
{
	int i;
	int joy_count;
	
	/* Initilize the Joystick, and disable all later joystick code if an error occured */
	if (!use_joy) return;
	
	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK))
		return;
	
	joy_count = SDL_NumJoysticks();
	
	if (!joy_count)
		return;
	
	/* now try and open one. If, for some reason it fails, move on to the next one */
	for (i = 0; i < joy_count; i++)
	{
		sdl_joy = SDL_JoystickOpen(i);
		if (sdl_joy)
		{
			sdl_joy_num = i;
			break;
		}	
	}
	
	/* make sure that Joystick event polling is a go */
	SDL_JoystickEventState(SDL_ENABLE);
}

static void overlay_init()
{
	if (!use_yuv) return;
	
	if (use_yuv < 0)
		if (vmode[0] < 320 || vmode[1] < 288)
			return;
	
	overlay = SDL_CreateYUVOverlay(320, 144, SDL_YUY2_OVERLAY, screen);
	
	if (!overlay) return;
	
	if (!overlay->hw_overlay || overlay->planes > 1)
	{
		SDL_FreeYUVOverlay(overlay);
		overlay = 0;
		return;
	}
	
	SDL_LockYUVOverlay(overlay);
	
	fb.w = 160;
	fb.h = 144;
	fb.pelsize = 4;
	fb.pitch = overlay->pitches[0];
	fb.ptr = overlay->pixels[0];
	fb.yuv = 1;
	fb.cc[0].r = fb.cc[1].r = fb.cc[2].r = fb.cc[3].r = 0;
	fb.dirty = 1;
	fb.enabled = 1;
	
	overlay_rect.x = 0;
	overlay_rect.y = 0;
	overlay_rect.w = vmode[0];
	overlay_rect.h = vmode[1];
	
	/* Color channels are 0=Y, 1=U, 2=V, 3=Y1 */
	switch (overlay->format)
	{
		/* FIXME - support more formats */
	case SDL_YUY2_OVERLAY:
	default:
		fb.cc[0].l = 0;
		fb.cc[1].l = 24;
		fb.cc[2].l = 8;
		fb.cc[3].l = 16;
		break;
	}
	
	SDL_UnlockYUVOverlay(overlay);
}

void vid_init()
{
	int flags;
	
	if (!vmode[0] || !vmode[1])
	{
		int scale = 1; //rc_getint("scale");
		if (scale < 1) scale = 1;
		vmode[0] = 320 * scale;
		vmode[1] = 240 * scale;
	}
	
	flags = SDL_ANYFORMAT | SDL_HWPALETTE | SDL_HWSURFACE;
	
	if (fullscreen)
		flags |= SDL_FULLSCREEN;
	
	if (SDL_Init(SDL_INIT_VIDEO))
		die("SDL: Couldn't initialize SDL: %s\n", SDL_GetError());
	
	if (!(screen = SDL_SetVideoMode(vmode[0], vmode[1], vmode[2], flags)))
		die("SDL: can't set video mode: %s\n", SDL_GetError());
	
	SDL_ShowCursor(0);
	
	joy_init();
	
	overlay_init();
	
	if (fb.yuv) return;
	
	SDL_LockSurface(screen);

	
	fb.w = screen->w;
	fb.h = screen->h;
	fb.pelsize = screen->format->BytesPerPixel;
	fb.pitch = screen->pitch;
	fb.indexed = fb.pelsize == 1;
	fb.ptr = screen->pixels;
	fb.cc[0].r = screen->format->Rloss;
	fb.cc[0].l = screen->format->Rshift;
	fb.cc[1].r = screen->format->Gloss;
	fb.cc[1].l = screen->format->Gshift;
	fb.cc[2].r = screen->format->Bloss;
	fb.cc[2].l = screen->format->Bshift;
	
	SDL_UnlockSurface(screen);
	
	fb.enabled = 1;
	fb.dirty = 0;




	
}


void ev_poll()
{
	event_t ev;
	SDL_Event event;
	int axisval;
	
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_ACTIVEEVENT:
			if (event.active.state == SDL_APPACTIVE)
				fb.enabled = event.active.gain;
			break;
		case SDL_KEYDOWN:
			if ((event.key.keysym.sym == SDLK_RETURN) && (event.key.keysym.mod & KMOD_ALT))
				SDL_WM_ToggleFullScreen(screen);
			ev.type = EV_PRESS;
			ev.code = mapscancode(event.key.keysym.sym);
			ev_postevent(&ev);
			break;
		case SDL_KEYUP:
			ev.type = EV_RELEASE;
			ev.code = mapscancode(event.key.keysym.sym);
			ev_postevent(&ev);
			break;
		case SDL_JOYAXISMOTION:
			switch (event.jaxis.axis)
			{
			case 0: /* X axis */
				axisval = event.jaxis.value;
				if (axisval > joy_commit_range)
				{
					if (Xstatus==2) break;
					
					if (Xstatus==0)
					{
						ev.type = EV_RELEASE;
						ev.code = K_JOYLEFT;
						ev_postevent(&ev);				 		
					}
					
					ev.type = EV_PRESS;
					ev.code = K_JOYRIGHT;
					ev_postevent(&ev);
					Xstatus=2;
					break;
				}	   				   
				
				if (axisval < -(joy_commit_range))
				{
					if (Xstatus==0) break;
					
					if (Xstatus==2)
					{
						ev.type = EV_RELEASE;
						ev.code = K_JOYRIGHT;
						ev_postevent(&ev);				 		
					}
					
					ev.type = EV_PRESS;
					ev.code = K_JOYLEFT;
					ev_postevent(&ev);
					Xstatus=0;
					break;
				}	   				   
				
				/* if control reaches here, the axis is centered,
				 * so just send a release signal if necisary */
				
				if (Xstatus==2)
				{
					ev.type = EV_RELEASE;
					ev.code = K_JOYRIGHT;
					ev_postevent(&ev);
				}
				
				if (Xstatus==0)
				{
					ev.type = EV_RELEASE;
					ev.code = K_JOYLEFT;
					ev_postevent(&ev);
				}	       
				Xstatus=1;
				break;
				
			case 1: /* Y axis*/ 
				axisval = event.jaxis.value;
				if (axisval > joy_commit_range)
				{
					if (Ystatus==2) break;
					
					if (Ystatus==0)
					{
						ev.type = EV_RELEASE;
						ev.code = K_JOYUP;
						ev_postevent(&ev);				 		
					}
					
					ev.type = EV_PRESS;
					ev.code = K_JOYDOWN;
					ev_postevent(&ev);
					Ystatus=2;
					break;
				}	   				   
				
				if (axisval < -joy_commit_range)
				{
					if (Ystatus==0) break;
					
					if (Ystatus==2)
					{
						ev.type = EV_RELEASE;
						ev.code = K_JOYDOWN;
						ev_postevent(&ev);
					}
					
					ev.type = EV_PRESS;
					ev.code = K_JOYUP;
					ev_postevent(&ev);
					Ystatus=0;
					break;
				}	   				   
				
				/* if control reaches here, the axis is centered,
				 * so just send a release signal if necisary */
				
				if (Ystatus==2)
				{
					ev.type = EV_RELEASE;
					ev.code = K_JOYDOWN;
					ev_postevent(&ev);
				}
				
				if (Ystatus==0)
				{
					ev.type = EV_RELEASE;
					ev.code = K_JOYUP;
					ev_postevent(&ev);
				}
				Ystatus=1;
				break;
			}
			break;
		case SDL_JOYBUTTONUP:
			if (event.jbutton.button>15) break;
			ev.type = EV_RELEASE;
			ev.code = K_JOY0 + event.jbutton.button;
			ev_postevent(&ev);
			break;
		case SDL_JOYBUTTONDOWN:
			if (event.jbutton.button>15) break;
			ev.type = EV_PRESS;
			ev.code = K_JOY0+event.jbutton.button;
			ev_postevent(&ev);
			break;
		case SDL_QUIT:
			exit(1);
			break;
		default:
			break;
		}
	}
}

void vid_setpal(int i, int r, int g, int b)
{
	SDL_Color col;
	
	col.r = r; col.g = g; col.b = b;
	
	SDL_SetColors(screen, &col, i, 1);
}

void vid_preinit()
{
}

void vid_close()
{
	if (overlay)
	{
		SDL_UnlockYUVOverlay(overlay);
		SDL_FreeYUVOverlay(overlay);
	}
	else SDL_UnlockSurface(screen);
	SDL_Quit();
	fb.enabled = 0;
}

void vid_settitle(char *title)
{
	SDL_WM_SetCaption(title, title);
}

void vid_begin()
{
	if (overlay)
	{
		SDL_LockYUVOverlay(overlay);
		fb.ptr = overlay->pixels[0];
		return;
	}
	SDL_LockSurface(screen);
	fb.ptr = screen->pixels;
}

void vid_end()
{
	if (overlay)
	{
		SDL_UnlockYUVOverlay(overlay);
		if (fb.enabled)
			SDL_DisplayYUVOverlay(overlay, &overlay_rect);
		return;
	}
	SDL_UnlockSurface(screen);
	if (fb.enabled) SDL_Flip(screen);
}

/** Do no return until that all buttons are released */
void sys_waitnobutton(){
	Uint8 * keyboard;
	int n, i;
	int clean=0;
	DPRINT("Wait to release all buttons...");
	while(!clean){
		// get events
		SDL_PumpEvents();
		// check the state of the buttons of the keyboard
		keyboard = SDL_GetKeyState(&n);
		clean=1;
		for(i=0; i<n&&i<290; i++){ // in a keyboard, key 300 is always on
			if(keyboard[i]){
				clean=0;
				break;
			}
		}
		// wait half a second
		if(!clean) sys_sleep(100000);
		// TODO: check the state of a joypad
	}
	DPRINT("done\n");
}

int repeatbutton=0;
int lastbutton=0;
int sys_waitbutton(){
	Uint8 * keyboard;
	int n;
	int button=0;
	DPRINT("Waiting for a button...");
	while(!button){
		sys_sleep(100000);
		// get events
		SDL_PumpEvents();
		// check the state of the buttons of the keyboard
		keyboard = SDL_GetKeyState(&n);
		
		if(keyboard[BUTTON_UP]) button=BUTTON_UP;
		if(keyboard[BUTTON_DOWN]) button=BUTTON_DOWN;
		if(keyboard[BUTTON_LEFT]) button=BUTTON_LEFT;
		if(keyboard[BUTTON_RIGHT]) button=BUTTON_RIGHT;
		if(keyboard[BUTTON_OK]) button=BUTTON_OK;
		if(keyboard[BUTTON_CANCEL]) button=BUTTON_CANCEL;
		if(keyboard[BUTTON_PREVS]) button=BUTTON_PREVS;
		if(keyboard[BUTTON_SCROLLUP]) button=BUTTON_SCROLLUP;
		if(keyboard[BUTTON_SCROLLDOWN]) button=BUTTON_SCROLLDOWN;
		
		if(button==0){
			lastbutton=BUTTON_NONE;
			repeatbutton=0;
		}
	}
	if(button!=lastbutton){
		repeatbutton=0;
		lastbutton=button;
	}else{
		// two phases: hold the button for a while
		// and quick repeat
		repeatbutton++;
		if(repeatbutton<5)
			button=BUTTON_NONE;
	}
#ifdef DEBUG
	printf("button=%d\n", button);
#endif
	return button;
	
}

/** Redraws the screen */
void vid_redraw(void){
	if (fb.enabled) SDL_Flip(screen);	
}

/** Saves a preview of the current game into fname.
 * The preview is a dump of the current screen. */
int vid_savepreview(char *fname){
#ifdef DOUBLEBF
	// FIXME: previews are only enabled in the 16bits mode
#ifdef DEBUG
	printf("Save preview %s\n", fname);
#endif
	return writepng(fname, fb.offset, 160, 144, 160*2);
#endif
}

/** Shows a message on the screeen waiting for a confirmation from the user */
int sys_confirm(char *msg){
	int button;
#ifdef DEBUG
	printf("Confirm: %s\n", msg);
#endif	
	vid_redraw();
	sys_waitnobutton();
	button = sys_waitbutton();
	sys_waitnobutton();
	return button;
}

int msg_font_map[256] = {
	36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
	36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
	36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
	36, 36, 36, 36, 36, 36, 36, 36, 37, 39,
	36, 36, 36, 36, 36, 38,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	42, 42, 36, 36, 36, 41, 36,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
	37, 38, 39, 36, 36, 36,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
	36, 36, 36, 36, 36
};


int main(int argc, char** argv){
	int i=0;
	int y=0;
	int button;
	struct dirinfo * di=NULL;
	char *dir=NULL;
	char *rname;
	int selected, running=0;
	image background, bar;
	font_image font;
	
	dir=strdup("/home/juanvi/gp2x");
	
	vid_init();
	load_png(&background, "../etc/menu.png");
	load_png(&bar, "../etc/selector.png");
	load_font(&font, "../etc/msg_font.png", 13);
	font.map = msg_font_map;
	y = bar.h *5;	
	
	running = 1;
	selected = 0;
	while(running){
		vid_begin();
		draw_sprite(&background, 0, 0, 0, 0, background.w, background.h);
		free_dirinfo(di); FREE(di);
		di = list_dir(dir, 10);
		y = 50;
		for(i=0; di->type[i]!=TYPE_EMPTY; i++){
			rname = human_readable(di, i);
			draw_string(&font, rname, 18, y, 1, fb.w);
			FREE(rname);
			y+=font.size;
		}
		draw_sprite_trans(&bar, 0, 0, 0, (selected*font.size)+50, bar.w, bar.h);
		rname = human_readable(di, selected);
		draw_string(&font, rname, 18, (selected*font.size)+50, 0, fb.w);
		
		draw_string(&font, "ABC xyz(/)?", 0, 0, 0, fb.w);
		FREE(rname);
		vid_end();
		
		button=sys_waitbutton();
		switch(button){
		case BUTTON_DOWN: selected=(selected+1)%10; break;
		case BUTTON_UP: selected=(selected-1)%10; break;
		case BUTTON_OK:
			if(di->type[selected] == TYPE_FILE){ // Select file
				printf("elected file %s\n", di->file[selected]);
			}else{ // CD dir
				printf("Selected dir %s\n", di->file[selected]);
				FREE(dir);
				dir = strdup(di->file[selected]);
				if(!dir) die("Error", "Cannot change dir");
				free_dirinfo(di); FREE(di);
				selected = 0;
			}
			break;
		case BUTTON_CANCEL: running = 0; break;
		}
	}
	free_dirinfo(di);
	FREE(di);
	exit(0);
}


