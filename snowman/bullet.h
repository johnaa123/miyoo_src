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
  
typedef struct sbullet *pbullet;
typedef struct sbullet {
	Sint32 x,y;
	Sint32 dx,dy;
	int lifetime;
	char good;
	Uint16 color;
	pbullet next;
} tbullet;

pbullet firstBullet = NULL;

void deleteAllBullets()
{
	while (firstBullet!=NULL)
	{
		pbullet temp=firstBullet->next;
		free(firstBullet);
		firstBullet=temp;
	}
}

void newBullet(Sint32 x,Sint32 y,Sint32 dx,Sint32 dy, int lifetime,char good,Uint16 color)
{
	if (good==1)
	{
		if (ballcount==1 || (ballcount==2 && ballsize[1]<=(1<<(SP_ACCURACY-5))))
			return;
		removesnow(2);
	}
	if (good==1)
		spSoundPlay(shot_chunk,-1,0,0,0);
	pbullet bullet=(pbullet)malloc(sizeof(tbullet));
	bullet->x=x;
	bullet->y=y;
	bullet->dx=dx;
	bullet->dy=dy;
	bullet->lifetime=lifetime;
	bullet->good=good;
	bullet->color=color;
	bullet->next=firstBullet;
	firstBullet=bullet;	
}

void calcBullet()
{
	pbullet before=NULL;
	pbullet bullet=firstBullet;
	while (bullet!=NULL)
	{
		bullet->x+=bullet->dx;
		bullet->y+=bullet->dy;
		bullet->lifetime--;
		if (bullet->lifetime<0)
		{
			if (before==NULL)
				firstBullet=bullet->next;
			else
				before->next=bullet->next;
			newexplosion(PARTICLES,bullet->x,bullet->y,0,1024,bullet->color);
			free(bullet);
			bullet=before;
		}
		before=bullet;
		if (bullet!=NULL)
			bullet=bullet->next;
	}
	
}

void drawBullet(Sint32 x,Sint32 y,Sint32 dx,Sint32 dy)
{
	Sint32 minx=-dx;
	Sint32 maxx=+dx;
	Sint32 miny=-dy;
	Sint32 maxy=+dy;	
	pbullet bullet=firstBullet;
	while (bullet!=NULL)
	{
		if (bullet->x-x < minx ||
				bullet->x-x > maxx ||
				y-bullet->y < miny ||
				y-bullet->y > maxy)
		{
		 bullet=bullet->next;
		 continue; 
		}		
		spEllipse3D(bullet->x-x,y-bullet->y,0,3<<(SP_ACCURACY-3),3<<(SP_ACCURACY-3),bullet->color);
		bullet=bullet->next;
	}	
}

