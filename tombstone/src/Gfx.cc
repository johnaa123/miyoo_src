/*
 *  Copyright (C) 2010 Florent Bedoiseau (electronique.fb@free.fr)
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

// Circle algo : Comes from GLCD.C Stephane REY
// LUDO : iso_8859_y 8x8 font (based on console-tool package)

#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

#ifdef PSP
#include <pspctrl.h>
#endif
#include "Gfx.h"

// Graphical section
UInt16 locateX=0;
UInt16 locateY=0;

extern UInt8 font_8x8_8859_01[]; // Declared into Font.cc
#ifdef USE_ALL_FONT
extern UInt8 font_8x8_8859_02[]; 
extern UInt8 font_8x8_8859_03[]; 
extern UInt8 font_8x8_8859_04[]; 
extern UInt8 font_8x8_8859_05[]; 
extern UInt8 font_8x8_8859_06[]; 
extern UInt8 font_8x8_8859_07[]; 
extern UInt8 font_8x8_8859_08[];
extern UInt8 font_8x8_8859_09[];
extern UInt8 font_8x8_8859_10[]; 
#endif

UInt8 *font8x8 = font_8x8_8859_01; // Default font
SDL_Surface *screen = 0;
SDL_Surface *render = 0;

// General graphical routines
void GfxInit () {
  /* Initialize the SDL library */
 if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  
	SDL_ShowCursor(SDL_DISABLE);

  /* Clean up on exit */
  atexit(SDL_Quit);
  
  /*
   * Initialize the display in a 640x480 16-bit palettized mode,
   * requesting a software surface
   */
# ifdef PSP
  render = SDL_SetVideoMode(GFX_RENDER_W, GFX_RENDER_H, GFX_RENDER_DEPTH, 
                                SDL_ANYFORMAT|SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_HWPALETTE);
# elif defined(LINUX_MODE)
  render = SDL_SetVideoMode(GFX_RENDER_W, GFX_RENDER_H, GFX_RENDER_DEPTH, 
                                SDL_SWSURFACE);
# elif defined(PND_MODE) || defined(DINGUX_MODE)
  render = SDL_SetVideoMode(GFX_RENDER_W, GFX_RENDER_H, GFX_RENDER_DEPTH, /*SDL_FULLSCREEN|*/SDL_SWSURFACE);
# else
  render = SDL_SetVideoMode(GFX_RENDER_W, GFX_RENDER_H, GFX_RENDER_DEPTH, SDL_SWSURFACE);
# endif
  if ( render == NULL ) {
    fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",GFX_RENDER_W,
            GFX_RENDER_H, GFX_RENDER_DEPTH, SDL_GetError());
    exit(1);
  }
  screen = SDL_CreateRGBSurface(SDL_SWSURFACE, GFX_SCREEN_W, GFX_SCREEN_H, 
    render->format->BitsPerPixel, render->format->Rmask, render->format->Gmask, render->format->Bmask, 0);

  GfxSetNoClip (0);
 
  GfxClear (0, GfxColor(0,0,0));
 
  locateY = 0;
  locateX = 0;
  font8x8 = font_8x8_8859_01; // Default font
}

UInt16 GfxGetScreenW() {
    if (!screen) return 0;
    return screen->w;
}

UInt16 GfxGetScreenH() {
    if (!screen) return 0;
    return screen->h;
}

void GfxQuit () {
  if (screen) {
    SDL_FreeSurface (screen);
    screen=0;
  }
}


/* Touch Screen functions */
void GfxGetMouseXY (UInt16 *x, UInt16 *y) {
  int xx, yy;
  SDL_PumpEvents();
  SDL_GetMouseState(&xx, &yy);
  *x = xx;
  *y = yy;
}

UInt8 GfxGetMouseButton () {
  int x,y;
  SDL_PumpEvents();
  UInt8 bp = SDL_GetMouseState(&x, &y);
  if (bp & 0x01) return 1;
  return 0;
}

UInt8 GfxKeyTouched () {
# ifdef PSP
  SceCtrlData c;
  sceCtrlPeekBufferPositive(&c, 1);
  c.Buttons &= 0xffff;
  if (! c.Buttons) return 0;
  if (c.Buttons & PSP_CTRL_CROSS) return 1;
# else
  SDL_Event event;
  int val = SDL_PollEvent(&event);
  if (!val) return 0;

  if (event.type == SDL_KEYDOWN) return 1;
# endif
  return 0;
}

