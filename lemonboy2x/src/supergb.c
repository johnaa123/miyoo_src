/*

Lemonboy - Gameboy emulator based on Gnuboy for Gp2x/Wiz/Pandora
Copyright (C) 2009 Juan Vera del Campo <juanvvc@gmail.com>

GEST - Gameboy emulator
Copyright (C) 2003-2004 TM

VisualBoyAdvance - a Gameboy and GameboyAdvance emulator
Copyright (C) 1999-2004 by Forgotten


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"

#include "defs.h"
#include "regs.h"
#include "mem.h"
#include "hw.h"
#include "lcd.h"
#include "rc.h"

#ifdef DEBUG
#include "gui/imaging.h"
#include "fb.h"
#endif

#include "supergb.h"

#define SGB_NONE 0
#define SGB_RESET 1
#define SGB_PACKET_TRANSMIT 2

extern int mix_frames;

int sgb_ft = 1;

int border_uploaded = 0;
int col0_used = 0;

unsigned short sgb_border_buffer[256*224];

int bit_received = 0;
int bits_received = 0;
int packets_received = 0;
int sgb_state = SGB_NONE; 
int sgb_index = 0;
int sgb_command = 0;
int sgb_multiplayer = 0;
int sgb_fourplayers = 0;
int sgb_nextcontrol = 0x0F;
int sgb_readingcontrol = 0;
int sgb_CGB_support = 0;
int sgb_mask = 0;
int sgb_attraction_mode = 1;
int sgb_file_transfer = 1;

int cycles_SGB;

byte *sgb_sound_score= NULL;

byte *sgb_borderchar = NULL; // info about the tiles (sent with CHR_TRN)
byte *sgb_border = NULL;     // tiles on screen (sent with PCT_TRN)
image sgb_border_image;   // the image of the border
int sgb_border_image_ready = FALSE; // is the border image ready?

unsigned short sgb_palette[8*16]; // info about the palettes ( 8 palettes of 16 colors of 16 bits each)
unsigned short sgb_palette_memory[512*4];
byte sgb_buffer[16*7];
byte sgb_screenbuffer[4160];
byte sgb_ATF[20 * 18];
byte sgb_ATF_list[45 * 20 * 18];

void sgb_draw_border_tile(int x, int y, int tile, int attr)
{
	unsigned short *dest = sgb_border_buffer + x + y*256;
	
	byte *tileAddress = &sgb_borderchar[tile * 32];
	byte *tileAddress2 = &sgb_borderchar[tile * 32 + 16];
	
	byte l = 8;
	
	byte palette = ((attr >> 2 ) & 7);
	
	if(palette < 4)
		palette += 4;
	
	palette *= 16;
	
	byte xx = 0;
	byte yy = 0;
	
	int flipX = attr & 0x40;
	int flipY = attr & 0x80;
	
	while(l > 0) 
	{
		byte mask = 0x80;
		byte a = *tileAddress++;
		byte b = *tileAddress++;
		byte c = *tileAddress2++;
		byte d = *tileAddress2++;
		
		while(mask > 0) 
		{  
			byte color = 0;
			if(a & mask)
				color++;
			if(b & mask)
				color+=2;
			if(c & mask)
				color+=4;
			if(d & mask)
				color+=8;
			
			if((x<48 || x>208) && (y<40 || y>184) && palette+color == 64) 
				col0_used = 1;	// detect if color 0 is used in border, 
						// so we must redraw it if palette changed
	
			byte xxx = xx;
			byte yyy = yy;
			
			if(flipX)
			xxx = 7 - xx;
			if(flipY)
			yyy = 7 - yy;
	
			unsigned short c = sgb_palette[palette + color];
			*(dest + yyy*256+xxx) = c;
	
			mask >>= 1;
	
			xx++;
		}
		yy++;
		xx = 0;
		l--;
		mask = 0x80;
	}
}

/** Draws the border on the screen */
void draw_border()
{
	int c, r, g, b;
	int i;
	byte *dest;
	short *src;
	short csrc[3];

	// initial information of the image
	dest = sgb_border_image.data;
	src = sgb_border_buffer;
	sgb_border_image.w = 256;
	sgb_border_image.h = 224;
	
	// convert sgb_border_buffer into the format of a gnuboy framebuffer 
	for(i=0; i<256 * 224; i++){
		csrc[0] = ((*src)&0x1f)<<3; // red: bits 0-5. Multiply to be in 256 levels
		csrc[1] = (((*src)>>5)&0x1f)<<3; // green: bits 5-10
		csrc[2] = (((*src)>>10)&0x1f)<<3; // blue: bits 10-15
		r = (csrc[0] >> fb.cc[0].r) << fb.cc[0].l;
		g = (csrc[1] >> fb.cc[1].r) << fb.cc[1].l;
		b = (csrc[2] >> fb.cc[2].r) << fb.cc[2].l;
		c = r|g|b;
		switch(fb.pelsize){
		case 1:
			dest[0] = (c&0xff);
			break;
		case 2:
			dest[0] = (byte)(c&0xff);
			dest[1] = (byte)((c>>8)&0xff);
			break;
		default: //case 3: case 4:
			dest[0] = (byte)(c&0xff);
			dest[1] = (byte)((c>>8)&0xff);
			dest[2] = (byte)((c>>16)&0xff);
			break;
		}
		src++;
		dest+=fb.pelsize;
	}
	// draw the sprite on the screen
	vid_draw_border(&sgb_border_image);
	sgb_border_image_ready = TRUE;
}

