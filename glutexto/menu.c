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

typedef struct sMenuItem *pMenuItem;
typedef struct sMenuItem {
	char name[64];
	int ( *callBack )( int action, char* menu_name );
	pMenuItem next;
} tMenuItem;

pMenuItem firstMenuItem = NULL;

int menu_count = 0;
int menu_sel = 0;
char menu_name[64];

#define MENU_WIDTH 3/5

void draw_menu()
{
	draw_without_flip();
	spRectangle(screen->w*MENU_WIDTH,screen->h/4,0,screen->w*2/3,(4+menu_count)*font->maxheight,BACKGROUND_COLOR);
	int pos = screen->h/4-(4+menu_count)*font->maxheight/2;
	spFontDrawMiddle(screen->w*MENU_WIDTH,pos,0,menu_name,font);
	pos+=font->maxheight*2;
	int i;
	pMenuItem item = firstMenuItem;
	for (i = 0;i < menu_count; i++)
	{
		if ( i == menu_sel )
		{
			char buffer[64];
			sprintf(buffer,"> %s <",item->name);
			spFontDrawMiddle(screen->w*MENU_WIDTH,pos,0,buffer,font);
			int l = spFontWidth(buffer,font);
			spLine(screen->w*MENU_WIDTH-l/2,pos+font->maxheight-1,0,screen->w*MENU_WIDTH+l/2,pos+font->maxheight-1,0,FONT_COLOR);
		}
		else
			spFontDrawMiddle(screen->w*MENU_WIDTH,pos,0,item->name,font);
		pos+=font->maxheight;
		item = item->next;
	}
	spFontDrawMiddle(screen->w*MENU_WIDTH,screen->h/4+(2+menu_count)*font->maxheight/2,0,"[o]Choose    [c]Back",font);
	spFlip();
}

int calc_menu(Uint32 steps)
{
	if (spGetInput()->axis[1] < 0)
	{
		spGetInput()->axis[1] = 0;
		menu_sel = (menu_sel+menu_count-1) % menu_count;
	}
	if (spGetInput()->axis[1] > 0)
	{
		spGetInput()->axis[1] = 0;
		menu_sel = (menu_sel+1)%menu_count;
	}
	if (spGetInput()->button[SP_PRACTICE_CANCEL])
	{
		spGetInput()->button[SP_PRACTICE_CANCEL] = 0;
		return 1;
	}
	if (spGetInput()->button[SP_PRACTICE_OK])
	{
		spGetInput()->button[SP_PRACTICE_OK] = 0;
		pMenuItem item = firstMenuItem;
		int i;
		for (i = 0;i < menu_sel; i++)
			item = item->next;
		if (item->callBack)
			return item->callBack(0,item->name);
	}
	if (spGetInput()->axis[0] < 0)
	{
		spGetInput()->axis[0] = 0;
		pMenuItem item = firstMenuItem;
		int i;
		for (i = 0;i < menu_sel; i++)
			item = item->next;
		if (item->callBack)
			return item->callBack(-1,item->name);
	}
	if (spGetInput()->axis[0] > 0)
	{
		spGetInput()->axis[0] = 0;
		pMenuItem item = firstMenuItem;
		int i;
		for (i = 0;i < menu_sel; i++)
			item = item->next;
		if (item->callBack)
			return item->callBack(1,item->name);
	}
	return 0;
}

void run_menu()
{
	spLoop(draw_menu,calc_menu,10,resize,NULL);
}

void delete_menu()
{
	while (firstMenuItem)
	{
		pMenuItem next = firstMenuItem->next;
		free(firstMenuItem);
		firstMenuItem = next;
	}
	menu_sel = 0;
	menu_count = 0;
}

void add_menu(char* name,int ( *callBack )( int action, char* menu_name ))
{
	pMenuItem item = (pMenuItem)malloc(sizeof(tMenuItem));
	sprintf(item->name,"%s",name);
	item->callBack = callBack;
	item->next = firstMenuItem;
	firstMenuItem = item;
	menu_count++;
}

int menu_exit(int action,char* menu_name)
{
	if (action == 0)
	{
		exit_now = 1;
		return 1;
	}
	return 0;
}

