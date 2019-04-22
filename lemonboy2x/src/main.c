
// NOTE: this file is completely changed by Juan Vera <juanvvc@hotmail.com>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//char *strdup();

#include <stdarg.h>
#include <signal.h>

#include "input.h"
#include "rc.h"


#include "Version"

#include "gui/utils.h"
#include "gui/options.h"


char *rom_path = NULL;
char *rom_name = NULL;
struct dirinfo *palettes=NULL;
int palettes_sel=0;

static int run_menu=1;
static first_signal = 1;

static char *defaultconfig[] =
{
	NULL
};

static void banner()
{
	printf("\nLemonboy2x (SDL and Gp2x GUI for gnuboy) " VERSION "\n");
}

static void copyright()
{
	banner();
	printf(
"Copyright (C) 2000-2001 Laguna and Gilgamesh\n"
"Copyright (C) 2009 Juanvvc\n"
"Portions contributed by other authors; see CREDITS for details.\n"
"\n"
"This program is free software; you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation; either version 2 of the License, or\n"
"(at your option) any later version.\n"
"\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU General Public License\n"
"along with this program; if not, write to the Free Software\n"
"Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n"
"\n");
}

static void usage(char *name)
{
	copyright();
	printf("Type %s --help for detailed help.\n\n", name);
	exit(1);
}

static void copying()
{
	copyright();
	exit(0);
}

static void help(char *name)
{
	banner();
	printf("Usage: %s [options] romfile\n", name);
	printf("\n"
"      --source FILE             read rc commands from FILE\n"
"      --bind KEY COMMAND        bind KEY to perform COMMAND\n"
"      --VAR=VALUE               set rc variable VAR to VALUE\n"
"      --VAR                     set VAR to 1 (turn on boolean options)\n"
"      --no-VAR                  set VAR to 0 (turn off boolean options)\n"
"      --showvars                list all available rc variables\n"
"      --help                    display this help and exit\n"
"      --version                 output version information and exit\n"
"      --copying                 show copying permissions\n"
"");
	exit(0);
}


static void version(char *name)
{
	printf("%s-" VERSION "\n", name);
	exit(0);
}


void doevents()
{
	event_t ev;
	int st;

	ev_poll();
	while (ev_getevent(&ev))
	{
		if (ev.type != EV_PRESS && ev.type != EV_RELEASE)
			continue;
		st = (ev.type != EV_RELEASE);
		rc_dokey(ev.code, st);
	}
}

static void shutdown()
{
	vid_close();
	pcm_close();
	if(palettes){ free_dirinfo(palettes); free(palettes); palettes=NULL; }
	sync();

#ifdef GP2X
#if defined(GP2X_MINIMAL) || defined(WIZLIB)
	gp2x_deinit();
#endif
	if(run_menu){
		chdir("/usr/gp2x");
		if(execl("gp2xmenu","gp2xmenu",NULL)==-1){
			printf("Error running the Gp2x menu");
			exit(1);
		}
	}
#endif
}

void die(char *fmt, ...)
{
	va_list ap;

	printf("Killing\n");

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

#ifdef GP2X
	if(run_menu){
		chdir("/usr/gp2x");
		if(execl("gp2xmenu","gp2xmenu",NULL)==-1){
			printf("Error running the Gp2x menu");
			exit(1);
		}
	}
	exit(1);
#else
	exit(1);
#endif
}

static int bad_signals[] =
{
	/* These are all standard, so no need to #ifdef them... */
	SIGINT, SIGSEGV, SIGTERM, SIGFPE, SIGABRT, SIGILL,
#ifdef SIGQUIT
	SIGQUIT,
#endif
#ifdef SIGPIPE
	SIGPIPE,
#endif
	0
};

static void fatalsignal(int s)
{
	if(first_signal){
		first_signal=0;
		die("Signal %d\n", s);
	}else{
#ifdef GP2X
		if(run_menu){
			chdir("/usr/gp2x");
			if(execl("gp2xmenu","gp2xmenu",NULL)==-1){
				printf("Error running the menu");
				exit(1);
			}
		}
		exit(1);
#else
		exit(1);
#endif
	}
}

