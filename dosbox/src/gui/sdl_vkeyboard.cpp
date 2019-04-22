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
 
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_gfxPrimitives_font.h>

#include "sdl_vkeyboard_font.h"
#include "sdl_vkeyboard_image.h"

typedef struct {
	int x, y;
	int w, h;
	int sym;
	bool pressed;
} KEY;

KEY keyvalues[5][21] = {
{
	{4, 5, 13, 14, SDLK_F1},          {17, 5, 13, 14, SDLK_F2}, {33, 5, 13, 14, SDLK_ESCAPE}, {46, 5, 13, 14, SDLK_BACKQUOTE},
	{59, 5, 13, 14, SDLK_1},          {72, 5, 13, 14, SDLK_2},  {85, 5, 13, 14, SDLK_3},      {98, 5, 13, 14, SDLK_4},
	{111, 5, 13, 14, SDLK_5},         {124, 5, 13, 14, SDLK_6}, {137, 5, 13, 14, SDLK_7},     {150, 5, 13, 14, SDLK_8},
	{163, 5, 13, 14, SDLK_9},         {176, 5, 13, 14, SDLK_0}, {189, 5, 13, 14, SDLK_MINUS}, {202, 5, 13, 14, SDLK_EQUALS},
	{215, 5, 13, 14, SDLK_BACKSPACE}, {231, 5, 13, 14, 0},      {244, 5, 13, 14, 0},          {257, 5, 13, 14, 0}, 
	{270, 5, 13, 14, SDLK_KP_DIVIDE}
},
{
	{4, 19, 13, 14, SDLK_F3},  {17, 19, 13, 14, SDLK_F4},           {33, 19, 20, 14, SDLK_TAB},           {53, 19, 13, 14, SDLK_q},
	{66, 19, 13, 14, SDLK_w},  {79, 19, 13, 14, SDLK_e},            {92, 19, 13, 14, SDLK_r},             {105, 19, 13, 14, SDLK_t},
	{118, 19, 13, 14, SDLK_y}, {131, 19, 13, 14, SDLK_u},           {144, 19, 13, 14, SDLK_i},            {157, 19, 13, 14, SDLK_o},
	{170, 19, 13, 14, SDLK_p}, {183, 19, 13, 14, SDLK_LEFTBRACKET}, {196, 19, 13, 14, SDLK_RIGHTBRACKET}, {209, 19, 19, 14, SDLK_BACKSLASH},
	{0, 0, 0, 0, 0},           {231, 19, 13, 14, SDLK_INSERT},      {244, 19, 13, 14, SDLK_HOME},         {257, 19, 13, 14, SDLK_PAGEUP},
	{270, 19, 13, 14, SDLK_KP_MULTIPLY}
},
{
	{4, 33, 13, 14, SDLK_F5},          {17, 33, 13, 14, SDLK_F6},      {33, 33, 24, 14, SDLK_CAPSLOCK}, {57, 33, 13, 14, SDLK_a}, 
	{70, 33, 13, 14, SDLK_s},          {83, 33, 13, 14, SDLK_d},       {96, 33, 13, 14, SDLK_f},        {109, 33, 13, 14, SDLK_g}, 
	{122, 33, 13, 14, SDLK_h},         {135, 33, 13, 14, SDLK_j},      {148, 33, 13, 14, SDLK_k},       {161, 33, 13, 14, SDLK_l}, 
	{174, 33, 13, 14, SDLK_SEMICOLON}, {187, 33, 13, 14, SDLK_QUOTE},  {200, 33, 28, 14, SDLK_RETURN},  {0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},                   {231, 33, 13, 14, SDLK_DELETE}, {244, 33, 13, 14, SDLK_END},     {257, 33, 13, 14, SDLK_PAGEDOWN},
	{270, 33, 13, 14, SDLK_KP_MINUS}
},
{
	{4, 47, 13, 14, SDLK_F7},      {17, 47, 13, 14, SDLK_F8},      {33, 47, 31, 14, SDLK_LSHIFT}, {64, 47, 13, 14, SDLK_z},
	{77, 47, 13, 14, SDLK_x},      {90, 47, 13, 14, SDLK_c},       {103, 47, 13, 14, SDLK_v},     {116, 47, 13, 14, SDLK_b}, 
	{129, 47, 13, 14, SDLK_n},     {142, 47, 13, 14, SDLK_m},      {155, 47, 13, 14, SDLK_COMMA}, {168, 47, 13, 14, SDLK_PERIOD},
	{181, 47, 13, 14, SDLK_SLASH}, {194, 47, 34, 14, SDLK_RSHIFT}, {0, 0, 0, 0, 0},               {0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0},               {231, 47, 13, 14, 0},           {244, 47, 13, 14, SDLK_UP},    {257, 47, 13, 14, 0},
	{270, 47, 13, 14, SDLK_KP_PLUS}
},
{
	{4, 61, 13, 14, SDLK_F9},      {17, 61, 13, 14, SDLK_F10},     {33, 61, 20, 14, SDLK_LCTRL},  {65, 61, 20, 14, SDLK_LALT},
	{85, 61, 90, 14, SDLK_SPACE},  {0, 0, 0, 0, 0},                {0, 0, 0, 0, 0},               {0, 0, 0, 0, 0}, 
	{0, 0, 0, 0, 0},               {0, 0, 0, 0, 0},                {0, 0, 0, 0, 0},               {0, 0, 0, 0, 0}, 
	{175,61, 20, 14, SDLK_RALT},   {208, 61, 20, 14, SDLK_RCTRL},  {0, 0, 0, 0, 0},               {0, 0, 0, 0, 0}, 
	{0, 0, 0, 0, 0},               {231, 61, 13, 14, SDLK_LEFT},   {244, 61, 13, 14, SDLK_DOWN},  {257, 61, 13, 14, SDLK_RIGHT},
	{270, 61, 13, 14, SDLK_KP_PERIOD}
}
};

