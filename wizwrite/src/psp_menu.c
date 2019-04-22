/*
 *  Copyright (C) 2008 Ludovic Jacomme (ludovic.jacomme@gmail.com)
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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "gp2x_psp.h"

#include "psp_global.h"
#include "psp_danzeff.h"
#include "psp_sdl.h"
#include "psp_menu.h"
#include "psp_menu_set.h"
#include "psp_help.h"
#include "psp_fmgr.h"
#include "psp_editor.h"

# define MENU_FILENAME     0
# define MENU_HELP         1
# define MENU_NEW          2
# define MENU_LOAD         3
# define MENU_SAVE         4
# define MENU_SETTINGS     5

# define MENU_BACK         6
# define MENU_EXIT         7

# define MAX_MENU_ITEM (MENU_EXIT + 1)

  static menu_item_t menu_list[] =
  {
    { "File: " },

    { "Help" },

    { "New" },
    { "Load" },
    { "Save" },

    { "Settings" },

    { "Back" },
    { "Exit" }
  };

  static int cur_menu_id = MENU_NEW;

void
string_fill_with_space(char *buffer, int size)
{
  int length = strlen(buffer);
  int index;

  for (index = length; index < size; index++) {
    buffer[index] = ' ';
  }
  buffer[size] = 0;
}

# define FILENAME_FIELD_WIDTH      40

# if defined(WIZ_MODE)
void
psp_menu_set_mouse_area( menu_item_t* menu_item, int x, int y, int y_step )
{
  menu_item->x0 = x;
  menu_item->x1 = x + (strlen(menu_item->title) + 2) * 6;
  menu_item->y0 = y + 1;
  menu_item->y1 = y + y_step - 2;
}

int
psp_menu_is_mouse_area( menu_item_t* menu_item, gp2xCtrlData* c )
{
  int mouse_x = c->Mx;
  int mouse_y = c->My;

  if (((mouse_x >= menu_item->x0) && (mouse_x <= menu_item->x1)) &&
      ((mouse_y >= menu_item->y0) && (mouse_y <= menu_item->y1))) {
    if (mouse_x > ( menu_item->x0 + menu_item->x1 ) / 2) {
      // Right
      return 1;
    }
    // Left
    return -1;
  }
  return 0;
}
# endif

void 
psp_display_screen_menu(void)
{
  char buffer[512];

  int menu_id = 0;
  int color   = 0;
  int x       = 0;
  int y       = 0;
  int y_step  = 0;

  psp_sdl_blit_menu();
  
  x      = 10;
  y      = 10;
  y_step = 10;
  
  for (menu_id = 0; menu_id < MAX_MENU_ITEM; menu_id++) {
    color = GP2X_MENU_TEXT_COLOR;
    if (cur_menu_id == menu_id) color = GP2X_MENU_SEL_COLOR;
    else 
    if (menu_id == MENU_EXIT) color = GP2X_MENU_WARNING_COLOR;
    else
    if (menu_id == MENU_HELP) color = GP2X_MENU_GREEN_COLOR;

    psp_sdl_back_print(x, y, menu_list[menu_id].title, color);
# if defined(WIZ_MODE)
    psp_menu_set_mouse_area( &menu_list[menu_id], x, y, y_step );
# endif

    if (menu_id == MENU_FILENAME) {
      int length = strlen(PSPWRITE.edit_filename);
      int first = 0;
      if (length > (FILENAME_FIELD_WIDTH-1)) {
        first = length - (FILENAME_FIELD_WIDTH-1);
      }
      sprintf(buffer, "%s", PSPWRITE.edit_filename + first);
      if (menu_id == cur_menu_id) strcat(buffer, "_");
      string_fill_with_space(buffer, FILENAME_FIELD_WIDTH+1);
      psp_sdl_back_print(50, y, buffer, color);

      y += y_step;

    } else
    if (menu_id == MENU_HELP) {
      y += y_step;
    } else
    if (menu_id == MENU_SAVE) {
      y += y_step;
    } else
    if (menu_id == MENU_BACK) {
      y += y_step;
    }

    y += y_step;
  }
}

int
psp_edit_menu_confirm_overwrite()
{
  gp2xCtrlData c;

  if (PSPWRITE.ask_overwrite) {

    struct stat aStat;
    if (stat(PSPWRITE.edit_filename, &aStat)) {
      PSPWRITE.ask_overwrite = 0;
      return 1;
    }

    psp_display_screen_menu();
    psp_sdl_back_print(150, 60, "X overwrite ? B cancel", GP2X_MENU_WARNING_COLOR);
    psp_sdl_flip();

    psp_kbd_wait_no_button();
    do
    {
      gp2xCtrlReadBufferPositive(&c, 1);
      c.Buttons &= GP2X_ALL_BUTTON_MASK;

      if (c.Buttons & GP2X_CTRL_CROSS) {
        return 1;
      } else 
      if (c.Buttons & GP2X_CTRL_SQUARE) {
        return 0;
      }

    } while (c.Buttons == 0);
    psp_kbd_wait_no_button();
  }
  return 0;
}

static int
psp_edit_menu_save()
{
  int error;

  if (PSPWRITE.ask_overwrite) {
    if (! psp_edit_menu_confirm_overwrite()) {
      return 0;
    }
  }
  PSPWRITE.ask_overwrite = 0;
  error = psp_editor_save();

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back_print(150, 60,  "File saved !", 
                       GP2X_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  else 
  {
    psp_display_screen_menu();
    psp_sdl_back_print(150, 60,  "Can't save file !", 
                       GP2X_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  return error;
}

int
psp_edit_menu_confirm_save()
{
  gp2xCtrlData c;

  if (PSPWRITE.is_modified) {

    psp_display_screen_menu();
    psp_sdl_back_print(150, 60, "Save changes ? A Save ", GP2X_MENU_WARNING_COLOR);
    psp_sdl_back_print(150, 70, "B ignore, TA cancel", GP2X_MENU_WARNING_COLOR);
    psp_sdl_flip();

    psp_kbd_wait_no_button();
    do
    {
      gp2xCtrlReadBufferPositive(&c, 1);

      if (c.Buttons & GP2X_CTRL_CROSS) {
        if (psp_edit_menu_save()) {
          /* failed -> cancel */
          return 0;
        }
      } else 
      if (c.Buttons & GP2X_CTRL_SQUARE) {
        return 0;
      } else 
      if (c.Buttons & GP2X_CTRL_CIRCLE) {
        break;
      }

    } while (c.Buttons == 0);
    psp_kbd_wait_no_button();
  }
  return 1;
}

