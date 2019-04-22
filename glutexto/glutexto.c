 /* This file is part of glutexto.
  * glutexto is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  * 
  * glutexto is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with glutexto.  If not, see <http://www.gnu.org/licenses/>
  * 
  * For feedback and questions about my Files and Projects please mail me,
  * Alexander Matthes (Ziz) , zizsdl_at_googlemail.com */

#define GCW_FEELING

#if defined GCW_FEELING && defined X86CPU
	#define TESTING
	#define GCW
	#undef X86CPU
#endif
#include <sparrow3d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#if defined GCW_FEELING && defined TESTING
	#define X86CPU
	#undef GCW
#endif

#define MAX_TAB_SIZE 16
#define FONT_LOCATION "./font/Chango-Regular.ttf"
#define FONT_SIZE 7
#define FONT_COLOR spGetRGB(255,255,255)
#define BACKGROUND_COLOR spGetRGB(96,32,0)
#define EDIT_BACKGROUND_COLOR spGetRGB(255,255,255)
#define EDIT_NUMBER_BACKGROUND_COLOR spGetRGB(200,200,200)
#define EDIT_TEXT_COLOR spGetRGB(32,0,0)
#define EDIT_LINE_COLOR spGetRGB(220,220,220)
#define SELECTED_BACKGROUND_COLOR spGetRGB(185,185,100)

typedef struct sFont *pFont;
typedef struct sFont {
	char name[256];
	char location[256];
	pFont next;
} tFont;

typedef struct sText *pText;
typedef struct sText {
	char* line;
	spTextBlockPointer block;
	int length;
	int reserved;
	pText prev,next;
} tText;

int wrapLines = 0;
pFont firstFont = NULL;
pFont selectedFont = NULL;
#define MIN_FONT_SIZE 6
#define MAX_FONT_SIZE 20
int fontSize = 11;
int exit_now = 0;
SDL_Surface* screen;
SDL_Surface* editSurface = NULL;
spFontPointer font = NULL;
spFontPointer fontInverted = NULL;
spFontPointer textFont = NULL;
pText text = NULL;
pText textEnd = NULL;
pText momLine = NULL;
int line_number = 0;
int line_pos = 0;
int showLines = 1;
char dialog_folder[512] = "/usr/local/home";
int next_in_a_row = 0;
int time_until_next = 0;
int blink = 0;
char enter_buffer[MAX_TAB_SIZE+2]="a"; //a whole utf8 letter or 8 spaces
int tab_mode = 0;
int tabs_as_spaces = 0;

void resize(Uint16 w,Uint16 h);
void draw_without_flip();

#include "error.c"
#include "text.c"
#include "settings.c"
#include "dialog.c"
#include "menu.c"

void search_fonts_in_directoy(char* d_name)
{
	spFileListPointer subresult;
	spFileGetDirectory(&subresult,d_name,0,0);
	spFileListPointer file = subresult;
	while (file)
	{
		if (strcmp(&file->name[strlen(file->name)-4],".ttf") == 0)
		{
			pFont font = (pFont)malloc(sizeof(tFont));
			char without_ttf[256];
			sprintf(without_ttf,"%s",file->name);
			without_ttf[strlen(without_ttf)-4] = 0;
			if (strcmp(&without_ttf[strlen(without_ttf)-8],"-Regular") == 0)
				without_ttf[strlen(without_ttf)-8] = 0;
			sprintf(font->name,"%s",&without_ttf[strlen(d_name)+1]);
			sprintf(font->location,"%s",file->name);
			font->next = firstFont;
			firstFont = font;
		}
		file = file->next;
	}
	spFileDeleteList(subresult);
}

void load_fonts()
{
	spFileListPointer result;
	spFileGetDirectory(&result,"./font",0,0);
	spFileListPointer directory = result;
	while (directory)
	{
		printf("Tested %s\n",directory->name);
		if (directory->type == SP_FILE_DIRECTORY)
		{
			search_fonts_in_directoy(directory->name);
		}
		directory = directory->next;
	}
	spFileDeleteList(result);
	char buffer[512];
	#if defined(GCW) || (defined(X86CPU) && !defined(WIN32))
		search_fonts_in_directoy(get_path(buffer,"font"));
	#endif
	search_fonts_in_directoy(get_path(buffer,"fonts"));
	selectedFont = firstFont;
}

