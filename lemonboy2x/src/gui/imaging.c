/* Utilities to manage images: loading and drawing

(C) Juanvvc 2008. 
readpng (C) 2007 Notaz

Under the GPL.

 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <png.h>

#include "imaging.h"
#include "options.h"

#include "defs.h"


extern struct fb fb;

int load_png(image *image, const char *fname)
{
	FILE *fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytepp row_ptr = NULL;
	int height, width, h;
	int ok=0;
	byte *dest, *dest2;
	int r, g, b, c;

	if (fname == NULL)
	{
		return FALSE;
	}
#ifdef DEBUG
	printf(__FILE__ ": Loading %s\n", fname);
#endif
	fp = fopen(fname, "rb");
	if (fp == NULL)
	{
		printf(__FILE__ ": failed to open: %s\n", fname);
		return FALSE;
	}
	DPRINT(__FILE__ ": File exists. Loading...");
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		DPRINT(__FILE__ ": png_create_read_struct() failed\n");
		fclose(fp);
		return FALSE;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		DPRINT(__FILE__ ": png_create_info_struct() failed\n");
		ok = FALSE;
		goto done;
	}
	// Start reading
	png_init_io(png_ptr, fp);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_STRIP_ALPHA | PNG_TRANSFORM_PACKING, NULL);
	row_ptr = png_get_rows(png_ptr, info_ptr);
	if (row_ptr == NULL)
	{
		DPRINT(__FILE__ ": png_get_rows() failed\n");
		ok = FALSE;
		goto done;
	}
	height = png_get_image_height(png_ptr, info_ptr); //info_ptr->height;
	width = png_get_image_width(png_ptr, info_ptr); //info_ptr->width;
	image->w=width;
	image->h=height;
	image->x=image->y=0;
	
	dest = calloc(height * width, fb.pelsize);
	dest2 = dest; // point to the first byte of dest

	if(dest == NULL)
	{
		DPRINT(__FILE__ ": malloc() failed\n");
		ok  = FALSE;
		goto done;
	}
	for (h = 0; h < height; h++)
	{
		unsigned char *src = row_ptr[h];
		int len = width;
		while (len--)
		{
			r = (src[0] >> fb.cc[0].r) << fb.cc[0].l;
			g = (src[1] >> fb.cc[1].r) << fb.cc[1].l;
			b = (src[2] >> fb.cc[2].r) << fb.cc[2].l;
			c = r|g|b;
			switch(fb.pelsize){
				case 1:
					dest[0] = (c&0xff);
					dest++;
					break;
				case 2:
					dest[0] = (byte)(c&0xff);
					dest[1] = (byte)((c>>8)&0xff);
					dest += fb.pelsize;
					break;
				default: //case 3: case 4:
					dest[0] = (byte)(c&0xff);
					dest[1] = (byte)((c>>8)&0xff);
					dest[2] = (byte)((c>>16)&0xff);
					dest += fb.pelsize;
					break;
			}
			src += 3;
		}
	}
	image->data=dest2;
	ok = TRUE;
	DPRINT(__FILE__ ": Image loaded");
done:
	png_destroy_read_struct(&png_ptr, info_ptr ? &info_ptr : NULL, (png_infopp)NULL);
	fclose(fp);
	if(!ok) image->data=NULL;
	return ok;
}

/** A defaul map for Fonts in printable ASCII: The image file has chars in ASCII order, from space to _ Then, there is no lower case letters */
static int font_default_map[128]={
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // firstsimbols
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // firstsimbols
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // firstsimbols
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, //<SPC>!"#$%&'()*+,-./
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // numbers
	26, 27, 28, 29, 30, 31, 32, //:;<=>?@
	33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, // capital letters
	59, 60, 61, 62, 63, 64, //[\]^_`
	33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, // small letters (maps to capital)
	0, 0, 0, 0, 0 //{|}~<DEL>
};

