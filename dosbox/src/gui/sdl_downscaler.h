

#define COLORMIX32(A,B,C,D) ((((A) >> 2) & 0x3F3F3F3F) + (((B) >> 2) & 0x3F3F3F3F) + (((C) >> 2) & 0x3F3F3F3F) + (((D) >> 2) & 0x3F3F3F3F))
#define COLORMIX16(A,B,C,D) ((((A) >> 2) & 0x39E7) + (((B) >> 2) & 0x39E7) + (((C) >> 2) & 0x39E7) + (((D) >> 2) & 0x39E7))

#define DEFINE_GFX_DOWNSCALE(SX,SY,DX,DY,BPP) \
void GFX_Downscale_##SX##x##SY##_to_##DX##x##DY##_##BPP(SDL_Surface *src, SDL_Surface *dst)	\
{																				\
	Bit##BPP##u *Dest = (Bit##BPP##u *)dst->pixels;								\
	Bit##BPP##u *Src  = (Bit##BPP##u *)src->pixels;								\
	\
	int x,y;																	\
	Dest += (DY-SY/2)/2*DX;														\
	for(y = SY/2; y--;) {														\
		for(x = SX/2; x--;) {													\
			__builtin_prefetch(Dest + 4, 1);									\
			__builtin_prefetch(Src + 4, 0);										\
			*Dest++ = COLORMIX##BPP(Src[0], Src[1], Src[SX], Src[SX+1]);		\
			Src += 2;															\
		}																		\
		Src += SX;																\
	}																			\
}

DEFINE_GFX_DOWNSCALE(640, 400, 320, 240, 16)
DEFINE_GFX_DOWNSCALE(640, 480, 320, 240, 16)
DEFINE_GFX_DOWNSCALE(640, 400, 320, 240, 32)
DEFINE_GFX_DOWNSCALE(640, 480, 320, 240, 32)

void (* GFX_PDownscale)(SDL_Surface *, SDL_Surface *) = NULL;

#define GFX_PDOWNSCALE(A,B) \
{\
	if(GFX_PDownscale) {	\
		if(SDL_MUSTLOCK(B)) SDL_LockSurface(B); \
		GFX_PDownscale(A, B);	\
		if(SDL_MUSTLOCK(B)) SDL_UnlockSurface(B); \
	}	\
}
	
