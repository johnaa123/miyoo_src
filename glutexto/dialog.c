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

int dialog_kind = 0;
char dialog_filename[512];
spFileListPointer dialog_list = NULL;
spFileListPointer dialog_list_mom = NULL;

#define DIALOG_SHOW_BEFORE 4
#define DIALOG_UP ".. (upper directory)"

void add_up(spFileListPointer *directory,char* root)
{
	if (strcmp(root,"/") == 0)
		return;
	spFileListPointer up = (spFileListPointer)malloc(sizeof(spFileList));
	up->next = *directory;
	if (*directory)
		(*directory)->prev = up;
	up->prev = NULL;
	sprintf(up->name,DIALOG_UP);
	up->type = SP_FILE_DIRECTORY;
	*directory = up;
}

void dialog_test_and_add(int* pos,spFileListPointer mom,int special)
{
	if (mom == NULL)
		return;
	char buffer[512];
	int i;
	for (i = strlen(mom->name)-1; i >= 0; i--)
	{
		if (mom->name[i] == '/')
			break;
	}
	i++;
	char filename[512];
	if (mom->type & SP_FILE_DIRECTORY)
		sprintf(filename,"[d] %s",&(mom->name[i]));
	else
		sprintf(filename,"[f] %s",&(mom->name[i]));
	if (special)
		sprintf(buffer,"> %s <",filename);
	else
		sprintf(buffer,"%s",filename);
	spFontDraw(5,*pos,0,buffer,fontInverted);
	if (special)
		spLine(5,*pos+fontInverted->maxheight-1,0,5+spFontWidth(buffer,font),*pos+fontInverted->maxheight-1,0,EDIT_TEXT_COLOR);		
	*pos+=fontInverted->maxheight+1;
}

void draw_dialog()
{
	//draw files
	spSelectRenderTarget(editSurface);
	spClearTarget( EDIT_BACKGROUND_COLOR );
	spFileListPointer before[DIALOG_SHOW_BEFORE];
	memset( before, 0, sizeof(spFileListPointer)*DIALOG_SHOW_BEFORE);
	
	int i;
	spFileListPointer mom = dialog_list_mom;
	for( i = DIALOG_SHOW_BEFORE-1; i >=0; i--)
	{
		mom = mom->prev;
		if (mom == NULL)
			break;
		before[i] = mom;
	}
	
	int pos = 5;
	for ( i = 0; i < DIALOG_SHOW_BEFORE; i++)
		dialog_test_and_add(&pos,before[i],0);
	dialog_test_and_add(&pos,dialog_list_mom,1);
	mom = dialog_list_mom->next;
	while (mom && pos < editSurface->h-fontInverted->maxheight)
	{
		dialog_test_and_add(&pos,mom,0);
		mom = mom->next;
	}
	
	
	//drawing all
	spSelectRenderTarget(spGetWindowSurface());
	spClearTarget( BACKGROUND_COLOR );
	spBlitSurfacePart( screen->w/2,(screen->h+fontInverted->maxheight)/2,0,editSurface,0,0,editSurface->w,editSurface->h-fontInverted->maxheight-2);
	switch (dialog_kind)
	{
		case 0:
			spFontDrawMiddle(screen->w/2,0,0,"Choose file to load",font);
			spFontDrawMiddle(screen->w/2,screen->h-font->maxheight,0,"[o]Choose [f]ile or enter [d]irectory    [c]Cancel",font);
			break;
		case 1:
			spFontDrawMiddle(screen->w/2,0,0,"Choose file/location to save",font);
			spFontDrawMiddle(screen->w/2,screen->h-font->maxheight,0,"[o]Choose [f]ile/[d]irectory   [3]New file   [c]Cancel",font);
			break;
	}
	char buffer[256];
	if (strcmp(dialog_folder,"/") == 0)
		sprintf(buffer,"Folder:   /");
	else
		sprintf(buffer,"Folder:   %s/",dialog_folder);
	spFontDrawMiddle(screen->w/2,font->maxheight,0,buffer,font);
	
	spFlip();
}

