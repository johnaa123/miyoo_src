#ifndef _OPTIONS_H

/** The user configurable options */
struct options {
	char *palette; // path to the palette
	char *frame; // path to the frame
	char *romsdir; //path to the roms dir
	int volumen; // volumen of the sound
	int scaling; // type of scaling. 0=normal, 1=fullscreen, 2=fullscreen maintainig ratio
	int sound; // sound on/off
	int autofskip; // auto frameskip on/off
	int vsync; // vsync on/off
	int showfps; // show FPS on/off
	int cpuclock; // CPU clock
	int samplerate; // samplerate for sound. Currently, only in etc/system.gp2x
	int previews; // previews actived or not
	int confirmsave; //confirms the saving of a state
	int confirmload; // confirms the load of a state
	int vdeformation; //vdeformation of the video in deformed double size mode (from 0 to 48)
	int vmodes[5]; // video modes to roll in cmd_videochange
	int voffset; // offset of the first line in scaling=3 (double size) From 0 to 24, 12 centers screen
 	int machine; // type of machine
	int subdirectories; // include or not subdirectories in the file menu
	int showsplash; // wether or not to show the splash window
};

extern struct options options;

#define OPTION_GB 0
#define OPTION_GBC 1
#define OPTION_GBA 2
#define OPTION_SGB 3

/** Initialize options to default values.
 * Actually, reading the default values from the configuration files is preferred. */
void options_init(void);
/** Save current options in a file */
int options_save(char* fname);
/** Apply the current options.
 * Some other options (sound, frameskip) must be applied on the fly in gp2x.c */
void options_apply();
#ifdef DEBUG
/** Print some options for debugging */
void options_print(void);
#endif

typedef enum videomodesT {
    vmode_normal=0,
#ifndef WIZLIB
    vmode_fs,
    vmode_fsasp,
#endif
    vmode_dbl,
    vmode_dbldef,
    vmode_ipol,
    vmode_s2x,
    vmode_s2xdef
} videomodesT;

extern videomodesT videomodes;

#define _OPTIONS_H
#endif