void draw_without_flip( void )
{
	//filling the edit field
	spSelectRenderTarget(editSurface);
	spClearTarget( EDIT_BACKGROUND_COLOR );
	int i;
	int pattern = 128+64+8+4;//0b11001100;
	spLetterPointer letter = spFontGetLetter(textFont,'A');
	int number_width = getNumberWidth();
	char buffer[256];
	int lines_per_screen = editSurface->h/textFont->maxheight;
	if (spIsKeyboardPolled() && spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)
		lines_per_screen = (editSurface->h-spGetVirtualKeyboard()->h)/textFont->maxheight;
	
	int lines_on_last_screen = lines_per_screen;
	int start_line = line_number - lines_per_screen/3;
	if (wrapLines)
	{
		pText mom = textEnd;
		int c = 0;
		lines_on_last_screen = 0;
		while (mom && c < lines_per_screen)
		{
			c += mom->block->line_count;
			lines_on_last_screen++;
			mom = mom->prev;
		}
		lines_on_last_screen--;

		start_line = line_number;
		c = 0;
		mom = momLine;
		while (mom && c < lines_per_screen/3)
		{
			c += mom->block->line_count;
			mom = mom->prev;
			start_line--;
		}		
	}
	if (start_line+lines_on_last_screen+1 > line_count)
		start_line = line_count-lines_on_last_screen+1;
	if (start_line < 1)
		start_line = 1;
	pText line = momLine;
	for (i = line_number; i > start_line; i--)
		line = line->prev;
	int number = start_line;

	char end = momLine->line[line_pos];
	momLine->line[line_pos] = 0;
	int momLineCursorPos = spFontWidth(momLine->line,textFont);
	momLine->line[line_pos] = end;
	
	int textShift = 1;

	if (!wrapLines && momLineCursorPos > editSurface->w*3/4)
		textShift = editSurface->w*3/4 - momLineCursorPos;
	
	int space_width = spFontWidth(" ",textFont);
	int last_line_count = 1;
	for (i = 0; i < editSurface->h && line; i += last_line_count*textFont->maxheight )
	{
		int text_extra = 0;
		if (showLines)
		{
			if (wrapLines)
				last_line_count = spFontDrawTextBlock(left,textShift+number_width,i,0,line->block,lines_per_screen*textFont->maxheight,0,textFont);
			else
				spFontDraw(textShift+number_width,i,0,line->line,textFont);
			spSetPattern8(pattern,pattern,pattern,pattern,pattern,pattern,pattern,pattern);
			spLine(number_width,i+last_line_count*letter->height,number_width,screen->w,i+last_line_count*letter->height,0,EDIT_LINE_COLOR);
			spDeactivatePattern();
			sprintf(buffer,"%i:",number);
			spRectangle(number_width/2-2,i+(last_line_count*(textFont->maxheight-2))/2,0,number_width,last_line_count*textFont->maxheight,EDIT_NUMBER_BACKGROUND_COLOR);
			spFontDrawRight(number_width-1,i,0,buffer,textFont);
			text_extra = number_width-1;
		}
		else
		{
			if (wrapLines)
				last_line_count = spFontDrawTextBlock(left,textShift,i,0,line->block,lines_per_screen*textFont->maxheight,0,textFont);
			else
				spFontDraw(textShift,i,0,line->line,textFont);
			spSetPattern8(pattern,pattern,pattern,pattern,pattern,pattern,pattern,pattern);
			spLine(0,i+last_line_count*letter->height,0,screen->w,i+last_line_count*letter->height,0,EDIT_LINE_COLOR);
			spDeactivatePattern();
		}
		if (line == momLine)
		{
			text_extra += momLineCursorPos;
			int extra_x = 0;
			int extra_y = 0;
			if (wrapLines)
			{
				int l_pos = line_pos;
				int i = 0;
				while (i < momLine->block->line_count-1 && l_pos-i > momLine->block->line[i].count) //for every line (i) l_pos have to modified more
				{
					extra_x -= momLine->block->line[i].width+space_width;
					extra_y += textFont->maxheight;
					l_pos -= momLine->block->line[i].count;
					i++;
				}
			}
			if (!(blink & 512))
			{
				spLine(extra_x+textShift+text_extra-1,
				       extra_y+i+letter->height-textFont->maxheight,0,
				       extra_x+textShift+text_extra+2,
				       extra_y+i+letter->height-textFont->maxheight,0,EDIT_TEXT_COLOR);
				spLine(extra_x+textShift+text_extra-1,
				       extra_y+i+letter->height,0,
				       extra_x+textShift+text_extra+2,
				       extra_y+i+letter->height,0,EDIT_TEXT_COLOR);
				spLine(extra_x+textShift+text_extra,
				       extra_y+i+letter->height-textFont->maxheight,0,
				       extra_x+textShift+text_extra,
				       extra_y+i+letter->height,0,EDIT_TEXT_COLOR);
			}
		}
		number++;
		line = line->next;
	}
	//drawing all
	spSelectRenderTarget(spGetWindowSurface());
	spClearTarget( BACKGROUND_COLOR );
	spBlitSurface( screen->w/2,screen->h/2,0,editSurface);
	if (text_changed)
		sprintf(buffer,"*%s",last_filename);
	else
		sprintf(buffer,"%s",last_filename);
	spFontDraw(0,0,0,buffer,font);

	if (spIsKeyboardPolled() && spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)
		spBlitSurface(screen->w/2,screen->h-spGetVirtualKeyboard()->h/2-font->maxheight,0,spGetVirtualKeyboard());

	if (!spIsKeyboardPolled())
	{
		spFontDrawMiddle(screen->w/2,0,0,"[M]Main",font);
		spFontDrawRight(screen->w,0,0,"[S]Options",font);
		spFontDrawMiddle(screen->w/2,screen->h-font->maxheight,0,\
			"[o]Enter text   "\
			"[3]Load   "\
			"[4]Save",font);
	}
	else
		spFontDrawMiddle(screen->w/2,screen->h-font->maxheight,0,\
			"[o]Letter  "\
			"[4]Done  "\
			"[M]Return  "\
			"[3]Space  "\
			"[l]\t  "\
			"[c]<-",font);
}

