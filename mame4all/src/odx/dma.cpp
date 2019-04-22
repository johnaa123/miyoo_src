#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/input.h>
#include <linux/soundcard.h>
//#include <SDL.h>
#include "dma.h"

volatile uint16_t *dma_ptr = NULL;

static int memdev = -1;
static int fbdev = -1;
static void *fb_vaddrs[2];
static unsigned int fb_paddrs[2];

#define DMA_SIZE (320 * 240 * 2 * 2)

void init_dma(void)
{
  memdev = open("/dev/mem", O_RDWR);
  if(memdev < 0)
  {
    printf("failed to open /dev/mem\n");
    return -1;
  }
	
	int ret;
	struct fb_fix_screeninfo fbfix;
  const char *main_fb_name = "/dev/fb0";
	fbdev = open(main_fb_name, O_RDWR);
	if (fbdev == -1) {
		printf("%s, failed to open %s\n", __func__, main_fb_name);
		exit(-1);
	}

	ret = ioctl(fbdev, FBIOGET_FSCREENINFO, &fbfix);
	if (ret == -1) {
		printf("%s, failed to do ioctl(FBIOGET_FSCREENINFO)\n", __func__);
		exit(1);
	}

	printf("framebuffer: \"%s\" @ %08lx\n", fbfix.id, fbfix.smem_start);
	fb_paddrs[0] = fbfix.smem_start;
	fb_paddrs[1] = fb_paddrs[0] + 320*240*2;

	if (fb_vaddrs[0] == NULL) {
		fb_vaddrs[0] = mmap(0, DMA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memdev, fb_paddrs[0]);
		if (fb_vaddrs[0] == MAP_FAILED) {
			printf("%s, failed to do mmap(fb_vaddrs)\n", __func__);
			exit(-1);
		}
		memset(fb_vaddrs[0], 0, DMA_SIZE);
	}
	printf("buffer mapped @%p\n", fb_vaddrs[0]);

	fb_vaddrs[1] = (char*)fb_vaddrs[0] + 320*240*2;
	memset(fb_vaddrs[0], 0, DMA_SIZE);
  dma_ptr = fb_vaddrs[0];
}

void close_dma(void)
{
  if(dma_ptr)
  {
    munmap(dma_ptr, DMA_SIZE);
    dma_ptr = NULL;
    close(memdev);
    memdev = -1;
    close(fbdev);
    fbdev = -1;
  }
}

