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
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_gfxPrimitives_font.h>

#include "nc1020.h"

#define MAX_BORDER  4
#define CONFIG_PATH	"wqx.cfg"

#define WQX_UP      0x1a
#define WQX_DOWN    0x1b
#define WQX_LEFT    0x3f
#define WQX_RIGHT   0x1f
#define WQX_ENTER   0x1d
#define WQX_ESC     0x3b
#define WQX_F1      0x10
#define WQX_F2      0x11
#define WQX_F3      0x12
#define WQX_F4      0x13
#define WQX_F5      0x0b
#define WQX_F6      0x0c
#define WQX_F7      0x0d
#define WQX_F8      0x0a
#define WQX_F9      0x09
#define WQX_F10     0x08
#define WQX_F11     0x0e
#define WQX_PGUP    0x37
#define WQX_PGDW    0x1e
#define WQX_HELP    0x38
#define WQX_Q       0x20
#define WQX_W       0x21
#define WQX_E       0x22
#define WQX_R       0x23
#define WQX_T       0x24
#define WQX_7       0x24
#define WQX_Y       0x25
#define WQX_8				0x25
#define WQX_U       0x26
#define WQX_9				0x26
#define WQX_I       0x27
#define WQX_O				0x18
#define WQX_P				0x1c
#define WQX_A				0x28
#define WQX_S				0x29
#define WQX_D				0x2a
#define WQX_F				0x2b
#define WQX_G				0x2c
#define WQX_4				0x2c
#define WQX_H				0x2d
#define WQX_5				0x2d
#define WQX_J				0x2e
#define WQX_6				0x2e
#define WQX_K				0x2f
#define WQX_L				0x19
#define WQX_Z				0x30
#define WQX_X				0x31
#define WQX_C				0x32
#define WQX_V				0x33
#define WQX_B				0x34
#define WQX_1				0x34
#define WQX_N				0x35
#define WQX_2				0x35
#define WQX_M				0x36
#define WQX_3				0x36
#define WQX_SHIFT   0x39
#define WQX_CAPS    0x3a
#define WQX_0       0x3c
#define WQX_DOT     0x3d
#define WQX_EQU     0x3e
#define WQX_POWER   0x0f

#define MY_UP       WQX_UP
#define MY_DOWN     WQX_DOWN
#define MY_LEFT     WQX_LEFT
#define MY_RIGHT    WQX_RIGHT
#define MY_LCTRL    WQX_ESC
#define MY_LALT     WQX_ENTER
#define MY_SPACE    WQX_A
#define MY_LSHIFT   WQX_B
#define MY_RETURN   WQX_F11
#define MY_ESCAPE   WQX_F9
#define MY_L        WQX_PGUP
#define MY_R        WQX_PGDW
#define MY_MENU     WQX_ESC

static int FRAME_RATE=60;
static int FRAME_INTERVAL=(1000/FRAME_RATE);
static int mSpeedUp=0;
static uint8_t ncscreen[1600]={0};
static int show_menu=0;
static int main_loop=1;
static int menu_index=0;
static SDL_Surface *sdl_screen=NULL;
static SDL_Surface *real_screen=NULL;
static SDL_Surface *keypad=NULL;
static SDL_Surface *border[MAX_BORDER]={0};
static int key_col=0, key_row=1;
static int key_changed=1;
static int background=1;
static int foreground=2;

const uint16_t bkc[] = {
  0xc618, 0x958b, 0xe800, 0x04e0, 0xeee0, 0x0610, 0xfc00, 0xffff
};

const uint16_t fgc [] ={
  0x0000, 0x0208, 0x0010
};

const char *kbs[] = {
  "POW", "F3", "F4", "DIC", "GAM", "CAL", "SCH", "APP", "TIM", "NET",
  "Q",  "W",  "E",  "R",  "T7", "Y8", "U9", "I",  "O",  "P", 
  "A",  "S",  "D",  "F",  "G4", "H5", "J6", "K",  "L",  "En", 
  "Z",  "X",  "C",  "V",  "B1", "N2", "M3", "Pu", "Up", "Pd", 
  "Hp", "Sh", "Cp", "Es", "0",  ".",  " =", "Lf", "Dw", "Rg", 
};

