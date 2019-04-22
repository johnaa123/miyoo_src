/*
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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/fb.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_gfxPrimitives_font.h>

#define MIYOO_FB0_SET_FPBP    _IOWR(0x104, 0, unsigned long)
#define MIYOO_FB0_GET_FPBP    _IOWR(0x105, 0, unsigned long)

#define MIYOO_FBP_FILE        "/mnt/.fpbp.conf"

static SDL_Surface *sdl_screen;
static SDL_Surface *real_screen;
static SDL_Event event;
static int item=0;
static int new_bp=8;
static int new_fp=8;
static int def_bp=8;
static int def_fp=8;
static int test_bp=8;
static int test_fp=8;

static void write_conf(const char *file, int val)
{
  int fd;
  char buf[10]={0};
  
  fd = open(file, O_WRONLY | O_CREAT | O_TRUNC);
  if(fd > 0){
    sprintf(buf, "%d", val);
    write(fd, buf, strlen(buf));
    close(fd);
  }
}

static void update_setting(int dev, int val)
{
  SDL_FillRect(sdl_screen, &sdl_screen->clip_rect, SDL_MapRGB(sdl_screen->format, 0x00, 0x00, 0x00));
  SDL_BlitSurface(sdl_screen, NULL, real_screen, NULL);
  SDL_Flip(real_screen);
  ioctl(dev, MIYOO_FB0_SET_FPBP, val);
}

static void setting_show(void)
{
  char buf[255]={0};
  int pos = item*10+5;

  boxRGBA(sdl_screen, 0, 0, 75, 61, 0xff, 0x00, 0x00, 0xff);
  boxRGBA(sdl_screen, 1, 1, 74, 60, 0xff, 0xff, 0x80, 0xff); 
  sprintf(buf, "BP: %d", test_bp);
  stringRGBA(sdl_screen, 15, 5,  buf, 0x00, 0x00, 0x00, 0xff);
  sprintf(buf, "FP: %d", test_fp);
  stringRGBA(sdl_screen, 15, 15, buf, 0x00, 0x00, 0x00, 0xff);
  stringRGBA(sdl_screen, 15, 30, "Exit", 0x00, 0x00, 0x00, 0xff);
  stringRGBA(sdl_screen, 15, 40, "Apply", 0x00, 0x00, 0x00, 0xff);
  stringRGBA(sdl_screen, 15, 50, "Default", 0x00, 0x00, 0x00, 0xff);
  
  if(item >= 2){
    pos+= 5;
  }
  boxRGBA(sdl_screen, 5, pos, 10, pos+5, 0xff, 0x00, 0x00, 0xff);
}

int main(int argc, char* args[])
{
  int dev=-1;
  int flip_idx=0;
  int main_loop=1;
  unsigned long val;

  dev = open("/dev/miyoo_fb0", O_RDWR);
  if(dev < 0){
    printf("%s, failed to open miyoo_fb0\n", __func__);
    return -1;
  }
  ioctl(dev, MIYOO_FB0_GET_FPBP, &val);
  def_fp = (val & 0xf000) >> 12;
  def_bp = (val & 0x0f00) >> 8;
  test_fp = new_fp = (val & 0x00f0) >> 4;
  test_bp = new_bp = (val & 0x000f) >> 0;
  
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("%s, failed to SDL_Init\n", __func__);
    return -1;
  }
 
  real_screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if(real_screen == NULL){
    printf("%s, failed to SDL_SetVideMode\n", __func__);
    return -1;
  }
  sdl_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 16, 0, 0, 0, 0);
  if(sdl_screen == NULL){
    printf("%s, failed to create screen\n", __func__);
    return -1;
  }
  SDL_ShowCursor(0);

  main_loop = 1;
  while(main_loop){
    if(SDL_PollEvent(&event)){
      switch(event.type){
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym){
        case SDLK_RCTRL:
          main_loop = 0;
          break;
        case SDLK_UP:
          item-= 1;
          item%= 5;
          if(item < 0){
            item = 4;
          }
          break;
        case SDLK_DOWN:
          item+= 1;
          item%= 5;
          break;
        case SDLK_LEFT:
          if(item == 0){
            if(test_bp > 2){
              test_bp-= 1;
              update_setting(dev, (test_fp << 4) | test_bp);
            }
          }
          else if(item == 1){
            if(test_fp > 2){
              test_fp-= 1;
              update_setting(dev, (test_fp << 4) | test_bp);
            }
          }
          break;
        case SDLK_RIGHT:
          if(item == 0){
            if(test_bp < 15){
              test_bp+= 1;
              update_setting(dev, (test_fp << 4) | test_bp);
            }
          }
          else if(item == 1){
            if(test_fp < 15){
              test_fp+= 1;
              update_setting(dev, (test_fp << 4) | test_bp);
            }
          }
          break;
        case SDLK_LALT:
          switch(item){
          case 2:
            main_loop = 0;
            break;
          case 3:
            new_fp = test_fp;
            new_bp = test_bp;
            update_setting(dev, (new_fp << 4) | new_bp);
            write_conf(MIYOO_FBP_FILE, (new_fp << 4) | new_bp);
            break;
          case 4:
            test_fp = new_fp = def_fp;
            test_bp = new_bp = def_bp;
            update_setting(dev, (new_fp << 4) | new_bp);
            break;
          }
          break;
        }
      }
    }
    if(main_loop == 0){
      update_setting(dev, (new_fp << 4) | new_bp);
      break;
    }
    switch(flip_idx){
    case 0:
      SDL_FillRect(sdl_screen, &sdl_screen->clip_rect, SDL_MapRGB(sdl_screen->format, 0xff, 0x00, 0x00));
      break;
    case 1:
      SDL_FillRect(sdl_screen, &sdl_screen->clip_rect, SDL_MapRGB(sdl_screen->format, 0x00, 0xff, 0x00));
      break;
    case 2:
      SDL_FillRect(sdl_screen, &sdl_screen->clip_rect, SDL_MapRGB(sdl_screen->format, 0x00, 0x00, 0xff));
      break;
    }
    flip_idx+= 1;
    if(flip_idx >= 3){
      flip_idx = 0;
    }

    setting_show();
    SDL_BlitSurface(sdl_screen, NULL, real_screen, NULL);
    SDL_Flip(real_screen);
    SDL_Delay(60);
  }
  SDL_FreeSurface(sdl_screen);
	SDL_Quit();
  close(dev);
  return 0;
}