void sgb_render_border()
{
	byte *fromAddress = sgb_border;
	byte x, y;
  
	for(y = 0; y < 28; y++) 
	{
		for(x = 0; x < 32; x++) 
		{
			byte tile = *fromAddress++;
			byte attr = *fromAddress++;
		  
			sgb_draw_border_tile(x*8,y*8,tile,attr);
		}
	}
	draw_border();
}

int sgb_init()
{
	//sgb_borderchar = new byte[32 * 256];
	if((sgb_borderchar = malloc(sizeof(byte) * 32 * 256))==NULL)
		return FALSE;
	//sgb_border = new byte[2048];
	if((sgb_border = malloc(sizeof(byte) * 2048))==NULL){
		FREE(sgb_borderchar);
		return FALSE;
	}
	//sgb_sound_score = new byte[8192];
	if((sgb_sound_score = malloc(sizeof(byte) * 8192))==NULL){
		FREE(sgb_borderchar);
		FREE(sgb_border);
		return FALSE;
	}
	if((sgb_border_image.data = malloc(256 * 224 * fb.pelsize))==NULL){
		FREE(sgb_borderchar);
		FREE(sgb_border);
		FREE(sgb_sound_score);
		return FALSE;
	}
	sgb_border_image_ready = FALSE;
	return TRUE;
}

void sgb_end()
{
	FREE(sgb_borderchar);
	FREE(sgb_border);
	FREE(sgb_sound_score);
	FREE(sgb_border_image.data);
	sgb_border_image_ready = FALSE;
}

static byte tile_signed_trans[256] =
{
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
	0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
	0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
	0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
	0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
	0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
	0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f
};

/** Used to read the VRAM buffer in long commands.
The contents of the VRAM are saved in sgb_screenbuffer */
void sgb_render_screen()
{
	unsigned short mapAddress = 0x9800;
	int i, j, k;

	//if(GB->memory[0xFF40] & 0x08)
	if(REG(RI_LCDC) & 0x08)
		mapAddress = 0x9c00;

	unsigned short patternAddress = 0x8800;

	int flag = 1;
	
	//if(GB->memory[0xFF40] & 0x10) 
	if(REG(RI_LCDC) & 0x10)
	{
		patternAddress = 0x8000;
		flag = 0;
	}
	
	byte *toAddress = sgb_screenbuffer;

	for(i = 0; i < 13; i++) 
	{
		for(j = 0; j < 20; j++) 
		{
			int tile = mem_read(mapAddress);//GB->mem_map[mapAddress>>12][mapAddress&0xfff];
			mapAddress++;
		
			if(flag) 
				tile = tile_signed_trans[tile];
			for(k = 0; k < 16; k++)
			{
				int temp = patternAddress + tile*16 + k;
				*toAddress++ = mem_read(temp);//GB->mem_map[temp>>12][temp&0xfff];
			}
		}
		mapAddress += 12;
	}
}