const char *kbfs[] = {
  "WQX_POWER",  "WQX_F3",     "WQX_F4",   "WQX_F5",   "WQX_F9",    "WQX_F7",   "WQX_F8",   "WQX_F11",  "WQX_F10",  "WQX_F6",
  "WQX_Q",      "WQX_W",      "WQX_E",    "WQX_R",    "WQX_T(7)",  "WQX_Y(8)", "WQX_U(9)", "WQX_I",    "WQX_O",    "WQX_P", 
  "WQX_A",      "WQX_S",      "WQX_D",    "WQX_F",    "WQX_G(4)",  "WQX_H(5)", "WQX_J(6)", "WQX_K",    "WQX_L",    "WQX_ENTER", 
  "WQX_Z",      "WQX_X",      "WQX_C",    "WQX_V",    "WQX_B(1)",  "WQX_N(2)", "WQX_M(3)", "WQX_PGUP", "WQX_UP",   "WQX_PGDW", 
  "WQX_HELP",   "WQX_SHIFT",  "WQX_CAPS", "WQX_ESC",  "WQX_0",     "WQX_.",    "WQX_=",    "WQX_LEFT", "WQX_DOWN", "WQX_RIGHT",
};

const uint32_t kbk[] = {
  WQX_POWER,  WQX_F3,     WQX_F4,   WQX_F5,   WQX_F9, WQX_F7,   WQX_F8,   WQX_F11,  WQX_F10,  WQX_F6,
  WQX_Q,      WQX_W,      WQX_E,    WQX_R,    WQX_T,  WQX_Y,    WQX_U,    WQX_I,    WQX_O,    WQX_P, 
  WQX_A,      WQX_S,      WQX_D,    WQX_F,    WQX_G,  WQX_H,    WQX_J,    WQX_K,    WQX_L,    WQX_ENTER, 
  WQX_Z,      WQX_X,      WQX_C,    WQX_V,    WQX_B,  WQX_N,    WQX_M,    WQX_PGUP, WQX_UP,   WQX_PGDW, 
  WQX_HELP,   WQX_SHIFT,  WQX_CAPS, WQX_ESC,  WQX_0,  WQX_DOT,  WQX_EQU,  WQX_LEFT, WQX_DOWN, WQX_RIGHT, 
};

static int keymap[12]={
  38, 48, 47, 49, 43, 39, 29, 37, 8, 7, 1, 2
};

uint32_t kbb[] = {
  0xca7f45, 0xb3a4bb, 0xb3a4bb, 0xb3a4bb, 0xb3a4bb, 0xb3a4bb, 0xb3a4bb, 0xb3a4bb, 0xb3a4bb, 0xb3a4bb,
  0xd6d9d0, 0xd6d9d0, 0xd6d9d0, 0xd6d9d0, 0x3e5895, 0x3e5895, 0x3e5895, 0xd6d9d0, 0xd6d9d0, 0xd6d9d0,
  0xd6d9d0, 0xd6d9d0, 0xd6d9d0, 0xd6d9d0, 0x3e5895, 0x3e5895, 0x3e5895, 0xd6d9d0, 0xd6d9d0, 0xca7f45,
  0xd6d9d0, 0xd6d9d0, 0xd6d9d0, 0xd6d9d0, 0x3e5895, 0x3e5895, 0x3e5895, 0xd6d9d0, 0xd6d9d0, 0xd6d9d0,
  0xd6d9d0, 0xd6d9d0, 0xd6d9d0, 0xd6d9d0, 0x3e5895, 0x3e5895, 0x3e5895, 0xd6d9d0, 0xd6d9d0, 0xd6d9d0,
};

uint32_t kbf[] = {
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0xeeeeee, 0xeeeeee, 0xeeeeee, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0xeeeeee, 0xeeeeee, 0xeeeeee, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0xeeeeee, 0xeeeeee, 0xeeeeee, 0x000000, 0x000000, 0x000000,
  0x8000ff, 0x000000, 0x000000, 0x000000, 0xeeeeee, 0xeeeeee, 0xeeeeee, 0x000000, 0x000000, 0x000000,
};

