
#include <stdio.h>

#include "defs.h"
#include "regs.h"
#include "hw.h"
#include "cpu.h"
#include "mem.h"
#include "lcd.h"
#include "rc.h"

#include "fb.h"

#ifdef SUPERGB
#include "supergb.h"
#endif

#define FRAMES_PER_SEC 60

static int framelen = 16666;
static int framecount;

rcvar_t emu_exports[] =
{
	RCV_INT("framelen", &framelen),
	RCV_INT("framecount", &framecount),
	RCV_END
};







void emu_init()
{
	
}


/*
 * emu_reset is called to initialize the state of the emulated
 * system. It should set cpu registers, hardware registers, etc. to
 * their appropriate values at powerup time.
 */

void emu_reset()
{
	hw_reset();
	lcd_reset();
	cpu_reset();
	mbc_reset();
	sound_reset();
	
#ifdef SUPERGB
	if(hw.sgb){
		DPRINT("Reseting SGB");
		sgb_end();
		sgb_init();
		sgb_reset();
		sgb_reset_state();
	}
#endif
	
#ifdef DEBUG
	printf("Emu reseted\n");
#endif
}





int emu_step()
{
	return cpu_emulate(cpu.lcdc);
}



/* This mess needs to be moved to another module; it's just here to
 * make things work in the mean time. */

void *sys_timer();

void emu_run()
{
	void *timer = sys_timer();
	int delay;
	int cycles,next_tick,this_tick;
	

	vid_begin();
	lcd_begin();
#ifdef SUPERGB
	//if(hw.sgb > 0){ sgb_init(); sgb_reset(); }
#endif
	for (;;)
	{
		cpu_emulate(2280);
		while (R_LY > 0 && R_LY < 144)
			emu_step();

#ifndef GP2X_MINIMAL
		//vid_end();
		rtc_tick();
 		sound_mix();
 		if (!pcm_submit())
 		{
			delay = framelen - sys_elapsed(timer);
			sys_sleep(delay);
			sys_elapsed(timer);
 		}
#else
		// the Gp2x is so small that any think to gain speed is welcome
		// the above functions are empty in the gp2x, and there always should
		// be a framelimit mechanism
		rtc_tick();
		delay = framelen - sys_elapsed(timer);

		//printf("SDL_GetTicks:%d\nFramelen:%d\nSys_elapsed(timer):%d\nDelay:%d",SDL_GetTicks(),framelen,sys_elapsed(timer),delay);
		//sys_sleep(delay);
		
	        this_tick = SDL_GetTicks();
		if ( this_tick < next_tick ) 
			SDL_Delay(next_tick-this_tick);

		//next_tick = this_tick + (1000/FRAMES_PER_SEC); 
		next_tick = this_tick + 17; 		// 1000/60 = 16,66



		sys_elapsed(timer);
#endif
		doevents();
		vid_begin();

		if (framecount) { if (!--framecount) die("finished\n"); }
		
		if (!(R_LCDC & 0x80))
			cpu_emulate(32832);
		
		while (R_LY > 0) /* wait for next frame */
		{
			cycles = emu_step();
			
#ifdef SUPERGB
			if(hw.sgb && cycles_SGB) // SGB command timeout
			{
				cycles_SGB -= cycles;
				if(cycles_SGB <= 0)
				{
					cycles_SGB = 0;
					sgb_reset_state();
					
					cycles_SGB += CYCLES_SGB_TIMEOUT;
				}
			}
#endif
		}
	}
}












