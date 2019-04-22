 /* This file is part of snowman.
  * Snowman is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  * 
  * Snowman is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with snowman.  If not, see <http://www.gnu.org/licenses/>
  * 
  * For feedback and questions about my Files and Projects please mail me,
  * Alexander Matthes (Ziz) , zizsdl_at_googlemail.com */
  
char removesnow(int count)
{
	if (ballcount==1)
		return 1;
	if (ballcount==2)
	{
		ballsize[1]-=count<<(SP_ACCURACY-5);
		gotchasmall=500;
		valuesmall=-count;		
		if (ballsize[1]==0)
			ballcount--;
		else
		if (ballsize[1]<0)
			return 1;
	}
	if (ballcount==3)
	{
		ballsize[0]-=count<<(SP_ACCURACY-5);
		if (ballsize[0]<0)
		{
			ballcount--;
			gotchabig=500;
			valuebig=-count-(ballsize[0]>>(SP_ACCURACY-5));
			ballsize[1]+=ballsize[0];
			ballsize[0]=0; 
			gotchasmall=500;
			valuesmall=-count-valuebig;
		}
		else
		{
			gotchabig=500;
			valuebig=-count;
		}
	}
	return 0;
}

char addonesnow()
{
	if (ballcount==3)
	{
		if (ballsize[0]>=(13<<(SP_ACCURACY-4)))
			return 0;
		ballsize[0]+=(1<<(SP_ACCURACY-5));
		gotchabig=500;
		valuebig=1;
		spSoundPlay(hu_chunk,-1,0,0,0);
		return 1;
	}
	if (ballcount==1)
	{
		//ballsize[2]+=(1<<(SP_ACCURACY-5));
		//if (ballsize[2]>=(7<<(SP_ACCURACY-4)))
		{
			ballsize[1]=0;
			ballcount++;
		}
	}
	if (ballcount==2)
	{
		ballsize[1]+=(1<<(SP_ACCURACY-5));
		gotchasmall=500;
		valuesmall=1;
		if (ballsize[1]>=(9<<(SP_ACCURACY-4)))
		{
			ballsize[0]=0;
			ballcount++;
		}
		spSoundPlay(hu_chunk,-1,0,0,0);
		return 1;
	}
	return 0;
}


void drawcharacter(Sint32 x,Sint32 y,Sint32 z,char right)
{
	Sint32* modellViewMatrix=spGetMatrix();
	int i;
	int red=((spCos(damaged*(1<<(SP_ACCURACY-6)))*127)>>SP_ACCURACY)+128;
	for (i=3-ballcount;i<3;i++)
	{
		y+=ballsize[i];
		Sint32 r=ballsize[i]+(1<<(SP_ACCURACY-5));
		//spEllipse(x,y,z,r,r,spGetRGB(255,255,255));
		Sint32 matrix[16];		
		memcpy(matrix,modellViewMatrix,64);
		spTranslate(x,y,z);
		if (i==0)
			spRotateZ(angle);
		if (i==1)
		{
			if (ballcount==3)
				spRotateZ(-angle);
			else
				spRotateZ(angle);
		}
		switch (i)
		{
			case 0:case 1:
				spBindTexture(sphere);
				spQuadTex3D(-r, r,0, 0, 0,
										-r,-r,0, 0,63,
										 r,-r,0,63,63,
										 r, r,0,63, 0,spGetRGB(255,red,red));
				break;
			case 2:
				if (right)
					spBindTexture(sphere_right);
				else
					spBindTexture(sphere_left);
				spQuadTex3D(-r*2, r,0,	0, 0,
										-r*2,-r,0,	0,63,
										 r*2,-r,0,127,63,
										 r*2, r,0,127, 0,spGetRGB(255,red,red));
				break;
		}
		memcpy(modellViewMatrix,matrix,64);
		y+=ballsize[i];
	}
	if (broom_exist)
	{
		int sum=0;
		int i;
		for (i=3-ballcount;i<3;i++)
			sum+=ballsize[i]*2;
		Sint32 matrix[16];
		memcpy(matrix,modellViewMatrix,64);
		modellViewMatrix[12]+=x;
		modellViewMatrix[13]+=y-(sum>>1);
		modellViewMatrix[14]+=z+(1<<SP_ACCURACY);		
		if (right)
		{
			spRotateZ( SP_PI>>2);
			if (in_hit>768)
				spRotateZ(-((864-in_hit)*SP_PI)>>7);
			else
				spRotateZ(-(in_hit*SP_PI)>>10);
		}
		else
		{
			spRotateZ( 7*SP_PI>>2);
			if (in_hit>768)
				spRotateZ(((864-in_hit)*SP_PI)>>7);
			else
				spRotateZ((in_hit*SP_PI)>>10);
		}
		
		if (in_hit>768)
			spMesh3DwithPos(0,(864-in_hit)<<(SP_ACCURACY-6),0,broom,0);
		else
			spMesh3DwithPos(0,(		in_hit)<<(SP_ACCURACY-9),0,broom,0);
		memcpy(modellViewMatrix,matrix,64);
	}
}

