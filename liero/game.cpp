#include "game.hpp"
#include "reader.hpp"
#include "viewport.hpp"
#include "worm.hpp"
#include "filesystem.hpp"
#include "gfx.hpp"
#include "sfx.hpp"
#include "weapsel.hpp"
#include "constants.hpp"
#include <cstdlib>
//#include "text.hpp" // TEMP

#include <iostream>

Game game;

int stoneTab[3][4] =
{
	{98, 60, 61, 62},
	{63, 75, 85, 86},
	{89, 90, 97, 96}
};

void Texts::loadFromEXE()
{
	const char *texts[] =
	{
		"[RANDOM]",
		"Random",
		"REGENERATE LEVEL",
		"RELOAD LEVEL",
		"LIERO v1.33 Copyright Mets\x84nEl\x84imet 1998,1999",
		"Liero v1.33 (c) Mets\x84nEl\x84imet 1998,1999",
#if defined(PLATFORM_GCW0)
		"R - Save Options",
		"L - Load Options",
#else
		"S - Save Options",
		"L - Load Options",
#endif
		"Current Options: No filename",
		"Current Options: ",
		"Kill'em All",
		"Game of Tag",
		"Capture the Flag",
		"Simple CtF",
		"LIVES",
		"TIME TO LOSE",
		"FLAGS TO WIN",
		"OFF",
		"ON",
		"Human",
		"CPU",
		"Menu",
		"Bonus",
		"Banned",
#if defined(PLATFORM_GCW0)
		"Button SELECT",
#else
		"Esc",
#endif
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"0",
		"+",
		"`",
#if defined(PLATFORM_GCW0)
		"Button R",
#else
		"Backspace",
#endif
#if defined(PLATFORM_GCW0)
		"Button L",
#else
		"Tab",
#endif
		"Q",
		"W",
		"E",
		"R",
		"T",
		"Y",
		"U",
		"I",
		"O",
		"P",
		"",
		"^",
#if defined(PLATFORM_GCW0)
		"Button START",
#else
		"Enter",
#endif
#if defined(PLATFORM_GCW0)
		"Button A",
#else
		"Left Crtl",
#endif
		"A",
		"S",
		"D",
		"F",
		"G",
		"H",
		"J",
		"K",
		"L",
		"�",
		"",
		"�",
#if defined(PLATFORM_GCW0)
		"Button X",
#else
		"Left Shift",
#endif
		"'",
		"Z",
		"X",
		"C",
		"V",
		"B",
		"N",
		"M",
		",",
		".",
		"-",
		"Right Shift",
		"* (Pad)",
#if defined(PLATFORM_GCW0)
		"Button B",
#else
		"Left Alt",
#endif
#if defined(PLATFORM_GCW0)
		"Button Y",
#else
		"Space",
#endif
		"Caps Lock",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"Num Lock",
		"Scroll Lock",
		"7 (Pad)",
		"8 (Pad)",
		"9 (Pad)",
		"- (Pad)",
		"4 (Pad)",
		"5 (Pad)",
		"6 (Pad)",
		"+ (Pad)",
		"1 (Pad)",
		"2 (Pad)",
		"3 (Pad)",
		"0 (Pad)",
		", (Pad)",
		"",
		"",
		"<",
		"F11",
		"F12",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Enter (Pad)",
		"Right Ctrl",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Print Screen",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"/ (Pad)",
		"",
		"Print Screen",
		"Right Alt",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Home",
		"Up",
		"Page Up",
		"",
		"Left",
		"",
		"Right",
		"",
		"End",
		"Down",
		"Page Down",
		"Insert",
		"Delete",
		"",
		"",
		"",
		"",
		"",
		"Select your weapons:",
		"Level: Random",
		"Level: \"",
		"\"",
		"Randomize",
		"DONE!",
		"Reloading...",
		"Press fire to go...",
		"Kills: ",
		"Lives: ",
		"Select level:",
		"Weapon",
		"Availability",
		"At least one weapon must\nbe available in the menu!",
//		// Unused?
//		"",
//		"",
//		""
	};

	random = texts[0];
	random2 = texts[1];
	regenLevel = texts[2];
	reloadLevel = texts[3];
	
	copyright1 = texts[4];
	copyright2 = texts[5];
	saveoptions = texts[6];
	loadoptions = texts[7];
	curOptNoFile = texts[8];
	curOpt = texts[9];
	
	for(int i = 0; i < 4; ++i)
	{
		gameModes[i] = texts[10+i];
	}
	
	gameModeSpec[0] = texts[14];
	gameModeSpec[1] = texts[15];
	gameModeSpec[2] = texts[16];
	
	onoff[0] = texts[17];
	onoff[1] = texts[18];
	
	controllers[0] = texts[19];
	controllers[1] = texts[20];
	
	for(int i = 0; i < 3; ++i)
	{
		weapStates[i] = texts[21+i];
	}
		
	for(int i = 1; i < 177; ++i) // First key starts at 1
	{
		keyNames[i] = texts[24+i-1];
	}
	
	selWeap = texts[200];
	levelRandom = texts[201];
	levelIs1 = texts[202];
	levelIs2 = texts[203];
	randomize = texts[204];
	done = texts[205];
	
	reloading = texts[206];
	pressFire = texts[207];
	
	kills = texts[208];
	lives = texts[209];
	
	selLevel = texts[210];
	
	weapon = texts[211];
	availability = texts[212];
	noWeaps = texts[213];
	
	copyrightBarFormat = 64;
}