void sgb_reset()
{
	int i;
	sgb_ft = 1;
	
	sgb_command = 0;
	sgb_index = 0;
	sgb_mask = 0;
	bit_received = 0;
	bits_received = 0;
	packets_received = 0;
	sgb_state = SGB_NONE;	
	
	sgb_CGB_support = 0;
	
	sgb_multiplayer = 0;
	sgb_fourplayers = 0;
	sgb_nextcontrol = 0x0F;
	sgb_attraction_mode = 1;
	sgb_file_transfer = 1;
	
	if(!hw.cgb){
		for(i = 0; i < 64;) 
		{
			sgb_palette[i++] = 0;
			sgb_palette[i++] = (0x0c) | (0x0c << 5) | (0x0c << 10);
			sgb_palette[i++] = (0x15) | (0x15 << 5) | (0x15 << 10);
			sgb_palette[i++] = (0x1f) | (0x1f << 5) | (0x1f << 10);
		}
		for(i=0; i<64; i+=4) gnuboy_update_palette(i);
	}else{
		for(i = 0; i < 24;) 
		{
			sgb_palette[i++] = (0x1f) | (0x1f << 5) | (0x1f << 10);
			sgb_palette[i++] = (0x15) | (0x15 << 5) | (0x15 << 10);
			sgb_palette[i++] = (0x0c) | (0x0c << 5) | (0x0c << 10);
			sgb_palette[i++] = 0;
		}
	}
	
	memset(sgb_palette_memory, 0, 512*4);
	memset(sgb_ATF, 0, 20*18);
	memset(sgb_ATF_list, 0, 45 * 20 * 18);
	memset(sgb_buffer, 0, 16 * 7);
	if(sgb_borderchar) memset(sgb_borderchar, 0, 32*256);
	if(sgb_border){
		memset(sgb_border, 0, 2048);
		for(i = 1; i < 2048; i+=2)
			sgb_border[i] = 1 << 2;
	}
	if(sgb_sound_score) memset(sgb_sound_score,0,8192);
	
}

void sgb_reset_state()
{
	cycles_SGB = 0;
	
	sgb_state = SGB_NONE;
}

void sgb_multiplayer_req()
{
	if(sgb_buffer[1]&1) 
	{
		sgb_multiplayer = 1;
		if(sgb_buffer[1]&2)
			sgb_fourplayers = 1;
		else
			sgb_fourplayers = 0;
		
		if(sgb_ft) // fix for Killer Instinct
		{
			sgb_ft = 0;
			sgb_nextcontrol = 0x0E;  
		} else
			sgb_nextcontrol = 0x0F;				  
	} else 
	{
		sgb_fourplayers = 0;
		sgb_multiplayer = 0;
		sgb_nextcontrol = 0x0F;
	}
}

#ifndef DEBUG
int pindex = 0;
#endif

void gnuboy_update_palette(int p)
{
	int i;
// 	char cmd[128];

	for(i=p; i<p+4; i++){
		pal_write(2*i, sgb_palette[i] & 0xff);
		pal_write(2*i+1, (sgb_palette[i]>>8) & 0xff);
	}
	
// 	sprintf(cmd, "set dmg_bgp  0x%x, 0x%x 0x%x 0x%x", sgb_palette[0], sgb_palette[1], sgb_palette[2], sgb_palette[3]);
// 	rc_command(cmd);
// 	DPRINT1("%s", cmd);
// 	sprintf(cmd, "set dmg_wndp  0x%x, 0x%x 0x%x 0x%x", sgb_palette[4], sgb_palette[5], sgb_palette[6], sgb_palette[7]);
// 	rc_command(cmd);
// 	DPRINT1("%s", cmd);
// 	sprintf(cmd, "set dmg_obp0  0x%x, 0x%x 0x%x 0x%x", sgb_palette[8], sgb_palette[9], sgb_palette[10], sgb_palette[11]);
// 	rc_command(cmd);
// 	DPRINT1("%s", cmd);
// 	sprintf(cmd, "set dmg_obp1  0x%x, 0x%x 0x%x 0x%x", sgb_palette[12], sgb_palette[13], sgb_palette[14], sgb_palette[15]);
// 	rc_command(cmd);
// 	DPRINT1("%s", cmd);
// 	rc_command("set colorfilter false");

#ifndef DEBUG
	// save the palette
 	{
	image img;
	int r, g, b, c;
	char fname[256];
	byte *dest;
	img.data = malloc(128 * fb.pelsize);
	img.w = 16;
	img.h = 128 / 16;
	dest = img.data;
	for(i=0; i<128; i++){
		r = ((sgb_palette[i])&0x1f)<<3; // red: bits 0-5. Multiply to be in 256 levels
		g = (((sgb_palette[i])>>5)&0x1f)<<3; // green: bits 5-10
		b = (((sgb_palette[i])>>10)&0x1f)<<3; // blue: bits 10-15
		
		r = (r >> fb.cc[0].r) << fb.cc[0].l;
		g = (g >> fb.cc[1].r) << fb.cc[1].l;
		b = (b >> fb.cc[2].r) << fb.cc[2].l;
		c= r|g|b;
		
		switch(fb.pelsize){
		case 1:
			dest[0] = (c&0xff);
			break;
		case 2:
			dest[0] = (byte)(c&0xff);
			dest[1] = (byte)((c>>8)&0xff);
			break;
		default: //case 3: case 4:
			dest[0] = (byte)(c&0xff);
			dest[1] = (byte)((c>>8)&0xff);
			dest[2] = (byte)((c>>16)&0xff);
			break;
		}
		dest += fb.pelsize;
	}
	sprintf(fname, "pal%03d.png", pindex++);
	save_png(fname, img.data, img.w, img.h, 0);
	}
#endif
}

