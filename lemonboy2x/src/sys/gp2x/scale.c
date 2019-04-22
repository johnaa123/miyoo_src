#ifdef WIZ_BUILD

#include "../common.h"
#define IN_STRIDE 320

#else

#include <stdio.h>
#define IN_STRIDE 240

#endif

#define IN_WIDTH 240
#define IN_HEIGHT 160
#define IN_BPP 2
#define IN_SIZE (IN_WIDTH * IN_HEIGHT * IN_BPP)
#define OUT_WIDTH 320
#define OUT_HEIGHT 213
#define OUT_HEIGHT_WHOLE 240
#define OUT_BPP 2
#define OUT_SIZE (OUT_WIDTH * OUT_HEIGHT_WHOLE * OUT_BPP)

static unsigned int *buf_in, *buf_out;
typedef unsigned char color_t;
typedef color_t colors_t[3];

#ifdef TESTBUILD

#include <stdlib.h>
#include <string.h>

#undef IN_STRIDE
#define IN_STRIDE 240

static int
SDL_GetTicks(void)
{
	return 0;
}

static void
readit(void)
{
	FILE *f;

	f = fopen("in.rgb", "r");
	//fread(buf_in, sizeof(buf_in), 1, f);
	fread(buf_in, IN_SIZE, 1, f);
	fclose(f);
}

static void
writeit(void)
{
	FILE *f;

	f = fopen("out.rgb", "w");
	fwrite(buf_out, OUT_SIZE, 1, f);
	fclose(f);
}

/*
static void
writeit_b(void)
{
	FILE *f;

	f = fopen("out_b.rgb", "w");
	fwrite(b, sizeof(b), 1, f);
	fclose(f);
}
*/
#endif

static color_t *
get(int x, int y, colors_t out)
{
	int pos;
	unsigned int value;

	pos = (y * IN_STRIDE + x) * IN_BPP;

	/* select `int` from buffer */
	value = buf_in[pos / (sizeof(int))];

	/* select pixel from buffer */
	switch (pos % sizeof(int))
	{
	case 0:
		value = value & 0xFFFF;
		break;
	case 2:
		value = (value >> 16) & 0xFFFF;
		break;
	}

	/* select color from pixel */
	out[0] = value & 0x1F;
	out[1] = (value >> 5) & 0x3F;
	out[2] = (value >> 11) & 0x1F;

	return out;
}

static void
set(int x, int y, colors_t color)
{
	int pos;
	unsigned int value, tmp;

	pos = (y * OUT_WIDTH + x) * OUT_BPP;

	/* select `int` from buffer */
	tmp = buf_out[pos / sizeof(int)];

	/* select color from pixel */
	value = ((color[0] & 0x1F) <<  0) |
		((color[1] & 0x3F) <<  5) |
		((color[2] & 0x1F) << 11);

	/* select pixel from buffer */
	switch (pos % sizeof(int))
	{
	case 0:
		tmp = tmp & 0xFFFF0000UL;
		tmp = tmp | value;
		break;
	case 2:
		tmp = tmp & 0xFFFF;
		tmp = tmp | (value << 16);
		break;
	}

	/* put */
	buf_out[pos / sizeof(int)] = tmp;
}

static
void cp0(int dx, int dy, int sx, int sy)
{
	colors_t colors;

	set(dx, dy, get(sx, sy, colors));
}

static
void cpx(int dx, int dy, int sx, int sy, int zx)
{
	int color;
	colors_t colorsa, colorsb, colors;

	get(sx, sy, colorsa);
	get(sx + zx, sy, colorsb);
	for (color = 0; color < 3; color ++)
	{
		int val;

		val = 5 * colorsa[color] + 3 * colorsb[color];
		colors[color] = (val + 4) / 8;
	}
	set(dx, dy, colors);
}

static
void cpy(int dx, int dy, int sx, int sy, int zy)
{
	int color;
	colors_t colorsa, colorsb, colors;

	get(sx, sy, colorsa);
	get(sx, sy + zy, colorsb);
	for (color = 0; color < 3; color ++)
	{
		int val;

		val = 5 * colorsa[color] + 3 * colorsb[color];
		colors[color] = (val + 4) / 8;
	}
	set(dx, dy, colors);
}