Game::~Game()
{
	clearViewports();
	clearWorms();
}

void Game::initGame()
{
	clearWorms();
	clearViewports();
	
	bonuses.clear();
	wobjects.clear();
	sobjects.clear();
	bobjects.clear();
	nobjects.clear();
	
	Worm* worm1 = new Worm(&game.settings.wormSettings[0], 0, 19);
	Worm* worm2 = new Worm(&game.settings.wormSettings[1], 1, 20);
	
	addViewport(new Viewport(Rect(0, 0, 158, 158), worm1, 0, 504, 350));
	addViewport(new Viewport(Rect(160, 0, 158+160, 158), worm2, 218, 504, 350));
	
	addWorm(worm1);
	addWorm(worm2);
	
	// TODO: Move as much of this as possible into the Worm ctor
	for(std::size_t i = 0; i < worms.size(); ++i)
	{
		Worm& w = *worms[i];
		w.makeSightGreen = false;
		w.lives = game.settings.lives;
		w.ready = true;
		w.movable = true;
		
		if(game.rand(2) > 0)
		{
			w.aimingAngle = itof(32);
			w.direction = 0;
		}
		else
		{
			w.aimingAngle = itof(96);
			w.direction = 1;
		}

		w.health = w.settings->health;
		w.visible = false;
		w.killedTimer = 150;
		
		w.currentWeapon = 1; // This is later changed to 0, why is it here?

/* Done in WormWeapon ctor
		for(int i = 0; i < game.settings.selectableWeapons; ++i)
		{
			w.weapons[i].available = true;
			w.weapons[i].delayLeft = 0;
			w.weapons[i].ammo = 0;
			
		}*/
	}
	
	gotChanged = false;
	lastKilled = 0;
}

void Game::clearViewports()
{
	for(std::size_t i = 0; i < viewports.size(); ++i)
		delete viewports[i];
	viewports.clear();
}

void Game::addViewport(Viewport* vp)
{
	viewports.push_back(vp);
}



void Game::processViewports()
{
	for(std::size_t i = 0; i < viewports.size(); ++i)
	{
		viewports[i]->process();
	}
}

void Game::drawViewports()
{
	gfx.clear();
	for(std::size_t i = 0; i < viewports.size(); ++i)
	{
		viewports[i]->draw();
	}
}

void Game::clearWorms()
{
	for(std::size_t i = 0; i < worms.size(); ++i)
		delete worms[i];
	worms.clear();
}

void Game::resetWorms()
{
	for(std::size_t i = 0; i < worms.size(); ++i)
	{
		Worm& w = *worms[i];
		w.health = w.settings->health;
		w.lives = settings.lives; // Not in the original!
		w.kills = 0;
		w.visible = false;
		w.killedTimer = 150;
		
		w.currentWeapon = 1;
	}
}

void Game::addWorm(Worm* worm)
{
	worms.push_back(worm);
}

