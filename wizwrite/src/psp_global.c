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

#include <stdio.h>
#include <zlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "SDL.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "psp_global.h"
#include "psp_editor.h"
#include "psp_sdl.h"

  PSPWRITE_t PSPWRITE;

void
psp_global_default()
{
  PSPWRITE.dos_mode   = 1;
  PSPWRITE.move_on_text = 1;
  PSPWRITE.expand_tab = 0;
  PSPWRITE.tab_stop = 4;
  PSPWRITE.psp_cpu_clock = GP2X_DEF_EMU_CLOCK;
  PSPWRITE.transparent = 1;

  PSPWRITE.fg_color = COLOR_BLACK;
  PSPWRITE.bg_color = COLOR_IMAGE;

  PSPWRITE.psp_font_size = 0;
  PSPWRITE.screen_w  = GP2X_SDL_SCREEN_WIDTH  / psp_all_fonts[ 0 ].width;
  PSPWRITE.screen_h  = GP2X_SDL_SCREEN_HEIGHT / psp_all_fonts[ 0 ].height;
  PSPWRITE.wrap_w    = PSPWRITE.screen_w;
  PSPWRITE.wrap_mode = 1;

  strcpy(PSPWRITE.psp_txtdir, PSPWRITE.psp_homedir);
  strcat(PSPWRITE.psp_txtdir, PSP_DEFAULT_TXT_DIR);
}

void
psp_global_init()
{
  memset(&PSPWRITE, 0, sizeof(PSPWRITE));
  getcwd(PSPWRITE.psp_homedir, sizeof(PSPWRITE.psp_homedir));
  PSPWRITE.is_modified = 0;

  psp_global_default();

  psp_global_load_config();

  gp2xPowerSetClockFrequency(PSPWRITE.psp_cpu_clock);
}

int
psp_global_save_config()
{
  char  FileName[MAX_PATH+1];
  FILE* FileDesc;
  int   error;

  snprintf(FileName, MAX_PATH, "%s/pspwrite.cfg", PSPWRITE.psp_homedir);
  error = 0;
  FileDesc = fopen(FileName, "w");
  if (FileDesc != (FILE *)0 ) {
    fprintf(FileDesc, "dos_mode=%d\n", PSPWRITE.dos_mode);
    fprintf(FileDesc, "expand_tab=%d\n", PSPWRITE.expand_tab);
    fprintf(FileDesc, "fg_color=%d\n", PSPWRITE.fg_color);
    fprintf(FileDesc, "screen_w=%d\n", PSPWRITE.screen_w);
    fprintf(FileDesc, "screen_h=%d\n", PSPWRITE.screen_h);
    fprintf(FileDesc, "wrap_mode=%d\n", PSPWRITE.wrap_mode);
    fprintf(FileDesc, "wrap_w=%d\n", PSPWRITE.wrap_w);
    fprintf(FileDesc, "bg_color=%d\n", PSPWRITE.bg_color);
    fprintf(FileDesc, "tab_stop=%d\n", PSPWRITE.tab_stop);
    fprintf(FileDesc, "psp_font_size=%d\n", PSPWRITE.psp_font_size);
    fprintf(FileDesc, "psp_cpu_clock=%d\n", PSPWRITE.psp_cpu_clock);
    fprintf(FileDesc, "transparent=%d\n", PSPWRITE.transparent);
    fprintf(FileDesc, "move_on_text=%d\n", PSPWRITE.move_on_text);
    fprintf(FileDesc, "psp_txtdir=%s\n", PSPWRITE.psp_txtdir);
    fclose(FileDesc);

  } else {
    error = 1;
  }

  return error;
}

unsigned char
psp_convert_utf8_to_iso_8859_1(unsigned char c1, unsigned char c2)
{
  unsigned char res = 0;
  if (c1 == 0xc2) res = c2;
  else
  if (c1 == 0xc3) res = c2 | 0x40;
  return res;
}


int
psp_global_load_config()
{
  char  Buffer[512];
  char  FileName[MAX_PATH+1];
  char *Scan;
  FILE* FileDesc;
  int   error;
  unsigned int Value;

  snprintf(FileName, MAX_PATH, "%s/pspwrite.cfg", PSPWRITE.psp_homedir);
  error = 0;
  FileDesc = fopen(FileName, "r");
  if (FileDesc == (FILE *)0 ) return 1;

  while (fgets(Buffer,512, FileDesc) != (char *)0) {

    Scan = strchr(Buffer,'\n');
    if (Scan) *Scan = '\0';
    /* For this #@$% of windows ! */
    Scan = strchr(Buffer,'\r');
    if (Scan) *Scan = '\0';
    if (Buffer[0] == '#') continue;

    Scan = strchr(Buffer,'=');
    if (! Scan) continue;

    *Scan = '\0';
    Value = atoi(Scan+1);
    if (!strcasecmp(Buffer,"dos_mode")) PSPWRITE.dos_mode = Value;
    else
    if (!strcasecmp(Buffer,"expand_tab")) PSPWRITE.expand_tab = Value;
    else
    if (!strcasecmp(Buffer,"fg_color")) PSPWRITE.fg_color = Value;
    else
    if (!strcasecmp(Buffer,"screen_w")) PSPWRITE.screen_w = Value;
    else
    if (!strcasecmp(Buffer,"screen_h")) PSPWRITE.screen_h = Value;
    else
    if (!strcasecmp(Buffer,"wrap_w")) PSPWRITE.wrap_w = Value;
    else
    if (!strcasecmp(Buffer,"wrap_mode")) PSPWRITE.wrap_mode = Value;
    else
    if (!strcasecmp(Buffer,"bg_color")) PSPWRITE.bg_color = Value;
    else
    if (!strcasecmp(Buffer,"tab_stop")) PSPWRITE.tab_stop = Value;
    else
    if (!strcasecmp(Buffer,"move_on_text")) PSPWRITE.move_on_text = Value;
    else
    if (!strcasecmp(Buffer,"psp_cpu_clock")) PSPWRITE.psp_cpu_clock = Value;
    else
    if (!strcasecmp(Buffer,"transparent")) PSPWRITE.transparent = Value;
    else
    if (!strcasecmp(Buffer,"psp_font_size")) PSPWRITE.psp_font_size = Value;
    else
    if (!strcasecmp(Buffer, "psp_txtdir")) {
      strcpy(PSPWRITE.psp_txtdir, Scan+1);
      struct stat       aStat;
      if (stat(PSPWRITE.psp_txtdir, &aStat)) {
        strcpy(PSPWRITE.psp_txtdir, PSPWRITE.psp_homedir);
        strcat(PSPWRITE.psp_txtdir, PSP_DEFAULT_TXT_DIR);
      }
    }
  }

  fclose(FileDesc);

  return 0;
}


