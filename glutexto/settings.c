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

#if defined(GCW) || (defined(X86CPU) && !defined(WIN32))
char* get_path(char* buffer,char* file)
{
	sprintf(buffer,"%s/.config/glutexto",getenv("HOME"));
	spCreateDirectoryChain(buffer);
	sprintf(buffer,"%s/.config/glutexto/%s",getenv("HOME"),file);
	return buffer;
}
#else
char* get_path(char* buffer,char* file)
{
	sprintf(buffer,"./%s",file);
	return buffer;
}
#endif

void load_settings()
{
	char buffer[256];
	SDL_RWops *file=SDL_RWFromFile(get_path(buffer,"settings.ini"),"r");
	if (file == NULL)
	{
		while (selectedFont)
		{
			if (strcmp(selectedFont->name,"LiberationSans") == 0)
				break;
			selectedFont = selectedFont->next;
		}
		if (selectedFont == NULL)
			selectedFont = firstFont;
	}
	else
	{
		while (spReadOneLine(file,buffer,256) == 0)
		{
			char* value = strchr(buffer,':');
			if (!value)
				continue;
			value+=2;
			if (strstr(buffer,"Font: ") == buffer)
			{
				selectedFont = firstFont;
				while (selectedFont && strcmp(value,selectedFont->name) != 0)
					selectedFont = selectedFont->next;
				if (selectedFont == NULL)
					selectedFont = firstFont;
			}
			else
			if (strstr(buffer,"Show Lines: ") == buffer)
				showLines = (atoi(value) != 0);
			else
			if (strstr(buffer,"Wrap Lines: ") == buffer)
				wrapLines = (atoi(value) != 0);
			else
			if (strstr(buffer,"Tab Length: ") == buffer)
			{
				tab_mode = atoi(value);
				if (tab_mode < 0)
					tab_mode = 0;
				if (tab_mode > MAX_TAB_SIZE)
					tab_mode = MAX_TAB_SIZE;
			}
			else
			if (strstr(buffer,"Tabs As Spaces: ") == buffer)
				tabs_as_spaces = (atoi(value) != 0);
			else
			if (strstr(buffer,"Font Size: ") == buffer)
			{
				fontSize = atoi(value);
				if (fontSize < MIN_FONT_SIZE)
					fontSize = MIN_FONT_SIZE;
				if (fontSize > MAX_FONT_SIZE)
					fontSize = MAX_FONT_SIZE;
			}
			else
			if (strstr(buffer,"Dialog Folder: ") == buffer)
				sprintf(dialog_folder,"%s",value);
		}
		SDL_RWclose(file);
	}
	if (spFileExists(dialog_folder) == 0)
		sprintf(dialog_folder,"/usr/local/home");
	if (spFileExists(dialog_folder) == 0)
		sprintf(dialog_folder,"/home");
	if (spFileExists(dialog_folder) == 0)
		sprintf(dialog_folder,"/");
}

void save_settings()
{
	char buffer[256];
	SDL_RWops *file=SDL_RWFromFile(get_path(buffer,"settings.ini"),"w");
	sprintf(buffer,"Font: %s",selectedFont->name);
	spWriteOneLine(file,buffer);
	sprintf(buffer,"Font Size: %i",fontSize);
	spWriteOneLine(file,buffer);
	sprintf(buffer,"Show Lines: %i",showLines);
	spWriteOneLine(file,buffer);
	sprintf(buffer,"Wrap Lines: %i",wrapLines);
	spWriteOneLine(file,buffer);
	sprintf(buffer,"Dialog Folder: %s",dialog_folder);
	spWriteOneLine(file,buffer);
	sprintf(buffer,"Tab Length: %i",tab_mode);
	spWriteOneLine(file,buffer);
	sprintf(buffer,"Tabs As Spaces: %i",tabs_as_spaces);
	spWriteOneLine(file,buffer);
	SDL_RWclose(file);
}
