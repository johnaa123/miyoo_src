/*  EnergySaver for GP2X

	File: cpugp2x.c
	
	Copyright (C) 2006  Kounch
	Parts Copyright (c) 2005 Rlyeh
	Parts Copyright (c) 2006 Hermes/PS2Reality

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <stdio.h>     //Entrada/Salida basica
#include <sys/mman.h>  //Para poder hacer mmap y acceder a los registros de memoria
#include <unistd.h>
#include <fcntl.h>     //Para poder tener acceso al dispositivo /dev/mem y /dev/fb

#include "cpugp2x.h"

/* Declaracion de variables globales */
#define SYS_CLK_FREQ 7372800

/*Registros del sistema*/
#ifndef __HAYMINIMAL__
 unsigned long   psp_dev[8]={0,0,0,0,0,0,0,0};
 volatile unsigned short *psp_memregs;
 volatile unsigned long  *psp_memregl;
#else
 extern unsigned long psp_dev[8];
 extern volatile unsigned short *psp_memregs;
 extern volatile unsigned long  *psp_memregl;
#endif
volatile unsigned short *MEM_REG;

static struct 
{
  unsigned short SYSCLKENREG,SYSCSETREG,FPLLVSETREG,DUALINT920,DUALINT940,DUALCTRL940;
}
system_reg;


/*Funcion que abre los dispositivos (basada en Minimal Lib)*/
void cpu_init()
{
  /*Inicialización para acceder a la memoria de la consola */
#ifndef __HAYMINIMAL__
  if(!psp_dev[2])  psp_dev[2] = open("/dev/mem", O_RDWR);
  psp_memregl=(unsigned long  *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, psp_dev[2], 0xc0000000);
  psp_memregs=(unsigned short *)psp_memregl;
#endif
  MEM_REG=&psp_memregs[0];
}

/*Funcion que cierra todos los dispositivos que se hayan abierto (basada en Minimal Lib)*/
int cpu_deinit(void)
{
  cpu_set_clock( GP2X_DEF_CLOCK );
#ifndef __HAYMINIMAL__
  if(psp_dev[2]) close(psp_dev[2]);
#endif
  sync();
  return (0);
}

/* Guardar registros de la GP2X (basado en cpuctrl de Hermes/PS2R)*/
void save_system_regs()
{
  system_reg.SYSCSETREG=MEM_REG[0x91c>>1];
  system_reg.FPLLVSETREG=MEM_REG[0x912>>1]; 
  system_reg.SYSCLKENREG=MEM_REG[0x904>>1];
  system_reg.DUALINT920=MEM_REG[0x3B40>>1];
  system_reg.DUALINT940=MEM_REG[0x3B42>>1];  
  system_reg.DUALCTRL940=MEM_REG[0x3B48>>1];	
}

/* Recuperar registros de la GP2X (basado en cpuctrl de Hermes/PS2R)*/
void load_system_regs()
{
  MEM_REG[0x91c>>1]=system_reg.SYSCSETREG;
  MEM_REG[0x910>>1]=system_reg.FPLLVSETREG;  
  MEM_REG[0x3B40>>1]=system_reg.DUALINT920;
  MEM_REG[0x3B42>>1]=system_reg.DUALINT940;
  MEM_REG[0x3B48>>1]=system_reg.DUALCTRL940;
  MEM_REG[0x904>>1]=system_reg.SYSCLKENREG;
}

/* Obtener la frecuencia del reloj (basado en cpu_speed de Hermes/PS2R)*/
unsigned get_speed_clock(void)
{
  int n,m;
  unsigned sysfreq=0;
  unsigned lfreq[25]={60,66,80,100,120,133,150,166,200,210,220,240,250,266,270,275,280,285,290,295,300,305,310,315,320}; // por encima de 220 MHz solo funciona con div 1
  
  sysfreq=get_freq_920_CLK();
  sysfreq*=get_920_Div()+1;
  m=(sysfreq/1000000)+4;
  n=0;
  
  for(n=24;n>1;n--)
	{
    if(lfreq[n]<m) break; 
	}

  return lfreq[n];
}

/* Ajustar la frecuencia del reloj (basado en cpuctrl de Hermes/PS2R)*/
static int loc_clock_in_mhz = 60;
void cpu_set_clock(int clock_in_mhz)
{
  int div;
  if (clock_in_mhz == loc_clock_in_mhz) return;
  loc_clock_in_mhz = clock_in_mhz;
  
  div=(clock_in_mhz*14)/200;
  if(div>63) div=63;

  set_display_clock_div(64 | div /*(16+8*(clock_in_mhz>=220))*/);
  set_FCLK(clock_in_mhz);
  set_DCLK_Div(0);
  set_920_Div(0);

  return;
}

unsigned int
cpu_get_clock()
{
  return loc_clock_in_mhz;
}

