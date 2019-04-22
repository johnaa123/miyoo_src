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
  
#include <string.h>
#include <sparrow3d.h>
#include "splashscreen.h"
#define BUTTON_SHOT 0
#define BUTTON_TRIPPLE 1
#define BUTTON_JUMP 2
#define BUTTON_BROOM 3
#define BUTTON_BALL 4
#define BUTTON_HELP 5
//#define SCALE_UP
spFontPointer font = NULL;
spFontPointer font_red = NULL;
spFontPointer font_green = NULL;
#ifdef SCALE_UP
SDL_Surface* real_screen;
#endif
SDL_Surface* screen = NULL;
#define CLOUD_COUNT 16
SDL_Surface* cloud[CLOUD_COUNT];

#define TIME_BETWEEN_TWO_JUMPS 20

int gameMode = 0; //easy

Uint16* mapPixel;
int mapLine;

spNetC4AProfilePointer profile;

char music_name[512] = "";
int acceleration = 0;

void addBorder( spFontPointer font, Uint16 fontColor,Uint16 backgroundColor)
{
	int i;
	for (i = 0; i < (spGetSizeFactor() >> SP_ACCURACY)-1; i++)
		spFontAddBorder( font, fontColor );
	for (i = 0; i < (spGetSizeFactor()*3/2 >> SP_ACCURACY); i++)
		spFontAddBorder( font, backgroundColor );
	printf("%i %i\n",spGetSizeFactor(),spGetSizeFactor() >> SP_ACCURACY);
}

//#define FONT "./data/Pompiere-Regular.ttf"
#define FONT "./data/Signika-Regular.ttf"
#define FONT_SIZE 11
#define FONT_MUL 0.8f

void initSnow();

void resize( Uint16 w, Uint16 h )
{
	#ifdef SCALE_UP
	if (screen)
		spDeleteSurface(screen);
	screen = spCreateSurface(real_screen->w/2,real_screen->h/2);
	#endif
	spSelectRenderTarget(screen);
	//Setup of the new/resized window
	spSetPerspective( 50.0, ( float )screen->w / ( float )screen->h, 1.0, 100 );

	int scale = 0;
	#ifdef SCALE_UP
	scale++;
	#endif
	//Font Loading
	if ( font )
		spFontDelete( font );
	font = spFontLoad( FONT, FONT_SIZE * spGetSizeFactor() >> SP_ACCURACY+scale );
	spFontAdd( font, SP_FONT_GROUP_ASCII, 65535 ); //whole ASCII
	addBorder( font, 65535, spGetRGB(128,128,128) );
	spFontSetButtonStrategy(SP_FONT_INTELLIGENT);
	spFontAddButton( font, 'R', SP_BUTTON_START_NAME, spGetRGB(32,32,32), 65535 ); //Return == START
	spFontAddButton( font, 'B', SP_BUTTON_SELECT_NAME, spGetRGB(32,32,32), 65535 ); //Backspace == SELECT
	spFontAddButton( font, 'q', SP_BUTTON_L_NAME, spGetRGB(32,32,32), 65535 ); // q == L
	spFontAddButton( font, 'e', SP_BUTTON_R_NAME, spGetRGB(32,32,32), 65535 ); // e == R
	spFontAddButton( font, 'a', SP_BUTTON_LEFT_NAME, spGetRGB(32,32,32), 65535 ); //a == left button
	spFontAddButton( font, 'd', SP_BUTTON_RIGHT_NAME, spGetRGB(32,32,32), 65535 ); // d == right button
	spFontAddButton( font, 'w', SP_BUTTON_UP_NAME, spGetRGB(32,32,32), 65535 ); // w == up button
	spFontAddButton( font, 's', SP_BUTTON_DOWN_NAME, spGetRGB(32,32,32), 65535 ); // s == down button
	spFontMulWidth( font, spFloatToFixed(FONT_MUL));
	


	if ( font_red )
		spFontDelete( font_red );
	font_red = spFontLoad( FONT, FONT_SIZE * spGetSizeFactor() >> SP_ACCURACY+scale );
	spFontAdd( font_red, SP_FONT_GROUP_ASCII, spGetRGB(255,128,128) ); //whole ASCII
	addBorder( font_red, spGetRGB(255,128,128), spGetRGB(128,64,64) );
	spFontMulWidth( font_red, spFloatToFixed(FONT_MUL));
	
	if ( font_green )
		spFontDelete( font_green );
	font_green = spFontLoad( FONT, FONT_SIZE * spGetSizeFactor() >> SP_ACCURACY+scale );
	spFontAdd( font_green, SP_FONT_GROUP_ASCII, spGetRGB(128,255,128) ); //whole ASCII
	addBorder( font_green, spGetRGB(128,255,128), spGetRGB(64,128,64) );
	spFontAddButton( font_green, 'R', SP_BUTTON_START_NAME, spGetRGB(32,64,32), spGetRGB(192,255,192) ); //Return == START
	spFontMulWidth( font_green, spFloatToFixed(FONT_MUL));

	//Creating Clouds
	int i;
	for (i = 0; i < CLOUD_COUNT; i++)
	{
		if (cloud[i])
			spDeleteSurface(cloud[i]);
		cloud[i] = spCreateSurface((96+rand()%96)*spGetSizeFactor() >> SP_ACCURACY,(48+rand()%48)*spGetSizeFactor() >> SP_ACCURACY);
		spSelectRenderTarget(cloud[i]);
		spClearTarget(SP_ALPHA_COLOR);
		int step = 16*spGetSizeFactor() >> SP_ACCURACY;
		int x;
		spSetZSet(0);
		spSetZTest(0);
		for (x = step+2; x < cloud[i]->w-step-2; x+=rand()%step)
		{
			int y_from = (x-cloud[i]->w/2)*(x-cloud[i]->w/2)*(cloud[i]->h/2)/((cloud[i]->w/2)*(cloud[i]->w/2));
			int y_to = cloud[i]->h - y_from;
			int y;
			for (y = y_from+step+2; y <= y_to-step-2; y+=rand()%step)
			{
				int c = rand()%16;
				spEllipse(x,y,-1,step,step,spGetFastRGB(190+c,190+c,190+c));
			}
		}
		spAddBorder(cloud[i],spGetFastRGB(150,150,150),SP_ALPHA_COLOR);
		spAddBorder(cloud[i],spGetFastRGB(100,100,100),SP_ALPHA_COLOR);
	}
	spSelectRenderTarget(screen);
	initSnow();
}

