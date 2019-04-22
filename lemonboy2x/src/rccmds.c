


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "rc.h"
#include "hw.h"

#include "gui/menu.h"
#include "gui/utils.h"
#include "gui/options.h"

extern struct dirinfo *palettes;
extern int palettes_sel;

extern void paldirty(void);

/*
 * define the command functions for the controller pad.
 */

#define CMD_PAD(b, B) \
static int (cmd_ ## b)(int c, char **v) \
{ pad_set((PAD_ ## B), v[0][0] == '+'); return 0; } \
static int (cmd_ ## b)(int c, char **v)

CMD_PAD(up, UP);
CMD_PAD(down, DOWN);
CMD_PAD(left, LEFT);
CMD_PAD(right, RIGHT);
CMD_PAD(a, A);
CMD_PAD(b, B);
CMD_PAD(start, START);
CMD_PAD(select, SELECT);


/*
 * the set command is used to set rc-exported variables.
 */

static int cmd_set(int argc, char **argv)
{
	if (argc < 3)
		return -1;
	return rc_setvar(argv[1], argc-2, argv+2);
}



/*
 * the following commands allow keys to be bound to perform rc commands.
 */

static int cmd_bind(int argc, char **argv)
{
	if (argc < 3)
		return -1;
	return rc_bindkey(argv[1], argv[2]);
}

static int cmd_unbind(int argc, char **argv)
{
	if (argc < 2)
		return -1;
	return rc_unbindkey(argv[1]);
}

static int cmd_unbindall()
{
	rc_unbindall();
	return 0;
}

static int cmd_source(int argc, char **argv)
{
	int r;
	if (argc < 2)
		return -1;
	r = rc_sourcefile(argv[1]);
#ifdef DEBUG
	if(r == -1)
		printf("Error loading file %s\n", argv[1]);
	else
		printf("Options loaded: %s\n", argv[1]);
#endif
	return r;
}

static int cmd_quit()
{
	exit(0);
	/* NOT REACHED */
}

static int cmd_reset()
{
	emu_reset();
	return 0;
}

static int cmd_savestate(int argc, char **argv)
{
pcm_pause(1); pcm_volume(0);
	sys_waitnobutton();
	if(!options.confirmsave||sys_confirm("Save state?")){
		state_save(argc > 1 ? atoi(argv[1]) : -1);
		sync();
		sys_waitnobutton();
		vid_message("State saved", 60);
	}
	pcm_volume(options.volumen); pcm_pause(0);
	return 0;
}

static int cmd_loadstate(int argc, char **argv)
{
pcm_pause(1); pcm_volume(0);
	sys_waitnobutton();
	if(!options.confirmload||sys_confirm("Load State?")){
		state_load(argc > 1 ? atoi(argv[1]) : -1);
		sys_waitnobutton();
		vid_message("State loaded", 60);
	}
	pcm_volume(options.volumen); pcm_pause(0);
	return 0;
}

static int cmd_menu(int argc, char **argv)
{
pcm_pause(1); pcm_volume(0);
	sys_waitnobutton();
	menu_show();
	pcm_volume(options.volumen); pcm_pause(0);
	return 0;
}

static int cmd_videochange(int argc, char **argv)
{
	int i=0;
	pcm_pause(1); pcm_volume(0);
	sys_waitnobutton();
	// search the current scaling in options.vmodes and get the next
	// if the current scaling is not in vmodes, get the first option.
	// (-1 marks the end of the vector. It is an error if vmodes do not have
	// at least one mode and a -1)
	while(1){
		if(options.vmodes[i]==-1){
			options.scaling=options.vmodes[0];
			break;
		}
		if(options.vmodes[i]==options.scaling){
			if(options.vmodes[i+1]==-1)
				options.scaling=options.vmodes[0];
			else
				options.scaling=options.vmodes[i+1];
			break;
		}
		i++;
	}
	vid_init();
	//black_screen();
	pcm_volume(options.volumen); pcm_pause(0);
	return 0;
}

static int cmd_palettes(int argc, char **argv)
{
	char *cmd;
	
	sys_waitnobutton();

	if(palettes){
		if(palettes->num_files > 0){
			palettes_sel = (palettes_sel+1)%palettes->num_files;
			cmd = malloc(strlen(palettes->file[palettes_sel]) + strlen(PALETTES_DIR) + 10);
			sprintf(cmd, "source %s/%s", PALETTES_DIR, palettes->file[palettes_sel]);
			rc_command(cmd);
			pal_dirty();
#ifdef DEBUG
			printf("Palettes: %s\n", palettes->file[palettes_sel]);
#endif
			vid_message(palettes->file[palettes_sel], 60);
			free(cmd);
		}
	}else{
		palettes = list_dir(PALETTES_DIR, LISTINFO_MAX_FILES, NO_DIRS);
		if(!palettes)
			return -1;
		palettes_sel = 0;
		return cmd_palettes(0, NULL);
	}
	return 0;
}

/*
 * table of command names and the corresponding functions to be called
 */

rccmd_t rccmds[] =
{
	RCC("set", cmd_set),
	RCC("bind", cmd_bind),
	RCC("unbind", cmd_unbind),
	RCC("unbindall", cmd_unbindall),
	RCC("source", cmd_source),
	RCC("reset", cmd_reset),
	RCC("quit", cmd_quit),
	RCC("savestate", cmd_savestate),
	RCC("loadstate", cmd_loadstate),
	RCC("videochange", cmd_videochange),
	
	RCC("+up", cmd_up),
	RCC("-up", cmd_up),
	RCC("+down", cmd_down),
	RCC("-down", cmd_down),
	RCC("+left", cmd_left),
	RCC("-left", cmd_left),
	RCC("+right", cmd_right),
	RCC("-right", cmd_right),
	RCC("+a", cmd_a),
	RCC("-a", cmd_a),
	RCC("+b", cmd_b),
	RCC("-b", cmd_b),
	RCC("+start", cmd_start),
	RCC("-start", cmd_start),
	RCC("+select", cmd_select),
	RCC("-select", cmd_select),
	
	RCC("menu", cmd_menu),
	RCC("palettes", cmd_palettes),
	
	RCC_END
};





int rc_command(char *line)
{
	int i, argc, ret;
	char *argv[128], *linecopy;

	linecopy = malloc(strlen(line)+1);
	strcpy(linecopy, line);
	
	argc = splitline(argv, (sizeof argv)/(sizeof argv[0]), linecopy);
	if (!argc)
	{
		free(linecopy);
		return -1;
	}
	
	for (i = 0; rccmds[i].name; i++)
	{
		if (!strcmp(argv[0], rccmds[i].name))
		{
			ret = rccmds[i].func(argc, argv);
			free(linecopy);
			return ret;
		}
	}

	/* printf("unknown command: %s\n", argv[0]); */
	free(linecopy);
	
	return -1;
}