void Game::loadMaterials()
{
	FILE* exe = openFile(joinPath(lieroDataRoot, "materials.dat"));

	for(int i = 0; i < 256; ++i)
	{
		materials[i].flags = 0;
	}
	
	unsigned char bits[32];
	
	for(int i = 0; i < 5; ++i)
	{
		fread(bits, 1, 32, exe);
		
		for(int j = 0; j < 256; ++j)
		{
			int bit = ((bits[j >> 3] >> (j & 7)) & 1);
			materials[j].flags |= bit << i;
		}
	}
	
	fread(bits, 1, 32, exe);
	
	for(int j = 0; j < 256; ++j)
	{
		int bit = ((bits[j >> 3] >> (j & 7)) & 1);
		materials[j].flags |= bit << 5;
	}
}

struct Read32
{
	static inline int run(FILE* f)
	{
		return readSint32(f);
	}
};

struct Read16
{
	static inline int run(FILE* f)
	{
		return readSint16(f);
	}
};

struct Read8
{
	static inline int run(FILE* f)
	{
		return readUint8(f);
	}
};

struct ReadBool
{
	static inline bool run(FILE* f)
	{
		return readUint8(f) != 0;
	}
};

template<typename T>
struct Dec
{
	static inline int run(FILE* f)
	{
		return T::run(f) - 1;
	}
};

template<typename Reader, typename T, int N, typename U>
inline void readMembers(FILE* f, T(&arr)[N], U (T::*mem))
{
	for(int i = 0; i < N; ++i)
	{
		(arr[i].*mem) = Reader::run(f);
	}
}

