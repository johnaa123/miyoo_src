/*
 *  Copyright (C) 2007 Ludovic Jacomme (ludovic.jacomme@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

# ifndef _PSP_GLOBAL_H_
# define _PSP_GLOBAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gp2x_psp.h"
#include "gp2x_cpu.h"

# define MAX_PATH    512
# define PSP_ALL_BUTTON_MASK 0xFFFF
# define GP2X_ALL_BUTTON_MASK PSP_ALL_BUTTON_MASK
# define PSP_MOVE_BUTTON_MASK (PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_DOWN)

# define PSP_DEFAULT_TXT_DIR "/txt/"

# define GP2X_FONT_6X10  0
# define GP2X_FONT_8X8   1

  typedef struct PSPWRITE_t {

    char psp_homedir[MAX_PATH];
    char psp_txtdir[MAX_PATH];
    char edit_filename[MAX_PATH];
    int  is_modified;
    int  ask_overwrite;
    int  psp_cpu_clock;
    int  psp_font_size;

    int  transparent;
    int  dos_mode;
    int  expand_tab;
    int  tab_stop;
    int  fg_color;
    int  bg_color;
    int  screen_w;
    int  screen_h;
    int  wrap_w;
    int  wrap_mode;
    int  move_on_text;

  } PSPWRITE_t;

  extern PSPWRITE_t PSPWRITE;

  extern int psp_global_save_config();
  extern int psp_global_load_config();
  extern void psp_global_init();

  extern unsigned char psp_convert_utf8_to_iso_8859_1(unsigned char c1, unsigned char c2);

#ifdef __cplusplus
}
#endif

# endif
