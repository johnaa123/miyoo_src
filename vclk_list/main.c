#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/fb.h>

int main(int argc, char* args[])
{
  uint32_t n, k, m, i;
  long ac[4096];
  uint32_t reg[4096];

  i = 0;
  for(n=1; n<=128; n++){
    for(m=1; m<=16; m++){
      ac[i] = (24 * n) / m;
      reg[i] = (((n-1) << 8) | ((m-1) << 0));
      i+= 1;
    }
  }

  for(m=0; m<4096; m++){
    for(n=0; n<4096; n++){
      if(ac[n] > ac[m]){
        k = ac[n];
        ac[n] = ac[m];
        ac[m] = k;
        
        k = reg[n];
        reg[n] = reg[m];
        reg[m] = k;
      }
    }
  }

  for(m=0; m<4096; m++){
    if(ac[m] > 600){
      continue;
    }
    printf("video %04dMHz 0x%08x\n", ac[m], reg[m]);
  }
  return 0;
}