char testX(Sint32 x,Sint32 ox)
{
	biggest = getBiggest();
	bx =((x>>(SP_ACCURACY))+1)>>1;
	bxl=(((x-ballsize[biggest])>>(SP_ACCURACY))+1)>>1;
	bxr=(((x+ballsize[biggest])>>(SP_ACCURACY))+1)>>1;
	by =((y>>(SP_ACCURACY))+1)>>1;
	byb=by;
	if (((2*by-1)<<SP_ACCURACY)!=y)
		byb+=1;
	int sum=0;
	int i;
	for (i=3-ballcount;i<3;i++)
		sum+=ballsize[i]*2;
	byt =((((y-sum)>>(SP_ACCURACY))+1)>>1);
	bym =(byt + byb)>>1;
	//Solid Block on the left?
	if (bxl>=0 && bxl<level->width)
	{
		if (byb>0 && byb<=level->height &&
				level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]!= NULL &&
				level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->form > 0)
		{
			if ((level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->functionmask & 4) == 4 && ox>x) //right open
				return 0; 
			if ((level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->functionmask & 8) == 8 && ox<x) //left open
				return 0; 
			return 1;
		}
		if (byt>=0 && byt<level->height &&
				level->symbollist[level->layer[1][bxl+(byt)*level->width]]!= NULL &&
				level->symbollist[level->layer[1][bxl+(byt)*level->width]]->form > 0)
		{
			if ((level->symbollist[level->layer[1][bxl+(byt)*level->width]]->functionmask & 4) == 4 && ox>x) //right open
				return 0; 
			if ((level->symbollist[level->layer[1][bxl+(byt)*level->width]]->functionmask & 8) == 8 && ox<x) //left open
				return 0; 
			return 1;
		}
		if (bym>=0 && bym<level->height &&
				level->symbollist[level->layer[1][bxl+(bym)*level->width]]!= NULL &&
				level->symbollist[level->layer[1][bxl+(bym)*level->width]]->form > 0)
		{
			if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 4) == 4 && ox>x) //right open
				return 0; 
			if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 8) == 8 && ox<x) //left open
				return 0; 
			return 1;
		}
	}
	//Solid Block on the right?
	if (bxr>=0 && bxr<level->width)
	{
		if (byb>0 && byb<=level->height &&
				level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]!= NULL &&
				level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->form > 0)
		{
			if ((level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->functionmask & 8) == 8 && ox<x) //left open
				return 0; 
			if ((level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->functionmask & 4) == 4 && ox>x) //right open
				return 0; 
			return 2;
		}
		if (byt>=0 && byt<level->height &&
				level->symbollist[level->layer[1][bxr+(byt)*level->width]]!= NULL &&
				level->symbollist[level->layer[1][bxr+(byt)*level->width]]->form > 0)
		{
			if ((level->symbollist[level->layer[1][bxr+(byt)*level->width]]->functionmask & 8) == 8 && ox<x) //left open
				return 0; 
			if ((level->symbollist[level->layer[1][bxr+(byt)*level->width]]->functionmask & 4) == 4 && ox>x) //right open
				return 0; 
			return 2;
		}
		if (bym>=0 && bym<level->height &&
				level->symbollist[level->layer[1][bxr+(bym)*level->width]]!= NULL &&
				level->symbollist[level->layer[1][bxr+(bym)*level->width]]->form > 0)
		{
			if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 8) == 8 && ox<x) //left open
				return 0; 
			if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 4) == 4 && ox>x) //right open
				return 0; 
			return 2;
		}
	}
	return 0;
}

