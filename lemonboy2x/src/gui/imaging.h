
/* Utilities to manage images: loading and drawing

(C) Juanvvc 2008. 
readpng (C) 2007 Notaz

Under the GPL.

 */

#ifndef _IMAGING_H

#include <png.h>
#include "fb.h"

typedef struct image {
	int x;  // x position
	int y; // y position
	int w; // width
	int h; // height
	byte *data; // image data (in framebuffer format)
} image;

typedef struct font_image {
	image *img; // the image of the font
	int *map; // the map chars->order in the image
	int num_chars; // number of chars in the image
	int num_colors; // number of colors in the image
	int size; // height of the chars
	int *info; // x offset of chars in the image
	int defchar; // default char if not pressent in the mapping.
} font_image;


/** Loads a PNG into a image.
Mostly from gpfce2x by Notaz
Returns 1 if ok, 0 otherwise */
int load_png(image *dest, const char *fname);
/** Loads a PNG font into a image.
 Returns 1 if ok, 0 otherwise */
int load_font(font_image *dest, const char *fname, int font_height);
/** Unloads a font image */
void unload_font(font_image *);
/** Unloads an image */
void unload_png(image *);
/** Redraws the screen (only YUB layer) */
void redraw(void);
/** Draws a char on the screen. It suppose that font is a 32 bits image */
int draw_char(font_image *font, char c, int x, int y, int color);
/** Draws a string on fht screen */
void draw_string(font_image *font, char* string, int x, int y, int color, int menu_xmax);
/** Draws a sprite on the screen
data a 32bits YUV image
(xo, yo) the offset in data
(x, y) the position on the screen
(w, h) the size of the drawing */
void draw_sprite(image* data, int xo, int yo, int x, int y, int w, int h);
//void draw_sprite_rgb(struct image* data, int xo, int yo, int x, int y, int w, int h);
/** The same that draw_sprite, but uses the first pixel as transparent color */
void draw_sprite_trans(image* data, int xo, int yo, int x, int y, int w, int h);
/** Writes a 16 bits RGB array into a PNG
fname: name of the file
data: 16 bits RGB in the minilib format (5+6+5)
w: width of the image
h: height of the image
pitch distance from the last pixel of a row to the first of the next row (usually, 0)
Returns 1 if saved, 0 otherwise
Most of this code is from gshot by Notaz */
int save_png(const char *fname, byte *data, int w, int h, int pitch);

#define _IMAGING_H
#endif
