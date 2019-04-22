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

#define SPLASH_WAIT ((SP_PI/96)*4)
#define MESH_SIZE 21
Sint32 saved_points[MESH_SIZE*2][3];
typedef SDL_Surface *PSDL_Surface;
int splash_counter;
Sint32 splash_rotation;
PSDL_Surface sparrow;
PSDL_Surface logo;
spModelPointer sparrow_mesh;
spModelPointer logo_mesh;

void draw_splash(void)
{
  spClearTarget(0);
  spResetZBuffer();
	spIdentity();
	spTranslate(0,0,-3<<SP_ACCURACY);
	spSetZTest(0);
	spSetZSet(0);
	spMesh3D( sparrow_mesh, 1 );
	spMesh3D( logo_mesh, 1 );
  spFlip();  	
}

int calc_splash(Uint32 steps)
{
  splash_rotation+=steps*32;
  splash_counter -= steps;
  if (splash_counter <=0)
    return 1;
  PspInput engineInput = spGetInput();
  if ((engineInput->button[SP_BUTTON_START] ||
      engineInput->button[SP_BUTTON_A] || engineInput->button[SP_BUTTON_B] ||
      engineInput->button[SP_BUTTON_X] || engineInput->button[SP_BUTTON_Y]))
  {
    engineInput->button[SP_BUTTON_START] = 0;
    engineInput->button[SP_BUTTON_A] = 0;
    engineInput->button[SP_BUTTON_B] = 0;
    engineInput->button[SP_BUTTON_X] = 0;
    engineInput->button[SP_BUTTON_Y] = 0;
    return 1;
  }
  //Update Rotation
  int i;
  for (i = 0; i < MESH_SIZE*2; i ++)
  {
		Sint32 rotation = -(splash_rotation*3 + i*SP_PI/MESH_SIZE/8);
		sparrow_mesh->texPoint[i].x = saved_points[i][0];
		sparrow_mesh->texPoint[i].y = spMul(spCos(rotation),saved_points[i][1]) + spMul(spSin(rotation),saved_points[i][2]);
		sparrow_mesh->texPoint[i].z = spMul(spSin(rotation),saved_points[i][1]) + spMul(spCos(rotation),saved_points[i][2]);
		rotation += SP_PI;
		logo_mesh->texPoint[i].x = saved_points[i][0];
		logo_mesh->texPoint[i].y = spMul(spCos(rotation),saved_points[i][1]) + spMul(spSin(rotation),saved_points[i][2]);
		logo_mesh->texPoint[i].z = spMul(spSin(rotation),saved_points[i][1]) + spMul(spCos(rotation),saved_points[i][2]);
	}
  
  return 0;
}

void run_splashscreen(void (*resize)(Uint16 w,Uint16 h))
{
	//spSetPerspectiveTextureMapping(1);
  splash_rotation = 0;
  sparrow = spLoadSurface("./data/sparrow.png");
  logo = spLoadSurface("./data/snowman.png");
  sparrow_mesh = spMeshLoadObj( "./data/splash.obj", sparrow, 65535 );
  logo_mesh = spMeshLoadObj( "./data/splash.obj", logo, 65535 );
  int i;
  for (i = 0; i < MESH_SIZE*2; i ++)
  {
		saved_points[i][0] = sparrow_mesh->texPoint[i].x;
		saved_points[i][1] = sparrow_mesh->texPoint[i].y;
		saved_points[i][2] = sparrow_mesh->texPoint[i].z;
	}
  splash_counter = SPLASH_WAIT;
  spLoop(draw_splash,calc_splash,10,resize,NULL);
  spMeshDelete( sparrow_mesh );
  spMeshDelete( logo_mesh );
  SDL_FreeSurface(sparrow);
  SDL_FreeSurface(logo);
  //spSetPerspectiveTextureMapping(0);
}

