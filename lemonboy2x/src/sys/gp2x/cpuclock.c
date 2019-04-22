/*  cpuctrl for GP2X
    Copyright (C) 2005  Hermes/PS2Reality 
	the gamma-routine was provided by theoddbot
	parts (c) Rlyehs Work & (C) 2006 god_at_hell 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/


#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

#define SYS_CLK_FREQ 7372800
volatile unsigned short *MEM_REG;
unsigned long   gp2x_dev;
unsigned short *gp2x_memregs;

void Set920Clock(int MHZ)
{
#if 0
	printf("Set 920 clock at %d\n", MHZ);
	
	if(!gp2x_dev) gp2x_dev = open("/dev/mem", O_RDWR);
	gp2x_memregs=(unsigned short *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, gp2x_dev, 0xc0000000);

	// Init CPU control
	MEM_REG = &gp2x_memregs[0];

	// Set 920t clock
	unsigned v;
	unsigned mdiv,pdiv=3,scale=0;
	MHZ *= 1000000;
	mdiv = (MHZ*pdiv) / SYS_CLK_FREQ;
	mdiv = ((mdiv-8)<<8) & 0xff00;
	pdiv = ((pdiv-2)<<2) & 0xfc;
	scale &= 3;
	v = mdiv | pdiv | scale;
	MEM_REG[0x910>>1] = v;
	
	while(MEM_REG[0x0902>>1] & 1);    //Wait for the frequentie to be ajused
#endif
}

// normal =	set_RAM_Timings(8, 16, 3, 8, 8, 8, 8);
void set_RAM_Timings(int tRC, int tRAS, int tWR, int tMRD, int tRFC, int tRP, int tRCD)
{
#if 0
	tRC -= 1; tRAS -= 1; tWR -= 1; tMRD -= 1; tRFC -= 1; tRP -= 1; tRCD -= 1; // ???
	MEM_REG[0x3802>>1] = ((tMRD & 0xF) << 12) | ((tRFC & 0xF) << 8) | ((tRP & 0xF) << 4) | (tRCD & 0xF);
	MEM_REG[0x3804>>1] = /*0x9000 |*/ ((tRC & 0xF) << 8) | ((tRAS & 0xF) << 4) | (tWR & 0xF);
#endif
}


/*
void set_gamma(int g100)
{
	float gamma = (float) g100 / 100;
	int i;
	printf ("set gamma = %f\r\n",gamma);
	gamma = 1/gamma;

    //enable gamma
    MEM_REG[0x2880>>1]&=~(1<<12);

    MEM_REG[0x295C>>1]=0;
    for(i=0; i<256; i++)
    {
		unsigned char g;
        unsigned short s;
        g =(unsigned char)(255.0*pow(i/255.0,gamma));
        s = (g<<8) | g;
		MEM_REG[0x295E>>1]= s;
        MEM_REG[0x295E>>1]= g;
    }
}
*/