char testX_down(Sint32 x,Sint32 ox)
{
	biggest = getBiggest();
	int sum=0;
	int i;
	for (i=3-ballcount;i<3;i++)
		sum+=ballsize[i]*2;
	bx =((x>>(SP_ACCURACY))+1)>>1;
	bxl=(((x-ballsize[biggest])>>(SP_ACCURACY))+1)>>1;
	bxr=(((x+ballsize[biggest])>>(SP_ACCURACY))+1)>>1;
	bym = (((y-sum/2)>>SP_ACCURACY)+1)/2;
	//Solid Block on the left?
	if (bxl>=0 && bxl<level->width)
	{
		if (bym>=0 && bym<level->height &&
				level->symbollist[level->layer[1][bxl+(bym)*level->width]]!= NULL &&
				level->symbollist[level->layer[1][bxl+(bym)*level->width]]->form > 0)
		{
			if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 4) == 4 && ox>x) //right open
				return 0; 
			if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 8) == 8 && ox<x) //left open
				return 0; 
			return 1;
		}
	}
	//Solid Block on the right?
	if (bxr>=0 && bxr<level->width)
	{
		if (bym>=0 && bym<level->height &&
				level->symbollist[level->layer[1][bxr+(bym)*level->width]]!= NULL &&
				level->symbollist[level->layer[1][bxr+(bym)*level->width]]->form > 0)
		{
			if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 8) == 8 && ox<x) //left open
				return 0; 
			if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 4) == 4 && ox>x) //right open
				return 0; 
			return 2;
		}
	}
}

char fattest(Sint32 *x,Sint32 ox)
{
	char i=testX_down((*x),ox);
	if (i==1)
	{
		while (i==1)
		{
			i=testX_down((*x),ox);
			(*x)+=(1<<(SP_ACCURACY-4));
			printf("Left\n");
		}
		return 1;
	}
	if (i==2)
	{
		while (i==2)
		{
			i=testX_down((*x),ox);
			(*x)-=(1<<(SP_ACCURACY-4));
			printf("Right\n");
		}
		return 1;
	}
	return 0;
}

void playerEnemyInteraction()
{
	if (damaged)
		return;
	int sum=0;
	int i;
	penemy enemy=level->firstenemy;
	while (enemy!=NULL)
	{
		//Distance
		int hit = 0;
		if ( enemy->symbol->measures[2] == enemy->symbol->measures[3])
		{
			//Testing every Snowman circle with the enemy circle
			Sint32 Y = y;
			for (i=3-ballcount;i<3;i++)
			{
				Y-=ballsize[i];
				//Squared distance:
				if (spMax(x-enemy->x,Y-enemy->y) < (1 << SP_ACCURACY+3))
				{
					Sint32 d2 = spSquare(x-enemy->x >> 4)+spSquare(Y-enemy->y >> 4);
					if (d2 <= spSquare(enemy->symbol->measures[2]+ballsize[i] >> 4))
					{
						hit = 1;
						break;
					}
				}
				Y-=ballsize[i];
			}
		}
		else
		{
			sum = 0;
			for (i=3-ballcount;i<3;i++)
				sum+=ballsize[i]*2;
			biggest=getBiggest();
			if ( x+ballsize[biggest] >= enemy->x-enemy->symbol->measures[2] &&
				 x-ballsize[biggest] <= enemy->x+enemy->symbol->measures[2] &&
				 y-sum               <= enemy->y+enemy->symbol->measures[3] &&
				 y                   >= enemy->y-enemy->symbol->measures[3] ) //Hit
				hit = 1;
		}
		if (hit)
		{
			damaged=SP_PI>>(SP_ACCURACY-8);
			if (removesnow(3))
			{
				fade2=1024;
				spSoundPlay(negative_chunk,-1,0,0,0);
				return;
			}
			return;
		}
		enemy=enemy->next;
	}
}