#define TRIPLE_SHOOT_TIME 100
int triple_shoot_pos = 0;
int triple_shoot_time = 0;

int volume;
int volumefactor;

#include "level.h"
#include "particle.h"

#define PARTICLES 16
#define ENEMY_COUNT 8

int show_snow = 1;

int jump_min_time;
SDL_Surface* sphere;
SDL_Surface* sphere_left;
SDL_Surface* sphere_right;
SDL_Surface* flake;
SDL_Surface* door_open;
SDL_Surface* door_closed;
SDL_Surface* door_boss_open;
SDL_Surface* door_boss_closed;
SDL_Surface* enemySur[ENEMY_COUNT];
spModelPointer broom;

Sint32 w=0;
Sint32 x,y;
Sint32 camerax,cameray;
int ballcount;
Sint32 ballsize[3];
Sint32 speedup;
Sint32 facedir;
int cloudcount;
Sint32 cloudx[16];
Sint32 cloudy[16];
Sint32 cloudz[16];
Sint32 clouds[16];
int gotchasmall;
int gotchabig;
int valuesmall;
int valuebig;

int fade;
int fade2;
int fader,fadeg,fadeb;
int damaged;

int bx;
int bxl;
int bxr;
int by;
int byb;
int sum;
int byt;
int bym;
int biggest;
int in_hit;

int enemyKilled;

plevel level = NULL;

spSound* shot_chunk;
spSound* jump_chunk;
spSound* ballshot_chunk;
spSound* positive_chunk;
spSound* negative_chunk;
spSound* hu_chunk;
char broom_exist;

Sint32 angle;

int levelcount;

char pausemode;
char helpmode;
char exitmode;

int getBiggest()
{
	if (ballsize[0]>=ballsize[2] && ballsize[0]>=ballsize[1])
		return 0;
	if (ballsize[1]>=ballsize[2] && ballsize[1]>=ballsize[0])
		return 1;
  return 2;
}

#include "snow.h"
#include "enemy.h"
#include "drawlevel.h"
#include "drawcharacter.h"
#include "bullet.h"
#include "ballbullet.h"

int pause_sel = 0;

void init_game(plevel level,char complete)
{
	acceleration = 0;
	//loading the score
	if (level->scoreName[0] != 0)
		level->topScore = loadtime(level->scoreName);
	level->score = 120.0f;
	jump_min_time = 0;
	angle=0;
	resetallparticle();
	in_hit = 0;
	broom_exist=0;
	ballBulletExists=0;
	enemyKilled=0;
	damaged=0;
	deleteAllBullets();
	gotchasmall=0;
	gotchabig=0;
	x=level->startx<<(SP_ACCURACY+1);
	camerax=x;
	y=level->starty<<(SP_ACCURACY+1);
	cameray=y;
	y+=1<<SP_ACCURACY;
	speedup=0;
	if (complete)
	{
		ballcount=1;
		ballsize[0]=0;//13<<(SP_ACCURACY-4);
		ballsize[1]=0;//9<<(SP_ACCURACY-4);
		ballsize[2]=7<<(SP_ACCURACY-4);
	}
	cloudcount=level->width*level->height/50+1;
	if (cloudcount>16)
		cloudcount=16;
	int i;
	for (i=0;i<cloudcount;i++)
	{
		cloudx[i]=(rand()%(2*level->width+40)-20)<<SP_ACCURACY;
		cloudy[i]=(rand()%(2*level->height+40)-20)<<SP_ACCURACY;
		cloudz[i]=-(rand()%20+10)<<SP_ACCURACY;
		clouds[i]=rand()%CLOUD_COUNT;
	}
	facedir=1;
}

