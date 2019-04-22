/*
 * sdl.c
 * sdl interfaces -- based on svga.c 
 *
 * (C) 2001 Damian Gryski <dgryski@uwaterloo.ca>
 * Joystick code contributed by David Lau
 * Sound code added by Laguna
 *
 * Licensed under the GPLv2, or later.
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>


#include "fb.h"
#include "input.h"
#include "rc.h"
#include "sys.h"

#include "gui/menu.h"
#include "gui/imaging.h"
#include "gui/options.h"

#ifdef SUPERGB
#include "hw.h"
#include "supergb.h"
#endif

#ifdef GP2X_ASM
#define MEMCPY gp2x_memcpy
#define MEMSET memset
#else
#define MEMCPY memcpy
#define MEMSET memset
#endif

struct fb fb;

static int use_yuv = -1;
static int fullscreen = 0;
static int use_altenter = 1;
static int use_joy = 1, sdl_joy_num;
static SDL_Joystick * sdl_joy = NULL;
static const int joy_commit_range = 3276;
static char Xstatus, Ystatus;

static SDL_Surface *screen;
static SDL_Overlay *overlay;
static SDL_Rect overlay_rect;



static int vmode[3] = { 0, 0, 16 };

static char *sys_msg=NULL; // the system message. If null, there is not a current system message
static int sys_msg_time=0; // the number of frames that the new system message should be on screen
// a small font
// int msg_font_map[256] = {
// 	36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
// 	36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
// 	36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
// 	36, 36, 36, 36, 36, 36, 36, 36, 37, 39,
// 	36, 36, 36, 36, 36, 38,
// 	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
// 	42, 42, 36, 36, 36, 41, 36,
// 	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
// 	37, 38, 39, 36, 36, 36,
// 	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
// 	36, 36, 36, 36, 36
// };

static void print_text(byte *screen, int x, int y, char *text);

rcvar_t vid_exports[] =
{
	RCV_VECTOR("vmode", &vmode, 3),
	RCV_BOOL("yuv", &use_yuv),
	RCV_BOOL("fullscreen", &fullscreen),
	RCV_BOOL("altenter", &use_altenter),
	RCV_END
};

rcvar_t joy_exports[] =
{
	RCV_BOOL("joy", &use_joy),
	RCV_END
};

/* keymap - mappings of the form { scancode, localcode } - from sdl/keymap.c */
extern int keymap[][2];

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
	
	//load_font(&msg_font, MSG_FONT, MSG_FONT_SIZE);
	//msg_font.map = msg_font_map;
	
	SDL_UnlockYUVOverlay(overlay);
	DPRINT("Using YUV overlay");
}