static void catch_signals()
{
	int i;
	for (i = 0; bad_signals[i]; i++)
		signal(bad_signals[i], fatalsignal);
}



static char *base(char *s)
{
	char *p;
	p = strrchr(s, '/');
	if (p) return p+1;
	return s;
}


int main(int argc, char *argv[])
{
	int i;
	char *opt, *arg, *cmd, *s = 0;

	/* Avoid initializing video if we don't have to */
	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "--help"))
			help(base(argv[0]));
		else if (!strcmp(argv[i], "--version"))
			version(base(argv[0]));
		else if (!strcmp(argv[i], "--copying"))
			copying();
		else if (!strcmp(argv[i], "--bind")) i += 2;
		else if (!strcmp(argv[i], "--source")) i++;
#ifdef GP2X
		else if (!strcmp(argv[i], "--nomenu")) run_menu=0;
#endif
		else if (!strcmp(argv[i], "--showvars"))
		{
			show_exports();
			exit(0);
		}
		else if (argv[i][0] == '-' && argv[i][1] == '-');
		else if (argv[i][0] == '-' && argv[i][1]);
		else rom_path = argv[i];
	}
	
	rom_name = only_rom_name(rom_path);
	init_exports();
	options_init();
#ifdef DEBUG
	options_print();
#endif

	/* If we have special perms, drop them ASAP! */
	vid_preinit();

	s = strdup(argv[0]);
/*	sys_sanitize(s);
	sys_initpath(s);*/

	for (i = 0; defaultconfig[i]; i++)
		rc_command(defaultconfig[i]);

	if(rom_path){
		cmd = malloc(strlen(rom_path) + 11);
		sprintf(cmd, "source %s", rom_name);
		s = strchr(cmd, '.');
		if (s) *s = 0;
		strcat(cmd, ".rc");
		rc_command(cmd);
	}

	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "--bind"))
		{
			if (i + 2 >= argc) die("missing arguments to bind\n");
			cmd = malloc(strlen(argv[i+1]) + strlen(argv[i+2]) + 9);
			sprintf(cmd, "bind %s \"%s\"", argv[i+1], argv[i+2]);
			rc_command(cmd);
			free(cmd);
			i += 2;
		}
		else if (!strcmp(argv[i], "--source"))
		{
			if (i + 1 >= argc) die("missing argument to source\n");
			cmd = malloc(strlen(argv[i+1]) + 6);
			sprintf(cmd, "source %s", argv[++i]);
			rc_command(cmd);
			free(cmd);
		}
		else if (!strncmp(argv[i], "--no-", 5))
		{
			opt = strdup(argv[i]+5);
			while ((s = strchr(opt, '-'))) *s = '_';
			cmd = malloc(strlen(opt) + 7);
			sprintf(cmd, "set %s 0", opt);
			rc_command(cmd);
			free(cmd);
			free(opt);
		}
		else if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			opt = strdup(argv[i]+2);
			if ((s = strchr(opt, '=')))
			{
				*s = 0;
				arg = s+1;
			}
			else arg = "1";
			while ((s = strchr(opt, '-'))) *s = '_';
			while ((s = strchr(arg, ','))) *s = ' ';
			
			cmd = malloc(strlen(opt) + strlen(arg) + 6);
			sprintf(cmd, "set %s %s", opt, arg);
			
			rc_command(cmd);
			free(cmd);
			free(opt);
		}
		/* short options not yet implemented */
		else if (argv[i][0] == '-' && argv[i][1]);
	}

	/* FIXME - make interface modules responsible for atexit() */
	atexit(shutdown);
	catch_signals();
	vid_init();
	pcm_init();
	
	if(!rom_path)
	{
		if(options.showsplash) show_about();
		menu_show();
	}
	else
	{
		rom_path = strdup(rom_path);
		sys_sanitize(rom_path);
		loader_init(rom_path);
		emu_reset();
		// FIXME: read default options
		options_apply();
	}
	emu_run();

	/* never reached */
	return 0;
}










