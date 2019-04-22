#ifndef _PALETTE_H
#define _PALETTE_H

#include "defs.h"

void pal_lock(byte n);
byte pal_getcolor(int c, int r, int g, int b);
void pal_release(byte n);
void pal_expire();
void pal_set332();

#endif