void set_palette(int num1,int num2)
{
	if(num1==0 && col0_used)
	{
		sgb_palette[64] = sgb_palette[80] = sgb_palette[96] = sgb_palette[112] = sgb_buffer[1]|(sgb_buffer[2]<<8);  
		sgb_render_border();
	}
	sgb_palette[num1*4] = sgb_buffer[1]|(sgb_buffer[2]<<8);
	sgb_palette[num1*4+1] = sgb_buffer[3]|(sgb_buffer[4]<<8);
	sgb_palette[num1*4+2] = sgb_buffer[5]|(sgb_buffer[6]<<8);
	sgb_palette[num1*4+3] = sgb_buffer[7]|(sgb_buffer[8]<<8);
	
	sgb_palette[num2*4] = sgb_buffer[1]|(sgb_buffer[2]<<8);
	sgb_palette[num2*4+1] = sgb_buffer[0x9]|(sgb_buffer[0xA]<<8);
	sgb_palette[num2*4+2] = sgb_buffer[0xB]|(sgb_buffer[0xC]<<8);
	sgb_palette[num2*4+3] = sgb_buffer[0xD]|(sgb_buffer[0xE]<<8);

	gnuboy_update_palette(num1*4);
	gnuboy_update_palette(num2*4);
}

void sgb_setATF(byte n)
{
	if(n > 44)
		n = 44; 
	
	memcpy(sgb_ATF,&sgb_ATF_list[n * 20 * 18], 20 * 18);
}

void copy_palette()
{
	int i;
	int src1=((sgb_buffer[1]|(sgb_buffer[2]<<8))&0x1FF)*4;
	int src2=((sgb_buffer[3]|(sgb_buffer[4]<<8))&0x1FF)*4;
	int src3=((sgb_buffer[5]|(sgb_buffer[6]<<8))&0x1FF)*4;
	int src4=((sgb_buffer[7]|(sgb_buffer[8]<<8))&0x1FF)*4;

	for(i=0;i<4;++i)
	{
		sgb_palette[i]	 = sgb_palette_memory[i+src1];
		sgb_palette[i+4]  = sgb_palette_memory[i+src2];
		sgb_palette[i+8]  = sgb_palette_memory[i+src3];
		sgb_palette[i+12] = sgb_palette_memory[i+src4];
	}

	if(sgb_buffer[9]&0x80) 
		sgb_setATF(sgb_buffer[9] & 0x3F);	

	if(sgb_buffer[9]&0x40)
	{
		sgb_mask = 0;
		//GB->skip_frame = 1;
	}
	
	for(i=0; i<16; i+=4)
		gnuboy_update_palette(i);
}

void init_mem_palette()
{
	int i, j;
	sgb_render_screen();
  
	for(i = 0; i < 512*4; i++,j+=2) 
		sgb_palette_memory[i] = (sgb_screenbuffer[j]|(sgb_screenbuffer[j+1]<<8));
}