void draw()
{
	draw_without_flip();
	spFlip();
}

int calc(Uint32 steps)
{
	int momBlockLine = 0;
	int missedLetters = 0;
	if (wrapLines)
	{
		int l_pos = line_pos;
		int i = 0;
		while (i < momLine->block->line_count-1 && l_pos > momLine->block->line[i].count)
		{
			momBlockLine++;
			missedLetters+=momLine->block->line[i].count+1; //+space
			l_pos -= momLine->block->line[i].count;
			i++;
		}
	}
	int pos_in_line = line_pos - missedLetters;
	blink += steps;
	if (!spIsKeyboardPolled())
	{
		if (time_until_next > 0)
			time_until_next -= steps;
		if (spGetInput()->axis[1] < 0)
		{
			if (time_until_next <= 0)
			{
				if (wrapLines && momBlockLine > 0)
				{
					line_pos -= momLine->block->line[momBlockLine-1].count+1;
					while ((momLine->line[line_pos] & 192) == 128)
						line_pos--;
				}
				else
				if (momLine->prev)
				{
					momLine = momLine->prev;
					if (wrapLines)
					{
						line_pos = momLine->length-(momLine->block->line[momLine->block->line_count-1].count)+pos_in_line;
						while ((momLine->line[line_pos] & 192) == 128)
							line_pos--;
					}
					line_number--;
				}
				next_in_a_row++;
				time_until_next = 300/next_in_a_row;
				blink = 0;
				if (line_pos > momLine->length)
				{
					line_pos = momLine->length;
					while ((momLine->line[line_pos] & 192) == 128)
						line_pos--;
				}
			}
		}
		else
		if (spGetInput()->axis[1] > 0)
		{
			if (time_until_next <= 0)
			{
				if (wrapLines && momBlockLine+1 < momLine->block->line_count)
				{
					line_pos += momLine->block->line[momBlockLine].count+1;
					while ((momLine->line[line_pos] & 192) == 128)
						line_pos--;
				}
				else
				if (momLine->next)
				{
					momLine = momLine->next;
					line_pos = pos_in_line;
					while ((momLine->line[line_pos] & 192) == 128)
						line_pos--;
					line_number++;
				}
				next_in_a_row++;
				time_until_next = 300/next_in_a_row;
				blink = 0;
				if (line_pos > momLine->length)
				{
					line_pos = momLine->length;
					while ((momLine->line[line_pos] & 192) == 128)
						line_pos--;
				}
			}
		}
		else
		if (spGetInput()->button[SP_BUTTON_L] && momLine->prev)
		{
			if (time_until_next <= 0)
			{
				int i;
				for (i = 0; i < 32 && momLine->prev; i++)
					momLine = momLine->prev;
				line_number-=i;
				next_in_a_row++;
				time_until_next = 300/next_in_a_row;
				blink = 0;
				if (line_pos > momLine->length)
				{
					line_pos = momLine->length;
					while ((momLine->line[line_pos] & 192) == 128)
						line_pos--;
				}
			}
		}
		else
		if (spGetInput()->button[SP_BUTTON_R] && momLine->next)
		{
			if (time_until_next <= 0)
			{
				int i;
				for (i = 0; i < 32 && momLine->next; i++)
					momLine = momLine->next;
				line_number+=i;
				next_in_a_row++;
				time_until_next = 300/next_in_a_row;
				blink = 0;
				if (line_pos > momLine->length)
				{
					line_pos = momLine->length;
					while ((momLine->line[line_pos] & 192) == 128)
						line_pos--;
				}
			}
		}
		else
		if (spGetInput()->axis[0] < 0)
		{
			if (time_until_next <= 0)
			{
				if (line_pos > 0)
				{
					line_pos--;
					while (line_pos > 0 && (momLine->line[line_pos] & 192) == 128)
						line_pos--;
				}
				else
				if (momLine->prev)
				{
					momLine = momLine->prev;
					line_number--;
					line_pos = momLine->length;
					while ((momLine->line[line_pos] & 192) == 128)
						line_pos--;
				}
				next_in_a_row++;
				time_until_next = 300/next_in_a_row;
				blink = 0;
			}
		}
		else
		if (spGetInput()->axis[0] > 0)
		{
			if (time_until_next <= 0)
			{
				if (line_pos < momLine->length)
				{
					if (momLine->line[line_pos] & 128)
					{
						do
							line_pos++;
						while ((momLine->line[line_pos] & 192) == 128);
					}
					else
						line_pos++;
				}
				else
				if (momLine->next)
				{
					momLine = momLine->next;
					line_number++;
					line_pos = 0;
				}
				next_in_a_row++;
				time_until_next = 300/next_in_a_row;
				blink = 0;
			}
		}
		else
		{
			time_until_next = 0;
			next_in_a_row = 0;
		}
		if (spGetInput()->button[SP_BUTTON_START])
		{
			spGetInput()->button[SP_BUTTON_START] = 0;
			main_menu();
		}
		if (spGetInput()->button[SP_BUTTON_SELECT])
		{
			spGetInput()->button[SP_BUTTON_SELECT] = 0;
			options_menu();
		}
		if (spGetInput()->button[SP_PRACTICE_3])
		{
			spGetInput()->button[SP_PRACTICE_3] = 0;
			load_dialog();
		}
		if (spGetInput()->button[SP_PRACTICE_4])
		{
			spGetInput()->button[SP_PRACTICE_4] = 0;
			menu_save(0,NULL);
		}
		if (spGetInput()->button[SP_PRACTICE_OK])
		{
			spGetInput()->button[SP_PRACTICE_OK] = 0;
			spPollKeyboardInput(enter_buffer,sizeof(enter_buffer),SP_PRACTICE_OK_MASK);
		}
	}
	else
	{
		if (spGetInput()->button[SP_PRACTICE_4])
		{
			spGetInput()->button[SP_PRACTICE_4] = 0;
			spStopKeyboardInput();
		}
		if (spGetInput()->button[SP_BUTTON_R])
		{
			spGetInput()->button[SP_BUTTON_R] = 0;
			addReturn();
		}
		if (spGetInput()->button[SP_BUTTON_L])
		{
			spGetInput()->button[SP_BUTTON_L] = 0;
			if (tabs_as_spaces && tab_mode)
			{
				int i;
				for (i = 0; i < tab_mode; i++)
					enter_buffer[1+i] = ' ';
				enter_buffer[1+i] = 0;
			}
			else
			{
				enter_buffer[1] = '\t';
				enter_buffer[2] = 0;
			}
		}
	}
	if (enter_buffer[1] != 0)
	{
		addToLine(&enter_buffer[1]);
		enter_buffer[0] = 'a';
		enter_buffer[1] = 0;
		spGetInput()->keyboard.pos = 1;
	}
	if (enter_buffer[0] == 0)
	{
		removeFromLine();
		enter_buffer[0] = 'a';
		enter_buffer[1] = 0;
		spGetInput()->keyboard.pos = 1;
	}
	return exit_now;
}

