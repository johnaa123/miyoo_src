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
  
void drawlevel(plevel level,Sint32 mx,Sint32 my,Sint32 dx,Sint32 dy)
{
	//updating mini map
	int l,x,y;
	for (x = 0; x < level->width; x++)
		for (y = 0; y < level->height; y++)
		{
			psymbol now=level->symbollist[level->layer[1][x+y*level->width]];
			if (now && !now->no_map)
			{
				if (now->functionmask & 1)
				{
					int c=abs(spSin(w)>>(SP_ACCURACY-7))+127;
					if (c>255)
						c=255;
					mapPixel[x+(y)*mapLine] = spGetRGB(c,c,c);
				}
				else
				if (now->meshmask & 12)
					mapPixel[x+y*mapLine] = spGetRGB(0,255,255);
				else
					mapPixel[x+y*mapLine] = level->symbollist[level->layer[1][x+y*level->width]]->color;
			}
		}
	Sint32 minx=-dx;
	Sint32 maxx=+dx;
	Sint32 miny=-dy;
	Sint32 maxy=+dy;
	int startx = minx+mx >> SP_ACCURACY+1;
	if (startx < 0)
		startx = 0;
	int endx = maxx+mx >> SP_ACCURACY+1;
	if (endx > level->width)
		endx = level->width;
	int starty = -maxy+my >> SP_ACCURACY+1;
	if (starty < 0)
		starty = 0;
	int endy = -miny+my >> SP_ACCURACY+1;
	if (endy > level->height)
		endy = level->height;
	for (x=startx;x<endx;x++)
	{
		for (y=starty;y<endy;y++)
		{
			for (l=0;l<3;l++)
			{
				psymbol now=level->symbollist[level->layer[l][x+y*level->width]];
				while (now!=NULL)
				{
					if (now->mesh!=NULL)
					{
						if ((now->functionmask & 2)!=2 || now->needed_level<=levelcount)
							spMesh3DwithPos(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<SP_ACCURACY,now->mesh,0);
					}
					else
					if (now->functionmask & 1)
					{
						if (show_snow)
						{
							spSetAlphaTest(1);
							Sint32 matrix[16];
							memcpy( matrix, spGetMatrix(), 16 * sizeof( Sint32 ) ); //glPush()
							spTranslate(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<(SP_ACCURACY+1));
							spRotateY(w);
							if (spGetFPS() < 20)
							{
								spQuad3D(-3 << SP_ACCURACY-2, 0 << SP_ACCURACY-2,0,
										  0 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0,
										  3 << SP_ACCURACY-2, 0 << SP_ACCURACY-2,0,
										  0 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0,65535);
								spQuad3D( 0 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0,
										  3 << SP_ACCURACY-2, 0 << SP_ACCURACY-2,0,
										  0 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0,
										 -3 << SP_ACCURACY-2, 0 << SP_ACCURACY-2,0,65535);
							}
							else
							{
								spBindTexture(flake);
								spQuadTex3D(-3 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0, 0, 0,
											-3 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0, 0,63,
											3 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0,63,63,
											3 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0,63, 0,65535);
								spQuadTex3D( 3 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0,63, 0,
											 3 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0,63,63,
											-3 << SP_ACCURACY-2,-3 << SP_ACCURACY-2,0, 0,63,
											-3 << SP_ACCURACY-2, 3 << SP_ACCURACY-2,0, 0, 0,65535);
							}
							memcpy( spGetMatrix(), matrix, 16 * sizeof( Sint32 ) ); //glPop()
						}
						else
						{
							int c=abs(spSin(w)>>(SP_ACCURACY-7))+127;
							if (c>255)
								c=255;
							spEllipse3D(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<(SP_ACCURACY+1),
														abs(spSin(w)*3/4),/*3<<(SP_ACCURACY-2),*/3<<(SP_ACCURACY-2),
														spGetRGB(c,c,c)/*level->symbollist[level->layer[l][x+y*level->width]]->color*/);
						}
					}
					else
					if (now->meshmask & 2)
					{
						Sint32 tx,ty,tz,w;
						spProjectPoint3D(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<(SP_ACCURACY+1),&tx,&ty,&tz,&w,1);
						spSetAlphaTest(1);
						spFontDrawMiddle(tx,ty-font->maxheight/2,tz,now->function,font);
						spSetAlphaTest(0);
					}
					else
					if (now->meshmask & 32) //score
					{
						Sint32 tx,ty,tz,w;
						spProjectPoint3D(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,(l-1)<<(SP_ACCURACY+1),&tx,&ty,&tz,&w,1);
						spSetAlphaTest(1);
						char buffer[256];
						if (strchr(now->function,'.')) //'.' in filename found
						{
							if (strchr(now->function,'e')) //'e' in "easy" found
								sprintf(buffer,"E:%.1f",now->score);
							else
								sprintf(buffer,"H:%.1f",now->score);
						}
						else
						if (now->score >= 0.0f)
							sprintf(buffer,"%.1f",now->score);
						else
						switch ((int)now->score)
						{
							case -2: sprintf(buffer,"no conn."); break;
							default: sprintf(buffer,"no score"); break;
						}
						spFontDrawMiddle(tx,ty-font->maxheight/2,tz,buffer,font);
						spSetAlphaTest(0);
					}
					else
					if (now->meshmask & 12)
					{
						if (now->meshmask & 4)
						{
							if (enemyKilled<level->havetokill || now->needed_level>levelcount)
								spBindTexture(door_closed);
							else
								spBindTexture(door_open);
						}
						else
						{
							if (enemyKilled<level->havetokill || now->needed_level>levelcount)
								spBindTexture(door_boss_closed);
							else
								spBindTexture(door_boss_open);
						}
						spSetAlphaTest(1);
						Sint32 matrix[16];
						memcpy( matrix, spGetMatrix(), 16 * sizeof( Sint32 ) ); //glPush()
						spTranslate(((2*x)<<SP_ACCURACY)-mx,((-2*y)<<SP_ACCURACY)+my,((l-1)*4-1<<SP_ACCURACY-1));
						spQuadTex3D(-2 << SP_ACCURACY-1, 6 << SP_ACCURACY-1,0, 0, 0,
									-2 << SP_ACCURACY-1,-2 << SP_ACCURACY-1,0, 0,95,
									 2 << SP_ACCURACY-1,-2 << SP_ACCURACY-1,0,63,95,
									 2 << SP_ACCURACY-1, 6 << SP_ACCURACY-1,0,63, 0,65535);
						memcpy( spGetMatrix(), matrix, 16 * sizeof( Sint32 ) ); //glPop()
						spSetAlphaTest(0);
					}		
					now=now->next;
				}
			}
		}
	}
}

void drawclouds(Sint32 mx,Sint32 my,Sint32 dx,Sint32 dy)
{
	int i;
	for (i=0;i<cloudcount;i++)
		spBlit3D(cloudx[i]-mx,-cloudy[i]+my,cloudz[i],cloud[clouds[i]]);
}