void sgb_attribute_block()
{
	byte *fromAddress = &sgb_buffer[1];
	
	byte n_datasets = *fromAddress++;
	
	if(n_datasets > 12)
		n_datasets = 12;
		
	if(n_datasets == 0)
		n_datasets = 1;
	
	while(n_datasets) 
	{
		byte controlCode = (*fromAddress++) & 7;	 
		byte paletteDesignation = (*fromAddress++) & 0x3f;
		byte startH = (*fromAddress++) & 0x1f;
		byte startV = (*fromAddress++) & 0x1f;
		byte endH	= (*fromAddress++) & 0x1f;
		byte endV	= (*fromAddress++) & 0x1f;
	
		byte * toAddress = sgb_ATF;
		byte x, y;
		
		for(y = 0; y < 18; y++) 
		{
			for(x = 0; x < 20; x++) 
			{
			if(x < startH || y < startV || x > endH || y > endV) 
			{
				// outside
				if(controlCode & 0x04)
					*toAddress = (paletteDesignation >> 4) & 0x03;
			} else if(x > startH && x < endH && y > startV && y < endV) 
			{
				// inside
				if(controlCode & 0x01)
					*toAddress = paletteDesignation & 0x03;
			} else 
			{
				// surrounding line
				if(controlCode & 0x02)
					*toAddress = (paletteDesignation>>2) & 0x03;
				else if(controlCode == 0x01)
					*toAddress = paletteDesignation & 0x03;			
			}
			toAddress++;
			}
		}
		n_datasets--;
	}
}

void sgb_SetColumnPalette(byte col, byte p)
{
	byte y;
	if(col > 19)
		col = 19;

	p &= 3;
  
	byte *toAddress = &sgb_ATF[col];
  
	for(y = 0; y < 18; y++) 
	{
		*toAddress = p;
		toAddress += 20;
	}
}

void sgb_SetRowPalette(byte row, byte p)
{
	byte x;
	if(row > 17)
		row = 17;

	p &= 3;
  
	byte *toAddress = &sgb_ATF[row*20];
  
	for(x = 0; x < 20; x++) 
		*toAddress++ = p;
}

void sgb_attribute_line()
{
	byte *fromAddress = &sgb_buffer[1];
	
	byte n_datasets = *fromAddress++;
	
	if(n_datasets > 0x6e)
		n_datasets = 0x6e;
	
	while(n_datasets) 
	{
		byte line = *fromAddress++;
		byte num = line & 0x1f;
		byte pal = (line >> 5) & 0x03;
		if(line & 0x80) 
		{
			if(num > 17)
				num = 17;
			sgb_SetRowPalette(num,pal);
		} else 
		{
			if(num > 19)
				num = 19;
			sgb_SetColumnPalette(num,pal);
		}
		n_datasets--;
	}
}

void sgb_attribute_divide()
{
	byte control = sgb_buffer[1];
	byte coord	= sgb_buffer[2];
	byte colorBR = control & 3;
	byte colorAL = (control >> 2) & 3;
	byte colorOL = (control >> 4) & 3;
	byte i;

	if(control & 0x40) 
	{
		if(coord > 17)
			coord = 17;

		for(i = 0; i < 18; i++) 
		{
			if(i < coord)
			sgb_SetRowPalette(i, colorAL);
			else if ( i > coord)
			sgb_SetRowPalette(i, colorBR);
			else
			sgb_SetRowPalette(i, colorOL);
		}
	} else 
	{
		if(coord > 19)
			coord = 19;
	
		for(i = 0; i < 20; i++) 
		{
			if(i < coord)
			sgb_SetColumnPalette(i, colorAL);
			else if( i > coord)
			sgb_SetColumnPalette(i, colorBR);
			else
			sgb_SetColumnPalette(i, colorOL);		
		}
	}
}

void sgb_attribute_character()
{
	byte startH = sgb_buffer[1] & 0x1f;
	byte startV = sgb_buffer[2] & 0x1f;
	int n_datasets = (sgb_buffer[3]|(sgb_buffer[4]<<8)) & 0x1FF;
	int style = sgb_buffer[5] & 1;
	
	if(n_datasets > 360)
		n_datasets = 360;
			
	if(startH > 19)
		startH = 19;
	if(startV > 17)
		startV = 17;
	
	byte s = 6;
	byte *fromAddress = &sgb_buffer[6];
	byte v = *fromAddress++;
	
	if(style) 
	{
		while(n_datasets) 
		{
			byte p = (v >> s) & 3;
			if(p) // fixes Block Kuzushi
				sgb_ATF[startV * 20 + startH] = p;
			startV++;
			if(startV == 18) 
			{
				startV = 0;
				startH++;
				if(startH == 20)
					break;
			}
	
			if(s)
				s -= 2;
			else 
			{
				s = 6;
				v = *fromAddress++;
				n_datasets--;
			}
		}
	} else 
	{
		while(n_datasets) 
		{
			byte p = (v >> s) & 3;
			sgb_ATF[startV * 20 + startH] = p;
			startH++;
			if(startH == 20) 
			{
				startH = 0;
				startV++;
				if(startV == 18)
					break;
			}
			
			if(s)
				s -= 2;
			else 
			{
				s = 6;
				v = *fromAddress++;
				n_datasets--;
			}		
		}
	}
}