/*  (basado en cpuctrl de Hermes/PS2R)*/
unsigned get_freq_920_CLK()
{
  unsigned i;
  unsigned reg,mdiv,pdiv,scale;
  reg=MEM_REG[0x912>>1];
  mdiv = ((reg & 0xff00) >> 8) + 8;
  pdiv = ((reg & 0xfc) >> 2) + 2;
  scale = reg & 3;
  
  i = (MEM_REG[0x91c>>1] & 7)+1;
  return ((SYS_CLK_FREQ * mdiv) / (pdiv << scale))/i;
}

/*  (basado en cpuctrl de Hermes/PS2R)*/
unsigned short get_920_Div()
{
  return (MEM_REG[0x91c>>1] & 0x7); 
}

/*  (basado en cpuctrl de Hermes/PS2R)*/
unsigned get_display_clock_div()
{
return ((MEM_REG[0x924>>1]>>8));
}

/*  (basado en cpuctrl de Hermes/PS2R)*/
void set_display_clock_div(unsigned div)
{
  div=(div & 0xff)<<8;
  MEM_REG[0x924>>1]=(MEM_REG[0x924>>1] & ~(255<<8)) | div;
}

/* (basado en cpuctrl de Hermes/PS2R)*/
void set_FCLK(unsigned MHZ)
{
  unsigned v;
  unsigned mdiv,pdiv=3,scale=0;
  
  MHZ*=1000000;
  mdiv=(MHZ*pdiv)/SYS_CLK_FREQ;
  
  mdiv=((mdiv-8)<<8) & 0xff00;
  pdiv=((pdiv-2)<<2) & 0xfc;
  scale&=3;
  v=mdiv | pdiv | scale;
  MEM_REG[0x910>>1]=v;
}

/* (basado en cpuctrl de Hermes/PS2R)*/
void set_DCLK_Div( unsigned short div )
{  
  unsigned short v;
  v = (unsigned short)( MEM_REG[0x91c>>1] & (~(0x7 << 6)) );
  MEM_REG[0x91c>>1] = ((div & 0x7) << 6) | v; 
}

/* (basado en cpuctrl de Hermes/PS2R)*/
void set_920_Div(unsigned short div)
{
  unsigned short v;
  v = MEM_REG[0x91c>>1] & (~0x3);
  MEM_REG[0x91c>>1] = (div & 0x7) | v;   
}

/* (basado en cpuctrl de Hermes/PS2R)*/
void Disable_940()
{
Disable_Int_940();
MEM_REG[0x3B48>>1]|= (1 << 7);
MEM_REG[0x904>>1]&=0xfffe;
}

/* (basado en cpuctrl de Hermes/PS2R)*/
unsigned short Disable_Int_940()
{
unsigned short ret;
ret=MEM_REG[0x3B42>>1];
MEM_REG[0x3B42>>1]=0;
MEM_REG[0x3B46>>1]=0xffff;	
return ret;	
}

/* (basado en cpuctrl de Hermes/PS2R)*/
unsigned get_status_UCLK()
{
unsigned i;

i = MEM_REG[0x900>>1];
i = ((i >> 7) & 1) ;
if(i) return 0;
return 1;
}

/* (basado en cpuctrl de Hermes/PS2R)*/
unsigned get_status_ACLK()
{
unsigned i;

i = MEM_REG[0x900>>1];
i = ((i >> 8) & 1) ;
if(i) return 0;

return 1;
}

/* (basado en cpuctrl de Hermes/PS2R)*/
void set_status_UCLK(unsigned s)
{
if(s==0) MEM_REG[0x900>>1]|=128;
else MEM_REG[0x900>>1]&=~128;
}

/* (basado en cpuctrl de Hermes/PS2R)*/
void set_status_ACLK(unsigned s)
{

if(s==0) MEM_REG[0x900>>1]|=256;
else MEM_REG[0x900>>1]&=~256;
}

/* Apagar o encender LCD (basado en cpuctrl de Hermes/PS2R)*/
void set_display(int mode) // 1- display on 0-display off
{
  int i;
  if(mode)
	{
    
    MEM_REG[0x1062>>1] &= ~0x0c;
    MEM_REG[0x1066>>1] &= ~0x10;
    MEM_REG[0x106E>>1] &= ~0x06;
    MEM_REG[0x106E>>1] |= 0x08;
    
    MEM_REG[0x280A>>1]= 0xffff;
    MEM_REG[0x280C>>1]= 0xffff;
    MEM_REG[0x280E>>1]= 0xffff;
    MEM_REG[0x2802>>1]= 0;
    MEM_REG[0x2800>>1]|= 0x0001;
    
    for (i=0;i<1000000;i++);
    
    MEM_REG[0x1062>>1] |= 0x0c;
    MEM_REG[0x1066>>1] |= 0x10;
    MEM_REG[0x106E>>1] &= ~0x08;
    MEM_REG[0x106E>>1] |= 0x06;
    
	}
  else
	{
    MEM_REG[0x106E>>1]&=~4;
    MEM_REG[0x280A>>1]= 0;
    MEM_REG[0x280C>>1]= 0;
    MEM_REG[0x280E>>1]= 0;
    MEM_REG[0x2800>>1]= 4;
    MEM_REG[0x2802>>1]= 1;
	}
}

