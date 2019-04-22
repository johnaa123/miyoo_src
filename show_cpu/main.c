#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "font.h"

SDL_Surface *screen=NULL;
char path[255]={0};
char cmd[255]={0};
char content[255]={0};

void pstr(const char *str, uint16_t fg_color, uint16_t bg_color, uint32_t x, uint32_t y, void *_dest_ptr, uint32_t pitch, uint32_t pad, uint32_t h_offset, uint32_t height)
{
  uint16_t *dest_ptr = (uint16_t *)_dest_ptr + (y * pitch) + x;
  uint8_t current_char = str[0];
  uint32_t current_row;
  uint32_t glyph_offset;
  uint32_t i = 0, i2, i3, h;
  uint32_t str_index = 1;
  uint32_t current_x = x;

  if(y + height > 480)
    return;

  while(current_char)
  {
    if(current_char == '\n')
    {
      y += FONT_HEIGHT;
      current_x = x;
      dest_ptr = screen->pixels + (y * pitch) + x;
    }
    else
    {
      glyph_offset = _font_offset[current_char];
      current_x += FONT_WIDTH;
      glyph_offset += h_offset;
      for(i2 = h_offset, h = 0; i2 < FONT_HEIGHT && h < height; i2++, h++, glyph_offset++)
      {
        current_row = _font_bits[glyph_offset];
        for(i3 = 0; i3 < FONT_WIDTH; i3++)
        {
          if((current_row >> (15 - i3)) & 0x01)
            *dest_ptr = fg_color;
          else
            *dest_ptr = bg_color;
          dest_ptr++;
        }
        dest_ptr += (pitch - FONT_WIDTH);
      }
      dest_ptr = dest_ptr - (pitch * h) + FONT_WIDTH;
    }

    i++;

    current_char = str[str_index];

    if((i < pad) && (current_char == 0))
    {
      current_char = ' ';
    }
    else
    {
      str_index++;
    }

    if(current_x + FONT_WIDTH > 320 /* EDIT */)
    {
      while (current_char && current_char != '\n')
      {
        current_char = str[str_index++];
      }
    }
  }
}

int main(int argc, char* argv[])
{
  int loop=1;
  SDL_Event event;

  getcwd(path, sizeof(path));
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("%s, failed to SDL_Init\n", __func__);
    return -1;
  }
  SDL_ShowCursor(0);
 
  screen = SDL_SetVideoMode(320, 480, 16, SDL_HWSURFACE);
  SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0xc0));

  SDL_Surface* bmp = SDL_LoadBMP("bg.bmp");
  SDL_BlitSurface(bmp, NULL, screen, NULL);
  SDL_FreeSurface(bmp);

  sprintf(cmd, "cat /proc/cpuinfo > %s/cpu.txt", path);
  system(cmd);
  sprintf(cmd, "dmesg > %s/dmesg.txt", path);
  system(cmd);

  int x;
  SDL_Rect rt;
  SDL_Color color = {0, 0, 0};
	FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  sprintf(cmd, "%s/cpu.txt", path);
	fp = fopen(cmd, "r");
  for(x=0; x<16; x++){
		if((read = getline(&line, &len, fp)) != -1) {
      int i, j=strlen(line);
      for(i=0; i<j; i++){
        if(line[i] == '\r'){
          line[i] = 0;
        }
        if(line[i] == '\n'){
          line[i] = 0;
        }
        if(line[i] == '\t'){
          line[i] = ' ';
        }
      }
      
			sprintf(cmd, "%s", line);
      pstr(cmd, 0x0000, 0xffe0, 10, 250 + (x * 14), screen->pixels, (screen->pitch / 2), 0, 0, FONT_HEIGHT);
		}
  }
	fclose(fp);
  if(line){
  	free(line);
	}

	SDL_Flip(screen);
	while(loop){
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_KEYDOWN){
        loop = 0;
        break;
      }
    }
  }
 
  SDL_Quit();
  return 0;    
}

