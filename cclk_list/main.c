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
  uint32_t oc[512];
  uint32_t reg[512];

  i = 0;
  for(n=1; n<=32; n++){
    for(k=1; k<=4; k++){
      for(m=1; m<=4; m++){
        oc[i] = (24 * n * k) / m;
        reg[i] = (((n-1) << 8) | ((k-1) << 4) | ((m-1) << 0));
        i+= 1;
      }
    }
  }

  for(m=0; m<512; m++){
    for(n=0; n<512; n++){
      if(oc[n] > oc[m]){
        k = oc[n];
        oc[n] = oc[m];
        oc[m] = k;
        
        k = reg[n];
        reg[n] = reg[m];
        reg[m] = k;
      }
    }
  }

  for(m=0; m<512; m++){
    //printf("Clk %04dMHz 0x%08x\n", oc[m], reg[m]);
    printf("0x%04x%04x,\n", oc[m], reg[m]);
  }
  return 0;
}