void load_cfg(void)
{
	int x=0, len=0;
	FILE *fp=NULL;
	uint32_t buf[255]={0};

	fp = fopen(CONFIG_PATH, "rb");
	if(fp){
		fread(buf, sizeof(buf), 1, fp);
		fclose(fp);
		
		background = buf[0];
		foreground = buf[1];
		len = sizeof(keymap)/sizeof(keymap[0]);
		for(x=0; x<len; x++){
			keymap[x] = buf[x+2];
		}
	}
}

void write_cfg(void)
{
	int x=0, len=0;
	FILE *fp=NULL;
	uint32_t buf[255]={0};

	fp = fopen(CONFIG_PATH, "wb");
	if(fp){
		buf[0] = background;
		buf[1] = foreground;
		len = sizeof(keymap)/sizeof(keymap[0]);
		for(x=0; x<len; x++){
			buf[x+2] = keymap[x];
		}
		fwrite(buf, sizeof(buf), 1, fp);
		fclose(fp);
	}
}

int get_kbfs_index(uint32_t val)
{
  int x, len=(int)sizeof(kbk)/sizeof(kbk[0]);

  for(x=0; x<len; x++){
    if(kbk[x] == val){
      return x;
    }
  }
  return 0;
}

void setting_show(void)
{
  int loop=1, x=0;
  static int item=0;
  char buf[255]={0};
  SDL_Event event;
  SDL_Rect s={0}, d={0};
  SDL_Surface* p = SDL_CreateRGBSurface(SDL_SWSURFACE, 220, 180, 16, 0, 0, 0, 0);

  SDL_BlitSurface(keypad, NULL, sdl_screen, NULL);
  while(loop){
    if(SDL_PollEvent(&event)){
      switch(event.type){
      case SDL_KEYUP:
        switch(event.key.keysym.sym){
        case SDLK_F10:
        case SDLK_RCTRL:
        case SDLK_END:
          loop = 0;
          show_menu = 1;
          break;
        case SDLK_UP:
          item-= 1;
          item%= 14;
          if(item < 0){
            item = 13;
          }
          break;
        case SDLK_DOWN:
          item+= 1;
          item%= 14;
          break;
        case SDLK_LEFT:
          switch(item){
          case 0:
            background-= 1;
            background%= (int)(sizeof(bkc)/sizeof(bkc[0]));
            break;
          case 1:
            foreground-= 1;
            foreground%= (int)(sizeof(fgc)/sizeof(fgc[0]));
            break;
          default:
            keymap[item-2]-= 1;
            keymap[item-2]%= (int)(sizeof(kbk)/sizeof(kbk[0]));
            if(keymap[item-2] < 0){
              keymap[item-2] = ((sizeof(kbk)/sizeof(kbk[0])) - 1);
            }
            break;
          }
          break;
        case SDLK_RIGHT:
          switch(item){
          case 0:
            background+= 1;
            background%= (int)(sizeof(bkc)/sizeof(bkc[0]));
            break;
          case 1:
            foreground+= 1;
            foreground%= (int)(sizeof(fgc)/sizeof(fgc[0]));
            break;
          default:
            keymap[item-2]+= 1;
            keymap[item-2]%= (int)sizeof(kbk)/sizeof(kbk[0]);
            break;
          }
          break;
        case SDLK_LCTRL:
          wqx::SetKey(MY_LCTRL, 0);
          break;
        }
        break;
      }
    }

    boxRGBA(p, 0, 0, 220, 180, 0xff, 0x00, 0x00, 0xff);
    boxRGBA(p, 1, 1, 218, 178, 0xff, 0xff, 0x80, 0xff); 
    stringRGBA(p, 0, 10,  " === Settings ===", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 30,  "  Background   : ", 0x00, 0x00, 0x00, 0xff);
    boxRGBA(p, 135, 30, 160, 38, 0x00, 0x00, 0x00, 0xff);
    boxRGBA(p, 136, 31, 159, 37, (bkc[background] & 0xf800) >> 8, (bkc[background] & 0x07e0) >> 3, (bkc[background] & 0x001f) << 3, 0xff);

    stringRGBA(p, 0, 40,  "  Foreground   : ", 0x00, 0x00, 0x00, 0xff);
    boxRGBA(p, 135, 40, 160, 48, 0x00, 0x00, 0x00, 0xff);
    boxRGBA(p, 136, 41, 159, 47, (fgc[foreground] & 0xf800) >> 8, (fgc[foreground] & 0x07e0) >> 3, (fgc[foreground] & 0x001f) << 3, 0xff);
    
    stringRGBA(p, 0, 50,  "  Miyoo_Up     :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 60,  "  Miyoo_Down   :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 70,  "  Miyoo_Left   :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 80,  "  Miyoo_Right  :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 90,  "  Miyoo_A      :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 100, "  Miyoo_B      :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 110, "  Miyoo_TA     :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 120, "  Miyoo_TB     :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 130, "  Miyoo_Start  :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 140, "  Miyoo_Select :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 150, "  Miyoo_L      :", 0x00, 0x00, 0x00, 0xff);
    stringRGBA(p, 0, 160, "  Miyoo_R      :", 0x00, 0x00, 0x00, 0xff);
    boxRGBA(p, 5, (10*item)+30, 10, (10*item)+37, 0xff, 0x00, 0x00, 0xff);

    for(x=0; x<12; x++){
      stringRGBA(p, 135, (10*x)+50,  kbfs[get_kbfs_index(kbk[keymap[x]])], 0x00, 0x00, 0x00, 0xff);    
    }

    s.x = 0;
    s.y = 0;
    s.w = 220;
    s.h = 180;
    d.x = 50;
    d.y = 30;
    d.w = 220;
    d.h = 180;
    SDL_SoftStretch(p, &s, sdl_screen, &d);
    SDL_BlitSurface(sdl_screen, NULL, real_screen, NULL);
    SDL_Flip(real_screen);
    SDL_Delay(50);
  }
  SDL_FreeSurface(p);
	write_cfg();
}

