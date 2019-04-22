/* Gp2X interface for gnuboy
	(C) 2005 K-Teto
	(C) 2008 Juanvvc

	(marked parts) (C) 2008 Puck2099 

	Under the GPL
*/

#include <stdlib.h>
#include <stdio.h>

#ifdef WIZLIB
#include "../wiz/wiz_lib.h"
#else
#include "minimal.h"
#include "cpuclock.h"
#endif

#include "fb.h"
#include "lcd.h"
#include "pcm.h"
#include "input.h"
#include "rc.h"
#include "sys.h"

#include "defs.h"

#ifdef SUPERGB
#include "hw.h"
#include "supergb.h"
#endif

#include "gui/options.h"
#include "gui/imaging.h"

#include "../scale2x/scalebit.h"

#define ONE_FRAME 16666
//#define ONE_SECOND 1000000
#define ONE_SECOND 1000


//If def, uses an internal buffer between gnuboy and minimal. That is
//three buffers in total. It is a bit slower (not much) and solves the
//glitching screen problem in transitions.
// Besides, it is mandatory to support several video modes.
// (I didn't test to undefine this for a long time. Maybe it does not work any more)
#define DOUBLEBF

#ifdef GP2X_ASM
#define MEMCPY gp2x_memcpy
#define MEMSET memset
#else
#define MEMCPY memcpy
#define MEMSET memset
#endif

// Options are managed in options.c
rcvar_t vid_exports[] =
{
	RCV_END
};
rcvar_t joy_exports[] =
{
	RCV_END
};
#ifdef GP2X_SOUND_THREAD
rcvar_t pcm_exports[] =
{
	RCV_END
};
#endif

/////////////////////////////////////////////////////video//////////////////////////////////////////////////////////

struct fb fb;

static int last_tick=0;
static int frames_showed=0;
int frames_acum=0;
int frames_skip=0;
static unsigned long backuptimer;

/* Position of the FPS text */
static int fps_x=0, fps_y=0;

#ifdef DOUBLEBF
static byte *fakescreen=NULL;
#endif

static char *sys_msg=NULL;
static int sys_msg_time=0;

static int cpuclockold = 0;

void gp2x_video_RGB_flip(int arg)
{
  SDL_Flip(screen);
}

/** Shows a message on the screen during t frames */
void vid_message(char *msg, int t){
	sys_msg=strdup(msg);
	sys_msg_time=t;
}

void vid_init()
{
	DPRINT1("Video mode %d\n", options.scaling);
	fb.w = 320;
	fb.h = 240;
#ifdef DOUBLEBF
	fb.ptr = fakescreen;
#else
#if WIZLIB
	fb.ptr = (byte *)(fb1_16bit);
#else
	fb.ptr = (byte *)(gp2x_video_RGB[0].screen16);
#endif
#endif
	fb.pelsize = 2; 
	fb.pitch = 640;
	fb.indexed = 0;
	fb.cc[0].r = 3; // R long
	fb.cc[1].r = 2; //lG long
	fb.cc[2].r = 3; //lB long
	fb.cc[0].l = 11; //offset R
	fb.cc[1].l = 5; //offset G
	fb.cc[2].l = 0; //offset B
	fb.yuv = 0;
	fb.enabled = 1;
	fb.dirty = 0;

#ifndef DOUBLEBF
#ifndef WIZLIB
	//double size modes don't work if not DOUBLEBF: use hardware fullscreen instead
	if(options.scaling==vmode_dbl||options.scaling==vmode_dbldef) options.scaling=vmode_fs; // check on gp2x (was modes 3 and 5)
#endif
#endif

	switch(options.scaling){
	case vmode_s2xdef: // scale2x deformed
		fb.offset=0; fps_x=0; fps_y=0;
#ifndef WIZLIB
		gp2x_video_RGB_setscaling(160, 120);
#endif
		break;
	case vmode_s2x: // scale2x
		fb.offset=0; fps_x=0; fps_y=0;
#ifndef WIZLIB
		gp2x_video_RGB_setscaling(160, 120);
#endif
		break;
	case vmode_ipol: // interpolation
		fb.offset=(40+10*320)*fb.pelsize; fps_x = 40; fps_y = 10;
#ifndef WIZLIB
		gp2x_video_RGB_setscaling(160, 240);
#endif
		break;
	case vmode_dbldef: // double size, deformed
		fb.offset=0; fps_x=0; fps_y=0;
#ifndef WIZLIB
		gp2x_video_RGB_setscaling(160, 240);
#endif
		break;
	case vmode_dbl: // double size
		fb.offset=0; fps_x=0; fps_y=0;
#ifndef WIZLIB
		gp2x_video_RGB_setscaling(160, 120);
#endif
		break;
#ifndef WIZLIB		
	case vmode_fsasp://fullscreen, aspect ratio
		fb.offset=14*fb.pelsize; fps_x=14; fps_y=0;
		gp2x_video_RGB_setscaling(192, 144);
		break;
	case vmode_fs: // fullscreen
		fb.offset=0; fps_x=fps_y=0;
		gp2x_video_RGB_setscaling(160, 144);
		break;
#endif		
	default: //normal
		fb.offset=(80+48*320)*fb.pelsize; fps_x = 80; fps_y = 48;
#ifndef WIZLIB
		gp2x_video_RGB_setscaling(320, 240);
#endif
		break;
	}
}