int calc_dialog(Uint32 steps)
{
	if (spGetInput()->button[SP_PRACTICE_CANCEL])
	{
		spGetInput()->button[SP_PRACTICE_CANCEL] = 0;
		return 1;
	}
	if (time_until_next > 0)
		time_until_next -= steps;
	if (spGetInput()->axis[1] < 0 && dialog_list_mom->prev)
	{
		if (time_until_next <= 0)
		{
			dialog_list_mom = dialog_list_mom->prev;
			next_in_a_row++;
			time_until_next = 300/next_in_a_row;
		}
	}
	else
	if (spGetInput()->axis[1] > 0 && dialog_list_mom->next)
	{
		if (time_until_next <= 0)
		{
			dialog_list_mom = dialog_list_mom->next;
			next_in_a_row++;
			time_until_next = 300/next_in_a_row;
		}
	}
	else
	{
		time_until_next = 0;
		next_in_a_row = 0;
	}
	if (dialog_kind == 1 && spGetInput()->button[SP_PRACTICE_3])
	{
		spGetInput()->button[SP_PRACTICE_3] = 0;
		char filename[512] = "";
		if (get_string("Enter filename:",filename,512))
		{
			if (filename[0] != 0)
			{
				char buffer[512];
				if (strcmp(dialog_folder,"/") == 0)
					sprintf(buffer,"/%s",filename);
				else
					sprintf(buffer,"%s/%s",dialog_folder,filename);
				int yes = 1;
				if (spFileExists(buffer))
				{
					int i;
					for (i = strlen(buffer)-1; i >= 0; i--)
						if (buffer[i] == '/')
							break;
					i++;
					char buffer2[512];
					sprintf(buffer2,"Do you really want to overwrite\n%s?",&buffer[i]);
					yes = ask_yes_no(buffer2);
				}
				if (yes)
				{
					saveText(buffer);
					save_settings();
					return 1;
				}
			}
		}
	}
	if (dialog_kind == 0 && spGetInput()->button[SP_PRACTICE_OK])
	{
		spGetInput()->button[SP_PRACTICE_OK] = 0;
		if (dialog_list_mom->type & SP_FILE_DIRECTORY)
		{
			if (strcmp(dialog_list_mom->name,DIALOG_UP))
				sprintf(dialog_folder,"%s",dialog_list_mom->name);
			else
			{
				int i;
				for (i = strlen(dialog_folder)-1; i >= 0;i--)
					if (dialog_folder[i] == '/')
						break;
				dialog_folder[i] = 0;
				if (dialog_folder[0] == 0)
					sprintf(dialog_folder,"/");
			}
			spFileDeleteList(dialog_list);
			dialog_list = NULL;
			spFileGetDirectory( &dialog_list, dialog_folder, 0, 0);
			spFileSortList( &dialog_list, SP_FILE_SORT_BY_TYPE_AND_NAME);
			add_up(&dialog_list,dialog_folder);
			dialog_list_mom = dialog_list;
		}
		else
		{
			loadText(dialog_list_mom->name);
			save_settings();
			return 1;
		}
	}
	if (dialog_kind == 1 && spGetInput()->button[SP_PRACTICE_OK])
	{
		spGetInput()->button[SP_PRACTICE_OK] = 0;
		if (dialog_list_mom->type & SP_FILE_DIRECTORY)
		{
			if (strcmp(dialog_list_mom->name,DIALOG_UP))
				sprintf(dialog_folder,"%s",dialog_list_mom->name);
			else
			{
				int i;
				for (i = strlen(dialog_folder)-1; i >= 0;i--)
					if (dialog_folder[i] == '/')
						break;
				dialog_folder[i] = 0;
				if (dialog_folder[0] == 0)
					sprintf(dialog_folder,"/");
			}
			spFileDeleteList(dialog_list);
			dialog_list = NULL;
			spFileGetDirectory( &dialog_list, dialog_folder, 0, 0);
			spFileSortList( &dialog_list, SP_FILE_SORT_BY_TYPE_AND_NAME);
			add_up(&dialog_list,dialog_folder);
			dialog_list_mom = dialog_list;
		}
		else
		{
			int i;
			for (i = strlen(dialog_list_mom->name)-1; i >= 0; i--)
				if (dialog_list_mom->name[i] == '/')
					break;
			i++;
			char buffer[512];
			sprintf(buffer,"Do you really want to overwrite\n%s?",&dialog_list_mom->name[i]);
			if (ask_yes_no(buffer))
			{
				saveText(dialog_list_mom->name);
				save_settings();
				return 1;
			}
		}
	}
	return 0;
}

void run_dialog(int kind)
{
	dialog_filename[0] = 0;
	dialog_kind = kind;
	spFileGetDirectory( &dialog_list, dialog_folder, 0, 0);
	spFileSortList( &dialog_list, SP_FILE_SORT_BY_TYPE_AND_NAME);
	add_up(&dialog_list,dialog_folder);
	dialog_list_mom = dialog_list;
	spLoop(draw_dialog,calc_dialog,10,resize,NULL);
	spFileDeleteList(dialog_list);
	dialog_list = NULL;
}

void load_dialog()
{
	run_dialog(0);
}

void save_as_dialog()
{
	run_dialog(1);
}
