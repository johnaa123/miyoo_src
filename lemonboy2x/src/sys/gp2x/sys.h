
/* Gp2X interface for gnuboy
	(C) 2005 K-Teto
	(C) 2008 Juanvvc

	(marked parts) (C) 2008 Puck2099 

	Under the GPL
*/

#ifndef __GP2X_H
#define __GP2X_H

#include "gui/menu.h"

#ifdef WIZLIB
#include "../wiz/wiz_lib.h"
#else
#include "minimal.h"
#endif

#include "../sys.h"

/* Buttons for the menu intrface */
#define BUTTON_NONE GP2X_NOBUTTON
#define BUTTON_UP GP2X_UP
#define BUTTON_DOWN GP2X_DOWN
#define BUTTON_RIGHT GP2X_RIGHT
#define BUTTON_LEFT GP2X_LEFT
#define BUTTON_OK GP2X_B
#define BUTTON_PREVS GP2X_SELECT
#define BUTTON_CANCEL GP2X_X
#define BUTTON_PAGEUP GP2X_L
#define BUTTON_PAGEDOWN GP2X_R

// Entries in the options menu
#define MENU_OPTIONS_ITEMS 11
//available_options is defined in gp2x.c


#endif
