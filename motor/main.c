/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option)any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <linux/ioctl.h>
#include <sys/fcntl.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>      

#define MIYOO_KBD_GET_HOTKEY  _IOWR(0x100, 0, unsigned long)
#define MIYOO_FB0_PUT_OSD     _IOWR(0x100, 0, unsigned long)
#define MIYOO_SND_SET_VOLUME  _IOWR(0x100, 0, unsigned long)
#define MIYOO_VIR_SET_MODE    _IOWR(0x100, 0, unsigned long)

int main(int argc, char** argv)
{
  int fd=0;

  fd = open("/dev/miyoo_vir", O_RDWR);
  ioctl(fd, MIYOO_VIR_SET_MODE, 0);
  usleep(300000);
  ioctl(fd, MIYOO_VIR_SET_MODE, 1);
  close(fd);
  return 0;
}