static byte tmpline[160*2*4]; // up to pelsize=4 systems
void scaleline2x(void *dst, void *src){
    byte *t=tmpline;
    int tmpx, pixel1, pixel2;

    for( tmpx=0; tmpx<160; tmpx++ )
    {
	pixel1 = tmpx*2*fb.pelsize;
	pixel2 = (tmpx*2+1)*fb.pelsize;
	MEMCPY(t+pixel1, src, fb.pelsize);
	MEMCPY(t+pixel2, src, fb.pelsize);
	src+=fb.pelsize;
    }
    MEMCPY(dst, t, 320*fb.pelsize);
}

			static int done = 0;

/** Draws a frame on the screen */
void vid_begin(){
	unsigned long timer=gp2x_timer_read();
	unsigned long before=backuptimer;
	unsigned int lastenabled=fb.enabled;
	void *fs, *s, *s0, *s1;
	int d1, d2, d3, d4;
	int tempy;
	
	// a simple frameskip algorithm, valid for games with at least 30FPS
	// Algorithm: each two frames, check if the last frame was on time. If
	// it wasn't, skip the next frame
	if (options.autofskip){
		if(((timer - before)>ONE_FRAME)&fb.enabled){
			fb.enabled=0;
			frames_skip++;
		}else{
			fb.enabled=1;
		}
	}

	if(lastenabled){
#ifdef DOUBLEBF
		// if not DOUBLEBF, gnuboy used directly the screen and we do not need to do any more here.
		// if DOUBLEBF, blit fakescreen on the real screen
		switch(options.scaling){
		case vmode_s2xdef: // scale2x deformed
#ifdef WIZLIB
			s0=(void *)fb0_16bit;
			s1=(void *)fb1_16bit;
#else
			s=(void *)gp2x_video_RGB[0].screen16;
#endif
			scale(2, s0, 320*fb.pelsize, fakescreen, fb.pitch, fb.pelsize, 160, 144);

			int src_line, dst_line;
			for( dst_line=src_line=0; src_line<288; src_line++ )
			{
				if( src_line % 6 != 0 )
				{
					MEMCPY(s1+320*fb.pelsize*dst_line, s0+320*fb.pelsize*src_line, 320*fb.pelsize);
					dst_line++;
				}
			}			
			break;
		case vmode_s2x: // scale2x
			fs=fakescreen+options.voffset*fb.pitch; //+fb.offset(==0);
#ifdef WIZLIB
			s=(void *)fb1_16bit;
#else
			s=(void *)gp2x_video_RGB[0].screen16;
#endif
			scale(2, s, 320*fb.pelsize, fs, fb.pitch, fb.pelsize, 160, 144);
			break;
		case vmode_ipol: // INTERPOLATION
#ifdef WIZLIB			
			s0=(void *)fb0_16bit;
			s1=(void *)fb1_16bit;
#else
			s=(void *)gp2x_video_RGB[0].screen16;
#endif
			vid_black_screen(s0);
			upscale_aspect(s0, fakescreen);
			MEMCPY(s1, s0, 320*240*fb.pelsize);
			break;
		case vmode_dbldef: // DEFORMED DOUBLE SIZE
			tempy=144;
			d1=24 - (options.vdeformation>>1);
			d2=d1+options.vdeformation;
			d3=d2+96-options.vdeformation;
			d4=d3+options.vdeformation;
			//d5==144=f4+24-options.distorsion>>1;
			fs=fakescreen; //+fb.offset(==0);
#ifdef WIZLIB
			s=(void *)fb1_16bit;
#else
			s=(void *)gp2x_video_RGB[0].screen16;
#endif
			while(tempy--){
				scaleline2x(s, fs);
				s+=320*fb.pelsize;
				if(tempy>d1&&tempy<d4){
					if(tempy>d2&&tempy<d3){
						scaleline2x(s, fs);
						s+=320*fb.pelsize;
					}else if(tempy&1){
						scaleline2x(s, fs);
						s+=320*fb.pelsize;
					}
				}
				fs+=fb.pitch;
			}
			break;
		case vmode_dbl: // double size
			tempy=120;
			fs=fakescreen+options.voffset*fb.pitch; //+fb.offset(==0);
#ifdef WIZLIB
			s=(void *)fb1_16bit;
#else
			s=(void *)gp2x_video_RGB[0].screen16;
#endif
			while(tempy--){
			    scaleline2x(s, fs);
			    MEMCPY(s+320*fb.pelsize, s, 320*fb.pelsize);
			    s+=2*320*fb.pelsize;
			    fs+=fb.pitch;
			}
			break;
		default: // normal, fullscreen and fullscreen with ratio
			tempy=144;
			fs=fakescreen+fb.offset;
#ifdef WIZLIB
			s=(void *)fb1_16bit;
#else
			s=(void *)gp2x_video_RGB[0].screen16;
#endif
			s+=fb.offset;
			while(tempy--){
				MEMCPY(s, fs, 160*fb.pelsize);
				s+=320*fb.pelsize;
				fs+=fb.pitch;
			}
		}
#endif

		// this code from AlexKidd by Puck
		if (options.showfps) {
			if(lastenabled) frames_acum++;
			// change frames_showed, once per second
//			if (timer-last_tick > ONE_SECOND) {
			if (SDL_GetTicks()-last_tick > ONE_SECOND) {
				frames_showed = frames_acum+frames_skip;
				frames_acum = 0;
				frames_skip = 0;
				last_tick = SDL_GetTicks();
			}
			// show the number
#ifdef WIZLIB
			gp2x_gamelist_text_out_fmt(fps_x, fps_y,"%d ", frames_showed);
#else
			gp2x_printf(NULL, fps_x, fps_y,"%d ", frames_showed);
#endif
		}

#ifndef WIZLIB
		if(options.vsync) gp2x_video_waitvsync();
#endif

		if(sys_msg){
			if(sys_msg_time--){
#ifdef WIZLIB
				gp2x_gamelist_text_out_fmt(fps_x + 15, fps_y, sys_msg);
#else
				gp2x_printf(NULL, fps_x + 15, fps_y, sys_msg);
#endif
			}else{
				FREE(sys_msg);
			}
		}

#ifdef WIZLIB
		gp2x_video_flip_single();
#else
		gp2x_video_RGB_flip(0);
#endif
#ifndef DOUBLEBF
#ifdef WIZLIB
		fb.ptr=(byte *)(fb1_16bit);
#else
		fb.ptr=(byte *)(gp2x_video_RGB[0].screen16);
#endif
#endif
	}
	backuptimer = timer;
}