void vkb_show(SDL_Surface *surface, int select)
{
  int x=0, y=0, sindex=0;

  SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
  for(x=0; x<10; x++){
    SDL_Rect d={0};
    SDL_Surface* p = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 14, 16, 0, 0, 0, 0);
	  
    boxRGBA(p, 0, 0, 31, 13, 0x00, 0x00, 0x00, 0xff);
    if(select == sindex){
      boxRGBA(p, 1, 1, 30, 12, 0xff, 0x00, 0x00, 0xff); 
      stringRGBA(p, 4, 4, kbs[sindex], 0xff, 0xff, 0xff, 0xff);
    }
    else{
      boxRGBA(p, 1, 1, 30, 12, kbb[sindex] >> 16, kbb[sindex] >> 8, kbb[sindex], 0xff); 
      stringRGBA(p, 4, 4, kbs[sindex], kbf[sindex] >> 16, kbf[sindex] >> 8, kbf[sindex], 0xff);
    }
    sindex+= 1;
    d.x = (32*x);
    d.y = 1;
    d.w = 32;
    d.h = 28;
    SDL_SoftStretch(p, NULL, surface, &d);
    SDL_FreeSurface(p);
  }

  for(y=0; y<4; y++){
    for(x=0; x<10; x++){
      SDL_Rect d={0};
      SDL_Surface* p = SDL_CreateRGBSurface(SDL_SWSURFACE, 22, 12, 16, 0, 0, 0, 0);
      
      boxRGBA(p, 0, 0, 21, 11, 0x00, 0x00, 0x00, 0xff);
      if(select == sindex){
        boxRGBA(p, 1, 1, 38, 12, 0xff, 0x00, 0x00, 0xff); 
        stringRGBA(p, 3, 3, kbs[sindex], 0xff, 0xff, 0xff, 0xff);
      }
      else{
        boxRGBA(p, 1, 1, 20, 10, kbb[sindex] >> 16, kbb[sindex] >> 8, kbb[sindex], 0xff); 
        stringRGBA(p, 3, 3, kbs[sindex], kbf[sindex] >> 16, kbf[sindex] >> 8, kbf[sindex], 0xff);
      }
      sindex+= 1;
   
      d.x = (32*x);
      d.y = 191+(12*y);
      d.w = 32;
      d.h = 12;
      SDL_SoftStretch(p, NULL, surface, &d);
      SDL_FreeSurface(p);
    }
  }
}

