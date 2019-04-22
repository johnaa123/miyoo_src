#ifndef GP2X_SOUND_THREAD

#include "SDL/SDL.h"
#include "pcm.h"
#include "rc.h"

#ifdef GP2X_ASM
#define MEMCPY gp2x_memcpy
#define MEMSET memset
#else
#define MEMCPY memcpy
#define MEMSET memset
#endif

#include "pcm.h"


struct pcm pcm;


static int sound = 1;
static int samplerate = 44100;
static int stereo = 1;
static volatile int audio_done;

rcvar_t pcm_exports[] =
{
	RCV_BOOL("sound", &sound),
	RCV_INT("stereo", &stereo),
	RCV_INT("samplerate", &samplerate),
	RCV_END
};


static void audio_callback(void *blah, byte *stream, int len)
{
	memcpy(stream, pcm.buf, len);
	audio_done = 1;
}


void pcm_init()
{
	int i;
	SDL_AudioSpec as;

	if (!sound) return;
	
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	as.freq = samplerate;
	as.format = AUDIO_U16SYS;
	as.channels = 1 + stereo;
	as.samples = 4096; //samplerate / 60;
	//for (i = 1; i < as.samples; i<<=1);
	//as.samples = i;
	as.callback = audio_callback;
	as.userdata = 0;

	printf( "Sound setup with freq %d format %d channels %d samples %d\n", as.freq, as.format, as.channels, as.samples );
	if (SDL_OpenAudio(&as, 0) == -1)
		return;
	
	pcm.hz = as.freq;
	pcm.stereo = as.channels - 1;
	pcm.len = as.size;
	pcm.buf = malloc(pcm.len);
	pcm.pos = 0;
	memset(pcm.buf, 0, pcm.len);
	
	SDL_PauseAudio(0);
}

int pcm_submit()
{
	if (!pcm.buf) return 0;
	if (pcm.pos < pcm.len) return 1;
	while (!audio_done)
		SDL_Delay(4);
	audio_done = 0;
	pcm.pos = 0;
	return 1;
}

void pcm_close()
{
	if (sound) SDL_CloseAudio();
}

void pcm_pause(int p)
{
	SDL_PauseAudio(p);
}

void pcm_volume(int n)
{
	
}

#endif