void vid_init()
{
	int flags;

	if (!vmode[0] || !vmode[1])
	{
		int scale = rc_getint("scale");
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
	fb.offset = (fb.w * (fb.h - 144) / 2 + (fb.w - 160) / 2) * fb.pelsize;
	
	//load_font(&msg_font, MSG_FONT, MSG_FONT_SIZE);
	//msg_font.map = msg_font_map;	

	SDL_UnlockSurface(screen);

	fb.enabled = 1;
	fb.dirty = 0;
	DPRINT("Using normal screen");
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
	//msg_font.img = NULL;
}

void vid_close()
{
	//unload_font(&msg_font);
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
	if(sys_msg){ // && msg_font.img){
		//draw_string(&msg_font, sys_msg, 0, 0, 0, fb.w);
		print_text(fb.ptr, 80, 48, sys_msg);
		sys_msg_time--;
		if(!sys_msg_time){
			FREE(sys_msg);
		}
	}
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

/** Shows a message on the screen during t frames */
void vid_message(char *msg, int t){
	sys_msg=strdup(msg);
	sys_msg_time=t;
}

/** Redraws the screen */
void vid_redraw(void){
	if (fb.enabled){
		SDL_Flip(screen);
	}
}

/** Saves a preview of the current game into fname.
 * The preview is a dump of the current screen. */
int vid_savepreview(byte *tmpscreen, char *fname){
#ifdef DEBUG
	printf("Save preview %s\n", fname);
#endif
	return save_png(fname, tmpscreen + fb.offset, 160, 144, (fb.w - 160) * fb.pelsize);
}

/** Clears up the screen passed as argument.
 If the arguent is NULL cleans the current screen from the point of view of the user */
void vid_black_screen(byte * screen){
	if(screen == NULL) screen = fb.ptr;
	memset(screen, 0, fb.pelsize * fb.w * fb.h);
}

void vid_draw_border(image * border){
	vid_black_screen(NULL);
	draw_sprite(border, 0, 0, (fb.w - border->w) / 2, (fb.h - border->h) / 2, border->w, border->h);
	vid_redraw();
	vid_black_screen(NULL);
	draw_sprite(border, 0, 0, (fb.w - border->w) / 2, (fb.h - border->h) / 2, border->w, border->h);
}

///////////////////////////////////////sound/////////////////////////////

#include "pcm.h"


struct pcm pcm;


static int sound = 1;
static int samplerate = 44100;
static int stereo = 1;
static volatile int audio_done;

rcvar_t pcm_exports[] =
{
	RCV_BOOL("sound", &sound),
	RCV_INT("stereo", &stereo),
	RCV_INT("samplerate", &samplerate),
	RCV_END
};


static void audio_callback(void *blah, word *stream, int len)
{
	MEMCPY(stream, pcm.buf, len);
	audio_done = 1;
}


void pcm_init()
{
	int i;
	SDL_AudioSpec as;

	if (!sound) return;
	
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	as.freq = samplerate;
	as.format = AUDIO_U8;
	as.channels = 1 + stereo;
	as.samples = samplerate / 60;
	for (i = 1; i < as.samples; i<<=1);
	as.samples = i;
	as.callback = audio_callback;
	as.userdata = 0;
	if (SDL_OpenAudio(&as, 0) == -1)
		return;
	
	pcm.hz = as.freq;
	pcm.stereo = as.channels - 1;
	pcm.len = as.size;
	pcm.buf = malloc(pcm.len);
	pcm.pos = 0;
	MEMSET(pcm.buf, 0, pcm.len);
	
	SDL_PauseAudio(0);
}

int pcm_submit()
{
	int cnt=0;
	if (!pcm.buf) return 0;
	if (pcm.pos < pcm.len) return 1;
	while (!audio_done || cnt++>10)
		SDL_Delay(4);
	cnt = 0;
	audio_done = 0;
	pcm.pos = 0;
	return 1;
}

void pcm_close()
{
	if (sound) SDL_CloseAudio();
}

/** Pauses the sound */
void pcm_pause(int p){
	SDL_PauseAudio(p);
}

void pcm_volume(int n){
	
}

///////////////////////////////interface to the user///////////////////////

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
		
		// check the joypad
		if(sdl_joy){
			int axis;
			for(i=0; i<10; i++) if(SDL_JoystickGetButton(sdl_joy, i)){ clean=0; break; }
			axis = SDL_JoystickGetAxis(sdl_joy, 0);
			if(axis > joy_commit_range || axis < -joy_commit_range) clean=0;
			axis = SDL_JoystickGetAxis(sdl_joy, 1);
			if(axis > joy_commit_range || axis < -joy_commit_range) clean=0;
		}
		
		// wait half a second
		if(!clean) sys_sleep(100000);		
	}
}

int repeatbutton=0; // number of repetitions of the last button in sys_waitbutton
int lastbutton=0; // code of the last pressed button in sys_waitbutton

/** Waits for a button and returns its code.
 The conly supported buttons are the ones defined in syssdl.h
 
 Repetition: during half second a button is not repeated, even
 if it is hold.
 */
