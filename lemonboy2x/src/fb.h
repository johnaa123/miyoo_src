

#ifndef __FB_H__
#define __FB_H__


#include "defs.h"



struct fb
{
	int w, h; // width of the screen (¡not the gameboy!)
	byte *ptr; // pointer to the screen
	int pelsize; // bit size
	int pitch; // bytes for line of the screen (normally, fb.pelsize * fb.w)
	int indexed;
	int offset; // offset in ptr for first pixel. Centered, offset=((fb.w*fb.pelsize)>>1)- (80*fb.pelsize) * scale	+ ((fb.h>>1) - 72*scale) * fb.pitch;)
	struct
	{
		int l, r; // position of R, G and B components. See video driver
	} cc[4];
	int yuv;
	int enabled;
	int dirty;
};


extern struct fb fb;


#endif