int font_change(int action,char* menu_name)
{
	if (action == 1)
	{
		selectedFont = selectedFont->next;
		if (selectedFont == NULL)
			selectedFont = firstFont;		
	}
	else
	if (action == -1)
	{
		pFont before = NULL;
		pFont mom = firstFont;
		while (mom != selectedFont)
		{
			before = mom;
			mom = mom->next;
		}
		selectedFont = before;
		if (selectedFont == NULL)
		{
			mom = firstFont;
			while (mom)
			{
				selectedFont = mom;
				mom = mom->next;
			}
		}
	}
	if (action != 0)
	{
		sprintf(menu_name,"Font: %s",selectedFont->name);
		reloadTextFont();
		save_settings();
		updateWrapLines();
	}
	return 0;
}

int size_change(int action,char* menu_name)
{
	if (action == 1)
	{
		if (fontSize < MAX_FONT_SIZE)
			fontSize++;
	}
	else
	if (action == -1)
	{
		if (fontSize > MIN_FONT_SIZE)
			fontSize--;
	}
	if (action != 0)
	{
		sprintf(menu_name,"Font Size: %i",fontSize);
		reloadTextFont();
		save_settings();
		updateWrapLines();
	}
	return 0;
}

int line_change(int action,char* menu_name)
{
	showLines = 1-showLines;
	if (showLines)
		sprintf(menu_name,"Show lines: Yes");
	else
		sprintf(menu_name,"Show lines: No");
	save_settings();
	updateWrapLines();
	return 0;
}

int wrap_change(int action,char* menu_name)
{
	wrapLines = 1-wrapLines;
	if (wrapLines)
		sprintf(menu_name,"Wrap lines: Yes");
	else
		sprintf(menu_name,"Wrap lines: No");
	save_settings();
	updateWrapLines();
	return 0;
}

int tab_space_change(int action,char* menu_name)
{
	tabs_as_spaces = 1-tabs_as_spaces;
	if (tabs_as_spaces)
		sprintf(menu_name,"Insert spaces instead of tab: Yes");
	else
		sprintf(menu_name,"Insert spaces instead of tab: No");
	save_settings();
	return 0;
}

int tab_mode_change(int action,char* menu_name)
{
	if (action == 1)
	{
		if (tab_mode < MAX_TAB_SIZE)
			tab_mode++;
	}
	else
	if (action == -1)
	{
		if (tab_mode > 0)
			tab_mode--;
	}
	if (action != 0)
	{
		if (tab_mode)
			sprintf(menu_name,"Tab Mode: %i Spaces",tab_mode);
		else
			sprintf(menu_name,"Tab Mode: \t-Sign");
		reloadTextFont();
		save_settings();
		updateWrapLines();
	}
	return 0;
}

int menu_load(int action,char* menu_name)
{
	if (action == 0)
	{
		load_dialog();
		return 1;
	}
	return 0;
}

int menu_save_as(int action,char* menu_name)
{
	if (action == 0)
	{
		save_as_dialog();
		return 1;
	}
	return 0;
}

int menu_save(int action,char* menu_name)
{
	if (action == 0)
	{
		if (complete_filename[0] == 0)
			menu_save_as(action,menu_name);
		else
			saveText(complete_filename);
		return 1;
	}
	return 0;
}

int menu_new(int action,char* menu_name)
{
	if (action == 0)
	{
		newText();
		return 1;
	}
	return 0;
}

void main_menu()
{
	sprintf(menu_name,"Main menu");
	add_menu("Exit",menu_exit);
	add_menu("Save as",menu_save_as);
	add_menu("Save",menu_save);
	add_menu("Load",menu_load);
	add_menu("New",menu_new);
	run_menu();
	delete_menu();
}

void options_menu()
{
	char buffer[64];
	sprintf(menu_name,"Options menu");
		
	if (tabs_as_spaces)
		add_menu("Insert spaces instead of tab: Yes",tab_space_change);
	else
		add_menu("Insert spaces instead of tab: No",tab_space_change);
	if (tab_mode)
		sprintf(buffer,"Tab Mode: %i Spaces",tab_mode);
	else
		sprintf(buffer,"Tab Mode: \t-Sign");
	add_menu(buffer,tab_mode_change);
	if (wrapLines)
		add_menu("Wrap lines: Yes",wrap_change);
	else
		add_menu("Wrap lines: No",wrap_change);
	if (showLines)
		add_menu("Show lines: Yes",line_change);
	else
		add_menu("Show lines: No",line_change);
	sprintf(buffer,"Font Size: %i",fontSize);
	add_menu(buffer,size_change);
	sprintf(buffer,"Font: %s",selectedFont->name);
	add_menu(buffer,font_change);
	run_menu();
	delete_menu();	
}