void vid_setpal(int i, int r, int g, int b)
{
	// does nothing in 16 bits mode
}

/** Initializes the screen */
void vid_preinit()
{
	//Here options.samplerate is not set, yet
	//printf("Mode 16 bits. Samplerate %d\n", options.samplerate);
#ifdef WIZLIB
	gp2x_init(16, options.samplerate, 16, 1);
#else
	gp2x_init(ONE_SECOND, 16, options.samplerate, 16, 1, 60, 1);
#endif

#ifdef DOUBLEBF
	FREE(fakescreen);
	fakescreen = calloc(320*240, 2);
#endif
}

void vid_close(){
	// not calling to gp2x_deinit(): it will be called at the end of the program in main.shutdown()
}

void vid_settitle(char *title){
}

// void vid_end()
// {
// }

/** Clears up the screen passed as argument.
 If the arguent is NULL cleans the current screen from the point of view of the user */
void vid_black_screen(byte * screen){
	if(screen == NULL) screen = fb.ptr;
	memset(screen, 0, fb.pelsize * fb.w * fb.h);
}

/** Draws a game border on the screen */
void vid_draw_border(image * border){
	vid_black_screen(NULL);
	draw_sprite(border, 0, 0, (fb.w - border->w)/2, (fb.h - border->h)/2, border->w, border->h);
	vid_redraw();
	vid_black_screen(NULL);
	draw_sprite(border, 0, 0, (fb.w - border->w)/2, (fb.h - border->h)/2, border->w, border->h);
	vid_redraw();
}