static
void cpz(int dx, int dy, int sx, int sy, int zx, int zy)
{
	int color;
	colors_t colorsa, colorsb, colorsc, colorsd, colors;

	get(sx, sy, colorsa);
	get(sx, sy + zy, colorsb);
	get(sx + zx, sy, colorsc);
	get(sx + zx, sy + zy, colorsd);
	for (color = 0; color < 3; color ++)
	{
		int val;


		val = 25 * colorsa[color]
			+ 15 * colorsb[color]
			+ 15 * colorsc[color]
			+ 9 * colorsd[color];
		colors[color] = (val + 32) / 64;
	}
	set(dx, dy, colors);
}

static int counter = 0;

void upscale_aspect(unsigned short *dst, unsigned short *src)
{
	int x, y;
	int dx, dy;
	int sx, sy;
	unsigned int start;

	buf_out = (void *)(dst + 320 * 13);
	buf_in = (void *)src;
	start = SDL_GetTicks();

	sy = 0;
	dy = 0;
	for (y = 0; y < IN_HEIGHT / 3; y ++)
	{
		sx = 0;
		dx = 0;
		for (x = 0; x < IN_WIDTH / 3; x++)
		{
			cp0(dx + 0, dy + 0, sx + 0, sy + 0        );
			cpx(dx + 1, dy + 0, sx + 1, sy + 0, -1    );
			cpx(dx + 2, dy + 0, sx + 1, sy + 0,  1    );
			cp0(dx + 3, dy + 0, sx + 2, sy + 0        );

			cpy(dx + 0, dy + 1, sx + 0, sy + 1,     -1);
			cpz(dx + 1, dy + 1, sx + 1, sy + 1, -1, -1);
			cpz(dx + 2, dy + 1, sx + 1, sy + 1,  1, -1);
			cpy(dx + 3, dy + 1, sx + 2, sy + 1,     -1);

			cpy(dx + 0, dy + 2, sx + 0, sy + 1,      1);
			cpz(dx + 1, dy + 2, sx + 1, sy + 1, -1,  1);
			cpz(dx + 2, dy + 2, sx + 1, sy + 1,  1,  1);
			cpy(dx + 3, dy + 2, sx + 2, sy + 1,      1);

			cp0(dx + 0, dy + 3, sx + 0, sy + 2        );
			cpx(dx + 1, dy + 3, sx + 1, sy + 2, -1    );
			cpx(dx + 2, dy + 3, sx + 1, sy + 2,  1    );
			cp0(dx + 3, dy + 3, sx + 2, sy + 2        );
			sx += 3;
			dx += 4;

		}
		sy += 3;
		dy += 4;
	}

	/*
	 * last line
	 */
	sy = IN_HEIGHT - 1;
	dy = OUT_HEIGHT - 1;
	sx = 0;
	dx = 0;
	for (x = 0; x < IN_WIDTH / 3; x++)
	{
		cp0(dx + 0, dy + 0, sx + 0, sy + 0        );
		cpx(dx + 1, dy + 0, sx + 1, sy + 0, -1    );
		cpx(dx + 2, dy + 0, sx + 1, sy + 0,  1    );
		cp0(dx + 3, dy + 0, sx + 2, sy + 0        );
		sx += 3;
		dx += 4;

	}

	if (counter++ > 100)
	{
		printf("%d\n", SDL_GetTicks() - start);
		counter = 0;
	}
}

#ifndef WIZ_BUILD
#endif

#ifdef TESTBUILD
int
main(void)
{
	buf_in = malloc(IN_SIZE);
	memset(buf_in, 0, IN_SIZE);
	buf_out = malloc(OUT_SIZE);
	memset(buf_out, 0, OUT_SIZE);
	readit();
	//convert();
	upscale_aspect((void *)buf_out, (void *)buf_in);
	writeit();
	//convert_b();
	//writeit_b();
	return 0;
}
#endif