int load_font(font_image * font, const char * file, int s){
	byte * buf;
	byte t;
	int i, n;
#define MAX_CHARS 256
	// load the font image
	if((font->img = (image *)malloc(sizeof(image)))==NULL)
		return FALSE;
	if(load_png(font->img, file)!=TRUE){
		FREE(font->img);
		return FALSE;
	}
	// save some data: height of the font, number of colors, default char and map...
	font->size = s;
	font->num_colors = (font->img->h - 1) / s;
	// the user may change this after calling load_font
	font->defchar = 0;
	font->map = font_default_map;
	// create the info structure: look int the last line of the
	// image: a starts when there is a no-transparent color in the last line.
	if((font->info = malloc(sizeof(int)*MAX_CHARS))==NULL){
		unload_png(font->img);
		FREE(font->img);
		return FALSE;
	}
	buf = font->img->data;
	t = buf[0];
	buf += fb.pelsize * font->img->w * s * font->num_colors;
	n = 0;
	for(i=0; i<font->img->w * fb.pelsize && n<MAX_CHARS; i+=fb.pelsize){
		if(buf[i]!=t){
			font->info[n] = i/fb.pelsize;
			n++;
		}
	}
	if(n>0)
		font->info[n] = i/fb.pelsize;
	else
		font->info[0] = font->img->w;
	DPRINT("");
	font->num_chars = n;
	return TRUE;
}

void unload_font(font_image * font){
	if(!font) return;
	unload_png(font->img);
	FREE(font->info);
}

void unload_png(image * im){
	if(im) FREE(im->data);
}

/** Draws a char on the screen.
 * Returns the width of the character that was printed.
 */
int draw_char(font_image *font, char c, int x, int y, int color){
	int c_p, c_x, c_w;
	int ci = (int)c;
	
	if(!font->img){
		DPRINT("Trying to write without a font");
		return;
	}
	
	if(color > font->num_colors) color = font->num_colors - 1;
	if(ci<0) ci=0;
	c_p = font->map[ci];
	if(c_p > font->num_chars) c_p=font->defchar;
	c_x=(c_p==0?0:font->info[c_p-1]);
	c_w=font->info[c_p] - c_x;
#ifdef DEBUG
	//printf("%c maps to %d (+%d)\n", c, c_p, c_w);
#endif
	draw_sprite_trans(font->img, c_x, color * font->size, x, y, c_w, font->size);
	return c_w-1;
}

/** Draws a string on fht screen */
void draw_string(font_image *font, char* string, int x, int y, int color, int xmax){
#ifdef DEBUG
	//printf("Writing on screen: %s\n", string);
#endif
	if(!font->img){
		DPRINT("Trying to write without a font");
		return;
	}	
	while(string[0]){
		x+=draw_char(font, *string++, x, y, color);
		if(x>xmax) break;
	}
}

#ifdef DEBUG
#define PRINTPIXEL(P) for(i=0;i<fb.pelsize;i++){printf("%x",P[i]);};
#else
#define PRINTPIXEL(P) ;
#endif

/** Draws a sprite on the screen
(xo, yo) the offset in data
(x, y) the position on the screen
(w, h) the size of the drawing */
void draw_sprite(image *data, int xo, int yo, int x, int y, int w, int h){
	byte *datadata=data->data;
	byte *buf = &datadata[(xo+yo*data->w)*fb.pelsize];
	byte *tmpscreen= &fb.ptr[(x+(fb.w*y))*fb.pelsize];
	int temph;

	if(x>fb.w||y>fb.h) return;
	if(x+w>fb.w) w=fb.w-x;
	if(y+h>fb.h) h=fb.h-y;
	temph=h;
	
	while (temph--){
		memcpy(tmpscreen, buf, w * fb.pelsize);
		tmpscreen += fb.w * fb.pelsize;
		buf += data->w * fb.pelsize;
	}
}

/** The same that draw_sprite, but uses the first pixel as transparent color. Warning! This function is NOT optimized.
 */