/** Redraws the screen, exactly as it is, in both buffers */
void vid_redraw(void){
#ifdef DOUBLEBF
#ifdef WIZLIB
	MEMCPY((void*)fb1_16bit, fakescreen, fb.w * fb.h * fb.pelsize);
	gp2x_video_flip_single();
#else
	MEMCPY(gp2x_video_RGB[0].screen16, fakescreen, fb.w * fb.h * fb.pelsize);
	gp2x_video_RGB_flip(0);
#endif
#else
#ifdef WIZLIB
	MEMCPY((void*)fb1_16bit, fb.ptr, fb.w * fb.h * fb.pelsize);
	gp2x_video_flip_single();
	fb.ptr = fb1_16bit;
#else
	MEMCPY(gp2x_video_RGB[0].screen16, fb.ptr, fb.w * fb.h * fb.pelsize);
	gp2x_video_RGB_flip(0);
	fb.ptr = gp2x_video_RGB[0].screen16;
#endif
#endif
}

/////////////////////////////////////////////////////events//////////////////////////////////////////////////////////

void ev_poll()
{
	char volume[15];
	/*This macro belongs to rlyeh, thanks, you are the man!!*/
	#define padcombo(A,B) ev.code=A; ev.type=(pad & B ? EV_PRESS : EV_RELEASE); ev_postevent(&ev);
	// usage, padcombo(K_JOYUP.GP2X_UP);
	unsigned long pad=0;
	event_t ev;

#ifdef WIZLIB
	pad=gp2x_joystick_read(0);
#else
	pad=gp2x_joystick_read();
#endif

	padcombo (K_JOYUP,GP2X_UP);
	padcombo (K_JOYDOWN,GP2X_DOWN);
	padcombo (K_JOYLEFT,GP2X_LEFT);
	padcombo (K_JOYRIGHT,GP2X_RIGHT);
	padcombo (K_JOY0, GP2X_A);
	padcombo (K_JOY1, GP2X_B);
	padcombo(K_JOY2, GP2X_X);
	padcombo(K_JOY3, GP2X_Y);
	padcombo(K_JOY4, GP2X_L);
	padcombo(K_JOY5, GP2X_R);
	padcombo (K_JOY6,GP2X_SELECT);  // select
	padcombo (K_JOY7,GP2X_START); // start
	
	// sound buttons have a special management
// 	if(pad&(GP2X_VOL_UP|GP2X_VOL_DOWN)){
// 		// Vol+ and - pressed
// 		ev.code=K_JOY8; ev.type=EV_PRESS; ev_postevent(&ev);
// 	}else{
		// manage sound
		if ((pad & GP2X_VOL_UP) && (options.volumen <100) && options.sound){
				options.volumen++;
				pcm_volume(options.volumen);
				sprintf(volume, "Volume %d", options.volumen);
				vid_message(volume, 60);
		}
			
		if ((pad & GP2X_VOL_DOWN) && (options.volumen >0) && options.sound)	{
			options.volumen--;
				pcm_volume(options.volumen);
				sprintf(volume, "Volume %d", options.volumen);
				vid_message(volume, 60);
		}
// 	}
}