void Game::loadWeapons()
{
	const char *weaponsText[] =
	{
		"SHOTGUN",
		"CHAINGUN",
		"RIFLE",
		"BAZOOKA",
		"DIRTBALL",
		"EXPLOSIVES",
		"MINE",
		"WINCHESTER",
		"DOOMSDAY",
		"FLAMER",
		"GRENADE",
		"LARPA",
		"BLASTER",
		"BOUNCY MINE",
		"MINIGUN",
		"CLUSTER BOMB",
		"SUPER SHOTGUN",
		"HANDGUN",
		"GREENBALL",
		"BIG NUKE",
		"CRACKLER",
		"ZIMM",
		"MINI NUKE",
		"FLOAT MINE",
		"FAN",
		"HELLRAIDER",
		"CANNON",
		"BOUNCY LARPA",
		"LASER",
		"SPIKEBALLS",
		"NAPALM",
		"UZI",
		"MISSILE",
		"CHIQUITA BOMB",
		"BOOBY TRAP",
		"GRASSHOPPER",
		"MINI ROCKETS",
		"DART",
		"RB RAMPAGE",
		"GAUSS GUN",
	};

	FILE* exe = openFile(joinPath(lieroDataRoot, "weapons.dat"));
	
	readMembers<Read8>(exe, weapons, &Weapon::detectDistance);
	readMembers<ReadBool>(exe, weapons, &Weapon::affectByWorm);
	readMembers<Read8>(exe, weapons, &Weapon::blowAway);
	
	for(int i = 0; i < 40; ++i)
	{
		weapOrder[i + 1] = readUint8(exe) - 1;
	}
	
	readMembers<Read16>(exe, weapons, &Weapon::gravity);
	readMembers<ReadBool>(exe, weapons, &Weapon::shadow);
	readMembers<ReadBool>(exe, weapons, &Weapon::laserSight);
	readMembers<Dec<Read8> >(exe, weapons, &Weapon::launchSound);
	readMembers<ReadBool>(exe, weapons, &Weapon::loopSound);
	readMembers<Dec<Read8> >(exe, weapons, &Weapon::exploSound);
	readMembers<Read16>(exe, weapons, &Weapon::speed);
	readMembers<Read16>(exe, weapons, &Weapon::addSpeed);
	readMembers<Read16>(exe, weapons, &Weapon::distribution);
	readMembers<Read8>(exe, weapons, &Weapon::parts);
	readMembers<Read8>(exe, weapons, &Weapon::recoil);
	readMembers<Read16>(exe, weapons, &Weapon::multSpeed);
	readMembers<Read16>(exe, weapons, &Weapon::delay);
	readMembers<Read16>(exe, weapons, &Weapon::loadingTime);
	readMembers<Read8>(exe, weapons, &Weapon::ammo);
	readMembers<Dec<Read8> >(exe, weapons, &Weapon::createOnExp);
	readMembers<Dec<Read8> >(exe, weapons, &Weapon::dirtEffect);
	readMembers<Read8>(exe, weapons, &Weapon::leaveShells);
	readMembers<Read8>(exe, weapons, &Weapon::leaveShellDelay);
	readMembers<ReadBool>(exe, weapons, &Weapon::playReloadSound);
	readMembers<ReadBool>(exe, weapons, &Weapon::wormExplode);
	readMembers<ReadBool>(exe, weapons, &Weapon::explGround);
	readMembers<ReadBool>(exe, weapons, &Weapon::wormCollide);
	readMembers<Read8>(exe, weapons, &Weapon::fireCone);
	readMembers<ReadBool>(exe, weapons, &Weapon::collideWithObjects);
	readMembers<ReadBool>(exe, weapons, &Weapon::affectByExplosions);
	readMembers<Read8>(exe, weapons, &Weapon::bounce);
	readMembers<Read16>(exe, weapons, &Weapon::timeToExplo);
	readMembers<Read16>(exe, weapons, &Weapon::timeToExploV);
	readMembers<Read8>(exe, weapons, &Weapon::hitDamage);
	readMembers<Read8>(exe, weapons, &Weapon::bloodOnHit);
	readMembers<Read16>(exe, weapons, &Weapon::startFrame);
	readMembers<Read8>(exe, weapons, &Weapon::numFrames);
	readMembers<ReadBool>(exe, weapons, &Weapon::loopAnim);
	readMembers<Read8>(exe, weapons, &Weapon::shotType);
	readMembers<Read8>(exe, weapons, &Weapon::colourBullets);
	readMembers<Read8>(exe, weapons, &Weapon::splinterAmount);
	readMembers<Read8>(exe, weapons, &Weapon::splinterColour);
	readMembers<Dec<Read8> >(exe, weapons, &Weapon::splinterType);
	readMembers<Read8>(exe, weapons, &Weapon::splinterScatter);
	readMembers<Dec<Read8> >(exe, weapons, &Weapon::objTrailType);
	readMembers<Read8>(exe, weapons, &Weapon::objTrailDelay);
	readMembers<Read8>(exe, weapons, &Weapon::partTrailType);
	readMembers<Dec<Read8> >(exe, weapons, &Weapon::partTrailObj);
	readMembers<Read8>(exe, weapons, &Weapon::partTrailDelay);

	for(int i = 0; i < 40; ++i)
	{
		weapons[i].name = weaponsText[i];
		weapons[i].id = i;
	}

	// Special objects
	readMembers<Dec<Read8> >(exe, sobjectTypes, &SObjectType::startSound);
	readMembers<Read8>(exe, sobjectTypes, &SObjectType::numSounds);
	readMembers<Read8>(exe, sobjectTypes, &SObjectType::animDelay);
	readMembers<Read8>(exe, sobjectTypes, &SObjectType::startFrame);
	readMembers<Read8>(exe, sobjectTypes, &SObjectType::numFrames);
	readMembers<Read8>(exe, sobjectTypes, &SObjectType::detectRange);
	readMembers<Read8>(exe, sobjectTypes, &SObjectType::damage);
	readMembers<Read32>(exe, sobjectTypes, &SObjectType::blowAway); // blowAway has 13 slots, not 14. The last value will overlap with shadow.
	readMembers<ReadBool>(exe, sobjectTypes, &SObjectType::shadow);
	readMembers<Read8>(exe, sobjectTypes, &SObjectType::shake);
	readMembers<Read8>(exe, sobjectTypes, &SObjectType::flash);
	readMembers<Dec<Read8> >(exe, sobjectTypes, &SObjectType::dirtEffect);
	
	for(int i = 0; i < 14; ++i) // TODO: Unhardcode
	{
		sobjectTypes[i].id = i;
	}

	readMembers<Read8>(exe, nobjectTypes, &NObjectType::detectDistance);
	readMembers<Read16>(exe, nobjectTypes, &NObjectType::gravity);
	readMembers<Read16>(exe, nobjectTypes, &NObjectType::speed);
	readMembers<Read16>(exe, nobjectTypes, &NObjectType::speedV);
	readMembers<Read16>(exe, nobjectTypes, &NObjectType::distribution);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::blowAway);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::bounce);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::hitDamage);
	readMembers<ReadBool>(exe, nobjectTypes, &NObjectType::wormExplode);
	readMembers<ReadBool>(exe, nobjectTypes, &NObjectType::explGround);
	readMembers<ReadBool>(exe, nobjectTypes, &NObjectType::wormDestroy);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::bloodOnHit);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::startFrame);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::numFrames);
	readMembers<ReadBool>(exe, nobjectTypes, &NObjectType::drawOnMap);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::colourBullets);
	readMembers<Dec<Read8> >(exe, nobjectTypes, &NObjectType::createOnExp);
	readMembers<ReadBool>(exe, nobjectTypes, &NObjectType::affectByExplosions);
	readMembers<Dec<Read8> >(exe, nobjectTypes, &NObjectType::dirtEffect);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::splinterAmount);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::splinterColour);
	readMembers<Dec<Read8> >(exe, nobjectTypes, &NObjectType::splinterType);
	readMembers<ReadBool>(exe, nobjectTypes, &NObjectType::bloodTrail);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::bloodTrailDelay);
	readMembers<Dec<Read8> >(exe, nobjectTypes, &NObjectType::leaveObj);
	readMembers<Read8>(exe, nobjectTypes, &NObjectType::leaveObjDelay);
	readMembers<Read16>(exe, nobjectTypes, &NObjectType::timeToExplo);
	readMembers<Read16>(exe, nobjectTypes, &NObjectType::timeToExploV);
	
	for(int i = 0; i < 24; ++i) // TODO: Unhardcode
	{
		nobjectTypes[i].id = i;
	}
}