int sys_waitbutton(){
	Uint8 * keyboard;
	int n;
	int button=0;
	DPRINT("Waiting for a button...");
	while(!button){
		sys_sleep(100000);
		// get events
		SDL_PumpEvents();
		
		// TODO: use mapscancode
		
		// check the state of the buttons of the keyboard
		keyboard = SDL_GetKeyState(&n);
		if(keyboard[BUTTON_UP]) button=BUTTON_UP;
		if(keyboard[BUTTON_DOWN]) button=BUTTON_DOWN;
		if(keyboard[BUTTON_LEFT]) button=BUTTON_LEFT;
		if(keyboard[BUTTON_RIGHT]) button=BUTTON_RIGHT;
		if(keyboard[BUTTON_OK]) button=BUTTON_OK;
		if(keyboard[BUTTON_CANCEL]) button=BUTTON_CANCEL;
		if(keyboard[BUTTON_PREVS]) button=BUTTON_PREVS;
		if(keyboard[BUTTON_PAGEUP]) button=BUTTON_PAGEUP;
		if(keyboard[BUTTON_PAGEDOWN]) button=BUTTON_PAGEDOWN;
		//check the state of the buttons of the joystick
		
		if(sdl_joy){
#define joycombo(A, B) if(SDL_JoystickGetButton(sdl_joy, A)) button = B;
			joycombo(0, BUTTON_OK); //A
			joycombo(1, BUTTON_CANCEL); //B
			joycombo(6, BUTTON_PREVS); // Select
			joycombo(4, BUTTON_PAGEUP); // l
			joycombo(5, BUTTON_PAGEDOWN); //r
			if(button == 0){
				// check axis
				if(SDL_JoystickGetAxis(sdl_joy, 1) > joy_commit_range) button = BUTTON_DOWN;
				else if(SDL_JoystickGetAxis(sdl_joy, 1) < -joy_commit_range) button = BUTTON_UP;
				else if(SDL_JoystickGetAxis(sdl_joy, 0) > joy_commit_range) button = BUTTON_RIGHT;
 				else if(SDL_JoystickGetAxis(sdl_joy, 0) < -joy_commit_range) button = BUTTON_LEFT;
			}
		}
		
		
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

/** Shows a message on the screeen waiting for a confirmation from the user */
int sys_confirm(char *msg){
	int button;
#ifdef DEBUG
	printf("Confirm: %s\n", msg);
#endif
	vid_begin();
	vid_message(msg, 1);
	vid_end();
	sys_waitnobutton();
	button = sys_waitbutton();
	sys_waitnobutton();
	return button==BUTTON_OK;
}

/////////////////////////////////////////////////////options//////////////////////////////////////////////////////////


// Entries in the options menu
int available_options[MENU_OPTIONS_ITEMS]={
	OPTION_PALETTE,
	OPTION_FRAME,
	OPTION_MACHINE,
	OPTION_SOUND,
	OPTION_SHOWFPS,
	OPTION_AUTOSKIP,
	OPTION_SAVE_GLOBAL,
	OPTION_SAVE_GAME,
	OPTION_QUIT
};
int * get_available_options(){
	return available_options;
}

/** Aplies the configuration of the user */
void sys_options_apply(){
	char *cmd=NULL;
	
#define RANGE(M,X,V) V=V<M?M:(V>X?X:V)
	RANGE(0, 100, options.volumen);
	
	//  Starts the video
	//gp2x_deinit();
	vid_preinit();
	vid_init();
	if(options.sound)
		pcm_init();
	else
		pcm_close();
	lcd_begin();
	
	// set the frame
	// frames are 32 bit images that are drawn under the 8 bit RGB layer.
	// notice that we only need to draw the frame once, since games do exit their area.
	if(options.frame&&options.scaling==0){
#ifdef SUPERGB
		if(hw.sgb>0 & sgb_border_image_ready == TRUE){
			vid_draw_border(&sgb_border_image);
		}else{
#endif
			image frame_image;
			DPRINT1("Loading frame image: %s", options.frame);
			if(load_png(&frame_image,options.frame)){
				DPRINT1("Drawing frame: %s", options.frame);
				vid_draw_border(&frame_image);
				unload_png(&frame_image);
			}else{
				DPRINT("Error loading frame");
			}
#ifdef SUPERGB
		}
#endif
	}
	
	// set the palette
	// TODO: check wether in SuperGB mode this should be run
	cmd=malloc(256);
	if(cmd){
		sprintf(cmd, "source \"%s\"", options.palette);
		rc_command(cmd);
		pal_dirty();
		FREE(cmd);
	}
}

static unsigned char fontdata8x8[] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x3C,0x42,0x99,0xBD,0xBD,0x99,0x42,0x3C,0x3C,0x42,0x81,0x81,0x81,0x81,0x42,0x3C,
	0xFE,0x82,0x8A,0xD2,0xA2,0x82,0xFE,0x00,0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x64,0x74,0x7C,0x38,0x00,0x00,
	0x80,0xC0,0xF0,0xFC,0xF0,0xC0,0x80,0x00,0x01,0x03,0x0F,0x3F,0x0F,0x03,0x01,0x00,
	0x18,0x3C,0x7E,0x18,0x7E,0x3C,0x18,0x00,0xEE,0xEE,0xEE,0xCC,0x00,0xCC,0xCC,0x00,
	0x00,0x00,0x30,0x68,0x78,0x30,0x00,0x00,0x00,0x38,0x64,0x74,0x7C,0x38,0x00,0x00,
	0x3C,0x66,0x7A,0x7A,0x7E,0x7E,0x3C,0x00,0x0E,0x3E,0x3A,0x22,0x26,0x6E,0xE4,0x40,
	0x18,0x3C,0x7E,0x3C,0x3C,0x3C,0x3C,0x00,0x3C,0x3C,0x3C,0x3C,0x7E,0x3C,0x18,0x00,
	0x08,0x7C,0x7E,0x7E,0x7C,0x08,0x00,0x00,0x10,0x3E,0x7E,0x7E,0x3E,0x10,0x00,0x00,
	0x58,0x2A,0xDC,0xC8,0xDC,0x2A,0x58,0x00,0x24,0x66,0xFF,0xFF,0x66,0x24,0x00,0x00,
	0x00,0x10,0x10,0x38,0x38,0x7C,0xFE,0x00,0xFE,0x7C,0x38,0x38,0x10,0x10,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x18,0x00,0x18,0x18,0x00,
	0x6C,0x6C,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x7C,0x28,0x7C,0x28,0x00,0x00,
	0x10,0x38,0x60,0x38,0x0C,0x78,0x10,0x00,0x40,0xA4,0x48,0x10,0x24,0x4A,0x04,0x00,
	0x18,0x34,0x18,0x3A,0x6C,0x66,0x3A,0x00,0x18,0x18,0x20,0x00,0x00,0x00,0x00,0x00,
	0x30,0x60,0x60,0x60,0x60,0x60,0x30,0x00,0x0C,0x06,0x06,0x06,0x06,0x06,0x0C,0x00,
	0x10,0x54,0x38,0x7C,0x38,0x54,0x10,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x04,0x08,0x10,0x20,0x40,0x00,0x00,
	0x38,0x4C,0xC6,0xC6,0xC6,0x64,0x38,0x00,0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00,
	0x7C,0xC6,0x0E,0x3C,0x78,0xE0,0xFE,0x00,0x7E,0x0C,0x18,0x3C,0x06,0xC6,0x7C,0x00,
	0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x0C,0x00,0xFC,0xC0,0xFC,0x06,0x06,0xC6,0x7C,0x00,
	0x3C,0x60,0xC0,0xFC,0xC6,0xC6,0x7C,0x00,0xFE,0xC6,0x0C,0x18,0x30,0x30,0x30,0x00,
	0x78,0xC4,0xE4,0x78,0x86,0x86,0x7C,0x00,0x7C,0xC6,0xC6,0x7E,0x06,0x0C,0x78,0x00,
	0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x18,0x18,0x30,
	0x1C,0x38,0x70,0xE0,0x70,0x38,0x1C,0x00,0x00,0x7C,0x00,0x00,0x7C,0x00,0x00,0x00,
	0x70,0x38,0x1C,0x0E,0x1C,0x38,0x70,0x00,0x7C,0xC6,0xC6,0x1C,0x18,0x00,0x18,0x00,
	0x3C,0x42,0x99,0xA1,0xA5,0x99,0x42,0x3C,0x38,0x6C,0xC6,0xC6,0xFE,0xC6,0xC6,0x00,
	0xFC,0xC6,0xC6,0xFC,0xC6,0xC6,0xFC,0x00,0x3C,0x66,0xC0,0xC0,0xC0,0x66,0x3C,0x00,
	0xF8,0xCC,0xC6,0xC6,0xC6,0xCC,0xF8,0x00,0xFE,0xC0,0xC0,0xFC,0xC0,0xC0,0xFE,0x00,
	0xFE,0xC0,0xC0,0xFC,0xC0,0xC0,0xC0,0x00,0x3E,0x60,0xC0,0xCE,0xC6,0x66,0x3E,0x00,
	0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0x00,0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,
	0x06,0x06,0x06,0x06,0xC6,0xC6,0x7C,0x00,0xC6,0xCC,0xD8,0xF0,0xF8,0xDC,0xCE,0x00,
	0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00,0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0x00,
	0xC6,0xE6,0xF6,0xFE,0xDE,0xCE,0xC6,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,
	0xFC,0xC6,0xC6,0xC6,0xFC,0xC0,0xC0,0x00,0x7C,0xC6,0xC6,0xC6,0xDE,0xCC,0x7A,0x00,
	0xFC,0xC6,0xC6,0xCE,0xF8,0xDC,0xCE,0x00,0x78,0xCC,0xC0,0x7C,0x06,0xC6,0x7C,0x00,
	0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,
	0xC6,0xC6,0xC6,0xEE,0x7C,0x38,0x10,0x00,0xC6,0xC6,0xD6,0xFE,0xFE,0xEE,0xC6,0x00,
	0xC6,0xEE,0x3C,0x38,0x7C,0xEE,0xC6,0x00,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00,
	0xFE,0x0E,0x1C,0x38,0x70,0xE0,0xFE,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,
	0x60,0x60,0x30,0x18,0x0C,0x06,0x06,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,
	0x18,0x3C,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
	0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x06,0x3E,0x66,0x66,0x3C,0x00,
	0x60,0x7C,0x66,0x66,0x66,0x66,0x7C,0x00,0x00,0x3C,0x66,0x60,0x60,0x66,0x3C,0x00,
	0x06,0x3E,0x66,0x66,0x66,0x66,0x3E,0x00,0x00,0x3C,0x66,0x66,0x7E,0x60,0x3C,0x00,
	0x1C,0x30,0x78,0x30,0x30,0x30,0x30,0x00,0x00,0x3E,0x66,0x66,0x66,0x3E,0x06,0x3C,
	0x60,0x7C,0x76,0x66,0x66,0x66,0x66,0x00,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x00,
	0x0C,0x00,0x1C,0x0C,0x0C,0x0C,0x0C,0x38,0x60,0x60,0x66,0x6C,0x78,0x6C,0x66,0x00,
	0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0xEC,0xFE,0xFE,0xFE,0xD6,0xC6,0x00,
	0x00,0x7C,0x76,0x66,0x66,0x66,0x66,0x00,0x00,0x3C,0x66,0x66,0x66,0x66,0x3C,0x00,
	0x00,0x7C,0x66,0x66,0x66,0x7C,0x60,0x60,0x00,0x3E,0x66,0x66,0x66,0x3E,0x06,0x06,
	0x00,0x7E,0x70,0x60,0x60,0x60,0x60,0x00,0x00,0x3C,0x60,0x3C,0x06,0x66,0x3C,0x00,
	0x30,0x78,0x30,0x30,0x30,0x30,0x1C,0x00,0x00,0x66,0x66,0x66,0x66,0x6E,0x3E,0x00,
	0x00,0x66,0x66,0x66,0x66,0x3C,0x18,0x00,0x00,0xC6,0xD6,0xFE,0xFE,0x7C,0x6C,0x00,
	0x00,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00,0x00,0x66,0x66,0x66,0x66,0x3E,0x06,0x3C,
	0x00,0x7E,0x0C,0x18,0x30,0x60,0x7E,0x00,0x0E,0x18,0x0C,0x38,0x0C,0x18,0x0E,0x00,
	0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,0x70,0x18,0x30,0x1C,0x30,0x18,0x70,0x00,
	0x00,0x00,0x76,0xDC,0x00,0x00,0x00,0x00,0x10,0x28,0x10,0x54,0xAA,0x44,0x00,0x00,
};

static void print_text(byte *screen, int x, int y, char *text)
{
	unsigned int i,l;
	int s;
	screen=screen+(x + y * fb.w) * fb.pelsize;
	
	s = fb.w * fb.pelsize;
	
	for (i=0;i<strlen(text);i++) {			
		for (l=0;l<8;l++) {
			MEMSET(screen + l * s + 0 * fb.pelsize, (fontdata8x8[text[i]*8+l]&0x80?0xff:0), fb.pelsize);
			MEMSET(screen + l * s + 1 * fb.pelsize, (fontdata8x8[text[i]*8+l]&0x40?0xff:0), fb.pelsize);
			MEMSET(screen + l * s + 2 * fb.pelsize, (fontdata8x8[text[i]*8+l]&0x20?0xff:0), fb.pelsize);
			MEMSET(screen + l * s + 3 * fb.pelsize, (fontdata8x8[text[i]*8+l]&0x10?0xff:0), fb.pelsize);
			MEMSET(screen + l * s + 4 * fb.pelsize, (fontdata8x8[text[i]*8+l]&0x08?0xff:0), fb.pelsize);
			MEMSET(screen + l * s + 5 * fb.pelsize, (fontdata8x8[text[i]*8+l]&0x04?0xff:0), fb.pelsize);
			MEMSET(screen + l * s + 6 * fb.pelsize, (fontdata8x8[text[i]*8+l]&0x02?0xff:0), fb.pelsize);
			MEMSET(screen + l * s + 7 * fb.pelsize, (fontdata8x8[text[i]*8+l]&0x01?0xff:0), fb.pelsize);
		}
		screen += fb.pelsize * 8;
	} 
}