/////////////////////////////////////////////////////audio//////////////////////////////////////////////////////////

#ifdef GP2X_SOUND_THREAD
static volatile int audio_done;

struct pcm pcm={0, 0, 1, NULL, 0};
#ifndef WIZLIB
extern int gp2x_sound_running;
#else
int gp2x_sound_running = 0;
#endif

/** Pauses the sound */
void pcm_pause(int p){
#ifdef WIZLIB
	if(gp2x_sound_running) gp2x_sound_thread_mute(p);
#else
	if(gp2x_sound_running) gp2x_sound_pause(p);
#endif
}

void pcm_volume(int n){
	gp2x_sound_volume(n, n);
}

/** Ends the sound */
void pcm_close()
{
	if(!gp2x_sound_running) return;
#ifdef WIZLIB
	gp2x_sound_thread_stop();
	gp2x_sound_running = 0;
#else
	gp2x_sound_close();
#endif
	FREE(pcm.buf);
}

/** Initializes the sound */
void pcm_init()
{
	if(gp2x_sound_running) return;
	pcm_close();
	pcm.hz = options.samplerate;
	pcm.stereo = 1;
	pcm.len = options.samplerate/60;
	FREE(pcm.buf);
	pcm.buf = malloc(pcm.len);
	pcm.pos = 0;
	MEMSET(pcm.buf, 0, pcm.len);
	pcm_volume(options.volumen);
#ifdef WIZLIB
	gp2x_sound_thread_start(NULL);
	gp2x_sound_running=1;
#else
	gp2x_sound_start();
#endif
}

int pcm_submit()
{
	if (!pcm.buf) return 0;
	if (pcm.pos < pcm.len) return 1;
	while (!audio_done)
		SDL_Delay(4);
	audio_done = 0;
	pcm.pos = 0;
	return 1;
}
#endif

/** Returns a pointer to timer for the system */
long *sys_timer(){
	long *t;
	t=malloc(sizeof(int));
	*t=gp2x_timer_read();
	return t;
}

/** Sleeps for us microseconds */
void sys_sleep(int us){
	// FIXME: use microseconds, not miliseconds
	if(us<0) return;
	gp2x_timer_delay(us);
}

/** Returns the number of microseconds from the last
call to this function with timer t */
int sys_elapsed(long *t){
	long n;
	int d;
	n = gp2x_timer_read();
	d = (int)(n - *t); //*1000;
	*t = n;
	return d;
}

// FIXME: these functions are not used any more
/** Does nothing */
void sys_sanitize(){}
/** Does nothing */
void sys_checkdir(char* name, int rw){}

//////////////////////////////PREVIEWS///////////////////////////////

/** Saves a preview of the current game into fname.
 * The preview is a dump of the current screen. */
int vid_savepreview(byte *tmpscreen, char *fname){
#ifdef DOUBLEBF
	DPRINT1("Save preview %s", fname);
	if(fakescreen==NULL) return 0;
	return save_png(fname, tmpscreen+fb.offset, 160, 144, 160*2);
#endif
}

/** Shows a message on the screeen waiting for a confirmation from the user */
int sys_confirm(char *msg){
	int button;
	DPRINT1("Confirm: %s", msg);
#ifdef WIZLIB
	gp2x_gamelist_text_out_fmt(fps_x + 15, fps_y, msg);
#else
	gp2x_printf(NULL, fps_x + 15, fps_y, "%s Y/X", msg);
#endif
#ifdef WIZLIB
	gp2x_video_flip_single();
#else
	gp2x_video_RGB_flip(0);
#endif
	sys_waitnobutton();
	button=sys_waitbutton();
	sys_waitnobutton();
	return button==GP2X_Y;
}

static int lastbutton=0;
static int repeatbutton=0;