void draw_game(void)
{
	Sint32* modellViewMatrix=spGetMatrix();

	spClearTarget(level->backgroundcolor);
	spResetZBuffer();
	spIdentity();

	modellViewMatrix[14]=-25<<SP_ACCURACY;
	SDL_LockSurface(level->mini_map);
	mapPixel = level->mini_map->pixels;
	int i;
	mapLine = level->mini_map->pitch/level->mini_map->format->BytesPerPixel;
	for (i = 0; i < mapLine*level->mini_map->h; i++)
		mapPixel[i] = SP_ALPHA_COLOR;

	//#ifdef PANDORA
	//	Sint32 dx=25<<SP_ACCURACY;
	//	Sint32 dy=15<<SP_ACCURACY;
	//#else
		Sint32 dy=20<<SP_ACCURACY;
		Sint32 dx=dy*screen->w/screen->h;
	//#endif
	spSetZSet(0);
	spSetZTest(0);
	drawclouds(camerax,cameray-(4<<SP_ACCURACY),dx,dy);
	spSetZSet(1);
	spSetAlphaTest(1);
	drawSnow(camerax,cameray);
	spSetZSet(1);
	spSetZTest(1);
	spSetAlphaTest(0);
	drawlevel(level,camerax,cameray-(4<<SP_ACCURACY),dx,dy);
	spSetZSet(0);
	spSetAlphaTest(1);
	//updating mini map
	int mx = (spFixedToInt(x)+1)/2;
	int my = (spFixedToInt(y))/2;
	if (mx >= 0 && mx < level->mini_map->w && my >= 0 && my < level->mini_map->h)
		mapPixel[mx+my*mapLine] = spGetRGB(255,127,127);
	if (ballsize[0]>0)
	{
		my--;
		if (mx >= 0 && mx < level->mini_map->w && my >= 0 && my < level->mini_map->h)
			mapPixel[mx+my*mapLine] = spGetRGB(255,127,127);		
	}
	drawcharacter(x-camerax,cameray-y-(4<<SP_ACCURACY),0,facedir);
	spSetZSet(1);
	spSetZTest(1);
	drawenemies(camerax,cameray-(4<<SP_ACCURACY),dx,dy);
	spSetZSet(0);
	spSetZTest(0);
	drawBullet(camerax,cameray-(4<<SP_ACCURACY),dx,dy);
	drawBallBullet(camerax,cameray-(4<<SP_ACCURACY));
	drawparticle(camerax,cameray-(4<<SP_ACCURACY),0,dx,dy);
	SDL_UnlockSurface(level->mini_map);
	if (level->no_map)
		spFontDrawRight(screen->w,0,0,"No map",font_red);
	else
		spRotozoomSurface(screen->w-spFixedToInt(level->mini_map->w*spGetSizeFactor()),spFixedToInt(level->mini_map->h*spGetSizeFactor()),0,level->mini_map,spGetSizeFactor()*2,spGetSizeFactor()*2,0);
	
	if (level->music[0] && (level->score > 115.0f))
	{
		char buffer[256];
		sprintf(buffer,"Title: %s",level->music);
		spSetBlending(spFloatToFixed(level->score-115.0f)/5);
		spFontDraw(1,screen->h-font->maxheight*2,0,buffer,font);
		spSetBlending(SP_ONE);
	}
	
	char buffer[64];
	sprintf(buffer,"Killed %i/%i (Objective: %i)",enemyKilled,level->enemycount,level->havetokill);
	if (enemyKilled<level->havetokill)
		spFontDraw(1,1,0,buffer,font_red);
	else
		spFontDraw(1,1,0,buffer,font_green);


	if (level->scoreName[0] != 0)
	{
		sprintf(buffer,"(Best: %.1f)",level->topScore);
		spFontDrawRight(screen->w-1,screen->h-font->maxheight,0,buffer,font);
		sprintf(buffer,"Time: %.1f",level->score);
		if (level->score != 0.0f)
			spFontDrawRight(screen->w-1,screen->h-font->maxheight*2,0,buffer,font);
		else
			spFontDrawRight(screen->w-1,screen->h-font->maxheight*2,0,buffer,font_red);
	}
	sprintf(buffer,"Small Belly: %i/18	     Big Belly: %i/26",ballsize[1]>>(SP_ACCURACY-5),ballsize[0]>>(SP_ACCURACY-5));
	if (ballsize[1]<=0)
		spFontDraw(1,screen->h-font_red->maxheight,0,buffer,font_red);
	else
		spFontDraw(1,screen->h-font_green->maxheight,0,buffer,font_green);
	if (gotchasmall)
	{
		int part_text_length = spFontWidth("Small Belly:",font_green);
		if (valuesmall>0)
		{
			sprintf(buffer,"+%i",valuesmall);
			spFontDraw(1+part_text_length,screen->h-font->maxheight*2,0,buffer,font_green);
		}
		else
		if (valuesmall<0)
		{
			sprintf(buffer,"%i",valuesmall);
			spFontDraw(1+part_text_length,screen->h-font->maxheight*2,0,buffer,font_red);
		}
	}
	if (gotchabig)
	{
		int part_text_length = spFontWidth("Small Belly: 18/18	     Big Belly:",font_green);
		if (valuebig>0)
		{
			sprintf(buffer,"+%i",valuebig);
			spFontDraw(1+part_text_length,screen->h-font->maxheight*2,0,buffer,font_green);
		}
		else
		if (valuebig<0)
		{
			sprintf(buffer,"%i",valuebig);
			spFontDraw(1+part_text_length,screen->h-font->maxheight*2,0,buffer,font_red);
		}
	}
	if (fade)
	{
		if (fade>512)
			spAddColorToTarget(65535,((1024-fade)>>1)<<8);
		else
			spAddColorToTarget(65535,(fade >>1)<<8);
	}
	if (fade2)
	{
		if (fade2>512)
			spInterpolateTargetToColor(0,((1024-fade2)>>1)<<8);
		else
			spInterpolateTargetToColor(0,(fade2 >>1)<<8);
	}
	if (helpmode)
	{
		spInterpolateTargetToColor(0,SP_ONE/2);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*8,-1,"Controls:",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*7,-1,"Move with the D-Pad",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*6,-1,"{shot}: Shot (costs 2 snow) {triple}: 3 shoots",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*5,-1,"{jump}: Jump (costs 1 snow)",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*4,-1,"{broom}: Broom Bash - 2x damage with a broom!",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*3,-1,"{ball}: Ball Attack (costs your big belly)",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*2,-1,"{help}: (Un)show mini help",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*1,-1,"[R]: Pause and options",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*0,-1,"[B]: Exit",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*1,-1,"Kill as many enemies as sayed in the upper left",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*2,-1,"corner of the screen to be able to enter the next",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*3,-1,"level. But economize your snow, it is your health,",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*4,-1,"size and shoot energy at the same time!",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*5,-1,"If you pass a level in less than 2 minutes, the",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*6,-1,"rest time is added to your score, which you can",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*7,-1,"submit to the C4A online highscore in the main menu!",font);
	}
	if (pausemode)
	{
		spInterpolateTargetToColor(0,SP_ONE*3/4);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*16/2,-1,"Press [R] to unpause",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*13/2,-1,"Press [B] to quit",font);

		if (pause_sel < 4)
			spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*(9-2*pause_sel)/2,-1,">>                                                    <<",font);
		else
			spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*(8-2*pause_sel)/2,-1,">>                                     <<",font);

		if (gameMode)
			spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*9/2,-1,"Mode: Hard (needs level restart)",font);
		else
			spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*9/2,-1,"Mode: Easy (needs level restart)",font);
		sprintf(buffer,"Total volume: %i %%",volume*100/2048);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*7/2,-1,buffer,font);
		sprintf(buffer,"Music volume: %i %% of total volume",volumefactor*100/2048);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*5/2,-1,buffer,font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*3/2,-1,"Return to sublevel",font);
		
		int continue_change = spMapContinueChange();
		spFontDrawRight(screen->w*2/3,(screen->h>>1)+font->maxheight*0/2,-1,"Shooting button: ",font);
		if (continue_change == 0 && pause_sel == 4)
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*0/2,-1,"???",font);
		else
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*0/2,-1,spMapButtonByID(BUTTON_SHOT),font);
		spFontDrawRight(screen->w*2/3,(screen->h>>1)+font->maxheight*2/2,-1,"Tripple Shot button: ",font);
		if (continue_change == 0 && pause_sel == 5)
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*2/2,-1,"???",font);
		else
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*2/2,-1,spMapButtonByID(BUTTON_TRIPPLE),font);
		spFontDrawRight(screen->w*2/3,(screen->h>>1)+font->maxheight*4/2,-1,"Jump button: ",font);
		if (continue_change == 0 && pause_sel == 6)
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*4/2,-1,"???",font);
		else
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*4/2,-1,spMapButtonByID(BUTTON_JUMP),font);
		spFontDrawRight(screen->w*2/3,(screen->h>>1)+font->maxheight*6/2,-1,"Broom button: ",font);
		if (continue_change == 0 && pause_sel == 7)
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*6/2,-1,"???",font);
		else
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*6/2,-1,spMapButtonByID(BUTTON_BROOM),font);
		spFontDrawRight(screen->w*2/3,(screen->h>>1)+font->maxheight*8/2,-1,"Ball Attack button: ",font);
		if (continue_change == 0 && pause_sel == 8)
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*8/2,-1,"???",font);
		else
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*8/2,-1,spMapButtonByID(BUTTON_BALL),font);
		spFontDrawRight(screen->w*2/3,(screen->h>>1)+font->maxheight*10/2,-1,"Help button: ",font);
		if (continue_change == 0 && pause_sel == 9)
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*10/2,-1,"???",font);
		else
			spFontDraw(     screen->w*2/3,(screen->h>>1)+font->maxheight*10/2,-1,spMapButtonByID(BUTTON_HELP),font);
		if (continue_change == 1)
			spMapSave("snowman","controls.cfg");
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*14/2,-1,"Use the "SP_PAD_NAME" and [a] to select and change settings",font);
	}
	if (exitmode)
	{
		spInterpolateTargetToColor(0,SP_ONE*3/4);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)-font->maxheight*2,-1,"Do you really want to quit?",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)                  ,-1,"Press [R] to quit",font);
		spFontDrawMiddle(screen->w>>1,(screen->h>>1)+font->maxheight*2,-1,"Press [B] to cancel",font);
	}
	#ifdef SCALE_UP
	spScale2XSmooth(screen,real_screen);
	#endif
	spFlip();
}

