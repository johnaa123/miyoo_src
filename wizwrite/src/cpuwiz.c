/*  
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


#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include "cpuwiz.h"

 /*
 * quick tool to set LCD timings for Wiz
 * (c) notaz, 2009
 * code dedicated to public domain.
 *
 * HTOTAL:    X VTOTAL:  341
 * HSWIDTH:   1 VSWIDTH:   0
 * HASTART:  37 VASTART:  17
 * HAEND:   277 VAEND:   337
 *
 * 120Hz
 * pcd  8, 447: + 594us
 * pcd  9, 397: +  36us
 * pcd 10, 357: - 523us
 * pcd 11, 325: +1153us
 *
 * 'lcd_timings=397,1,37,277,341,0,17,337;clkdiv0=9'
 *
 * set LCD timings based on preformated string (see usage() below for params).
 * returns 0 on success.
 */
static int 
pollux_dpc_set(volatile unsigned short *memregs, const char *str)
{
	int timings[8], have_timings = 0;
	int pcd = 0, have_pcd = 0;
	const char *p;
	int i, ret;

	if (str == NULL)
		return -1;

	p = str;
	while (1)
	{
		if (strncmp(p, "lcd_timings=", 12) == 0)
		{
			int c;
			p += 12;
			ret = sscanf(p, "%d,%d,%d,%d,%d,%d,%d,%d",
				&timings[0], &timings[1], &timings[2], &timings[3],
				&timings[4], &timings[5], &timings[6], &timings[7]);
			if (ret != 8)
				break;
			/* skip seven commas */
			for (c = 0; c < 7 && *p != 0; p++)
				if (*p == ',')
						c++;
			if (c != 7)
				break;
			/* skip last number */
			while ('0' <= *p && *p <= '9')
				p++;
			have_timings = 1;
		}
		else if (strncmp(p, "clkdiv0=", 8) == 0)
		{
			char *ep;
			p += 8;
			pcd = strtoul(p, &ep, 10);
			if (p == ep)
				break;
			p = ep;
			have_pcd = 1;
		}
		else
			break;

		while (*p == ';' || *p == ' ')
			p++;
		if (*p == 0)
			goto parse_done;
	}

	fprintf(stderr, "dpc_set parser: error at '%s'\n", p);
	return -1;

parse_done:
	/* some validation */
	if (have_timings)
	{
		for (i = 0; i < 8; i++)
			if (timings[i] & ~0xffff) {
				fprintf(stderr, "dpc_set: invalid timing %d: %d\n", i, timings[i]);
				return -1;
			}
	}

	if (have_pcd)
	{
		if ((pcd - 1) & ~0x3f) {
			fprintf(stderr, "dpc_set: invalid clkdiv0: %d\n", pcd);
			return -1;
		}
	}

	/* write */
	if (have_timings)
	{
		for (i = 0; i < 8; i++)
			memregs[(0x307c>>1) + i] = timings[i];
	}

	if (have_pcd)
	{
		int tmp;
		pcd = (pcd - 1) & 0x3f;
		tmp = memregs[0x31c4>>1];
		memregs[0x31c4>>1] = (tmp & ~0x3f0) | (pcd << 4);
	}
	return 0;
}

/* Declaracion de variables globales */
#define SYS_CLK_FREQ 27

# if 0 //ZX: DISABLE !
static          unsigned int    gp2x_dev = 0;
static volatile unsigned short *gp2x_memregs;
static volatile unsigned long  *gp2x_memregl;
# endif

void 
cpu_init()
{
# if 0 //ZX: DISABLE !
  gp2x_dev = open("/dev/mem", O_RDWR);
  gp2x_memregl = (unsigned long  *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, gp2x_dev, 0xc0000000);
  gp2x_memregs = (unsigned short *)gp2x_memregl;

  // Get pollux_dpc_set variable 
  const char* dpc_set_str = getenv("pollux_dpc_set");
  if (! dpc_set_str) {
    // default timing to avoid diagonal issue
    dpc_set_str = "lcd_timings=397,1,37,277,341,0,17,337;dpc_clkdiv0=9";
  }
  pollux_dpc_set(gp2x_memregs, dpc_set_str);
# endif
}

void 
cpu_deinit()
{
# if 0 //ZX: DISABLE
  cpu_set_clock( GP2X_DEF_CLOCK );
  munmap((void *)gp2x_memregl, 0x10000);
  close(gp2x_dev);
# endif
  fcloseall();
  sync();
}

# if 0 //ZX: DISABLE
static void 
loc_set_FCLK(unsigned int clock_in_mhz)
{
  unsigned int v;
  unsigned int mdiv, pdiv, sdiv = 0;
  pdiv = 9;
  mdiv = (clock_in_mhz * pdiv) / SYS_CLK_FREQ;
  mdiv &= 0x3ff;
  v = (pdiv<<18) | (mdiv<<8) | sdiv;

  gp2x_memregl[0xf004>>2] = v;
  gp2x_memregl[0xf07c>>2] |= 0x8000;

  while (gp2x_memregl[0xf07c>>2] & 0x8000) ;
}
# endif

static unsigned int loc_clock_in_mhz = GP2X_DEF_CLOCK;

void 
cpu_set_clock(unsigned int clock_in_mhz)
{
# if 0 //ZX: DISABLE
  if (clock_in_mhz == loc_clock_in_mhz) return;
  loc_clock_in_mhz = clock_in_mhz;
  
  loc_set_FCLK(clock_in_mhz);
# endif
}

unsigned int
cpu_get_clock()
{
  return loc_clock_in_mhz;
}

