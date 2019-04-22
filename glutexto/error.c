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
 
char error_message[1024];
char string_message[1024];
char *string_buffer;

void draw_error()
{
	int height = 3;
	int i;
	for (i = 0; error_message[i]!=0; i++)
		if (error_message[i] == '\n')
			height++;
	spRectangle(screen->w/2,screen->h/2,0,screen->w*3/4,(height)*font->maxheight,BACKGROUND_COLOR);
	spFontDrawMiddle(screen->w/2,screen->h/2-(height)*font->maxheight/2,0,error_message,font);
	spFontDrawMiddle(screen->w/2,screen->h/2+(height-2)*font->maxheight/2,0,SP_PRACTICE_OK_NAME": Yes    [c]No",font);
	spFlip();
}

int calc_error(Uint32 steps)
{
	if (spGetInput()->button[SP_PRACTICE_OK])
	{
		spGetInput()->button[SP_PRACTICE_OK] = 0;
		return 1;
	}
	if (spGetInput()->button[SP_PRACTICE_CANCEL])
	{
		spGetInput()->button[SP_PRACTICE_CANCEL] = 0;
		return -1;
	}
	return 0;
}

int ask_yes_no(char* message)
{
	sprintf(error_message,"%s",message);
	return (spLoop( draw_error, calc_error, 10, resize, NULL ) == 1);
}

void draw_string()
{
	int height = 5;
	int i;
	for (i = 0; string_message[i]!=0; i++)
		if (string_message[i] == '\n')
			height++;
	spRectangle(screen->w/2,screen->h/2,0,screen->w*3/4,(height)*font->maxheight,BACKGROUND_COLOR);
	spFontDrawMiddle(screen->w/2,screen->h/2-(height)*font->maxheight/2,0,string_message,font);
	spFontDrawMiddle(screen->w/2,screen->h/2+(height-6)*font->maxheight/2,0,string_buffer,font);
	spFontDrawMiddle(screen->w/2,screen->h/2+(height-2)*font->maxheight/2,0,"[o]Enter letter    [3]Ok    [c]Cancel",font);
	if (spIsKeyboardPolled() && spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)
		spBlitSurface(screen->w/2,screen->h-spGetVirtualKeyboard()->h/2-font->maxheight,0,spGetVirtualKeyboard());
	spFlip();	
}

int calc_string(Uint32 steps)
{
	if (spGetInput()->button[SP_PRACTICE_3])
	{
		spGetInput()->button[SP_PRACTICE_3] = 0;
		return 1;
	}
	if (spGetInput()->button[SP_PRACTICE_CANCEL])
	{
		spGetInput()->button[SP_PRACTICE_CANCEL] = 0;
		return -1;
	}
	return 0;	
}

int get_string(char* text,char* buffer,int buffer_len)
{
	sprintf(string_message,"%s",text);
	string_buffer = buffer;
	spPollKeyboardInput(string_buffer,buffer_len,SP_PRACTICE_OK_MASK);
	int result = (spLoop( draw_string, calc_string, 10, resize, NULL ) == 1);
	spStopKeyboardInput();
	return result;
}