/** Waits for a button press.
Returns the code of the button */
int sys_waitbutton(){
	DPRINT("Waiting for a button");
	int pad=0;
	int button=0;
	while(!button){
		gp2x_timer_delay(100000);
#ifdef WIZLIB
		pad=gp2x_joystick_read(0);
#else
		pad=gp2x_joystick_read();
#endif
		if(pad&GP2X_UP) button=GP2X_UP;
		if(pad&GP2X_DOWN) button=GP2X_DOWN;
		if(pad&GP2X_RIGHT || pad&GP2X_R) button=GP2X_RIGHT;
		if(pad&GP2X_LEFT || pad&GP2X_L) button=GP2X_LEFT;
		if(pad&GP2X_A) button=GP2X_A;
		if(pad&GP2X_B) button=GP2X_B;
		if(pad&GP2X_Y) button=GP2X_Y;
		if(pad&GP2X_X) button=GP2X_X;
		if(pad&GP2X_L) button=GP2X_L;
		if(pad&GP2X_R) button=GP2X_R;
		if(pad&GP2X_SELECT) button=GP2X_SELECT;
		if(pad&GP2X_START) button=GP2X_START;
		if(button==0){
			lastbutton=GP2X_NOBUTTON;
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
			button=GP2X_NOBUTTON;
	}
	return button;
}

void sys_waitnobutton(){
	DPRINT("Waiting for no button");
	int pad=0;
	int button=0;
	do{
		button=0;
#ifdef WIZLIB
		pad=gp2x_joystick_read(0);
#else
		pad=0;
#endif
		if(pad&GP2X_UP) button=GP2X_UP;
		if(pad&GP2X_DOWN) button=GP2X_DOWN;
		if(pad&GP2X_RIGHT || pad&GP2X_R) button=GP2X_RIGHT;
		if(pad&GP2X_LEFT || pad&GP2X_L) button=GP2X_LEFT;
		if(pad&GP2X_A) button=GP2X_A;
		if(pad&GP2X_B) button=GP2X_B;
		if(pad&GP2X_Y) button=GP2X_Y;
		if(pad&GP2X_X) button=GP2X_X;
		if(pad&GP2X_L) button=GP2X_L;
		if(pad&GP2X_R) button=GP2X_R;
		gp2x_timer_delay(50);
	}while(button!=0);
}

/** Aplies the configuration of the user */
void sys_options_apply(){
	char *cmd=NULL;

#define RANGE(M,X,V) V=V<M?M:(V>X?X:V)
	// test proper values for options
#ifdef WIZLIB
	RANGE(0, 6, options.scaling);
#else
	RANGE(0, 8, options.scaling);
#endif
	RANGE(0, 48, options.vdeformation);
	RANGE(-1, 4, options.vmodes[0]);
	RANGE(-1, 4, options.vmodes[1]);
	RANGE(-1, 4, options.vmodes[2]);
	RANGE(-1, 4, options.vmodes[3]);
	options.vmodes[4]=-1;
	RANGE(0, 24, options.voffset);
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
	vid_black_screen(NULL);

	// set the frame
	// frames are 32 bit images that are drawn under the 8 bit RGB layer.
	// notice that we only need to draw the frame once, since games do exit their area.
	if(options.frame && options.scaling==vmode_normal){
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
	cmd=malloc(256);
	if(cmd){
		sprintf(cmd, "source \"%s\"", options.palette);
		rc_command(cmd);
		pal_dirty();
		FREE(cmd);
	}

	// set the clock
	if( cpuclockold != options.cpuclock )
	{
#ifdef WIZLIB
		gp2x_set_clock(options.cpuclock);
#else
		Set920Clock(options.cpuclock);
#endif
	}
	cpuclockold = options.cpuclock;
}

int available_options[MENU_OPTIONS_ITEMS]={
	OPTION_PALETTE,
	OPTION_SCALING,
	OPTION_MACHINE,
	OPTION_FRAME,	
	OPTION_SOUND,
	OPTION_SHOWFPS,
	OPTION_AUTOSKIP,
	OPTION_CPU,
	OPTION_SAVE_GLOBAL,
	OPTION_SAVE_GAME,
	OPTION_QUIT
};
/** Returns the available options for this system */
int * get_available_options(){
	return available_options;
}