int calc_game(Uint32 steps)
{
	PspInput engineInput = spGetInput();

	if (exitmode)
	{
		if (engineInput->button[SP_BUTTON_START])
		{
			engineInput->button[SP_BUTTON_START] = 0;
			return 1;
		}
		if (engineInput->button[SP_BUTTON_SELECT])
		{
			engineInput->button[SP_BUTTON_SELECT]=0;
			exitmode=1-exitmode;
			jump_min_time = 0;
		}
		return 0;
	}
	if (engineInput->button[SP_BUTTON_START])
	{
		engineInput->button[SP_BUTTON_START]=0;
		pausemode=1-pausemode;
		pause_sel = 0;
		jump_min_time = 0;
	}
	if (pausemode)
	{
		switch (spMapContinueChange())
		{
			case 0:
				return 0;
			case 1:
				spMapSave("snowman","controls.cfg");
				break;
		}

		if (engineInput->button[SP_BUTTON_SELECT])
		{
			engineInput->button[SP_BUTTON_SELECT]=0;
			exitmode=1-exitmode;
			jump_min_time = 0;
		}
		if (engineInput->axis[1] < 0 && pause_sel > 0)
		{
			engineInput->axis[1] = 0;
			pause_sel--;
		}
		if (engineInput->axis[1] > 0 && pause_sel < 9)
		{
			engineInput->axis[1] = 0;
			pause_sel++;
		}
		switch (pause_sel)
		{
			case 0:
				if (engineInput->axis[0] != 0 || engineInput->button[SP_BUTTON_LEFT])
				{
					engineInput->axis[0] = 0;
					engineInput->button[SP_BUTTON_LEFT] = 0;
					gameMode = 1-gameMode;
					savelevelcount();
				}
				break;
			case 1:
				if (engineInput->axis[0] < 0)
				{
					volume-=steps;
					if (volume<0)
						volume=0;
					spSoundSetVolume(volume>>4);
					spSoundSetMusicVolume(((volumefactor*volume)/(128<<4))>>5);
					savelevelcount();
				}
				if (engineInput->axis[0] > 0)
				{
					volume+=steps;
					if (volume>(128<<4))
						volume=128<<4;
					spSoundSetVolume(volume>>4);
					spSoundSetMusicVolume(((volumefactor*volume)/(128<<4))>>5);
					savelevelcount();
				}
				break;
			case 2:
				if (engineInput->axis[0] < 0)
				{
					volumefactor-=steps;
					if (volumefactor<0)
						volumefactor=0;
					spSoundSetMusicVolume(((volumefactor*volume)/(128<<4))>>5);
					savelevelcount();
				}
				if (engineInput->axis[0] > 0)
				{
					volumefactor+=steps;
					if (volumefactor>(128<<4))
						volumefactor=128<<4;
					spSoundSetMusicVolume(((volumefactor*volume)/(128<<4))>>5);
					savelevelcount();
				}
				break;
			case 3:
				if (engineInput->axis[0] != 0 || engineInput->button[SP_BUTTON_LEFT])
				{
					engineInput->button[SP_BUTTON_LEFT] = 0;
					engineInput->axis[0] = 0;
					fade2=1024;
					pausemode=0;
				}
				break;
			case 4:
				if (engineInput->axis[0] != 0 || engineInput->button[SP_BUTTON_LEFT])
				{
					engineInput->button[SP_BUTTON_LEFT] = 0;
					engineInput->axis[0] = 0;
					spMapStartChangeByID(BUTTON_SHOT);
				}
				break;
			case 5:
				if (engineInput->axis[0] != 0 || engineInput->button[SP_BUTTON_LEFT])
				{
					engineInput->button[SP_BUTTON_LEFT] = 0;
					engineInput->axis[0] = 0;
					spMapStartChangeByID(BUTTON_TRIPPLE);
				}
				break;
			case 6:
				if (engineInput->axis[0] != 0 || engineInput->button[SP_BUTTON_LEFT])
				{
					engineInput->button[SP_BUTTON_LEFT] = 0;
					engineInput->axis[0] = 0;
					spMapStartChangeByID(BUTTON_JUMP);
				}
				break;
			case 7:
				if (engineInput->axis[0] != 0 || engineInput->button[SP_BUTTON_LEFT])
				{
					engineInput->button[SP_BUTTON_LEFT] = 0;
					engineInput->axis[0] = 0;
					 spMapStartChangeByID(BUTTON_BROOM);
				}
				break;
			case 8:
				if (engineInput->axis[0] != 0 || engineInput->button[SP_BUTTON_LEFT])
				{
					engineInput->button[SP_BUTTON_LEFT] = 0;
					engineInput->axis[0] = 0;
					spMapStartChangeByID(BUTTON_BALL);
				}
				break;
			case 9:
				if (engineInput->axis[0] != 0 || engineInput->button[SP_BUTTON_LEFT])
				{
					engineInput->button[SP_BUTTON_LEFT] = 0;
					engineInput->axis[0] = 0;
					spMapStartChangeByID(BUTTON_HELP);
				}
				break;
				
		}
		return 0;
	}
	if (engineInput->button[SP_BUTTON_SELECT]) //Reload
	{
		engineInput->button[SP_BUTTON_SELECT] = 0;
		fade2=1024;
		sprintf(level->failback,"%s",level->name);
	}	
	if (spMapGetByID(BUTTON_HELP))
	{
		spMapSetByID(BUTTON_HELP,0);
		helpmode=1-helpmode;
	}
	if (fade)
	{
		int i;
		for (i=0;i<steps && fade>0;i++)
		{
			fade--;
			if (fade==511)
			{
				if (level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->functionmask & 64)
					return 1;
				printf("Sprich Freund und tritt ein!\n");
				char buffer[256];
				if ((level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->functionmask & 128) && profile==NULL)
					sprintf(buffer,"%s.error",level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->function);
				else
					sprintf(buffer,"%s",level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->function);
				char reset=(level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->functionmask & 32) == 32;
				freeLevel(level);
				level=loadlevel(buffer);
				init_game(level,reset);
				ballsize[0] = 0;
				if (gameMode != 0)
				{
					ballcount = 1;
					ballsize[1] = 0;
				}
			}
			int new_volume;
			if (level->music_change && fade < 512)
			{
				if (fade > 256)
					new_volume = (((volumefactor*volume)/(128<<4))>>5)*(fade-256)/256;
				else
				if (fade == 256)
				{
					spSoundStopMusic(0);
					spSoundSetMusic(music_name);
					spSoundPlayMusic(0, -1);
					new_volume = 0;
				}				
				else
					new_volume = (((volumefactor*volume)/(128<<4))>>5)*(256-fade)/256;
				spSoundSetMusicVolume(new_volume);
			}
		}
		return 0;
	}
	if (fade2)
	{
		int i;
		for (i=0;i<steps && fade2>0;i++)
		{
			fade2--;
			if (fade2==511)
			{
				printf("Sprich Freund und tritt ein!\n");
				char buffer[256];
				sprintf(buffer,"%s",level->failback);
				freeLevel(level);
				level=loadlevel(buffer);
				init_game(level,1);
			}
			int new_volume;
			if (level->music_change && fade2 < 512)
			{
				if (fade2 > 256)
					new_volume = (((volumefactor*volume)/(128<<4))>>5)*(fade2-256)/256;
				else
				if (fade2 == 256)
				{
					spSoundStopMusic(0);
					spSoundSetMusic(music_name);
					spSoundPlayMusic(0, -1);
					new_volume = 0;
				}				
				else
					new_volume = (((volumefactor*volume)/(128<<4))>>5)*(256-fade2)/256;
				spSoundSetMusicVolume(new_volume);
			}
		}
		return 0;
	}
	if (gotchasmall)
	{
		gotchasmall-=steps;
		if (gotchasmall<0)
			gotchasmall=0;
	}
	if (gotchabig)
	{
		gotchabig-=steps;
		if (gotchabig<0)
			gotchabig=0;
	}
	level->score-=(float)steps/1000.0f;
	if (level->score < 0.0f)
		level->score = 0.0f;

	w+=(steps*256)%(2*SP_PI);
	//Time based movement
	int step;
	for (step=0;step<steps;step++)
	{
		if (jump_min_time>0)
			jump_min_time--;
		//Camera
		Sint32 dx=(x-camerax)>>7;
		Sint32 dy=((y-cameray)*3)>>7;
		camerax+=dx;
		cameray+=dy;

		Sint32 ox=x;
		Sint32 oy=y;

		calcparticle();

		//Enemys
		moveenemies();
		bulletEnemy();

		//Bullets
		calcBullet();
		calcBallBullet();
		bulletEnemyInteraction();
		bulletPlayerInteraction();
		bulletEnvironmentInteraction();

		if (broom_exist && in_hit<=0 && spMapGetByID(BUTTON_BROOM))
			in_hit=864;
		if (in_hit==768)
			broomEnemyInteraction(facedir);
		if (in_hit>0)
			in_hit--;

		//Setting some values
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
		char tofat=0;
		//Snow left?
		if (bxl>=0 && bxl<level->width)
		{
			 if (byb>0 && byb<=level->height && level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]!= NULL)
			 {
				 if ((level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->functionmask & 1) == 1)
				 {
					 if (addonesnow())
					 {
						 level->layer[1][bxl+(byb-1)*level->width]=' ';
						 tofat=fattest(&x,ox);
					 }
				 }
				 else
				 if ((level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->functionmask & 16) == 16)
				 {
					 level->layer[1][bxl+(byb-1)*level->width]=' ';
					 broom_exist=1;
					 spSoundPlay(positive_chunk,-1,0,0,0);
				 }
			 }
			 if (byt>=0 && byt<level->height && level->symbollist[level->layer[1][bxl+(byt)*level->width]]!= NULL)
			 {
				 if ((level->symbollist[level->layer[1][bxl+(byt)*level->width]]->functionmask & 1) == 1)
				 {
					 if (addonesnow())
					 {
						 level->layer[1][bxl+(byt)*level->width]=' ';
						 tofat=fattest(&x,ox);
					 }
				 }
				 else
				 if ((level->symbollist[level->layer[1][bxl+(byt)*level->width]]->functionmask & 16) == 16)
				 {
					 level->layer[1][bxl+(byt)*level->width]=' ';
					 broom_exist=1;
					 spSoundPlay(positive_chunk,-1,0,0,0);
				 }
			 }
			 if (bym>=0 && bym<level->height && level->symbollist[level->layer[1][bxl+(bym)*level->width]]!= NULL)
			 {
				 if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 1) == 1)
				 {
					 if (addonesnow())
					 {
						 level->layer[1][bxl+(bym)*level->width]=' ';
						 tofat=fattest(&x,ox);
					 }
				 }
				 else
				 if ((level->symbollist[level->layer[1][bxl+(bym)*level->width]]->functionmask & 16) == 16)
				 {
					 level->layer[1][bxl+(bym)*level->width]=' ';
					 broom_exist=1;
					 spSoundPlay(positive_chunk,-1,0,0,0);
				 }
			 }
		}
		//Snow right?
		if (bxr>=0 && bxr<level->width)
		{
			 if (byb>0 && byb<=level->height && level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]			 != NULL)
			 {
				 if ((level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->functionmask & 1) == 1)
				 {
					 if (addonesnow())
					 {
						 level->layer[1][bxr+(byb-1)*level->width]=' ';
						 tofat=fattest(&x,ox);
					 }
				 }
				 else
				 if ((level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->functionmask & 16) == 16)
				 {
					 level->layer[1][bxr+(byb-1)*level->width]=' ';
					 broom_exist=1;
					 spSoundPlay(positive_chunk,-1,0,0,0);
				 }
			 }
			 if (byt>=0 && byt<level->height && level->symbollist[level->layer[1][bxr+(byt)*level->width]]			 != NULL)
			 {
				 if ((level->symbollist[level->layer[1][bxr+(byt)*level->width]]->functionmask & 1) == 1)
				 {
					 if (addonesnow())
					 {
						 level->layer[1][bxr+(byt)*level->width]=' ';
						 tofat=fattest(&x,ox);
					 }
				 }
				 else
				 if ((level->symbollist[level->layer[1][bxr+(byt)*level->width]]->functionmask & 16) == 16)
				 {
					 level->layer[1][bxr+(byt)*level->width]=' ';
					 broom_exist=1;
					 spSoundPlay(positive_chunk,-1,0,0,0);
				 }
			 }
			 if (bym>=0 && bym<level->height && level->symbollist[level->layer[1][bxr+(bym)*level->width]]			 != NULL)
			 {
				 if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 1) == 1)
				 {
					 if (addonesnow())
					 {
						 level->layer[1][bxr+(bym)*level->width]=' ';
						 tofat=fattest(&x,ox);
					 }
				 }
				 else
				 if ((level->symbollist[level->layer[1][bxr+(bym)*level->width]]->functionmask & 16) == 16)
				 {
					 level->layer[1][bxr+(bym)*level->width]=' ';
					 broom_exist=1;
					 spSoundPlay(positive_chunk,-1,0,0,0);
				 }
			 }
		}

		//	Player
		if (!tofat)
		{
			if (engineInput->axis[0]==-1)
			{
				acceleration+=2<<(SP_ACCURACY-14);
				if (acceleration > (2<<(SP_ACCURACY-7)))
					acceleration = 2<<(SP_ACCURACY-7);
				//x-=2<<(SP_ACCURACY-7);
				x-=acceleration;
				angle+=2<<(SP_ACCURACY-8);
				if (angle>=2*SP_PI)
					angle-=2*SP_PI;
				facedir=0;
			}
			else
			if (engineInput->axis[0]== 1)
			{
				acceleration+=2<<(SP_ACCURACY-14);
				if (acceleration > (2<<(SP_ACCURACY-7)))
					acceleration = 2<<(SP_ACCURACY-7);
				//x+=2<<(SP_ACCURACY-7);
				x+=acceleration;
				angle-=2<<(SP_ACCURACY-8);
				if (angle<0)
					angle+=2*SP_PI;
				facedir=1;
			}
			else
				acceleration = 0;
			//Hm, where am I?

			if (testX(x,ox))
				x=ox;
		}

		//new calculation after the X-Check
		biggest = getBiggest();
		bx =((x>>(SP_ACCURACY))+1)>>1;
		bxl=(((x-ballsize[biggest])>>(SP_ACCURACY))+1)>>1;
		bxr=(((x+ballsize[biggest])>>(SP_ACCURACY))+1)>>1;

		if (by>level->height) //Fuck, I am dead...
		{
			fade2=1024;
			spSoundPlay(negative_chunk,-1,0,0,0);
			return 0;
		}
		//printf("bx: %i by-1: %i byt: %i \"%c\"	 \"%c\"\n",bx,by-1,byt,level->layer[1][bx+byt*level->width],level->layer[1][bx+(by-1)*level->width]);
		//negative gravitation?
		if (speedup<0 &&
				(bxr<0 || bxl>=level->width || 1))
		{
			Sint32 sum=0;
			int i;
			for (i=3-ballcount;i<3;i++)
				sum+=ballsize[i]*2;
			byt =((((y-sum)>>(SP_ACCURACY))+1)>>1);
			if (!(bxr<0 || bxl>=level->width || byt<0 ||
					((level->symbollist[level->layer[1][bxl+byt*level->width]]			 == NULL ||
						level->symbollist[level->layer[1][bxl+byt*level->width]]->form <= 0) &&
					 (level->symbollist[level->layer[1][bxr+byt*level->width]]			 == NULL ||
						level->symbollist[level->layer[1][bxr+byt*level->width]]->form <= 0))))
				speedup=0;
			else
			{
				speedup+=1<<(SP_ACCURACY-13);
				y+=speedup;
				by=((y>>(SP_ACCURACY))+1)>>1;
			}
		}
		else //gravitation?
		if (bxr<0 || bxl>=level->width || by<0 || by>=level->height ||
			 ((bxl<0						 || (level->symbollist[level->layer[1][bxl+by*level->width]]			 == NULL ||
															 level->symbollist[level->layer[1][bxl+by*level->width]]->form <= 0))	&&
				(bxr>=level->width || (level->symbollist[level->layer[1][bxr+by*level->width]]			 == NULL ||
															 level->symbollist[level->layer[1][bxr+by*level->width]]->form <= 0))	))
		{
			speedup+=1<<(SP_ACCURACY-13);
			if (speedup > (3<<(SP_ACCURACY-5)))
				speedup = (3<<(SP_ACCURACY-5));
			y+=speedup;
			by=((y>>(SP_ACCURACY))+1)>>1;
		}
		else
		{
			speedup=0;
			y=(by-1)<<(SP_ACCURACY+1);
			y+=1<<SP_ACCURACY;
		}
		if (damaged>0)
			damaged--;
		playerEnemyInteraction();
	}
	//Jump
	if (spMapGetByID(BUTTON_JUMP) && jump_min_time <= 0)
	{
		jump_min_time = TIME_BETWEEN_TWO_JUMPS;
		int biggest=getBiggest();
		int bxl=(((x-ballsize[biggest])>>(SP_ACCURACY))+1)>>1;
		int bxr=(((x+ballsize[biggest])>>(SP_ACCURACY))+1)>>1;
		int by =((y>>(SP_ACCURACY))+1)>>1;
		if (by>=0 && by<level->height && !(bxr<0 || bxl>=level->width ||
				((level->symbollist[level->layer[1][bxl+by*level->width]]			 == NULL ||
					level->symbollist[level->layer[1][bxl+by*level->width]]->form <= 0) &&
				 (level->symbollist[level->layer[1][bxr+by*level->width]]			 == NULL ||
					level->symbollist[level->layer[1][bxr+by*level->width]]->form <= 0)))) //opposite of gravitaion conditions
		if (ballsize[1]>(0<<(SP_ACCURACY-5)))
		{
			spMapSetByID(BUTTON_JUMP,0);
			removesnow(1);
			newexplosion(PARTICLES,x,y,0,1024,spGetRGB(255,255,255));
			speedup=-23<<(SP_ACCURACY-9);
			spSoundPlay(jump_chunk,-1,0,0,0);
		}
	}

	//Shooting
	if (spMapGetByID(BUTTON_SHOT))
	{
		spMapSetByID(BUTTON_SHOT,0);
		int sum=0;
		int i;
		for (i=3-ballcount;i<3;i++)
			sum+=ballsize[i]*2;
		newBullet(x,y-(sum>>1),(facedir)?(1<<(SP_ACCURACY-5)):(-1<<(SP_ACCURACY-5)),0,1000,1,spGetRGB(255,255,255));
	}
	if (spMapGetByID(BUTTON_TRIPPLE) && triple_shoot_pos == 0)
	{
		spMapSetByID(BUTTON_TRIPPLE,0);
		triple_shoot_pos = 1;
		triple_shoot_time = TRIPLE_SHOOT_TIME;
		int sum=0;
		int i;
		for (i=3-ballcount;i<3;i++)
			sum+=ballsize[i]*2;
		newBullet(x,y-(sum>>1),(facedir)?(1<<(SP_ACCURACY-5)):(-1<<(SP_ACCURACY-5)),0,1000,1,spGetRGB(255,255,255));
	}
	if (triple_shoot_pos)
	{
		triple_shoot_time -= steps;
		while (triple_shoot_time < 0)
		{
			triple_shoot_time += TRIPLE_SHOOT_TIME;
			int sum=0;
			int i;
			for (i=3-ballcount;i<3;i++)
				sum+=ballsize[i]*2;
			newBullet(x,y-(sum>>1),(facedir)?(1<<(SP_ACCURACY-5)):(-1<<(SP_ACCURACY-5)),0,1000,1,spGetRGB(255,255,255));
			triple_shoot_pos++;
			if (triple_shoot_pos > 2)
			{
				triple_shoot_pos = 0;
				triple_shoot_time = 0;
				break;
			}
		}
	}
	if (spMapGetByID(BUTTON_BALL))
	{
		spMapSetByID(BUTTON_BALL,0);
		fireBallBullet();
	}

	//clouds
	int i;
	for (i=0;i<cloudcount;i++)
	{
		cloudx[i]-=steps*(1<<(SP_ACCURACY-9));
		if (cloudx[i]<(-50<<SP_ACCURACY))
		{
			cloudx[i]+=(2*level->width+100)<<SP_ACCURACY;
			cloudy[i]=(rand()%(2*level->height+40)-20)<<SP_ACCURACY;
			cloudz[i]=-(rand()%20+10)<<SP_ACCURACY;
		}
	}

	//Door
	if (engineInput->axis[1]==-1)
		if (bx>=0 && bx<level->width)
			if (byb>0 && byb<=level->height &&
			    level->symbollist[level->layer[1][bx+(byb-1)*level->width]]!= NULL &&
					(level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->functionmask & 2) == 2)
				if (enemyKilled>=level->havetokill && level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->needed_level<=levelcount)
				{
					if (level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->needed_level<0)
					{
						if (levelcount<-level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->needed_level)
							levelcount=-level->symbollist[level->layer[1][bx+(byb-1)*level->width]]->needed_level;
						savelevelcount();
					}
					if (level->score > level->topScore)
					{
						level->topScore = level->score;
						savetime(level->scoreName,level->topScore);
					}
					fade=1024;
					return 0;
				}

	sum=0;
	for (i=3-ballcount;i<3;i++)
		sum+=ballsize[i]*2;
	calcSnow(steps);
	return 0;
}