UInt16 GfxKeyPressed () {
#ifdef PSP

  SceCtrlData c;
  sceCtrlPeekBufferPositive(&c, 1);
  c.Buttons &= 0xffff;
  if (!c.Buttons) return GFX_KEY_NONE;
  if (c.Buttons & PSP_CTRL_CROSS) return GFX_KEY_DOWN;
  // TODO
#else
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    if (event.type == SDL_KEYDOWN) {
      return event.key.keysym.sym;
    }
  }
#endif
  return GFX_KEY_NONE;
}

bool isGfxKeyPressed (UInt16 keycode) {
    SDL_Event event;
    SDL_PollEvent(&event);
    UInt8 *keys = SDL_GetKeyState(NULL);
    return keys[keycode];
}

void GfxWaitTouched () {
# ifdef PSP
  SceCtrlData c;
  do {
    sceCtrlReadBufferPositive(&c, 1);
    c.Buttons &= 0xffff;
  } while (! (c.Buttons & PSP_CTRL_CROSS));

  do {
    sceCtrlReadBufferPositive(&c, 1);
    c.Buttons &= 0xffff;
  } while (c.Buttons & PSP_CTRL_CROSS);
# else
  SDL_Event event;
  UInt8 fin = 0;
  while (fin == 0) {
    SDL_WaitEvent(&event);
	
    switch (event.type) {
    case SDL_KEYDOWN:
      fin = 1;
      break;
    case SDL_QUIT:
      exit(0);
      break;
    default:
      break;
    } 
  }
# endif
}

// Screen only routines
void GfxLocate (UInt16 l, UInt16 c) {
  if (l >= GFX_CHAR_H) return;
  if (c >= GFX_CHAR_W) return;
  locateY = l;
  locateX = c;
}

void GfxChar (UInt8 ch, UInt32 fgColor, UInt32 bgColor) {
  UInt8 i, data;
  UInt8 line, iline;
  UInt16 x = locateX*FONT_WIDTH;
  UInt16 y = locateY*FONT_HEIGHT;

  for (iline = 0; iline < FONT_HEIGHT; iline++ ) {
    if (font8x8 == 0) line = 0xFF;
    else line = font8x8[(ch * FONT_WIDTH) + iline];
	
    for (i=0; i < FONT_WIDTH; i++) {
      data = (line << i) & 0x80;

      if (data) putpixel(screen, x+i, y+iline, fgColor); // ink color (foreground)
      else  putpixel(screen, x+i, y+iline, bgColor); // paper color (background)

    }
  }
  locateX++;
  if (locateX >= GFX_CHAR_W) {
    locateX=0;
    locateY++;
    if (locateY >= GFX_CHAR_H) locateY=0;
  }
}

void GfxString (const char *dataPtr, UInt32 fgColor, UInt32 bgColor) {
  while ( *dataPtr ) {
    GfxChar(*dataPtr++, fgColor, bgColor);
  }
}

// Screen and/or bitmap
void GfxClear (gfxbitmap *bitmap, UInt32 color) {
  SDL_Surface *data;
  
  if (!bitmap) data=screen;
  else data=bitmap;

  GfxFillRectangle (data, 0, 0, data->w, data->h, color);
}

void GfxSelectFont (FontClass fnt) {
#ifdef USE_ALL_FONT
  switch (fnt) {
  case Iso8859_01:
    font8x8 = font_8x8_8859_01;
    break;
  case Iso8859_02:
    font8x8 = font_8x8_8859_02;
    break;
  case Iso8859_03:
    font8x8 = font_8x8_8859_03;
    break;
  case Iso8859_04:
    font8x8 = font_8x8_8859_04;
    break;
  case Iso8859_05:
    font8x8 = font_8x8_8859_05;
    break;
  case Iso8859_06:
    font8x8 = font_8x8_8859_06;
    break;
  case Iso8859_07:
    font8x8 = font_8x8_8859_07;
    break;
  case Iso8859_08:
    font8x8 = font_8x8_8859_08;
    break;
  case Iso8859_09:
    font8x8 = font_8x8_8859_09;
    break;
  case Iso8859_10:
    font8x8 = font_8x8_8859_10;
    break;
  default:
    font8x8 = font_8x8_8859_01;
    break;
  }
#endif
}

