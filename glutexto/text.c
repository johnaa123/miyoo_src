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
  
int line_count = 0;
int text_changed = 0;
char last_filename[512] = "New document";
char complete_filename[512] = "New document";

void add_tab(spFontPointer font)
{
	spFontAddButton(font,'T',"Tab",FONT_COLOR,BACKGROUND_COLOR);
	spFontAdd(font,"\t",FONT_COLOR);
	spLetterPointer letter = spFontGetLetter(font,'\t');
	if (letter->surface)
		spDeleteSurface(letter->surface);
	if (tab_mode && font == textFont)
	{
		spLetterPointer space = spFontGetLetter(font,' ');
		letter->surface = spCreateSurface(space->width*tab_mode,font->maxheight);
		SDL_FillRect(letter->surface,NULL,SP_ALPHA_COLOR);
		letter->width = space->width*tab_mode;
	}
	else
	{
		spLetterPointer button = spFontGetButton(font,'T');
		letter->surface = spCopySurface(button->surface);
		letter->width = button->width*9/8;
	}
}

#define MAX_CHARS 65536
char textStringBuffer[MAX_CHARS] = SP_FONT_GROUP_ASCII;

void reloadTextFont()
{
	spFontSetShadeColor(EDIT_BACKGROUND_COLOR);
	if (textFont)
	{
		spFontGetLetterString(textFont,textStringBuffer,MAX_CHARS);
		spFontDelete(textFont);
	}
	textFont = spFontLoad(selectedFont->location,fontSize*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(textFont,textStringBuffer,EDIT_TEXT_COLOR);//whole ASCII
	add_tab(textFont);
}

int getNumberWidth()
{
	if (showLines)
	{
		int count = line_count;
		int max_line = 2;
		while (count > 0)
		{
			count = count/10;
			max_line++;
		}
		char buffer[256];
		int i;
		for (i = 0; i < max_line; i++)
			buffer[i]='8';
		buffer[i]=0;
		return spFontWidth(buffer,textFont);
	}
	return 0;
}

void updateWrapLine(pText line)
{
	if 	(wrapLines == 0)
		return;
	if (line->block)
		spDeleteTextBlock(line->block);
	line->block = spCreateTextBlock( line->line, editSurface->w-1-getNumberWidth(), textFont);
}

void updateWrapLines()
{
	if (wrapLines)
	{
		pText mom = text;
		while (mom)
		{
			updateWrapLine(mom);
			mom = mom->next;
		}
	}
	else
	{
		pText mom = text;
		while (mom)
		{
			if (mom->block)
			{
				spDeleteTextBlock(mom->block);
				mom->block = NULL;
			}
			mom = mom->next;
		}
	}
}

void clearText()
{
	while (text)
	{
		free(text->line);
		if (text->block)
			spDeleteTextBlock(text->block);
		pText next = text->next;
		free(text);
		text = next;
	}
	textEnd = NULL;
	line_count = 0;
	line_number = 0;
}

pText addTextLine(char* line,pText after)
{
	pText newText = (pText)malloc(sizeof(tText));
	newText->prev = after;
	if (after)
	{
		newText->next = after->next;
		if (after->next)
			after->next->prev = newText;
		after->next = newText;
	}
	else
	{
		newText->next = text;
		text = newText;
	}
	newText->length = strlen(line);
	newText->reserved = (newText->length+4)*5/4;
	newText->line = (char*)malloc(newText->reserved);
	newText->block = NULL;
	if (newText->next == NULL);
		textEnd = newText;
	memcpy(newText->line,line,newText->length+1);
	int old_line_count = getNumberWidth();
	line_count++;
	if (getNumberWidth() != old_line_count)
		updateWrapLines();
	else
		updateWrapLine(newText);
	spFontAdd(textFont,line,EDIT_TEXT_COLOR);
	return newText;
}

void addToLine(char* newBuffer)
{
	int l = strlen(newBuffer);
	if (l + momLine->length + 1 > momLine->reserved) //Realloc
	{
		momLine->reserved = (momLine->length + l +4)*5/4;
		char* new_reserved = (char*)malloc(momLine->reserved);
		memcpy(new_reserved,momLine->line,momLine->length+1);
		free(momLine->line);
		momLine->line = new_reserved;
	}
	char end_line[momLine->length-line_pos+1];
	memcpy(end_line,&(momLine->line[line_pos]),momLine->length-line_pos+1);
	sprintf(&(momLine->line[line_pos]),"%s%s",newBuffer,end_line);
	momLine->length += l;
	line_pos += l;
	text_changed = 1;
	updateWrapLine(momLine);
	spFontAdd(textFont,newBuffer,EDIT_TEXT_COLOR);
}


void addReturn()
{
	pText oldMomLine = momLine;
	int l = strlen(&(momLine->line[line_pos]));
	momLine = addTextLine(&(momLine->line[line_pos]),momLine);
	oldMomLine->line[line_pos] = 0;
	oldMomLine->length-=l;
	line_number++;
	line_pos = 0;	
	updateWrapLine(momLine);
	updateWrapLine(oldMomLine);
}

void removeFromLine()
{
	char end_line[momLine->length-line_pos+1];
	memcpy(end_line,&(momLine->line[line_pos]),momLine->length-line_pos+1);
	if (line_pos-1 < 0)
	{
		if (momLine->prev)
		{
			//Removing myself
			if (momLine == textEnd)
				textEnd = textEnd->prev;
			momLine->prev->next = momLine->next;
			if (momLine->next)
				momLine->next->prev = momLine->prev;
			free(momLine->line);
			if (momLine->block)
				spDeleteTextBlock(momLine->block);
			pText prev = momLine->prev;
			free (momLine);
			momLine = prev;
			line_number--;
			line_count--;
			int l = momLine->length;
			line_pos = l;
			addToLine(end_line);
			line_pos = l;
		}
	}
	else
	{
		line_pos--;
		sprintf(&(momLine->line[line_pos]),"%s",end_line);
		momLine->length--;
		text_changed = 1;
		updateWrapLine(momLine);
	}
}


void newText()
{
	if (text_changed)
	{
		char buffer[1024];
		sprintf(buffer,"You didn't save\n%s.\nDo you really want to create\na new document?",last_filename);
		if (ask_yes_no(buffer) == 0)
			return;
	}
	clearText();
	momLine = addTextLine("",NULL);
	line_number = 1;
	line_pos = 0;
	text_changed = 0;
	sprintf(last_filename,"New document");
	complete_filename[0] = 0;
}

void loadText(char* filename)
{
	if (text_changed)
	{
		char buffer[1024];
		int i;
		for (i = strlen(filename)-1;i >= 0; i--)
			if (filename[i] == '/')
				break;
		i++;
		sprintf(buffer,"You didn't save\n%s.\nDo you really want to open\n%s?",last_filename,&(filename[i]));
		if (ask_yes_no(buffer) == 0)
			return;
	}
	int i;
	for (i = strlen(filename)-1; i >= 0; i--)
		if (filename[i] == '/')
			break;
	i++;
	sprintf(last_filename,"%s",&filename[i]);
	sprintf(complete_filename,"%s",filename);
	printf("Loading %s...\n",filename);
	SDL_RWops *file=SDL_RWFromFile(filename,"r");
	if (file == NULL)
		return;
	clearText();
	char buffer[65536];
	pText last = text;
	while (spReadOneLine(file,buffer,65536) == 0)
		last = addTextLine(buffer,last);
	if (last == NULL)
		last = addTextLine("",last);
	momLine = text;
	line_number = 1;
	line_pos = 0;
	SDL_RWclose(file);
	text_changed = 0;
}

void saveText(char* filename)
{
	printf("Saving %s...\n",filename);
	SDL_RWops *file=SDL_RWFromFile(filename,"w");
	if (file == NULL)
		return;
	int i;
	for (i = strlen(filename)-1; i >= 0; i--)
		if (filename[i] == '/')
			break;
	i++;
	sprintf(last_filename,"%s",&filename[i]);
	sprintf(complete_filename,"%s",filename);
	pText line = text;
	while (line)
	{
		spWriteOneLine(file,line->line);
		line = line->next;
	}
	SDL_RWclose(file);
	text_changed = 0;
}