void init_snowman()
{
	levelcount=10;
	pausemode=0;
	volume=128<<4;
	volumefactor=128<<4;
	loadlevelcount();
	sphere=spLoadSurface("./data/sphere.png");
	sphere_left=spLoadSurface("./data/sphere_left.png");
	sphere_right=spLoadSurface("./data/sphere_right.png");
	flake=spLoadSurface("./data/snowflake.png");
	door_open=spLoadSurface("./data/door.png");
	door_closed=spLoadSurface("./data/door_closed.png");
	door_boss_open=spLoadSurface("./data/door_boss.png");
	door_boss_closed=spLoadSurface("./data/door_boss_closed.png");
	int i;
	for (i = 0; i < ENEMY_COUNT; i++)
	{
		char buffer[256];
		sprintf(buffer,"./data/enemy%i.png",i);
		enemySur[i]=spLoadSurface(buffer);
	}
	broom=spMeshLoadObj("./data/broom.obj",spLoadSurface("./data/broom.png"),spGetRGB(255,255,255));
	fade=0;
	fade2=0;
	spSoundInit();
	spSoundSetChannels(32);
	spSoundSetVolume(volume>>4);
	spSoundSetMusicVolume(((volumefactor*volume)/(128<<4))>>5);
	shot_chunk=spSoundLoad("./sounds/plop.wav");
	ballshot_chunk=spSoundLoad("./sounds/plop2.wav");
	jump_chunk=spSoundLoad("./sounds/shot.wav");
	positive_chunk=spSoundLoad("./sounds/positive.wav");
	hu_chunk=spSoundLoad("./sounds/hu.wav");
	negative_chunk=spSoundLoad("./sounds/negative.wav");
	spSetLight(1);
	spSetAmbientLightColor(1 << SP_ACCURACY-1,1 << SP_ACCURACY-1,1 << SP_ACCURACY-1);
	spSetLightColor(1,1 << SP_ACCURACY,1 << SP_ACCURACY,1 << SP_ACCURACY);
	spUsePrecalculatedNormals(1);
	//Mapping
	spMapPoolAdd(SP_BUTTON_LEFT ,"[a]");
	spMapPoolAdd(SP_BUTTON_RIGHT,"[d]");
	spMapPoolAdd(SP_BUTTON_UP   ,"[w]");
	spMapPoolAdd(SP_BUTTON_DOWN ,"[s]");
	spMapPoolAdd(SP_BUTTON_L    ,"[q]");
	spMapPoolAdd(SP_BUTTON_R    ,"[e]");
	spMapButtonAdd(BUTTON_SHOT,"shot","Shot",SP_BUTTON_LEFT);
	spMapButtonAdd(BUTTON_TRIPPLE,"tripple","Tripple Shot",SP_BUTTON_DOWN);
	spMapButtonAdd(BUTTON_JUMP,"jump","Jump",SP_BUTTON_UP);
	spMapButtonAdd(BUTTON_BROOM,"broom","Broom",SP_BUTTON_RIGHT);
	spMapButtonAdd(BUTTON_BALL,"ball","ball",SP_BUTTON_R);
	spMapButtonAdd(BUTTON_HELP,"help","Help",SP_BUTTON_L);
	spMapSetStrategy(SP_MAPPING_SWITCH);
	spMapLoad("snowman","controls.cfg");
	spMapSave("snowman","controls.cfg");
}

