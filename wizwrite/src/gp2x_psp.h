# ifndef __GP2X_PSP_H__
# define __GP2X_PSP_H__

# ifdef __cplusplus
extern "C" {
# endif

typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef struct gp2xCtrlData {
  int    Buttons;
  u32    TimeStamp;
  u16    Mouse;
  u16    Mx;
  u16    My;

} gp2xCtrlData;

typedef struct gp2xCtrlData SceCtrlData;

# define GP2X_CTRL_UP         0x00001
# define GP2X_CTRL_RIGHT      0x00002
# define GP2X_CTRL_DOWN       0x00004
# define GP2X_CTRL_LEFT       0x00008
# define GP2X_CTRL_TRIANGLE   0x00010
# define GP2X_CTRL_CIRCLE     0x00020
# define GP2X_CTRL_CROSS      0x00040
# define GP2X_CTRL_SQUARE     0x00080
# define GP2X_CTRL_SELECT     0x00100
# define GP2X_CTRL_START      0x00200
# define GP2X_CTRL_LTRIGGER   0x00400
# define GP2X_CTRL_RTRIGGER   0x00800
# define GP2X_CTRL_FIRE       0x01000
# define GP2X_CTRL_VOLUP      0x02000
# define GP2X_CTRL_VOLDOWN    0x04000
# define GP2X_CTRL_MASK       0x07fff

# define GP2X_CTRL_A          GP2X_CTRL_SQUARE
# define GP2X_CTRL_B          GP2X_CTRL_CIRCLE
# define GP2X_CTRL_Y          GP2X_CTRL_TRIANGLE
# define GP2X_CTRL_X          GP2X_CTRL_CROSS

# define PSP_CTRL_UP         GP2X_CTRL_UP        
# define PSP_CTRL_RIGHT      GP2X_CTRL_RIGHT     
# define PSP_CTRL_DOWN       GP2X_CTRL_DOWN      
# define PSP_CTRL_LEFT       GP2X_CTRL_LEFT      
# define PSP_CTRL_TRIANGLE   GP2X_CTRL_TRIANGLE  
# define PSP_CTRL_CIRCLE     GP2X_CTRL_CIRCLE    
# define PSP_CTRL_CROSS      GP2X_CTRL_CROSS     
# define PSP_CTRL_SQUARE     GP2X_CTRL_SQUARE    
# define PSP_CTRL_SELECT     GP2X_CTRL_SELECT    
# define PSP_CTRL_START      GP2X_CTRL_START     
# define PSP_CTRL_LTRIGGER   GP2X_CTRL_LTRIGGER  
# define PSP_CTRL_RTRIGGER   GP2X_CTRL_RTRIGGER  
# define PSP_CTRL_FIRE       GP2X_CTRL_FIRE      
# define PSP_CTRL_VOLUP      GP2X_CTRL_VOLUP     
# define PSP_CTRL_VOLDOWN    GP2X_CTRL_VOLDOWN   
# define PSP_CTRL_MASK       GP2X_CTRL_MASK      

# define GP2X_MOUSE_BTN_PRESS      0x0001
# define GP2X_MOUSE_BTN_RELEASE    0x0002
# define GP2X_MOUSE_CLICK          0x0010
# define GP2X_MOUSE_GST_UP         0x0100
# define GP2X_MOUSE_GST_DOWN       0x0200
# define GP2X_MOUSE_GST_LEFT       0x0400
# define GP2X_MOUSE_GST_RIGHT      0x0800


#if defined(WIZ_MODE) || defined(GP2X_MODE)

//gp2x buttons codes

#define GP2X_UP              (0)
#define GP2X_DOWN            (4)
#define GP2X_LEFT            (2)
#define GP2X_RIGHT           (6)
#define GP2X_UPLEFT          (1)
#define GP2X_UPRIGHT         (7)
#define GP2X_DOWNLEFT        (3)
#define GP2X_DOWNRIGHT       (5)

#define GP2X_A               (12)
#define GP2X_B               (13)
#define GP2X_X               (14)
#define GP2X_Y               (15)
#define GP2X_L               (10)
#define GP2X_R               (11)
#define GP2X_FIRE            (18)
#define GP2X_START           (9)        
#define GP2X_SELECT          (8)
#define GP2X_VOLUP           (16)
#define GP2X_VOLDOWN         (17)

#else

//some keys of the keyboard to emulate gp2x

#define GP2X_UP             SDLK_UP
#define GP2X_LEFT           SDLK_LEFT
#define GP2X_RIGHT          SDLK_RIGHT
#define GP2X_DOWN           SDLK_DOWN

#define GP2X_A              SDLK_SPACE
#define GP2X_B              SDLK_LALT
#define GP2X_X              SDLK_LCTRL
#define GP2X_Y              SDLK_LSHIFT
#define GP2X_L              SDLK_TAB
#define GP2X_R              SDLK_BACKSPACE
#define GP2X_START          SDLK_RETURN
#define GP2X_SELECT         SDLK_RCTRL

#define GP2X_VOLUP          1
#define GP2X_VOLDOWN        2
#define GP2X_DOWNLEFT       3
#define GP2X_DOWNRIGHT      4
#define GP2X_UPLEFT         5
#define GP2X_UPRIGHT        6
#define GP2X_FIRE           SDLK_a

#endif

#define GP2X_NOEVENT -1

#define DELAY_KEY_FIRST 250
#define DELAY_KEY_REPEAT 80
    
  extern int  gp2xCtrlReadBufferPositive(gp2xCtrlData* c, int v);
  extern int  gp2xCtrlPeekBufferPositive(gp2xCtrlData* c, int v);
  extern void gp2xPowerSetClockFrequency(int freq);

# define scePowerSetClockFrequency(A,B,C) gp2xPowerSetClockFrequency((A))

  extern int  gp2xGetSoundVolume();
  extern void gp2xDecreaseVolume();
  extern void gp2xIncreaseVolume();

# if defined(GP2X_MODE)
  extern int  gp2xInsmodMMUhack();
  extern int  gp2xRmmodMMUhack();
# endif

# ifdef __cplusplus
}
# endif

# endif