void sgb_set_ATFlist()
{
	int i;
	sgb_render_screen();
	
	byte *fromAddress = sgb_screenbuffer;
	byte *toAddress	= sgb_ATF_list;
	
	for(i=0;i<45*20*18;i+=4)
	{
		*toAddress++ = (*fromAddress>>6)&3;
		*toAddress++ = (*fromAddress>>4)&3;
		*toAddress++ = (*fromAddress>>2)&3;
		*toAddress++ = *fromAddress&3;
		++fromAddress;
	}
}

void sgb_chr_trn()
{
	int address;
	
	sgb_render_screen();	
	// Are they the first 128 tiles, or the second 128 set?
	// This is marked in the first bit of the second byte of the command
	address = (sgb_buffer[1] & 1) * (128*32);
	memcpy(&sgb_borderchar[address], sgb_screenbuffer, 128 * 32);

}

void sgb_pct_trn()
{
	int i, j;
	
	sgb_render_screen();
		
	// save info about the tiles
	memcpy(sgb_border, sgb_screenbuffer, 2048);
	
	// palettes
	j=2048;
	for(i = 64; i < 128; i++,j+=2) 
		sgb_palette[i] = (sgb_screenbuffer[j]|(sgb_screenbuffer[j+1]<<8));
		
	sgb_palette[64] = sgb_palette[80] = sgb_palette[96] = sgb_palette[112] = sgb_palette[0];
	
	if(sgb_palette[113] == 0xf09f && sgb_palette[125] == 0) // fix for Ohasta Yamachan & Reimondo
		sgb_palette[113] = sgb_palette[125] = sgb_palette[0];

	sgb_render_border();
}

void sgb_sound_transfer()
{
// 	// used by (among others):
// 	// *Animaniacs
// 	// *Donkey Kong (sends 4 bytes to 0x4B08 ?)		
// 	// *Hunchback of Notre Dame
// 	// *Pocahontas		
// 	// *Toy Story
// 	  
// 	//sgb_render_screen();
// 		
// 	int data_len = sgb_screenbuffer[0]|(sgb_screenbuffer[1]<<8);
// 	unsigned short data_address = sgb_screenbuffer[2]|(sgb_screenbuffer[3]<<8);
// 					
// 	if(data_address >= 0x2B00 && data_address < 0x4B00) // APU-RAM Sound Score Area
// 		memcpy(sgb_sound_score+(data_address-0x2B00),sgb_screenbuffer+4,data_len);
//   /* else
// 	{
// 	  char buffer[100];
// 	  sprintf(buffer,"%x,%x",data_address,data_len);
// 	  debug_print(buffer);		 
// 	}*/
}