void quit_snowman()
{
	spSoundDelete(shot_chunk);
	spSoundDelete(ballshot_chunk);
	spSoundDelete(jump_chunk);
	spSoundDelete(negative_chunk);
	spSoundDelete(hu_chunk);
	spSoundStopMusic(0);
	printf("Deleted Sounds\n");
	spSoundQuit();
	printf("Quit Sound\n");
	spDeleteSurface(sphere);
	spDeleteSurface(sphere_left);
	spDeleteSurface(sphere_right);
	spDeleteSurface(flake);
	spDeleteSurface(door_closed);
	spDeleteSurface(door_open);
	spDeleteSurface(door_boss_closed);
	spDeleteSurface(door_boss_open);
	int i;
	for (i = 0; i < CLOUD_COUNT; i++)
		spDeleteSurface(cloud[i]);
	for (i = 0; i < ENEMY_COUNT; i++)
		spDeleteSurface(enemySur[i]);
	printf("Deleted Surfaces\n");
	spMeshDelete(broom);
	printf("Deleted Meshs\n");
}

int main(int argc, char **argv)
{
	int i;
	for (i = 0; i 	< CLOUD_COUNT; i++)
		cloud[i] = NULL;
	spSetDefaultWindowSize( 800, 480 );
	spInitCore();
	spInitNet();
	profile = spNetC4AGetProfile();
	//Setup
	#ifdef SCALE_UP
	real_screen = spCreateDefaultWindow();
	resize( real_screen->w, real_screen->h );
	#else
	screen = spCreateDefaultWindow();
	resize( screen->w, screen->h );
	#endif
	run_splashscreen(resize);
	init_snowman();
	fade2 = 511;
	if (argc < 2)
		level=loadlevel("./levels/menu.slvl");
	else
		level=loadlevel(argv[1]);
	init_game(level,1);
	spLoop(draw_game,calc_game,10,resize,NULL);
	freeLevel(level);
	#ifdef SCALE_UP
	spDeleteSurface(screen);
	#endif
	quit_snowman();
	spQuitNet();
	spQuitCore();
	return 0;
}
