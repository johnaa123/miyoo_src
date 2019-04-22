


#ifndef __DEFS_H__
#define __DEFS_H__



#ifdef IS_LITTLE_ENDIAN
#define LO 0
#define HI 1
#else
#define LO 1
#define HI 0
#endif

typedef unsigned char byte;

typedef unsigned char un8;
typedef unsigned short un16;
typedef unsigned int un32;

typedef signed char n8;
typedef signed short n16;
typedef signed int n32;

typedef un16 word;
typedef word addr;

#define FREE(A) if(A){free(A); A=NULL;}
#define MIN(A, B) ((A)<(B)?(A):(B))
#define MAX(A, B) ((A)>(B)?(A):(B)) 

#define PALETTES_DIR "palettes"
#define OPTIONS_DIR "configs"
#define FRAMES_DIR "frames"
#define PREVIEWS_DIR "previews"

#ifdef DEBUG
#define DPRINT(M) printf(M "\n")
#define DPRINT1(M, X) printf(M "\n", X)
#define DPRINT2(M, X, Y) printf(M "\n", X, Y)
#define DPRINT3(M, X, Y, Z) printf(M "\n", X, Y, Z)
#else
#define DPRINT(M) 
#define DPRINT1(M, X) 
#define DPRINT2(M, X, Y) 
#define DPRINT3(M, X, Y, Z) 
#endif

#define TRUE 1
#define FALSE 0

#endif