void broomEnemyInteraction(char right)
{
	int sum=0;
	int i;
	for (i=3-ballcount;i<3;i++)
		sum+=ballsize[i]*2;
	Sint32 broomx=x;
	if (right)
		broomx+=+(2<<SP_ACCURACY);
	else
		broomx-=+(2<<SP_ACCURACY);
	Sint32 broomy=y-(sum>>1)+(1<<SP_ACCURACY);
	penemy ebefore=NULL;
	penemy enemy=level->firstenemy;
	while (enemy!=NULL)
	{
		//Distance
		int hit = 0;
		if ( enemy->symbol->measures[2] == enemy->symbol->measures[3])
		{
			//Squared distance:
			if (spMax(broomy-enemy->y,broomx-enemy->x) < (1 << SP_ACCURACY+3))
			{
				Sint32 d2 = spSquare(broomx-enemy->x)+spSquare(broomy-enemy->y);
				if (d2 > 0 && d2 <= spSquare(enemy->symbol->measures[2]+SP_ONE))
					hit = 1;
			}
		}
		else
		{
			if ( broomx >= enemy->x-enemy->symbol->measures[2]-(1<<SP_ACCURACY) &&
				 broomx <= enemy->x+enemy->symbol->measures[2]+(1<<SP_ACCURACY) &&
				 broomy >= enemy->y-enemy->symbol->measures[3]-(1<<SP_ACCURACY) &&
				 broomy <= enemy->y+enemy->symbol->measures[3]+(1<<SP_ACCURACY) ) //Hit
				hit = 1;
		}
		if (hit)
		{
			//newexplosion(PARTICLES,broomx,broomy,0,1024,spGetRGB(86,22,0));
			enemy->health-=2;
			newexplosion(2*PARTICLES,enemy->x,enemy->y,0,1024,enemy->symbol->color);
			if (enemy->health<=0)
			{
				enemyKilled++;
				if (ebefore==NULL)
					level->firstenemy=enemy->next;
				else
					ebefore->next=enemy->next;
				//newexplosion(PARTICLES,enemy->x,enemy->y,0,1024,enemy->symbol->color);
				free(enemy);
				enemy=ebefore;
			}
			break;
		}
		ebefore=enemy;
		if (enemy!=NULL)
			enemy=enemy->next;
	}
}

#if defined GCW || (defined DESKTOP && !defined WIN32)
char* get_path(char* buffer,char* file)
{
	sprintf(buffer,"%s/.config/snowman/%s",getenv("HOME"),file);
	return buffer;
}
#else
char* get_path(char* buffer,char* file)
{
	sprintf(buffer,"./%s",file);
	return buffer;
}
#endif

void savelevelcount()
{
	char buffer[256];
	spCreateDirectoryChain(get_path(buffer,""));
	SDL_RWops *file=SDL_RWFromFile(get_path(buffer,"levelcount.dat"),"wb");
	levelcount^=1337;
	SDL_RWwrite(file,&levelcount,sizeof(int),1);
	SDL_RWwrite(file,&volume,sizeof(int),1);
	SDL_RWwrite(file,&volumefactor,sizeof(int),1);
	SDL_RWwrite(file,&gameMode,sizeof(int),1);
	levelcount^=1337;
	SDL_RWclose(file);
}