void Game::loadTextures()
{
	FILE* exe = openFile(joinPath(lieroDataRoot, "textures.dat"));

	readMembers<ReadBool>(exe, textures, &Texture::nDrawBack);
	readMembers<Read8>(exe, textures, &Texture::mFrame);
	readMembers<Read8>(exe, textures, &Texture::sFrame);
	readMembers<Read8>(exe, textures, &Texture::rFrame);
}

void Game::loadOthers()
{
	FILE* exe = openFile(joinPath(lieroDataRoot, "others.dat"));

	for(int i = 0; i < 2; ++i)
	for(int j = 0; j < 2; ++j)
		bonusRandTimer[j][i] = readUint16(exe);

	for(int i = 0; i < 2; ++i)
	for(int j = 0; j < 7; ++j)
		aiParams.k[i][j] = readUint16(exe);

	for(int i = 0; i < 2; ++i)
		bonusSObjects[i] = readUint8(exe) - 1;
}

void Game::generateLevel()
{
	if(settings.randomLevel)
	{
		level.generateRandom();
	}
	else
	{
		// TODO: Check .LEV as well as .lev
#if defined(HOME_DIR)
		if(!level.load(joinPath(lieroConfigRoot, settings.levelFile + ".lev")))
			level.generateRandom();
#else
		if(!level.load(joinPath(lieroDataRoot, settings.levelFile + ".lev")))
			level.generateRandom();
#endif
	}
	
	oldRandomLevel = settings.randomLevel;
	oldLevelFile = settings.levelFile;
	
	if(settings.shadow)
	{
		level.makeShadow();
	}
}

void Game::saveSettings()
{
#if defined(HOME_DIR)
	settings.save(joinPath(lieroConfigRoot, settingsFile + ".DAT"));
#else
	settings.save(joinPath(lieroDataRoot, settingsFile + ".DAT"));
#endif
}

bool Game::loadSettings()
{
#if defined(HOME_DIR)
	return settings.load(joinPath(lieroConfigRoot, settingsFile + ".DAT"));
#else
	return settings.load(joinPath(lieroDataRoot, settingsFile + ".DAT"));
#endif
}

void Game::draw()
{
	drawViewports();
}

