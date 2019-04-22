
// Manages user configurable options.
// This file uses the rc subsystem to load options, and saves and apply some of
// the options that a user can modify during running time.

#include <stdio.h>
#include <string.h>

#include "../rc.h"
#include "../defs.h"

#include "options.h"
#include "utils.h"
#include "imaging.h"

// Funny thing to pass strings to the compiler with -D
#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x
//#ifndef GLOBAL_CONFIG_FILE
//#define GLOBAL_CONFIG_FILE "etc/system.pc"
//#endif

struct options options;

/** Initialize options to default values.
 * Actually, reading the default values from the configuration files is preferred. */
void options_init(){
	char cmd[256];
	options.frame = strdup("frames/default.png");
 	options.palette = strdup("palettes/default");
 	options.sound = TRUE;
	options.autofskip = TRUE;
	options.romsdir = strdup("roms");
	options.vsync = FALSE;
	options.scaling = 0;
	options.volumen = 80;
	options.showfps = FALSE;
	options.cpuclock = 200;
	options.samplerate = 44100;
	options.previews = TRUE;
	options.confirmsave = FALSE;
	options.confirmload = TRUE;
	options.vmodes[0] = 1; options.vmodes[1] = 3; options.vmodes[2] = -1;
	options.vdeformation = 24;
	options.voffset = 12;
	options.machine = 0;
	options.subdirectories = TRUE;
	options.showsplash = TRUE;
	
	// Lad system configuration
	DPRINT("Config file: " STRINGIFY(GLOBAL_CONFIG_FILE));
	rc_command("source " STRINGIFY(GLOBAL_CONFIG_FILE));
	// load default configuration
	sprintf(cmd, "source %s/%s", OPTIONS_DIR, "default.rc");
	rc_command(cmd);
}

/** Apply global options BEFORE initializing the system */
void options_apply(){
	switch(options.machine){
	case 0: //color gameboy
		rc_command("set gbamode false");
		rc_command("set sgbmode 0");
		break;
	case 1: // gameboy advance
		rc_command("set gbamode true");
		rc_command("set sgbmode 0");
		break;		
	case 2: // supergameboy (sgbmode=1 for SGB1 and sgbmode=2 for SGB2)
		rc_command("set gbamode false");
		rc_command("set sgbmode 1");
		rc_command("set colorfilter no");
		break;
	case 3: // supergameboy (sgbmode=1 for SGB1 and sgbmode=2 for SGB2)
		rc_command("set gbamode false");
		rc_command("set sgbmode 2");
		rc_command("set colorfilter no");
		break;
	}
}


/** Save current options in a file */
int options_save(char* fname){
 	FILE *fp;
 #ifdef DEBUG
 	printf("Saving options in %s\n", fname);
 #endif
 	if((fp=fopen(fname, "w"))==NULL)
 		return -1;
 	fprintf(fp, "set palette \"%s\"\n", options.palette);
 	fprintf(fp, "set frame \"%s\"\n", options.frame);
 	fprintf(fp, "set sound %d\n", options.sound);
 	fprintf(fp, "set autofskip %d\n", options.autofskip);
 	fprintf(fp, "set vsync %d\n", options.vsync);
 	fprintf(fp, "set scaling %d\n", options.scaling);
 	fprintf(fp, "set volumen %d\n", options.volumen);
 	fprintf(fp, "set showfps %d\n", options.showfps);
	fprintf(fp, "set cpuclock %d\n", options.cpuclock);
	fprintf(fp, "set machine %d\n", options.machine);
	
 	fflush(fp);
 	fclose(fp);
	return 0;
}

#ifdef DEBUG
/** Print some options for debugging */
void options_print(){
	printf("set palette \"%s\"\n", options.palette);
	printf("set frame \"%s\"\n", options.frame);
	printf("set sound %d\n", options.sound);
	printf("set autofskip %d\n", options.autofskip);
	printf("set vsync %d\n", options.vsync);
	printf("set scaling %d\n", options.scaling);
	printf("set volumen %d\n", options.volumen);
	printf("set showfps %d\n", options.showfps);
	printf("set cpuclock %d\n", options.cpuclock);
	printf("set machine %d\n", options.machine);

 	fflush(0);
}
#endif

/** Read options from the configuration files, using the rc subsystem */
rcvar_t options_exports[] =
{
	RCV_STRING("frame", &options.frame),
	RCV_STRING("palette", &options.palette),
	RCV_STRING("romsdir", &options.romsdir),
	RCV_BOOL("sound", &options.sound),
	RCV_BOOL("autofskip", &options.autofskip),
	RCV_BOOL("vsync", &options.vsync),
	RCV_INT("scaling", &options.scaling),
	RCV_INT("volumen", &options.volumen),
	RCV_BOOL("showfps", &options.showfps),
	RCV_INT("cpuclock", &options.cpuclock),
	RCV_INT("samplerate", &options.samplerate),
	RCV_BOOL("previews", &options.previews),
	RCV_BOOL("confirmsave", &options.confirmsave),
	RCV_BOOL("confirmload", &options.confirmload),
	RCV_BOOL("showsplash", &options.showsplash),
	RCV_BOOL("subdirectories", &options.subdirectories),
	RCV_VECTOR("vmodes", &options.vmodes[0], 5),
	RCV_VECTOR("vmodes", &options.vmodes[1], 5),
	RCV_VECTOR("vmodes", &options.vmodes[2], 5),
	RCV_VECTOR("vmodes", &options.vmodes[3], 5),
	RCV_VECTOR("vmodes", &options.vmodes[4], 5),
	RCV_INT("voffset", &options.voffset),
	RCV_INT("vdeformation", &options.vdeformation),
	RCV_INT("machine", &options.machine),
	RCV_END
};