void bulletEnemyInteraction()
{
	pbullet bbefore=NULL;
	pbullet bullet=firstBullet;
	while (bullet!=NULL)
	{
		if (!bullet->good)
		{
			bbefore=bullet;
			bullet=bullet->next;
			continue;
		}
		penemy ebefore=NULL;
		penemy enemy=level->firstenemy;
		while (enemy!=NULL)
		{
			//Distance
			int hit = 0;
			if ( enemy->symbol->measures[2] == enemy->symbol->measures[3])
			{
				//Squared distance:
				if (spMax(bullet->x-enemy->x,bullet->y-enemy->y) < (1 << SP_ACCURACY+3))
				{
					Sint32 d2 = spSquare(bullet->x-enemy->x)+spSquare(bullet->y-enemy->y);
					if (d2 > 0 && d2 <= spSquare(enemy->symbol->measures[2]))
						hit = 1;
				}
			}
			else
			{
				if ( bullet->x >= enemy->x-enemy->symbol->measures[2] &&
					 bullet->x <= enemy->x+enemy->symbol->measures[2] &&
					 bullet->y >= enemy->y-enemy->symbol->measures[3] &&
					 bullet->y <= enemy->y+enemy->symbol->measures[3] ) //Hit
						hit = 1;
			}
			if (hit)
			{
				if (bbefore==NULL)
					firstBullet=bullet->next;
				else
					bbefore->next=bullet->next;
				newexplosion(PARTICLES,bullet->x,bullet->y,0,1024,bullet->color);
				free(bullet);
				bullet=bbefore;
				newexplosion(PARTICLES,enemy->x,enemy->y,0,1024,enemy->symbol->color);
				enemy->health--;
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
		bbefore=bullet;
		if (bullet!=NULL)
			bullet=bullet->next;
	}
}

void bulletEnvironmentInteraction()
{
	pbullet bbefore=NULL;
	pbullet bullet=firstBullet;
	while (bullet!=NULL)
	{
		Sint32 bx =((bullet->x>>(SP_ACCURACY))+1)>>1;
		Sint32 by =((bullet->y>>(SP_ACCURACY))+1)>>1;
		if (bx>=0 && bx<level->width && by>=0 && by<level->height &&
				level->symbollist[level->layer[1][bx+by*level->width]]!=NULL &&
				level->symbollist[level->layer[1][bx+by*level->width]]->form>0)
		{
			if (bbefore==NULL)
				firstBullet=bullet->next;
			else
				bbefore->next=bullet->next;
			newexplosion(PARTICLES,bullet->x,bullet->y,0,1024,bullet->color);
			free(bullet);
			bullet=bbefore;
		}
		bbefore=bullet;
		if (bullet!=NULL)
			bullet=bullet->next;
	}
}

void bulletEnemy()
{
	int sum=0;
	int i;
	for (i=3-ballcount;i<3;i++)
		sum+=ballsize[i]*2;
	Sint32 dx,dy,dl;
	penemy enemy = level->firstenemy;
	while (enemy!=NULL)
	{
		if (enemy->lastshot>0)
			enemy->lastshot--;
		else
		if (enemy->x >= x-(40<<SP_ACCURACY) && enemy->x <= x+(40<<SP_ACCURACY) &&
				enemy->y >= y-(40<<SP_ACCURACY) && enemy->y <= y+(40<<SP_ACCURACY))
			switch (enemy->weapon)
			{
				case 1: //just like the snowmans weapon
					newBullet(enemy->x,enemy->y,(enemy->dx>=0)?(1<<(SP_ACCURACY-5)):(-1<<(SP_ACCURACY-5)),0,1000,0,enemy->symbol->color);
					enemy->lastshot=enemy->shotfq;
				break;
				case 2: //With targeting
					dx=x-enemy->x;
					dy=y-(sum>>1)-enemy->y;
					dl=spSqrt((dx>>SP_HALF_ACCURACY)*(dx>>SP_HALF_ACCURACY)+(dy>>SP_HALF_ACCURACY)*(dy>>SP_HALF_ACCURACY));
					if (dl!=0)
					{
						dx=((dx<<SP_HALF_ACCURACY)/dl)<<SP_HALF_ACCURACY;
						dy=((dy<<SP_HALF_ACCURACY)/dl)<<SP_HALF_ACCURACY;
						dx=dx>>5;
						dy=dy>>5;
					}
					newBullet(enemy->x,enemy->y,dx,dy,1000,0,enemy->symbol->color);
					enemy->lastshot=enemy->shotfq;
				break;
				
			}
		enemy=enemy->next;
	}
}

void bulletPlayerInteraction()
{
	if (damaged)
		return;	
	int sum=0;
	int i;
	for (i=3-ballcount;i<3;i++)
		sum+=ballsize[i]*2;
	biggest=getBiggest();
	
	pbullet bbefore=NULL;
	pbullet bullet=firstBullet;
	while (bullet!=NULL)
	{
		if (bullet->good)
		{
			bbefore=bullet;
			bullet=bullet->next;
			continue;
		}
		//Distance
		if ( bullet->x >= x-ballsize[biggest] &&
				 bullet->x <= x+ballsize[biggest] &&
				 bullet->y >= y-sum &&
				 bullet->y <= y			) //Hit
		{
			if (bbefore==NULL)
				firstBullet=bullet->next;
			else
				bbefore->next=bullet->next;
			newexplosion(PARTICLES,bullet->x,bullet->y,0,1024,bullet->color);
			free(bullet);
			bullet=bbefore;
			
			damaged=SP_PI>>(SP_ACCURACY-8);
			if (removesnow(3))
			{
				fade2=1024;
				spSoundPlay(negative_chunk,-1,0,0,0);
				return;
			}
		}
		bbefore=bullet;
		if (bullet!=NULL)
			bullet=bullet->next;
	}
}
