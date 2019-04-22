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
  
tbullet ballbullet;
Sint32 ballBulletSize;
char ballBulletExists;

void fireBallBullet()
{
	if (ballBulletExists)
		return;
	if (ballcount<3)
		return;
	ballBulletSize=ballsize[0];
	removesnow(ballsize[0]>>SP_ACCURACY-5);
	ballBulletExists=1;
	ballbullet.color=spGetRGB(255,255,255);
	ballbullet.dx=(facedir)?(5<<(SP_ACCURACY-9)):(-5<<(SP_ACCURACY-9));
	ballbullet.dy=0;
	ballbullet.lifetime=2000;
	ballbullet.x=x;
	ballbullet.y=y-ballBulletSize;
	spSoundPlay(ballshot_chunk,-1,0,0,0);
}

void drawBallBullet(Sint32 x,Sint32 y)
{
	if (!ballBulletExists)
		return;
	//spEllipse(bullet->x-x,y-bullet->y,0,3<<(SP_ACCURACY-3),3<<(SP_ACCURACY-3),bullet->color);
	Sint32 matrix[16];
	memcpy( matrix, spGetMatrix(), 16 * sizeof( Sint32 ) ); //glPush()
	spTranslate(ballbullet.x-x,y-ballbullet.y,0);
	spBindTexture(sphere);
	spQuadTex3D(-ballBulletSize, ballBulletSize,0, 0, 0,
							-ballBulletSize,-ballBulletSize,0, 0,63,
							 ballBulletSize,-ballBulletSize,0,63,63,
							 ballBulletSize, ballBulletSize,0,63, 0,ballbullet.color);
	memcpy( spGetMatrix(), matrix, 16 * sizeof( Sint32 ) ); //glPop()
}

void calcBallBullet()
{
	if (!ballBulletExists)
		return;
	ballbullet.x+=ballbullet.dx;
	ballbullet.y+=ballbullet.dy;
	ballbullet.lifetime--;
	char collision=0;
	if (ballbullet.lifetime<0)
		collision=1;
	penemy ebefore=NULL;
	penemy enemy=level->firstenemy;
	while (enemy!=NULL)
	{
		//Distance
		int hit = 0;
		if ( enemy->symbol->measures[2] == enemy->symbol->measures[3])
		{
			//Squared distance:
			if (spMax(ballbullet.x-enemy->x,ballbullet.y-enemy->y) < (1 << SP_ACCURACY+3))
			{
				Sint32 d2 = spSquare(ballbullet.x-enemy->x)+spSquare(ballbullet.y-enemy->y);
				if (d2 > 0 && d2 <= spSquare(enemy->symbol->measures[2]+ballBulletSize))
					hit = 1;
			}
		}
		else
		{
			if ( ballbullet.x+ballBulletSize >= enemy->x-enemy->symbol->measures[2] &&
				 ballbullet.x-ballBulletSize <= enemy->x+enemy->symbol->measures[2] &&
				 ballbullet.y+ballBulletSize >= enemy->y-enemy->symbol->measures[3] &&
				 ballbullet.y-ballBulletSize <= enemy->y+enemy->symbol->measures[3] ) //Hit
					hit = 1;
		}
		if (hit)
		{
			newexplosion(PARTICLES,ballbullet.x,ballbullet.y,0,1024,spGetRGB(255,255,255));
			ballBulletSize-=1<<(SP_ACCURACY-5);
			if (ballBulletSize<=0)
				ballBulletExists=0;
			enemy->health--;
			newexplosion(PARTICLES,enemy->x,enemy->y,0,1024,enemy->symbol->color);
			if (enemy->health<=0)
			{
				enemy->split_counter--;
				if (enemy->split_counter > 0)
				{
					int i;
					for (i = 0; i < 2; i++)
					{
						penemy newenemy = (penemy)malloc(sizeof(tenemy));
						newenemy->next = NULL;
						newenemy->health = enemy->maxhealth-1;
						newenemy->maxhealth=newenemy->health;
						newenemy->weapon = enemy->weapon;
						newenemy->shotfq = enemy->shotfq+200;
						newenemy->lastshot = 0;
						newenemy->split_counter = enemy->split_counter;
						penemy lastenemy = level->firstenemy;
						while (lastenemy->next)
							lastenemy = lastenemy->next;
						lastenemy->next=newenemy;
						newenemy->symbol=level->symbollist[enemy->symbol->symbol-1];
						newenemy->dx = (i%2 ? -1:1)<<(SP_ACCURACY-7);
						newenemy-> x = enemy->x;
						newenemy-> y = enemy->y + SP_ONE/2;
					}				
				}
				enemyKilled++;
				if (ebefore==NULL)
					level->firstenemy=enemy->next;
				else
					ebefore->next=enemy->next;
				free(enemy);
				enemy=ebefore;
			}
			break;
		}
		ebefore=enemy;
		if (enemy!=NULL)
			enemy=enemy->next;
	}
	
	int bx =(( ballbullet.x								 >>(SP_ACCURACY))+1)>>1;
	int bxl=(((ballbullet.x-ballBulletSize)>>(SP_ACCURACY))+1)>>1;
	int bxr=(((ballbullet.x+ballBulletSize)>>(SP_ACCURACY))+1)>>1;
	int byb=(((ballbullet.y+ballBulletSize)>>(SP_ACCURACY))+1)>>1;
	int byt=(((ballbullet.y-ballBulletSize)>>(SP_ACCURACY))+1)>>1;
	int bym=(( ballbullet.y								 >>(SP_ACCURACY))+1)>>1;
	//Solid Block on the left?
	if (bxl>=0 && bxl<level->width)
	{
		if (byb>0 && level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]			 != NULL &&
								 level->symbollist[level->layer[1][bxl+(byb-1)*level->width]]->form > 0)
			collision=1;
		if (byt>=0 && level->symbollist[level->layer[1][bxl+(byt)*level->width]]			 != NULL &&
									level->symbollist[level->layer[1][bxl+(byt)*level->width]]->form > 0)
			collision=1;
		if (bym>=0 && level->symbollist[level->layer[1][bxl+(bym)*level->width]]			 != NULL &&
									level->symbollist[level->layer[1][bxl+(bym)*level->width]]->form > 0)
			collision=1;
	}
	//Solid Block on the right?
	if (bxr>=0 && bxr<level->width)
	{
		if (byb>0 && level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]			 != NULL &&
								 level->symbollist[level->layer[1][bxr+(byb-1)*level->width]]->form > 0)
			collision=2;
		if (byt>=0 && level->symbollist[level->layer[1][bxr+(byt)*level->width]]			 != NULL &&
									level->symbollist[level->layer[1][bxr+(byt)*level->width]]->form > 0)
			collision=2;
		if (bym>=0 && level->symbollist[level->layer[1][bxr+(bym)*level->width]]			 != NULL &&
									level->symbollist[level->layer[1][bxr+(bym)*level->width]]->form > 0)
			collision=2;
	}
	if (collision)
	{	
		Sint32 c=ballBulletSize>>(SP_ACCURACY-4);
		int i;
		for (i=0;i<c;i++)
			newBullet(ballbullet.x,ballbullet.y,spCos((2*SP_PI*i)/c)>>5,spSin((2*SP_PI*i)/c)>>5,1000,2,ballbullet.color);
		ballBulletExists=0;
	}
}
