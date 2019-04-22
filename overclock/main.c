#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  int fd;
  uint32_t *mem;

  fd=open("/dev/mem", O_RDWR);
  if(fd < 0){
    printf("failed to open /dev/mem\n");
    return -1;
  }
  mem  = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x01c20000);
  printf("mem addr 0x%08x, val 0x%08x\n", mem, *mem);
	munmap(mem, 1024);
	close(fd);
  return 0;    
}