void loadlevelcount()
{
	char buffer[256];
	SDL_RWops *file=SDL_RWFromFile(get_path(buffer,"levelcount.dat"),"rb");
	if (file==NULL) //Compatibility
			return;
	SDL_RWread(file,&levelcount,sizeof(int),1);
	SDL_RWread(file,&volume,sizeof(int),1);
	SDL_RWread(file,&volumefactor,sizeof(int),1);
	SDL_RWread(file,&gameMode,sizeof(int),1);
	levelcount^=1337;
	SDL_RWclose(file);
}

void savetime(char* level,float t)
{
	if (level == NULL || level[0] == 0)
		return;
	char buffer[256];
	spCreateDirectoryChain(get_path(buffer,""));
	SDL_RWops *file=SDL_RWFromFile(get_path(buffer,level),"wb");
	SDL_RWwrite(file,&t,sizeof(float),1);
	SDL_RWclose(file);
}

float loadtime(char* level)
{
	char buffer[256];
	SDL_RWops *file=SDL_RWFromFile(get_path(buffer,level),"rb");
	if (file==NULL)
		return 0.0f; //2 minutes
	float t;
	SDL_RWread(file,&t,sizeof(float),1);
	SDL_RWclose(file);
	return t;
}

float loadall(char* kind)
{
	int i,j;
	float all = 0.0f;
	for (i = 1; i < 6; i++)
		for (j = 0; j < 5; j++)
		{
			char buffer[256];
			char filename[256];
			sprintf(filename,"%i-%i.%s",i,j,kind);
			SDL_RWops *file=SDL_RWFromFile(get_path(buffer,filename),"rb");
			float t;
			if (file==NULL)
				t = 0.0f;
			else
			{
				SDL_RWread(file,&t,sizeof(float),1);
				SDL_RWclose(file);
			}
			all+=t;
		}
	return all;
}

float loadall_i(int kind,plevel level)
{
	if (level->loaded_i_once == 0)
	{
		spNetC4AScorePointer score[2] =  {NULL,NULL};
		spNetC4ATaskPointer task[2] = {NULL,NULL};
		task[0] = spNetC4AGetScoreParallel(&(score[0]),NULL,"snowman_easy",15000);
		task[1] = spNetC4AGetScoreParallel(&(score[1]),NULL,"snowman_hard",15000);
		while (spNetC4AGetStatusParallel(task[0]) == SP_C4A_PROGRESS ||
				spNetC4AGetStatusParallel(task[1]) == SP_C4A_PROGRESS)
		{
			spClearTarget(65535);
			spFontDrawMiddle(screen->w>>1,screen->h-font->maxheight*2>>1,-1,"Loading scores of Snowman",font_green);
			char buffer[256];
			int t = spNetC4AGetTimeOutParallel(task[0]);
			if (spNetC4AGetStatusParallel(task[0]) == SP_C4A_PROGRESS)
				sprintf(buffer,"Easy: (%is)...",t/1000);
			else
				sprintf(buffer,"Easy: Done...");
			spFontDrawMiddle(screen->w>>1,screen->h-font->maxheight*0>>1,-1,buffer,font_green);
			t = spNetC4AGetTimeOutParallel(task[1]);
			if (spNetC4AGetStatusParallel(task[1]) == SP_C4A_PROGRESS)
				sprintf(buffer,"Hard: (%is)...",t/1000);
			else
				sprintf(buffer,"Hard: Done...");
			spFontDrawMiddle(screen->w>>1,screen->h+font->maxheight*2>>1,-1,buffer,font_green);
			spFlip();
			spSleep(200000);
		}
		int i;
		for (i = 0; i < 2; i++)
		{
			if (spNetC4AGetTaskResultParallel(task[i]) == 0)
			{
				if (score[i])
					level->cached_i[i] = (float)score[i]->score/10.0f;
				else
					level->cached_i[i] = -1.0f;
			}
			else
				level->cached_i[i] = -2.0f;
			spNetC4ADeleteTask(task[i]);
		}
		level->loaded_i_once = 1;
	}
	return level->cached_i[kind];
}