int
psp_edit_menu_exit(void)
{
  if (! psp_edit_menu_confirm_save()) {
    return 0;
  }

  gp2xCtrlData c;

  psp_display_screen_menu();
  psp_sdl_back_print(150, 60, "Press A to exit !", GP2X_MENU_WARNING_COLOR);
  psp_sdl_flip();

  psp_kbd_wait_no_button();

  do
  {
    gp2xCtrlReadBufferPositive(&c, 1);

    if (c.Buttons & GP2X_CTRL_CROSS) {
      psp_sdl_exit(0);
    }

  } while (c.Buttons == 0);

  psp_kbd_wait_no_button();

  return 0;
}

int
psp_edit_menu_new(void)
{
  if (psp_edit_menu_confirm_save()) {
    psp_editor_new();
  }
  return 0;
}

int
psp_edit_menu_load(void)
{
  int ret;

  if (! psp_edit_menu_confirm_save()) {
    return 0;
  }
  ret = psp_fmgr_menu(GP2X_FMGR_FORMAT_TXT);
  if (ret ==  1) /* load OK */
  {
    psp_display_screen_menu();
    psp_sdl_back_print(150,  60, "File loaded !", GP2X_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
    return 1;
  }
  else 
  if (ret == -1) /* Load Error */
  {
    psp_display_screen_menu();
    psp_sdl_back_print(150,  60, "Can't load file !", GP2X_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  return 0;
}

void
psp_edit_menu_filename_add_char(int a_char)
{
  int length = strlen(PSPWRITE.edit_filename);
  if (length < (MAX_PATH-2)) {
    PSPWRITE.edit_filename[length] = a_char;
    PSPWRITE.edit_filename[length+1] = 0;
  }
}

void
psp_edit_menu_filename_clear()
{
  PSPWRITE.edit_filename[0] = 0;
}

void
psp_edit_menu_filename_del()
{
  int length = strlen(PSPWRITE.edit_filename);
  if (length >= 1) {
    PSPWRITE.edit_filename[length - 1] = 0;
  }
}

int 
psp_edit_main_menu(void)
{
  gp2xCtrlData c;
  long        new_pad;
  long        old_pad;
  int         last_time;
  int         end_menu;

  int         danzeff_mode = 0;
  int         danzeff_key;

  int         step = 0;

  psp_kbd_wait_no_button();
  psp_sdl_select_font( 0 );

  old_pad   = 0;
  last_time = 0;
  end_menu  = 0;

  while (! end_menu)
  {
    psp_display_screen_menu();

    if (danzeff_mode) {
      danzeff_moveTo(-20, -20);
      danzeff_render( 0 );
    }
# if 0 //ZX: DEBUG
    {
      char buffer[128];
      extern int moved_x;
      extern int moved_y;
      sprintf(buffer, "mouse: %04x (%04d, %04d) (%04d, %04d)", 
           c.Mouse, c.Mx, c.My, moved_x, moved_y);
      psp_sdl_back_print(0,  0, buffer, GP2X_MENU_WARNING_COLOR);
    }
# endif
    psp_sdl_flip();

    while (1) {

      gp2xCtrlPeekBufferPositive(&c, 1);
      new_pad = c.Buttons;
# if defined(WIZ_MODE)
      if (c.Mouse) break;
# endif

      if ((old_pad != new_pad) || ((c.TimeStamp - last_time) > GP2X_MENU_MIN_TIME)) {
        last_time = c.TimeStamp;
        old_pad = new_pad;
        break;
      }
    }

# if defined(WIZ_MODE)
    int chk_menu_id = -1;
    for (chk_menu_id = 0; chk_menu_id < MAX_MENU_ITEM; chk_menu_id++) {
      int chk_area = psp_menu_is_mouse_area( &menu_list[chk_menu_id], &c);
      if (chk_area) {
        cur_menu_id = chk_menu_id;
        if (c.Mouse & GP2X_MOUSE_CLICK) {
# if 0
          if (chk_area < 0) new_pad |= GP2X_CTRL_LEFT;
          else
          if (chk_area > 0) new_pad |= GP2X_CTRL_RIGHT;
# else
          new_pad |= GP2X_CTRL_CROSS;
# endif
        }
        break;
      }
    }
    if (cur_menu_id != chk_menu_id)
# endif

    if (danzeff_mode) {

      if (c.Mouse & GP2X_MOUSE_BTN_RELEASE) {
        danzeff_key = danzeff_readMouse(&c);
      } else {
        danzeff_key = danzeff_readInput(&c);
      }

      if (danzeff_key > DANZEFF_START) {
        /* Disable utf8 */
        danzeff_key &= 0x7f;
        if (danzeff_key > ' ') {
          psp_edit_menu_filename_add_char(danzeff_key);
        } else
        if (danzeff_key == DANZEFF_CLEAR) {
          psp_edit_menu_filename_clear();
        } else
        if (danzeff_key == DANZEFF_DEL) {
          psp_edit_menu_filename_del();
        }

      } else 
      if ((danzeff_key == DANZEFF_START ) || 
          (danzeff_key == DANZEFF_SELECT)) 
      {
        danzeff_mode = 0;
        old_pad = new_pad = 0;

        psp_kbd_wait_no_button();
      }
      continue;
    }

    if ((new_pad & GP2X_CTRL_CROSS ) || 
        (new_pad & GP2X_CTRL_CIRCLE))
    {
      if (new_pad & (GP2X_CTRL_RIGHT|GP2X_CTRL_CROSS)) {
        step = 1;
      } else
      if (new_pad & (GP2X_CTRL_LEFT|GP2X_CTRL_CIRCLE)) {
        step = -1;
      }

      switch (cur_menu_id ) 
      {
        case MENU_LOAD      : if (psp_edit_menu_load()) {
                                end_menu = 1;
                              }
        break;              

        case MENU_NEW       : psp_edit_menu_new();
                              end_menu = 1;
        break;              

        case MENU_SAVE      : psp_edit_menu_save();
        break;              

        case MENU_SETTINGS   : psp_settings_menu();
                               old_pad = new_pad = 0;
        break;

        case MENU_BACK      : end_menu = 1;
        break;

        case MENU_EXIT      : psp_edit_menu_exit();
        break;

        case MENU_HELP      : psp_help_menu();
                              old_pad = new_pad = 0;
        break;              

        case MENU_FILENAME : if (step < 0) psp_edit_menu_filename_del();
        break;
      }

    } else
    if(new_pad & GP2X_CTRL_START) {
      danzeff_mode = 1;
    } else
    if(new_pad & GP2X_CTRL_UP) {

      if (cur_menu_id > 0) cur_menu_id--;
      else                 cur_menu_id = MAX_MENU_ITEM-1;

    } else
    if(new_pad & GP2X_CTRL_DOWN) {

      if (cur_menu_id < (MAX_MENU_ITEM-1)) cur_menu_id++;
      else                                 cur_menu_id = 0;

    } else  
    if(new_pad & GP2X_CTRL_SQUARE) {
      /* Cancel */
      end_menu = -1;
    } else 
    if(new_pad & GP2X_CTRL_SELECT) {
      /* Back to Console */
      end_menu = 1;
    } else
    if(new_pad & GP2X_CTRL_RTRIGGER) {
      /* TO_BE_DONE ! */
    } else
    if(new_pad & GP2X_CTRL_LTRIGGER) {
      /* TO_BE_DONE ! */
    }
  }

  psp_kbd_wait_no_button();

  return 0;
}

