/*
 *  Copyright (C) 2010 Florent Bedoiseau (electronique.fb@free.fr)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef TOMBSTONE_H
#define TOMBSTONE_H
#include "Gfx.h"

#include <list>

using namespace std;

class Game;

/* Numero des tiles */
#define FLOOR      0
#define CACTUS_1   1
#define CACTUS_2   2
#define PLAYER_1   3
#define PLAYER_2   4
#define PLAYER_3   5
#define PLAYER_4   6
#define MONSTER_1  7
#define MONSTER_2  8
#define TIR_1      9
#define TIR_2      10
#define SPLASH     11
#define BUISSON_1  12
#define BUISSON_2  13
#define BLOCK_1    14
#define BLOCK_2    15
#define SCORE      16
#define BOARD      17
#define GAMEOVER   18
#define NBMAX_TILES  19

#define OUT 888 // Not a tile ...

// @50Hz 1 tick=20ms
#if 0
#define PLAYER_TICK 5 /* 100ms */
#define PLAYER_TICK_CHANGE 2
#define SHOOT_TICK 10 /* 200ms */
#define MOVE_BUISSONS_TICK 6 /* 120ms */
#define CREATE_BUISSONS_TICK 200 /* 4s */
#define CREATE_MONSTERS_TICK 350 /* 7s */
#define CHANGE_CAGE_TICK 250 /* 5s */
#define MOVE_MONSTERS_TICK 5 /* 100ms */
#else
// @60Hz (PAL) 1 tick=16ms
#define PLAYER_TICK 6 /* 100ms */
#define PLAYER_TICK_CHANGE 2
#define SHOOT_TICK 12 /* 200ms */
#define MOVE_BUISSONS_TICK 7 /* 120ms */
#define CREATE_BUISSONS_TICK 240 /* 4s */
#define CREATE_MONSTERS_TICK 420 /* 7s */
#define CHANGE_CAGE_TICK 300 /* 5s */
#define MOVE_MONSTERS_TICK 6 /* 100ms */
#endif

#define NO_FIRE          0
#define FIRE_IN_PROGRESS 1
#define FIRE_SPLASH      2

class Sprite {
public:
  Sprite ();
  Sprite (int atile_num, int ax, int ay, Game *g);
  Sprite& operator=(const Sprite & aSprite);
  Sprite (const Sprite & aSprite);
  ~Sprite ();
  
  void incXY (int ix, int iy) { x+=ix; y+=iy; }
  void setXY (int xx, int yy) { x=xx; y=yy; }
  int getX (void) const { return x; }
  int getY (void) const { return y; }
  void getXY (int &xx, int &yy) { xx=x; yy=y; }
  
  int getTileNumber (void) const { return tileNumber; }
  void setTileNumber (int atile) { tileNumber = atile; }
  
  void Move (void);
  void Show (void);
  void Hide (void);
  
  int isShown (void) const { return shown; }
  
 private:
  int shown;
  
  int old_x;
  int old_y;
  int x;
  int y;
  int tileNumber;
  int oldtileNumber;
  Game *game;
};

class FPoint {
 public:
  FPoint () { x = 0; y = 0; }
  FPoint (int ax, int ay) { x=ax; y=ay; }
  FPoint& operator=(const FPoint &aFPoint);
  FPoint (const FPoint &aFPoint); 
  int getX () const { return x;}
  int getY () const { return y;}
  void getXY (int &xx, int &yy) const { xx=x; yy=y; }

 private:
  int x;
  int y;
};

typedef list<Sprite> ListOfSprites;
typedef list<FPoint> ListOfPoints;

typedef list<Sprite>::iterator ListOfSpritesIterator;
typedef list<FPoint>::iterator ListOfPointsIterator;

class Game {
public:
  Game ();
  ~Game ();
  int IsItTheEnd () const { return _end_flag;}
  void ShowMessage (UInt8 msgn);
  void ClearMessage ();

  int GetBoard (int x, int y) const {return _board [y][x];}
  void SetBoard (int x, int y, int aTile) { _board [y][x]=aTile; }
  void DrawPixmap (int x, int y, int aTile);
  int TimerEvent (void); 

 private:
  // Les tiles
  //const u8* tiles [];
  gfxbitmap * tiles [NBMAX_TILES];

  /* PLAYER */
  Sprite *player_sprite;
  int player_direction;
  int player_blocked;

  /* FIRE */
  Sprite *fire_sprite;
  int fire_direction;
  int fire_in_progress;

  /* Splash */
  Sprite *splash_sprite;
  int time_splash;
  int shoot_monster;
  
  /* MONSTER */
  int mx,my; /* Position possible de creation d'un monstre */
  int MonsterToBeCreated;
  
  /* CACTUS */
  int cx,cy; /* position du cactus blanc */
  
  ListOfSprites mListOfCactus; /* CACTUS */
  ListOfSprites mListOfBuissons; /* BUISSONS */
  ListOfSprites mListOfMonsters; /* MONSTERS */
  ListOfSprites mListOfCages; /* CAGE */
  
  int time_create_monster;
  int time_move_monster;
  int time_create_buissons;
  int time_move_buissons;
  int time_player;
  
  int _board [24][28];
  int _score;
  int _day;
  int _life;
  int _end_flag;

  // Changing
  void InitGame (void);
  void ChangeDay (void);
  void ChangeLife (void);
  void EndGame (void);
  
  // Creation
  void CreateTiles (void);
  void CreateAllCactus (void);
  void CreateAllBuissons (void);
  void CreateAllCages (int tileCage);
  void CreatePlayer (void);
  void CreateFire (int aTile, int x, int y);
  void CreateSplash (int x, int y);
  
  // Time
  void TimeCreateMonsters (void);
  void TimeCreateBuissons (void);
  void TimeMovePlayer (void);
  void TimeMoveBuissons (void);
  void TimeMoveMonsters (void);
  void TimeFirePlayer (void);
  void TimeFire(void);
  void TimeShoot (void);
  
  // Checking
  int CheckBounces (int x, int y) const;
  int CheckObstacles (int x, int y) const;
  
  // Showing
  void ShowBoard (void) const;
  void ShowScore (void) const;
  void ShowLowerScreen (void) const;

  // Managment
  int ComputeNewMonsterPosition (void);
  ListOfPoints ComputeThreeCactusList (int x, int y);

  void MoveMonsters (void);
  int MoveMonsterX (Sprite & aSprite, int dx);
  int MoveMonsterY (Sprite & aSprite, int dy);
  void ReplaceCactusByMonster (int x, int y);
  int RemoveXYObject (ListOfSprites & aListOfSprite, int x, int y);
  int CheckIfBlocked () const;
  void FindEmptySpace (int &x, int &y) const;

  // Destruction
  void DeleteTiles (void);
  void FreeAllBuissons (void);
  void FreeAllCactus (void);
  void FreeAllMonsters (void);
  void FreeAllCages (void);
  void FreeFire (void);
  void FreeSplash (void);
  void FreePlayer (void);
};

void ShakeDirection (int tab_dir[]);
int sign (int val);

#endif