void GfxStringWidth (const char *dataPtr, UInt16 &w, UInt16 &h) {
  if (!dataPtr) { 
    w = 0;
    h = 0;
  }

  w = strlen((const char *) dataPtr) * FONT_WIDTH; // Fix-font 8x8
  h = FONT_HEIGHT;
}

UInt16 GfxStringH (const char *dataPtr) {
    if (!dataPtr) return 0;
    UInt16 h = FONT_HEIGHT;
    return h;
}

UInt16 GfxStringW (const char *dataPtr) {
    if (!dataPtr) return 0;
    UInt16 w = strlen((const char *) dataPtr) * FONT_WIDTH; // Fix-font 8x8
    return w;
}

void GfxStringXY (gfxbitmap *bitmap, const char *dataPtr, UInt16 x, UInt16 y, UInt32 fgColor, UInt32 bgColor) {
  UInt8 xx = x;
  UInt16 width;
  if (!bitmap) width = GFX_SCREEN_W;
  else width = bitmap->w;

  while ( *dataPtr ) {
    GfxCharXY(bitmap, *dataPtr++, xx, y, fgColor, bgColor);
    xx+=FONT_WIDTH;
    if (xx >= width) return;
  }
}

void GfxCharXY (gfxbitmap *bitmap, UInt8 ch, UInt16 x, UInt16 y, UInt32 fgColor, UInt32 bgColor) {
  UInt8 i, data;
  UInt8 line, iline;
  UInt32 color;

  for (iline = 0; iline < FONT_HEIGHT; iline++ ) {
    if (font8x8 == 0) line = 0xFF;
    else line = font8x8[(ch * FONT_WIDTH) + iline];
	
    for (i=0; i < FONT_WIDTH; i++) {
      data = (line << i) & 0x80;

      if (data) color = fgColor; // ink color (foreground)
      else color = bgColor; // paper color (background)

      GfxPixel (bitmap, x+i, y+iline, color); // ink color (foreground)
    }
  }
}

void GfxPixel (gfxbitmap *bitmap, UInt16 x, UInt16 y, UInt32 color) {
  gfxbitmap *data;

  if (!bitmap) data = screen;
  else data = bitmap;

  if ( x >= data->w ) return;
  if ( y >= data->h ) return;

  putpixel (data, x, y, color);
}

UInt32 GfxColorPixel (gfxbitmap *bitmap, UInt16 x, UInt16 y) {
  gfxbitmap *data;

  if (!bitmap) data = screen;
  else data = bitmap;

  if ( x >= data->w ) return 0;
  if ( y >= data->h ) return 0;

  return getpixel (data, x, y); 
}

void GfxLine (gfxbitmap *bitmap, UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2, UInt32 color ) {
  Int32 dx, dy, stepx, stepy, fraction;

  dy = y2 - y1;
  dx = x2 - x1;

  if ( dy < 0 ) {
    dy    = -dy;
    stepy = -1;
  }
  else stepy = 1;

  if ( dx < 0 ) {
    dx    = -dx;
    stepx = -1;
  }
  else stepx = 1;

  dx <<= 1;
  dy <<= 1;

  GfxPixel(bitmap, x1, y1, color );

  if ( dx > dy ) {
    fraction = dy - (dx >> 1);
    while ( x1 != x2 ) {
      if ( fraction >= 0 ) {
	y1 += stepy;
	fraction -= dx;
      }
      x1 += stepx;
      fraction += dy;
      GfxPixel(bitmap, x1, y1, color );
    }
  }
  else {
    fraction = dx - (dy >> 1);
    while ( y1 != y2 ) {
      if ( fraction >= 0 ) {
	x1 += stepx;
	fraction -= dy;
      }
      y1 += stepy;
      fraction += dx;
      GfxPixel(bitmap, x1, y1, color );
    }
  }
}

void GfxDrawPixmap (gfxbitmap *bitmap, UInt16 x, UInt16 y, UInt16 w, UInt16 h, unsigned char *pixmap) {
  UInt16 xx, yy;
  UInt32 color;
  UInt16 idx;

  if (!pixmap) return;

  for (yy = 0; yy < h; yy++) {
    for (xx = 0; xx < w; xx++) {
      idx = (yy * w) + xx;
      color = pixmap[idx];
      GfxPixel (bitmap, x+xx, y+yy, color);
    }
  }
}