bool checkBonusSpawnPosition(int x, int y)
{
	Rect rect(x - 2, y - 2, x + 3, y + 3);
	
	rect.intersect(game.level.rect());
	
	for(int cx = rect.x1; cx < rect.x2; ++cx)
	for(int cy = rect.y1; cy < rect.y2; ++cy)
	{
		if(game.materials[game.level.pixel(cx, cy)].dirtRock())
			return false;
	}
	
	return true;
}

void createBonus()
{
	if(int(game.bonuses.size()) >= game.settings.maxBonuses)
		return;
		
	Bonus* bonus = game.bonuses.newObject();
	if(!bonus)
		return;
	
	for(std::size_t i = 0; i < 50000; ++i)
	{
		int ix = game.rand(C[BonusSpawnRectW]);
		int iy = game.rand(C[BonusSpawnRectH]);
		
		if(H[HBonusSpawnRect])
		{
			ix += C[BonusSpawnRectX];
			iy += C[BonusSpawnRectY];
		}
		
		if(checkBonusSpawnPosition(ix, iy))
		{
			int frame;
			
			if(H[HBonusOnlyHealth])
				frame = 1;
			else if(H[HBonusOnlyWeapon])
				frame = 0;
			else
				frame = game.rand(2);
			
			bonus->x = itof(ix);
			bonus->y = itof(iy);
			bonus->velY = 0;
			bonus->frame = frame;
			bonus->timer = game.rand(game.bonusRandTimer[frame][1]) + game.bonusRandTimer[frame][0];
			
			if(frame == 0)
			{
				do
				{
					bonus->weapon = game.rand(40); // TODO: Unhardcode
				}
				while(game.settings.weapTable[bonus->weapon] == 2);
			}
			
			game.sobjectTypes[7].create(ix, iy, 0);
			return;
		}
	} // 234F
	
	game.bonuses.free(bonus);
}

void Game::startGame(bool isStartingGame)
{
	gfx.pal.clear();
	
	if(isStartingGame)
	{
		if(settings.regenerateLevel
		|| settings.randomLevel != oldRandomLevel
		|| settings.levelFile != oldLevelFile)
		{
			generateLevel();
		}
	
		
		initGame();
		
		
		
		for(std::size_t i = 0; i < viewports.size(); ++i)
		{
			viewports[i]->x = 0;
			viewports[i]->y = 0;
		}

		selectWeapons();
		
		sfx.play(22, 22);

		cycles = 0;
		
		for(int w = 0; w < 40; ++w)
		{
			weapons[w].computedLoadingTime = (settings.loadingTime * weapons[w].loadingTime) / 100;
			if(weapons[w].computedLoadingTime == 0)
				weapons[w].computedLoadingTime = 1;
		}
	}
	
	int fadeAmount = isStartingGame ? 180 : 0;
	bool shutDown = false;
	
	do
	{
		++cycles;
		
		if(!H[HBonusDisable]
		&& settings.maxBonuses > 0
		&& rand(C[BonusDropChance]) == 0)
		{
			createBonus();
		}
			
		for(std::size_t i = 0; i < worms.size(); ++i)
		{
			worms[i]->process();
		}
		
		for(std::size_t i = 0; i < worms.size(); ++i)
		{
			worms[i]->ninjarope.process(*worms[i]);
		}
		
		switch(game.settings.gameMode)
		{
		case Settings::GMGameOfTag:
		{
			bool someInvisible = false;
			for(std::size_t i = 0; i < worms.size(); ++i)
			{
				if(!worms[i]->visible)
				{
					someInvisible = true;
					break;
				}
			}
			
			if(!someInvisible
			&& lastKilled
			&& (cycles % 70) == 0
			&& lastKilled->timer < settings.timeToLose)
			{
				++lastKilled->timer;
			}
		}
		break;
		}
		
		processViewports();
		drawViewports();
				
		for(BonusList::iterator i = bonuses.begin(); i != bonuses.end(); ++i)
		{
			i->process();
		}
		
		if((cycles & 1) == 0)
		{
			for(std::size_t i = 0; i < viewports.size(); ++i)
			{
				Viewport& v = *viewports[i];
				
				bool down = false;
				
				if(v.worm->killedTimer > 16)
					down = true;
					
				if(down)
				{
					if(v.bannerY < 2)
						++v.bannerY;
				}
				else
				{
					if(v.bannerY > -8)
						--v.bannerY;
				}
			}
		}
		
		for(SObjectList::iterator i = game.sobjects.begin(); i != game.sobjects.end(); ++i)
		{
			i->process();
		}
		
		// TODO: Check processing order of bonuses, wobjects etc.
		
		for(WObjectList::iterator i = wobjects.begin(); i != wobjects.end(); ++i)
		{
			i->process();
		}
		
		for(NObjectList::iterator i = nobjects.begin(); i != nobjects.end(); ++i)
		{
			i->process();
		}
		
		for(BObjectList::iterator i = bobjects.begin(); i != bobjects.end(); ++i)
		{
			i->process();
		}
		
		if((cycles & 3) == 0)
		{
			for(int w = 0; w < 4; ++w)
			{
				gfx.origpal.rotate(gfx.colourAnim[w].from, gfx.colourAnim[w].to);
			}
		}
		
		gfx.pal = gfx.origpal;
		
		if(fadeAmount <= 32)
			gfx.pal.fade(fadeAmount);

		if(gfx.screenFlash > 0)
		{
			gfx.pal.lightUp(gfx.screenFlash);
		}
		
		gfx.flip();
		gfx.process();
		
		if(gfx.screenFlash > 0)
			--gfx.screenFlash;
		
		if(isGameOver())
		{
			gfx.firstMenuItem = 1;
			shutDown = true;
		}
		
		for(std::size_t i = 0; i < viewports.size(); ++i)
		{
			if(viewports[i]->shake > 0)
				viewports[i]->shake -= 4000; // TODO: Read 4000 from exe?
		}

		if(gfx.testSDLKeyOnce(SDLK_ESCAPE)
		&& !shutDown)
		{
			gfx.firstMenuItem = 0;
			fadeAmount = 31;
			shutDown = true;
		}

		if(shutDown)
		{
			fadeAmount -= 1;
		}
		else if(!isStartingGame)
		{
			if(fadeAmount < 33)
			{
				fadeAmount += 1;
				if(fadeAmount >= 33)
					fadeAmount = 180;
			}
		}
	}
	while(fadeAmount > 0);
	
	gfx.clearKeys();
}

