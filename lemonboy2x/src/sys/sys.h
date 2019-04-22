
#ifndef __SYS_H
#define __SYS_H

#include "gui/imaging.h"

/** Initializes the video system for a new freame. Usually, draws THE GAME REGION on the screen */
void vid_init();
void vid_preinit();
void pcm_init();
void pcm_close();
void pcm_volume(int);

void sys_sleep();

/** Saves a PNG image of the game region in a file */
int vid_savepreview(byte*, char*);
/** Redraws the whole screen, as is */
void vid_redraw(void);
/** Clears up the screen passed as argument.
 If the arguent is NULL cleans the current screen from the point of view of the user */
void vid_black_screen(byte*);
int sys_waitbutton();
void sys_waitnobutton();
int sys_confirm(char* message);
/** Returns the available options for this system */
int * get_available_options();
/** Draws a game border on the screen */
void vid_draw_border(image *);
/** Apply options once the system is initialized */
void sys_options_apply();

/** The system framebuffer */
extern struct fb fb;

// Load the button configuration of each system
#if defined(GP2X_MINIMAL) || defined(WIZLIB)
#include "gp2x/sys.h"
#else
#include "sdl/sys.h"
#endif

#endif