void resize(Uint16 w,Uint16 h)
{
	//Setup of the new/resized window
	spSelectRenderTarget(spGetWindowSurface());
  
	spFontShadeButtons(1);
	//Font Loading
	spFontSetShadeColor(BACKGROUND_COLOR);
	if (font)
		spFontDelete(font);
	font = spFontLoad(FONT_LOCATION,FONT_SIZE*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(font,SP_FONT_GROUP_ASCII""SP_FONT_GROUP_GERMAN,FONT_COLOR);//whole ASCII
	spFontAddBorder(font,BACKGROUND_COLOR);
	spFontMulWidth(font,14<<SP_ACCURACY-4);
	add_tab(font);

	spFontAddButton( font, 'M', SP_BUTTON_START_NAME, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddButton( font, 'S', SP_BUTTON_SELECT_NAME, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddButton( font, 'l', SP_BUTTON_L_NAME, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddButton( font, 'r', SP_BUTTON_R_NAME, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddButton( font, 'o', SP_PRACTICE_OK_NAME, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddButton( font, 'c', SP_PRACTICE_CANCEL_NAME, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddButton( font, '3', SP_PRACTICE_3_NAME, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddButton( font, '4', SP_PRACTICE_4_NAME, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddArrowButton( font, '<', SP_BUTTON_ARROW_LEFT, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddArrowButton( font, '^', SP_BUTTON_ARROW_UP, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddArrowButton( font, '>', SP_BUTTON_ARROW_RIGHT, FONT_COLOR, BACKGROUND_COLOR );
	spFontAddArrowButton( font, 'v', SP_BUTTON_ARROW_DOWN, FONT_COLOR, BACKGROUND_COLOR );


	spFontSetShadeColor(EDIT_BACKGROUND_COLOR);
	if (fontInverted)
		spFontDelete(fontInverted);
	fontInverted = spFontLoad(FONT_LOCATION,FONT_SIZE*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(fontInverted,SP_FONT_GROUP_ASCII""SP_FONT_GROUP_GERMAN,EDIT_TEXT_COLOR);//whole ASCII
	spFontAddBorder(fontInverted,EDIT_BACKGROUND_COLOR);
	spFontMulWidth(fontInverted,14<<SP_ACCURACY-4);
	add_tab(fontInverted);


	if (editSurface)
		spDeleteSurface(editSurface);
	editSurface = spCreateSurface(w,h-2*font->maxheight);

	spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_ALWAYS,0,h-w*48/320-font->maxheight,w,w*48/320,spLoadSurface("./data/keyboard320.png"),spLoadSurface("./data/keyboardShift320.png"));
	spSetVirtualKeyboardBackspaceButton(SP_PRACTICE_CANCEL);
	spSetVirtualKeyboardSpaceButton(SP_PRACTICE_3);
}

void init_glutexto()
{
	spInitCore();
	spSetAffineTextureHack(0); //We don't need it :)
	spInitMath();
	screen = spCreateDefaultWindow();
	resize(screen->w,screen->h);
	spSetZSet(0);
	spSetZTest(0);
	load_fonts();
	load_settings();
	reloadTextFont();
	newText();
}

void quit_glutexto()
{
	clearText();
	spFontDelete(font);
	spFontDelete(fontInverted);
	spFontDelete(textFont);
	spQuitCore();
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	init_glutexto();
	spLoop( draw, calc, 10, resize, NULL );
	quit_glutexto();
	return 0;
}