byte sgb_execute_command()
{
	byte r = 0;
	sgb_command = sgb_buffer[0] >> 3;
	switch(sgb_command)
	{
	case 0x00: // PAL01: Set SGB Palette 0,1 Data
		DPRINT("Set Palette 0, 1");
		set_palette(0,1);
	break;
	case 0x01: // PAL23: Set SGB Palette 2,3 Data
		DPRINT("Set Palette 2, 3");
		set_palette(2,3);
	break;	
	case 0x02: // PAL03: Set SGB Palette 0,3 Data
		DPRINT("Set Palette 0, 3");
		set_palette(0,3);
	break;	  
	case 0x03: // PAL12: Set SGB Palette 1,2 Data
		DPRINT("Set Palette 1, 2");
		set_palette(1,2);
	break;	  
	case 0x04: // ATTR_BLK: "Block" Area Designation Mode 
		DPRINT("ATTR_BLK");
		sgb_attribute_block();
	break;
	case 0x05: // ATTR_LIN: "Line" Area Designation Mode 
		DPRINT("ATTR_LIN");
		sgb_attribute_line();
	break;
	case 0x06: // ATTR_DIV: "Divide" Area Designation Mode 
		DPRINT("ATTR_DIV");
		sgb_attribute_divide();
	break;	
	case 0x07: // ATTR_CHR: "1CHR" Area Designation Mode 
		DPRINT("ATTR_CHR");
		sgb_attribute_character();
	break;	
	case 0x08: // SOUND: Sound On/Off 
	/*	if(sgb_buffer[4]&1)
		{
			FILE* ff = fopen("saved.dat","w");
			fwrite(sgb_sound_score,1,8192,ff);
			fclose(ff);
		}*/

		//music score commands - sgb_buffer[4]
		// 0x00 - stop music / don't play
		// 0x01 - play music
		// 0x02 - ???
		// 0x80 - go to beginning ?????
		// 0x81 - go to beginning and start playing ?????			
	break;	  
	case 0x09: // SOU_TRN: Transfer Sound PRG/DATA
		DPRINT("Sound transfer");
		sgb_sound_transfer();
	break;	  
	case 0x0A: // PAL_SET: Set SGB Palette Indirect
		DPRINT("PAL_SET");
		copy_palette();
	break;	
	case 0x0B: // PAL_TRN: Set System Color Palette Data
		DPRINT("PAL_TRN");
		init_mem_palette();
	break;	
	case 0x0C: // ATRC_EN: Enable/disable Attraction Mode
		DPRINT("ATC_EN");
		sgb_attraction_mode = !(sgb_buffer[1]&1); // but what is it used for? :)
	break;	 
	case 0x0D: // TEST_EN: Speed Function	
	break;
	case 0x0E: // ICON_EN: SGB Function
		// sgb_buffer[1] bit 0 - use SGB built in palettes
		//not emulated...
		// sgb_buffer[1] bit 1 - allow controller set up screen
		//not emulated...
		// sgb_buffer[1] bit 2 - stop receiving SGB packets
		DPRINT("ICON_EN");
		sgb_file_transfer = !(sgb_buffer[1]&4); 
	break;
	case 0x0F: // DATA_SND: SUPER NES WRAM Transfer 1
		//debug_print("SNES WRAM transfer 1 !");
	break;
	case 0x10: // DATA_TRN: SUPER NES WRAM Transfer 2
		//debug_print("SNES WRAM transfer 2 !");	
	break;
	case 0x11: // MLT_REG: Controller 2 Request
		sgb_multiplayer_req();
		// set the response
		r = sgb_nextcontrol;
		DPRINT2("Multiplayer: response %x=%x", RI_P1, sgb_nextcontrol);
	break;	 
	case 0x12: // JUMP: Set SNES Program Counter
		//debug_print("Not emulated: \nProgram is trying to use the SNES CPU!");
	break;
	case 0x13: // CHR_TRN: Transfer Character Font Data	
		DPRINT("CHR_TRN");
		sgb_chr_trn();
	break;
	case 0x14: // PCT_TRN: Set Screen Data Color Data
		DPRINT("PCT_PRN");
		sgb_pct_trn();
	break;
	case 0x15: // ATTR_TRN: Set Attribute from ATF
		DPRINT("ATTR_TRN");
		sgb_set_ATFlist();
	break;
	case 0x16: // ATTR_SET: Set Data to ATF
		DPRINT("ATTR_SET");
		sgb_setATF(sgb_buffer[1] & 0x3F);
		if(sgb_buffer[1]&0x40)
			sgb_mask = 0;
	break;
	case 0x17: // MASK_EN: Game Boy Window Mask
// 	{
// 		int old_mask = sgb_mask;
// 		sgb_mask = sgb_buffer[1] & 0x03;	  
// 		switch(sgb_mask)
// 		{
// 			case 0:
// 				GB->skip_frame = 1;
// 			break;
// 			case 1:
// 				if(mix_frames && !old_mask)
// 				{
// 					if(GB->gfx_bit_count == 16)
// 					{
// 						copy_line16((unsigned short*)GB->gfx_buffer,(unsigned short*)GB->gfx_buffer_old,160*144);
// 					} else
// 					{
// 						copy_line32((unsigned long*)GB->gfx_buffer,(unsigned long*)GB->gfx_buffer_old,160*144);
// 					}
// 
// 					draw_screen();
// 				}
// 			break;
// 			case 2:
// 				fill_gfx_buffers(0UL);	
// 											 
// 				draw_screen();
// 			break;
// 			case 3:			 
// 				unsigned short c = sgb_palette[0];
// 				if(GB->gfx_bit_count==16)
// 					fill_gfx_buffers(gfx_pal16[c]|(gfx_pal16[c]<<16));
// 				else
// 					fill_gfx_buffers(gfx_pal32[c]);					
// 										  
// 				draw_screen();			 
// 			break;
// 		}
// 	}
	break;
	case 0x18: // OBJ_TRN: Super NES OBJ Mode
		//no games known to use this feature
		DPRINT("OBJ_TRN");
	break;	 
	case 0x19: // ???????: Unknown command
	{
		//sgb_buffer[1], bit 0 enable/disable something ? 
		/*
		char buffer[100];
		sprintf(buffer,"%x",sgb_buffer[1]);
		debug_print(buffer);  
		*/
		// used by at least Adventures of Lolo, Legend of Zelda DX, Desert Strike
		// Donkey Kong, Donkey Kong Land, Killer Instinct, Kirby 2, Kirby's Star Stacker,...
		DPRINT("UNKNOWN 0x19");
	}
	break;
	default:
  /* {
		char buffer[30];
		sprintf(buffer,"Unknown SGB command: %X",sgb_command);
		debug_print(buffer);
	}*/
	break;									  
	}

	memset(sgb_buffer, 0, 16 * 7);
	return r;
}