void draw_sprite_trans(image *data, int xo, int yo, int x, int y, int w, int h){
	byte *datadata=data->data;
	byte *buf = &datadata[(xo+yo*data->w)*fb.pelsize];
	byte *tmpscreen= &fb.ptr[(x+(fb.w*y))*fb.pelsize];
	int temph, tempw;
	int tr, i;
	byte *trans;
	
	// save in trans the transparent color
	if((trans=malloc(fb.pelsize))==NULL){
		DPRINT("Error while drawing: out of memory\n");
		return;
	}
	for(i=0; i<fb.pelsize; i++){
		trans[i]=datadata[i];
	}
	
	// calculate true dimensions of the image (not draw outside screen)	
	if(x>fb.w||y>fb.h) return;
	if(x<0) { w += x; x=0; }
	if(y<0) { h += y; y=0; }
	if(x+w>fb.w) w=fb.w-x;
	if(y+h>fb.h) h=fb.h-y;
	temph=h;
	tempw=w;
	
	while (temph--){
		tempw=w;
		while(tempw--){
			tr=1;
			// check the transparent color
			for(i=0; i<fb.pelsize; i++){
				if(buf[i]!=trans[i]){
					tr=0; break;
				}
			}
			if(!tr)	memcpy(tmpscreen, buf, fb.pelsize);
			tmpscreen += fb.pelsize;
			buf += fb.pelsize;
		}
		tmpscreen += (fb.w - w) * fb.pelsize;
		buf += (data->w - w) * fb.pelsize;
	}
}

void free_sprite(image *rect){
	if(rect){
		free(rect->data);
		free(rect);
	}
}

/** Writes a 16 bits RGB array into a PNG
fname: name of the file
data: 16 bits RGB in the minilib format (5+6+5)
w: width of the image
h: height of the image
pitch distance from the last pixel of a row to the first of the next row (usually, 0)
Returns 1 if saved, 0 otherwise
Most of this code is from gshot by Notaz */
int save_png(const char *fname, byte *data, int w, int h, int pitch)
{
	png_bytepp row_pointers=NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	FILE *f;
	int x, y;
	byte *ss = data;
	int d;
	
	/* create an array of row pointers */
	d = h*sizeof(row_pointers[0]);
	if((row_pointers=malloc(d))==NULL){
		printf(__FILE__ ": malloc failed");
		return FALSE;
	}
	// FIXME: test this malloc
	for(y=0; y<h; y++){
		row_pointers[y]=malloc(w*3);
	}
	for (y = 0; y < h; y++){
		unsigned char *d = row_pointers[y];
		unsigned int c;
		for (x = 0; x < w*3; x+=3){
			//s = (*(ss+1)<<8)|(*ss); ss+=2;
			switch(fb.pelsize){
			case 2: c = ss[0]|ss[1]<<8; break;
			case 3: case 4: c = ss[0]|ss[1]<<8|ss[2]<<16;
			}
			ss += fb.pelsize;
			d[x+0] = ((c>>fb.cc[0].l)<<fb.cc[0].r) & 0xff; //((s >> 11) * 264) >> 5; // *= 8.25
			d[x+1] = ((c>>fb.cc[1].l)<<fb.cc[1].r) & 0xff; //(((s >> 5) & 0x3f) * 260) >> 6; // *= 4.0625
			d[x+2] = ((c>>fb.cc[2].l)<<fb.cc[2].r) & 0xff; //((s & 0x1f) * 264) >> 5;
		}
		ss+=pitch;
	}

	f = fopen(fname, "wb");
	if (f == NULL) {
		printf(__FILE__ ": failed to open \"%s\"\n", fname);
		return FALSE;
	}

	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL) {
		printf(__FILE__ "png_create_write_struct() failed");
		goto fail1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		printf(__FILE__ "png_create_info_struct() failed");
		goto fail2;
	}

	if (setjmp(png_jmpbuf(png_ptr)) != 0) {
		printf(__FILE__ "error in png code\n");
		goto fail2;
	}

	png_init_io(png_ptr, f);

	png_set_IHDR(png_ptr, info_ptr, w, h,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, NULL);

	fclose(f);
	sync();
	
	for(y=0; y<h; y++) free(row_pointers[y]);
	free(row_pointers);
	
	return TRUE;

fail2:
	png_destroy_write_struct(&png_ptr, &info_ptr);
fail1:
	fclose(f);
	return FALSE;
}


