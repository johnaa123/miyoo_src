#ifndef __MENU_H__
#define __FASTMEM_H__

#define MENU_FONT_HEIGHT 14
#define MENU_X 10
#define MENU_Y 65
#define MENU_BACKGROUND "etc/menu.png"
#define MENU_BACKGROUND_SEL "etc/menus.png"
#define MENU_LENGTH_LIST 10
#define MENU_FOCUS 5
#define MENU_COLOR  1
#define MENU_SELECTED 0

#define MENU_FILES_X 10
#define MENU_FILES_Y 65
#define MENU_MSG_X 80
#define MENU_MSG_Y 45
#define MENU_MSG_COLOR 0

#define PREVIEW_X 150
#define PREVIEW_Y 63

#define MENU_FONT "etc/menu_font.png"
#define MENU_SELECTOR "etc/selector.png"
#define MENU_SELECTOR_SEL "etc/selectors.png"
#define ABOUT_IMAGE "etc/about.png"

/** Shows the main menu */
void menu_show(void);
/** Shows the about box */
void show_about(void);
/** Shows a message on the menu screen */
//void show_message(char *msg, int x, int y);

#define OPTION_PALETTE 0
#define OPTION_SCALING 1
#define OPTION_FRAME 2
#define OPTION_SOUND 3
#define OPTION_SHOWFPS 4
#define OPTION_AUTOSKIP 5
#define OPTION_CPU 6
#define OPTION_SAVE_GLOBAL 7
#define OPTION_SAVE_GAME 8
#define OPTION_QUIT 9
#define OPTION_MACHINE 10

 
#endif