void redraw_menu(SDL_Surface *surface)
{
  menu_index+= 1;
  if(menu_index >= 6){
    menu_index = 0;
    key_changed = 1;
  }
  
  if((menu_index >= 1) && (menu_index <= 4)){
    SDL_BlitSurface(border[menu_index-1], NULL, surface, NULL);
  }
  else if(menu_index == 5){
    setting_show();
  }
}

int main(int argc, char** argv)
{
  int x=0;
  char buf[255]={0};
  SDL_Event event;

	load_cfg();
  SDL_Init(SDL_INIT_VIDEO);
  real_screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if(real_screen == NULL){
    printf("failed init real screen\n");
    exit(-1);
  }
  sdl_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 16, 0, 0, 0, 0);
  if(sdl_screen == NULL){
    printf("failed init sdl screen\n");
    exit(-1);
  }
 
  keypad = SDL_LoadBMP("borders/keypad.bmp");
  if(keypad == NULL){
    printf("failed load keypad image\n");
    exit(-1);
  }
  for(x=0; x<MAX_BORDER; x++){
    sprintf(buf, "borders/border_%d.bmp", x+1);
    border[x] = SDL_LoadBMP(buf);
    if(border[x] == NULL){
      printf("failed load border image\n");
      exit(-1);
    }
  }
  SDL_BlitSurface(border[menu_index], NULL, sdl_screen, NULL);
  SDL_BlitSurface(border[3], NULL, real_screen, NULL);
  SDL_Flip(real_screen);

  wqx::Initialize("roms");
  wqx::LoadNC1020();
  wqx::Reset();

  using clock = std::chrono::steady_clock;
  auto next_frame = clock::now();

  main_loop = 1;
  while(main_loop){
    next_frame+= std::chrono::milliseconds(FRAME_INTERVAL);
    if(SDL_PollEvent(&event)){
      switch(event.type){
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym){
        case SDLK_F10:
        case SDLK_RCTRL:
        case SDLK_END:
          break;
        case SDLK_UP:
          if(menu_index != 0){
            wqx::SetKey(kbk[keymap[0]], 1);
          }
          break;
        case SDLK_DOWN:
          if(menu_index != 0){
            wqx::SetKey(kbk[keymap[1]], 1);
          }
          break;
        case SDLK_LEFT:
          if(menu_index != 0){
            wqx::SetKey(kbk[keymap[2]], 1);
          }
          break;
        case SDLK_RIGHT:
          if(menu_index != 0){
            wqx::SetKey(kbk[keymap[3]], 1);
          }
          break;
        case SDLK_LCTRL:
          wqx::SetKey(kbk[keymap[5]], 1);
          break;
        case SDLK_LALT:
          if(menu_index == 0){
            wqx::SetKey(kbk[(key_row * 10) + key_col], 1);
          }
          else{
            wqx::SetKey(kbk[keymap[4]], 1);
          }
          break;
        case SDLK_SPACE:
          wqx::SetKey(kbk[keymap[6]], 1);
          break;
        case SDLK_LSHIFT:
          wqx::SetKey(kbk[keymap[7]], 1);
          break;
        case SDLK_RETURN:
          wqx::SetKey(kbk[keymap[8]], 1);
          break;
        case SDLK_ESCAPE:
          wqx::SetKey(kbk[keymap[9]], 1);
          break;
        case SDLK_BACKSPACE:
          wqx::SetKey(kbk[keymap[10]], 1);
          break;
        case SDLK_TAB:
          wqx::SetKey(kbk[keymap[11]], 1);
          break;
        }
        break;
      case SDL_KEYUP:
        switch(event.key.keysym.sym){
        case SDLK_F10:
        case SDLK_RCTRL:
        case SDLK_END:
          show_menu = 1;
          break;
        case SDLK_UP:
          if(menu_index == 0){
            key_row-= 1;
            key_row%= 5;
            if(key_row < 0){
              key_row = 4;
            }
            key_changed = 1;
          }
          else{
            wqx::SetKey(kbk[keymap[0]], 0);
          }
          break;
        case SDLK_DOWN:
          if(menu_index == 0){
            key_row+= 1;
            key_row%= 5;
            key_changed = 1;
          }
          else{
            wqx::SetKey(kbk[keymap[1]], 0);
          }
          break;
        case SDLK_LEFT:
          if(menu_index == 0){
            key_col-= 1;
            key_col%= 10;
            if(key_col < 0){
              key_col = 9;
            }
            key_changed = 1;
          }
          else{
            wqx::SetKey(kbk[keymap[2]], 0);
          }
          break;
        case SDLK_RIGHT:
          if(menu_index == 0){
            key_col+= 1;
            key_col%= 10;
            key_changed = 1;
          }
          else{
            wqx::SetKey(kbk[keymap[3]], 0);
          }
          break;
        case SDLK_LCTRL:
          wqx::SetKey(kbk[keymap[5]], 0);
          break;
        case SDLK_LALT:
          if(menu_index == 0){
            wqx::SetKey(kbk[(key_row * 10) + key_col], 0);
            if(kbk[(key_row * 10) + key_col] == WQX_POWER){
              main_loop = 0;
            }
          }
          else{
            wqx::SetKey(kbk[keymap[4]], 0);
          }
          break;
        case SDLK_SPACE:
          wqx::SetKey(kbk[keymap[6]], 0);
          break;
        case SDLK_LSHIFT:
          wqx::SetKey(kbk[keymap[7]], 0);
          break;
        case SDLK_RETURN:
          wqx::SetKey(kbk[keymap[8]], 0);
          break;
        case SDLK_ESCAPE:
          wqx::SetKey(kbk[keymap[9]], 0);
          break;
        case SDLK_BACKSPACE:
          wqx::SetKey(kbk[keymap[10]], 0);
          break;
        case SDLK_TAB:
          wqx::SetKey(kbk[keymap[11]], 0);
          break;
        }
        break;
      }
    }
    
    if(show_menu){
      show_menu = 0;
      redraw_menu(sdl_screen);
    }

    if((menu_index == 0) && (key_changed)){
      key_changed = 0;
      vkb_show(sdl_screen, (key_row * 10) + key_col);
    }
  
    wqx::RunTimeSlice(FRAME_INTERVAL, mSpeedUp);
    wqx::CopyLcdBuffer(ncscreen);
	
    if(SDL_MUSTLOCK(sdl_screen)){
      SDL_LockSurface(sdl_screen);
    }
    uint16_t *dst = (uint16_t*)sdl_screen->pixels + (30*320);
    for (int y=0; y<80; y++) {
      for (int j=0; j<20; j++) {
        uint8_t p = ncscreen[20*y + j];
        for (int k=0; k<8; k++) {
          *(dst+0) = (uint16_t)((p & (1 << (7 - k))) != 0 ? fgc[foreground] : bkc[background]);
          *(dst+1) = *dst;
          *(dst+320) = *dst;
          *(dst+321) = *dst;
          dst+= 2;
        }
      }
      dst+= 320;
    } 
    if(SDL_MUSTLOCK(sdl_screen)){
      SDL_LockSurface(sdl_screen);
    }

    SDL_BlitSurface(sdl_screen, NULL, real_screen, NULL);
    SDL_Flip(real_screen);
    std::this_thread::sleep_until(next_frame);
  }
  SDL_FreeSurface(keypad);
  for(x=0; x<MAX_BORDER; x++){
    SDL_FreeSurface(border[x]);
  }
  SDL_Quit();
  return 0;
}

