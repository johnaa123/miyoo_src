/* 
 * Gmu Music Player
 *
 * Copyright (c) 2006-2010 Johannes Heimansberg (wejp.k.vu)
 *
 * File: hw_dingoo.c  Created: 090628
 *
 * Description: Dingoo specific stuff (button mapping etc.)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of
 * the License. See the file COPYING in the Gmu's main directory
 * for details.
 */
#include <stdio.h>
#include "hw_miyoo.h"
#include "oss_mixer.h"
#include "debug.h"

static int display_on_value = 100;
static int selected_mixer = -1;

#define MIYOO_LID_FILE  "/mnt/.backlight.conf"
#define MIYOO_LID_CONF  "/sys/devices/platform/backlight/backlight/backlight/brightness"

static int read_conf(const char *file)
{
  int i, fd;
  int val = 5;
  char buf[10]={0};
  
  fd = open(file, O_RDWR);
  if(fd < 0){
    val = -1;
  }
  else{
    read(fd, buf, sizeof(buf));
    for(i=0; i<strlen(buf); i++){
      if(buf[i] == '\r'){
        buf[i] = 0;
      }
      if(buf[i] == '\n'){
        buf[i] = 0;
      }
      if(buf[i] == ' '){
        buf[i] = 0;
      }
    }
    val = atoi(buf);
  }
  close(fd);
  return val;
}

int hw_open_mixer(int mixer_channel)
{
	int res = oss_mixer_open();
	selected_mixer = mixer_channel;
	wdprintf(V_INFO, "hw_miyoo", "Selected mixer: %d\n", selected_mixer);
	return res;
}

void hw_close_mixer(void)
{
	oss_mixer_close();
}

void hw_set_volume(int volume)
{
	if (selected_mixer >= 0) {
		if (volume >= 0) {
      oss_mixer_set_volume(selected_mixer, volume);
    }
	} 
  else {
		wdprintf(V_INFO, "hw_miyoo", "No suitable mixer available.\n");
	}
}

void hw_display_off(void)
{
	char  tmp[5];
	FILE *f;
	int   display_on_value_tmp = 0;

	wdprintf(V_DEBUG, "hw_miyoo", "Display off requested.\n");
  if ((f = fopen(MIYOO_LID_CONF, "w"))) {
    fprintf(f, "0\n");
    fclose(f);
  }
  display_on_value = -1;
}

void hw_display_on(void)
{
  int val;
	FILE *f;
  char buf[32]={0};

  val = read_conf(MIYOO_LID_FILE);
	wdprintf(V_DEBUG, "hw_miyoo", "Display on requested.\n");
  if ((f = fopen(MIYOO_LID_CONF, "w"))) {
    sprintf(buf, "%d\n", val);
	  fprintf(f, buf);
		fclose(f);
	}
}

void hw_detect_device_model(void)
{
}

const char *hw_get_device_model_name(void)
{
	return "Miyoo";
}