byte sgb_transfer(byte bdata)
{
	byte r = 0;
	bdata = bdata&0x30;
	switch(sgb_state)
	{
	case SGB_NONE:
		if(bdata == 0) 
		{
			sgb_state = SGB_RESET;
			cycles_SGB = CYCLES_SGB_TIMEOUT;
		} else 
		if(bdata == 0x30) 
		{
			if(sgb_multiplayer) 
			{
				if((sgb_readingcontrol & 7) == 7) 
				{
					sgb_readingcontrol = 0;

					sgb_nextcontrol--;					
					if(sgb_fourplayers) 
					{
						if(sgb_nextcontrol == 0x0B)
							sgb_nextcontrol = 0x0F;
					} else 
					{
						if(sgb_nextcontrol == 0x0D)
							sgb_nextcontrol = 0x0F;
					}
				} else 
					sgb_readingcontrol &= 3;
			}					
			cycles_SGB = 0;		
		} else 
		{
			if(bdata == 0x10)
				sgb_readingcontrol |= 0x02;
			else if(bdata == 0x20)
				sgb_readingcontrol |= 0x01;
			cycles_SGB = 0;		
		}
	break;
	case SGB_RESET:
		if(bdata == 0x30) 
		{
			sgb_state = SGB_PACKET_TRANSMIT;
			sgb_index = 0;
			bits_received = 0;
			cycles_SGB = CYCLES_SGB_TIMEOUT;
		} else if(bdata == 0x00) 
		{
			cycles_SGB = CYCLES_SGB_TIMEOUT;
			sgb_state = SGB_RESET;
		} else 
		{
			sgb_state = SGB_NONE;
			cycles_SGB = 0;
		}
	 break;
	 case SGB_PACKET_TRANSMIT:
		 if(bdata == 0) 
		 {
			 sgb_state = SGB_RESET;
			 cycles_SGB = 0;
		 } else if(bdata == 0x30)
		 {
			 if(bits_received == 128) 
			 {
				 bits_received = 0;
				 sgb_index = 0;
				 ++packets_received;
				 cycles_SGB = 0;
				 if(packets_received == (sgb_buffer[0] & 7)) 
				 {
					 r = sgb_execute_command();
					 packets_received = 0;
					 sgb_state = SGB_NONE;
					 cycles_SGB = 0;
				 }
			 } else 
			 {
				 if(bits_received < 128) 
				 {
					 sgb_buffer[packets_received * 16 + sgb_index] >>= 1;
					 sgb_buffer[packets_received * 16 + sgb_index] |= bit_received;
					 bits_received++;
					 if(!(bits_received & 7)) 
						 ++sgb_index;
					 cycles_SGB = CYCLES_SGB_TIMEOUT;
				 }
			 }
		 } else 
		 {
			 if(bdata == 0x20)
				 bit_received = 0x00;
			 else
				 bit_received = 0x80;
			 cycles_SGB = CYCLES_SGB_TIMEOUT;
		 }
		 sgb_readingcontrol = 0;
	 break;
	 default:
		 sgb_state = SGB_NONE;
		 cycles_SGB = 0;
	 break;
	 }
	return r;
}