void GfxRectangle (gfxbitmap *bitmap, UInt16 x, UInt16 y, UInt16 w, UInt16 h, UInt32 color) {
  UInt16 xx, yy, px, py;
  px = x + w;
  py = y + h;
  for (yy = y; yy < py; yy++) {
    GfxPixel (bitmap, x, yy, color);
    GfxPixel (bitmap, px-1, yy, color);
  }
  
  for (xx = x; xx < px; xx++) {
    GfxPixel (bitmap, xx, y, color);
    GfxPixel (bitmap, xx, py-1, color);
  }
}

void GfxFillRectangle (gfxbitmap *bitmap, UInt16 x, UInt16 y, UInt16 w, UInt16 h, UInt32 color) {
  SDL_Surface *data;
  if (!bitmap) data = screen;
  else data = bitmap;
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  SDL_FillRect(data,&r,color);
}

void GfxCircle (gfxbitmap *bitmap, UInt16 x, UInt16 y, UInt16 radius, UInt32 color) {
  Int16 s16tswitch=0;
  Int16 s16y=0;
  Int16 s16x=0;
  UInt16 u8d;

  u8d = y - x;
  s16y = radius;
  s16tswitch = 3 - 2 * radius;
  while (s16x <= s16y) {
    GfxPixel(bitmap,x + s16x, y + s16y, color);
    GfxPixel(bitmap,x + s16x, y - s16y, color);
	
    GfxPixel(bitmap,x - s16x, y + s16y, color);
    GfxPixel(bitmap,x - s16x, y - s16y, color);
	
    GfxPixel(bitmap,y + s16y - u8d, y + s16x, color);
    GfxPixel(bitmap,y + s16y - u8d, y - s16x, color);
    GfxPixel(bitmap,y - s16y - u8d, y + s16x, color); 
    GfxPixel(bitmap,y - s16y - u8d, y - s16x, color);
	
    if (s16tswitch < 0) {
      s16tswitch += (4 * s16x + 6);
    }
    else {
      s16tswitch += (4 * (s16x - s16y) + 10);
      s16y--;
    }
	
    s16x++;
  }
}

void GfxBlit (gfxbitmap *src, 
              gfxbitmap *dst, 
              UInt16 x_src, UInt16 y_src, 
              UInt16 x_dst, UInt16 y_dst, 
              UInt16 w, UInt16 h) {
  SDL_Surface *s, *d;
  
  if (!src) s=screen;
  else s=src;

  if (!dst) d=screen;
  else d=dst;
  
  if (dst == src) return;

  SDL_Rect sr, dr;
  sr.x = x_src;
  sr.y = y_src;
  sr.h = h;
  sr.w = w;

  dr.x = x_dst;
  dr.y = y_dst;
  dr.h = h;
  dr.w = w;

#if 1
  SDL_SetColorKey(s, 0, 0);
  SDL_BlitSurface(s, &sr, d, &dr);
#else 
  for (UInt16 hh = 0; hh < h; hh++) {
    for (UInt16 ww=0; ww < w; ww++) {
      UInt32 color = GfxColorPixel (s, x_src + ww, y_src + hh);
      GfxPixel(d, x_dst + ww, y_dst + hh, color);
    }
  }
#endif
}

void GfxSetNoClip (gfxbitmap *bitmap) {
  if (!bitmap) SDL_SetClipRect(screen, 0);
  else SDL_SetClipRect(bitmap, 0);
}

void GfxSetClip (gfxbitmap *bitmap, UInt16 xc, UInt16 yc, UInt16 wc, UInt16 hc) {
  UInt16 xx, yy, ww, hh;
  SDL_Rect r;
  SDL_Surface *data;

  if (!bitmap) data=screen;
  else data=bitmap;
  
  if (xc >= data->w) xx = data->w - 1;
  else xx = xc;
  if (yc >= data->h) yy = data->h - 1;
  else yy = yc;

  if (xx + wc >= data->w) ww = data->w - xx;
  else ww = wc;
  if (yy + hc >= data->h) hh = data->h - yy;
  else hh = hc;

  r.x = xx;
  r.y = yy;
  r.w = ww;
  r.h = hh;
  SDL_SetClipRect(data, &r);
}