bool Game::isGameOver()
{
	if(settings.gameMode == Settings::GMKillEmAll)
	{
		for(std::size_t i = 0; i < worms.size(); ++i)
		{
			if(worms[i]->lives <= 0)
				return true;
		}
	}
	else if(settings.gameMode == Settings::GMGameOfTag)
	{
		for(std::size_t i = 0; i < worms.size(); ++i)
		{
			if(worms[i]->timer >= game.settings.timeToLose)
				return true;
		}
	}
	else if(settings.gameMode == Settings::GMCtF
	|| settings.gameMode == Settings::GMSimpleCtF)
	{
		for(std::size_t i = 0; i < worms.size(); ++i)
		{
			if(worms[i]->flags >= game.settings.flagsToWin)
				return true;
		}
	}
	
	return false;
}

bool checkRespawnPosition(int x2, int y2, int oldX, int oldY, int x, int y)
{
	int deltaX = oldX;
	int deltaY = oldY - y;
	int enemyDX = x2 - x;
	int enemyDY = y2 - y;
	
	if((std::abs(deltaX) <= C[WormMinSpawnDistLast] && std::abs(deltaY) <= C[WormMinSpawnDistLast])
	|| (std::abs(enemyDX) <= C[WormMinSpawnDistEnemy] && std::abs(enemyDY) <= C[WormMinSpawnDistEnemy]))
		return false;
		
	int maxX = x + 3;
	int maxY = y + 4;
	int minX = x - 3;
	int minY = y - 4;
	
	if(maxX >= game.level.width) maxX = game.level.width - 1;
	if(maxY >= game.level.height) maxY = game.level.height - 1;
	if(minX < 0) minX = 0;
	if(minY < 0) minY = 0;
	
	for(int i = minX; i != maxX; ++i)
	for(int j = minY; j != maxY; ++j)
	{
		if(game.materials[game.level.pixel(i, j)].rock()) // TODO: The special rock respawn bug is here, consider an option to turn it off
			return false;
	}
	
	return true;
}