const char *keynames[4][5][21] = {
{
	{"\x82", "\x83", "\x80",     "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-",        "=",  "\x8e", "", "",     "", "/"},
	{"\x84", "\x85", "\x90\x91", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]",        "\\", "",     "\x9c", "\x9e", "\xa0", "*"},
	{"\x86", "\x87", "\x92\x93", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "\x94\x95", "",   "",     "\x9d", "\x9f", "\xa1", "-"},
	{"\x88", "\x89", "\x96\x97", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "\x96\x97", "",  "",   "",     "", "\x18", "", "+"},
	{"\x8a", "\x8b", "\x98\x99", "\x9a\x9b",  "",  "",  "",  "",  "",  "",  "",  "",  "\x9a\x9b", "\x98\x99", "", "", "", "\x1b", "\x19", "\x1a", "."},
},
{
	{"\x82", "\x83", "\x80",     "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-",        "=",  "\x8e", "", "",     "", "/"},
	{"\x84", "\x85", "\x90\x91", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]",        "\\", "",     "\x9c", "\x9e", "\xa0", "*"},
	{"\x86", "\x87", "\x92\x93", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "\x94\x95", "",   "",     "\x9d", "\x9f", "\xa1", "-"},
	{"\x88", "\x89", "\x96\x97", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "\x96\x97", "",  "",   "",     "", "\x18", "", "+"},
	{"\x8a", "\x8b", "\x98\x99", "\x9a\x9b",  "",  "",  "",  "",  "",  "",  "",  "",  "\x9a\x9b", "\x98\x99", "", "", "", "\x1b", "\x19", "\x1a", "."},
},
{
	{"\x82", "\x83", "\x80",     "~", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_",        "+",  "\x8e", "", "",     "", "/"},
	{"\x84", "\x85", "\x90\x91", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}",        "|", "",     "\x9c", "\x9e", "\xa0", "*"},
	{"\x86", "\x87", "\x92\x93", "A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"", "\x94\x95", "",   "",     "\x9d", "\x9f", "\xa1", "-"},
	{"\x88", "\x89", "\x96\x97", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", "\x96\x97", "",  "",   "",     "", "\x18", "", "+"},
	{"\x8a", "\x8b", "\x98\x99", "\x9a\x9b",  "",  "",  "",  "",  "",  "",  "",  "",  "\x9a\x9b", "\x98\x99", "", "", "", "\x1b", "\x19", "\x1a", "."},
},
{
	{"\x82", "\x83", "\x80",     "~", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_",        "+",  "\x8e", "", "",     "", "/"},
	{"\x84", "\x85", "\x90\x91", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "{", "}",        "|", "",     "\x9c", "\x9e", "\xa0", "*"},
	{"\x86", "\x87", "\x92\x93", "a", "s", "d", "f", "g", "h", "j", "k", "l", ":", "\"", "\x94\x95", "",   "",     "\x9d", "\x9f", "\xa1", "-"},
	{"\x88", "\x89", "\x96\x97", "z", "x", "c", "v", "b", "n", "m", "<", ">", "?", "\x96\x97", "",  "",   "",     "", "\x18", "", "+"},
	{"\x8a", "\x8b", "\x98\x99", "\x9a\x9b",  "",  "",  "",  "",  "",  "",  "",  "",  "\x9a\x9b", "\x98\x99", "", "", "", "\x1b", "\x19", "\x1a", "."},
}
};

struct VKEYB_Block {
	SDL_Surface *surface;
	SDL_Surface *image; // keyboard image
	struct CURSOR {
		int x, y, w, h;
	} cursor;
	int x, y;
	bool capslock;
	bool shift;
	bool lctrl;
	bool move;
};

static VKEYB_Block vkeyb;

bool vkeyb_active = false; // vkeyb show flag
bool vkeyb_last = false; // vkeyb delete flag
static bool vkeyb_bg = true; // background on/off

void VKEYB_Init(int bpp)
{
	if(!vkeyb.surface) {
		vkeyb.surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 287, 80, bpp, 0, 0, 0, 0);
	}
	SDL_SetColorKey(vkeyb.surface, SDL_SRCCOLORKEY, SDL_MapRGBA(vkeyb.surface->format, 0, 0, 0, 0xFF));

	//IMG_Init(0);
	vkeyb.image = IMG_LoadBMP_RW(SDL_RWFromMem(vkeyboard_image, sizeof(vkeyboard_image))/*SDL_RWFromFile("keyboard.bmp", "rb")*/);
	if(!vkeyb.image) printf("err %s\n", IMG_GetError());

	gfxPrimitivesSetFont(keyfont, 8, 8);

	vkeyb.cursor.x = 2;
	vkeyb.cursor.y = 1;
	vkeyb.cursor.w = 8;
	vkeyb.cursor.h = 8;
	vkeyb.x = 16;
	vkeyb.y = 120;
	vkeyb.lctrl = false;
}

void VKEYB_Deinit()
{
	if(vkeyb.surface) SDL_FreeSurface(vkeyb.surface);
	//IMG_Quit();
}

void move_cursor(int dx, int dy)
{
	// move either keyboard itself or a cursor
	if(vkeyb.move) {
		vkeyb.x += dx*8;
		vkeyb.y += dy*8;
	} else {
		if(dx) {
			if(vkeyb.cursor.x + dx >= 0 && vkeyb.cursor.x + dx <= 20) {
				while(keyvalues[vkeyb.cursor.y][vkeyb.cursor.x+dx].w == 0) if(dx>0) dx++; else dx--;
			} else dx = 0;
		}
		if(dy) {
			if(vkeyb.cursor.y + dy >= 0 && vkeyb.cursor.y + dy <= 4) {
				while(keyvalues[vkeyb.cursor.y+dy][vkeyb.cursor.x+dx].w == 0) dx--;
			} else dy = 0;
		}

		vkeyb.cursor.x += dx;
		vkeyb.cursor.y += dy;
	}
}

void GFX_ForceUpdate(); // in sdlmain.cpp

int VKEYB_CheckEvent(SDL_Event *event)
{
	int ret = 1;
	bool update_screen = false;
	bool keystate = (event->type == SDL_KEYDOWN) ? true : false;

	if(keystate && event->key.keysym.sym == SDLK_TAB) {
		vkeyb_active ^= 1;
		if(!vkeyb_active) vkeyb_last = true;
		ret = 0;
		update_screen = true;
		goto _exit;
	}

	// keep track of CAPS LOCK
	if(keystate && event->key.keysym.sym == SDLK_CAPSLOCK) {
		vkeyb.capslock ^= 1;
	}

	if(!vkeyb_active) return 1;

	// move cursor
	if(keystate) {
		switch(event->key.keysym.sym) {
		case SDLK_UP:
			if(!vkeyb.lctrl) move_cursor(0, -1);
			ret = 0;
			update_screen = true;
			break;
		case SDLK_DOWN:
			if(!vkeyb.lctrl) move_cursor(0, 1);
			ret = 0;
			update_screen = true;
			break;
		case SDLK_LEFT:
			if(!vkeyb.lctrl) move_cursor(-1, 0);
			ret = 0;
			update_screen = true;
			break;
		case SDLK_RIGHT:
			if(!vkeyb.lctrl) move_cursor(1, 0);
			ret = 0;
			update_screen = true;
			break;
		}
	}

	// Y - move keyboard
	if(event->key.keysym.sym == SDLK_LSHIFT /*|| event->key.keysym.sym == SDLK_RSHIFT*/) {
		vkeyb.move = keystate;
		ret = 0;
		update_screen = true;
		goto _exit;
	}

	// X - toggle between transparent and opaque keyboard
	if(keystate && event->key.keysym.sym == SDLK_SPACE) {
		vkeyb_bg ^= 1;
		ret = 0;
		update_screen = true;
		goto _exit;
	}

	// treat sticky CAPS LOCK press as normal
	if(event->key.keysym.sym == SDLK_LALT && keyvalues[vkeyb.cursor.y][vkeyb.cursor.x].sym == SDLK_CAPSLOCK) {
		goto _capslock;
	}

	// A - normal keypress
	if(event->key.keysym.sym == SDLK_LCTRL) {
		// add clear all pressed keys, maybe send release messages
		vkeyb.lctrl = keystate;
	_capslock:
		event->key.keysym.sym = (SDLKey)keyvalues[vkeyb.cursor.y][vkeyb.cursor.x].sym;
		event->type = keystate ? SDL_KEYDOWN : SDL_KEYUP;
		event->key.state = keystate ? SDL_PRESSED : SDL_RELEASED;
		event->key.keysym.scancode = 0;
		event->key.keysym.mod = KMOD_NONE;
		event->key.keysym.unicode = 0;

		if(keystate && event->key.keysym.sym == SDLK_CAPSLOCK) {
			keyvalues[vkeyb.cursor.y][vkeyb.cursor.x].pressed ^= 1;
			vkeyb.capslock ^= 1; 
		} else
			keyvalues[vkeyb.cursor.y][vkeyb.cursor.x].pressed = keystate;

		if(event->key.keysym.sym == SDLK_LSHIFT || event->key.keysym.sym == SDLK_RSHIFT)
			vkeyb.shift = keystate;

		ret = 1;
		update_screen = true;
		goto _exit;
	}

	// B - sticky keypress
	if(keystate && event->key.keysym.sym == SDLK_LALT) {
		keyvalues[vkeyb.cursor.y][vkeyb.cursor.x].pressed ^= 1;
		event->key.keysym.sym = (SDLKey)keyvalues[vkeyb.cursor.y][vkeyb.cursor.x].sym;
		event->type = keyvalues[vkeyb.cursor.y][vkeyb.cursor.x].pressed ? SDL_KEYDOWN : SDL_KEYUP;
		event->key.state = keyvalues[vkeyb.cursor.y][vkeyb.cursor.x].pressed ? SDL_PRESSED : SDL_RELEASED;
		event->key.keysym.scancode = 0;
		event->key.keysym.mod = KMOD_NONE;
		event->key.keysym.unicode = 0;

		if(event->key.keysym.sym == SDLK_LSHIFT || event->key.keysym.sym == SDLK_RSHIFT)
			vkeyb.shift ^= 1;

		ret = 1;
		update_screen = true;
	}

_exit:
	if(update_screen) GFX_ForceUpdate();
	return ret;
}

void VKEYB_BlitVkeyboard(SDL_Surface *surface)
{
	if(!vkeyb_active) return;

	if(vkeyb_bg)
		SDL_BlitSurface(vkeyb.image, NULL, vkeyb.surface, NULL);
	else
		SDL_FillRect(vkeyb.surface, NULL, 0);

	KEY *cur = &keyvalues[vkeyb.cursor.y][vkeyb.cursor.x];

	// draw cursor
	boxRGBA(vkeyb.surface, 
			cur->x + cur->w-1, 
			cur->y, 
			cur->x, 
			cur->y + cur->h-1, 
			0xF0, 0x40, 0x10, 0xFF);

	// respect capslock and shift and their combinations
	int mod = (vkeyb.capslock ? 1 : 0) | (vkeyb.shift ? 2 : 0);

	// draw keys
	for(int y = 0; y < 5; y++)
		for(int x = 0; x < 21; x++) {

			if(vkeyb_bg)
				stringRGBA(vkeyb.surface, keyvalues[y][x].x + 2, keyvalues[y][x].y + 2, keynames[mod][y][x], 0xa, 0xe, 0xa, 0xFF);
			else {
				if(vkeyb.cursor.x != x || vkeyb.cursor.y != y)
					stringRGBA(vkeyb.surface, keyvalues[y][x].x + 3, keyvalues[y][x].y + 3, keynames[mod][y][x], 0x50, 0x20, 0x59, 0xFF);
				stringRGBA(vkeyb.surface, keyvalues[y][x].x + 2, keyvalues[y][x].y + 2, keynames[mod][y][x], 0xFF, 0xEE, 0xFF, 0xFF);
			}

			if(keyvalues[y][x].pressed) {
				KEY *pr = &keyvalues[y][x];
				rectangleRGBA(vkeyb.surface, pr->x + pr->w - 2, pr->y + 1, pr->x + 1, pr->y + pr->h - 2, 0x90, 0xE0, 0x10, 0xFF);
			}
		}

	SDL_Rect dest;
	dest.x = vkeyb.x;
	dest.y = vkeyb.y;

	SDL_BlitSurface(vkeyb.surface, 0, surface, &dest);
}

void VKEYB_CleanVkeyboard(SDL_Surface *surface)
{
	SDL_Rect dest;
	dest.x = vkeyb.x;
	dest.y = vkeyb.y;
	dest.w = 287;
	dest.h = 80;

	SDL_FillRect(surface, &dest, 0);
	vkeyb_last = false;
}