void GfxGetClip (gfxbitmap *bitmap, UInt16 &x, UInt16 &y, UInt16 &w, UInt16 &h) {
  SDL_Surface *data;
  if (!bitmap) data=screen;
  else data=bitmap;

  SDL_Rect r;
  SDL_GetClipRect(data, &r);

  x = r.x;
  y = r.y;
  w = r.w;
  h = r.h;
}

UInt32 GfxColor (UInt8 r, UInt8 g, UInt8 b) {
#if 0
  r = r & 0xE0; // 3 bits
  g = g & 0xE0; // 3 bits 
  b = b & 0xC0; // 2 bits
  return (r | (g >> 3) | (b >> 6)); // RGB with 2 bits
#endif

  return SDL_MapRGB(screen->format, r, g, b);
}

gfxbitmap *GfxCreateBitmap (UInt16 w, UInt16 h) {
  UInt32 rmask, gmask, bmask, amask;
  
  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
  
#endif
  
  SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 
                              w, h, GFX_SCREEN_DEPTH,         
                              rmask, gmask, bmask, 0);
  SDL_Surface *n = SDL_DisplayFormat(surface);
  SDL_FreeSurface (surface);
  GfxClear (n, GfxColor(0,0,0));
  return n;
}

gfxbitmap *GfxCreateBitmapFromPixmap (UInt16 w, UInt16 h, unsigned char *pixmap) {
  gfxbitmap *bitmap = GfxCreateBitmap (w, h);
  GfxDrawPixmap (bitmap, 0, 0, w, h, pixmap);
  return bitmap;
}

gfxbitmap *GfxCreateBitmapFromGif (const char *giffile) {
#ifdef GIF_SUPPORT
  if (!giffile) return 0; // KO

  int err;
  GifFileType *GIFfile;
  struct SavedImage *image;
  unsigned char *data;
  UInt16 w, h, x, y;

  if ((GIFfile = DGifOpenFileName (giffile, &err)) == 0) {
    return 0; // Error
  }
  DGifSlurp (GIFfile); // Reading image file
  image = GIFfile->SavedImages; // Selecting first image
  data = (unsigned char *)image->RasterBits; // Start addr 

  w = image->ImageDesc.Width;
  h = image->ImageDesc.Height;

  gfxbitmap *bitmap = GfxCreateBitmap (w, h);

  /* Dumping datas */
  UInt8 r, g, b;
  UInt8 index;

  for (y=0; y < h; y++) {
    for (x=0; x < w; x++) {
      index = *data; // Data corressponds to the index in the LUT
      r = GIFfile->SColorMap->Colors[index].Red;
      g = GIFfile->SColorMap->Colors[index].Green;
      b = GIFfile->SColorMap->Colors[index].Blue;
	  
      UInt32 color = GfxColor (r, g, b); 
      GfxPixel (bitmap, x, y, color);

      data++;
    }
  }

  DGifCloseFile (GIFfile, &err);
  return bitmap;

#else
  return 0; // No GIF
#endif
}

void GfxDeleteBitmap (gfxbitmap *bitmap) {
  if (bitmap) {
    SDL_FreeSurface (bitmap);
  }
}


// For simulation only
void GfxUpdate ( ) 
{
  SDL_UpdateRect(screen, 0, 0, GFX_SCREEN_W, GFX_SCREEN_H);
# if 0 //ifdef PSP
  SDL_Flip(screen);
# endif
  SDL_SoftStretch( screen, NULL, render, NULL );
  SDL_Flip( render );
}

void putpixel(SDL_Surface *surface, UInt16 x, UInt16 y, UInt32 pixel) {

    int bpp = surface->format->BytesPerPixel;
    UInt8 *p = (UInt8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(UInt32 *)p = pixel;
        break;
    }
}

UInt32 getpixel (SDL_Surface *surface, UInt16 x, UInt16 y) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    UInt8 *p = (UInt8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(UInt16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(UInt32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void GfxWaitForVBL() {
    static Uint32 old = 0;

    Uint32 current = SDL_GetTicks();
    Uint32 delta = current - old;

    if (delta < 20) {
        SDL_Delay(20 - delta);
    }

    GfxUpdate () ;
    old = current;
}

