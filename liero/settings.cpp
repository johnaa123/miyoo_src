#include "settings.hpp"

#include "reader.hpp"
#include "keys.hpp"
#include "gfx.hpp"
#include "filesystem.hpp"

int const Settings::wormAnimTab[] =
{
	0,
	7,
	0,
	14
};

int maxBonuses;
int blood;
int timeToLose;
int flagsToWin;
int gameMode;
bool shadow;
bool loadChange;
bool namesOnBonuses;
bool regenerateLevel;
int lives;
int loadingTime;
bool randomLevel;
std::string levelFile;
bool map;
bool screenSync;

Settings::Settings()
: maxBonuses(4)
, blood(100)
, timeToLose(600)
, flagsToWin(20)
, gameMode(0)
, shadow(true)
, loadChange(true)
, namesOnBonuses(true)
, regenerateLevel(false)
, lives(15)
, loadingTime(100)
, randomLevel(true)
, map(true)
, screenSync(true)
{
	std::memset(weapTable, 0, sizeof(weapTable));
	
	wormSettings[0].colour = 32;
	wormSettings[1].colour = 41;
	
	wormSettings[0].selWeapX = 50; // TODO: Read from exe
	wormSettings[1].selWeapX = 210;

#if !defined(PLATFORM_GCW0)
	unsigned char defControls[2][7] =
	{
		{0x13, 0x21, 0x20, 0x22, 0x1D, 0x2A, 0x38},
		{0xA0, 0xA8, 0xA3, 0xA5, 0x75, 0x90, 0x36}
	};
#endif
	
	unsigned char defRGB[2][3] =
	{
		{26, 26, 63},
		{15, 43, 15}
	};

#if defined(PLATFORM_GCW0)

	// Default weapon settings
	weapTable[0] = 0;
	weapTable[1] = 0;
	weapTable[2] = 0;
	weapTable[3] = 0;
	weapTable[4] = 0;
	weapTable[5] = 1;
	weapTable[6] = 0;
	weapTable[7] = 0;
	weapTable[8] = 1;
	weapTable[9] = 1;
	weapTable[10] = 0;
	weapTable[11] = 1;
	weapTable[12] = 0;
	weapTable[13] = 1;
	weapTable[14] = 1;
	weapTable[15] = 1;
	weapTable[16] = 1;
	weapTable[17] = 0;
	weapTable[18] = 0;
	weapTable[19] = 1;
	weapTable[20] = 1;
	weapTable[21] = 1;
	weapTable[22] = 1;
	weapTable[23] = 1;
	weapTable[24] = 0;
	weapTable[25] = 1;
	weapTable[26] = 1;
	weapTable[27] = 1;
	weapTable[28] = 1;
	weapTable[29] = 1;
	weapTable[30] = 1;
	weapTable[31] = 0;
	weapTable[32] = 1;
	weapTable[33] = 1;
	weapTable[34] = 1;
	weapTable[35] = 1;
	weapTable[36] = 1;
	weapTable[37] = 0;
	weapTable[38] = 0;
	weapTable[39] = 1;

	// Player 1
	wormSettings[0].health = 100;
	wormSettings[0].controls[0] = 160;	// up
	wormSettings[0].controls[1] = 168;	// down
	wormSettings[0].controls[2] = 163;	// left
	wormSettings[0].controls[3] = 165;	// right
	wormSettings[0].controls[4] = 56;	// lalt
	wormSettings[0].controls[5] = 57;	// space
	wormSettings[0].controls[6] = 29;	// lshift
	wormSettings[0].controller = 0;		// Player
	wormSettings[0].name = "Player";
	wormSettings[0].randomName = false;
	// Player 2
	wormSettings[1].health = 100;
	wormSettings[1].controls[0] = 17;	// w
	wormSettings[1].controls[1] = 31;	// s
	wormSettings[1].controls[2] = 30;	// a
	wormSettings[1].controls[3] = 32;	// d
	wormSettings[1].controls[4] = 34;	// g
	wormSettings[1].controls[5] = 35;	// h
	wormSettings[1].controls[6] = 33;	// f
	wormSettings[1].controller = 1;		// AI
	wormSettings[1].name = "GCW0";
	wormSettings[1].randomName = false;

	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			wormSettings[i].rgb[j] = defRGB[i][j];
		}
	}
#else
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 7; ++j)
		{
			wormSettings[i].controls[j] = DOSToSDLKey(defControls[i][j]);
		}
		
		for(int j = 0; j < 3; ++j)
		{
			wormSettings[i].rgb[j] = defRGB[i][j];
		}
	}
#endif
}

template<int L, int H>
inline int limit(int v)
{
	if(v >= H)
		return H - 1;
	else if(v < L)
		return L;
		
	return v;
}

bool Settings::load(std::string const& path)
{
	FILE* opt = tolerantFOpen(path.c_str(), "rb");
	
	if(!opt)
		return false;
		
	std::size_t size = fileLength(opt);
	
	if(size < 155)
		return false; // .dat is too short
	
	maxBonuses = readUint8(opt);
	loadingTime = readUint16(opt);
	lives = readUint16(opt);
	timeToLose = readUint16(opt);
	flagsToWin = readUint16(opt);
	
	screenSync = readUint8(opt) != 0;
	map = readUint8(opt) != 0;
	wormSettings[0].controller = readUint8(opt) & 0x1;
	wormSettings[1].controller = readUint8(opt) & 0x1;
	randomLevel = readUint8(opt) != 0;
	blood = readUint16(opt);
	gameMode = readUint8(opt);
	namesOnBonuses = readUint8(opt) != 0;
	regenerateLevel = readUint8(opt) != 0;
	shadow = readUint8(opt) != 0;
	
	//fread(weapTable, 1, 40, opt);
	
	for(int i = 0; i < 40; ++i)
	{
		weapTable[i] = limit<0, 3>(fgetc(opt));
	}
	
	for(int i = 0; i < 2; ++i)
	for(int j = 0; j < 3; ++j)
		wormSettings[i].rgb[j] = readUint8(opt) & 63;
		
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			wormSettings[i].weapons[j] = readUint8(opt);
		}
	}

	wormSettings[0].health = readUint16(opt);
	wormSettings[1].health = readUint16(opt);

	for(int i = 0; i < 2; ++i)
	{
		wormSettings[i].name = readPascalString(opt, 21);
	}
	
	//fgetc(opt); // What's this?
	
	loadChange = readUint8(opt) != 0;
	
	// 0x7B-83 is the string LIERO
	
	fseek(opt, 0x84, SEEK_SET);
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 7; ++j)
		{
			wormSettings[i].controls[j] = limit<0, 177>(readUint8(opt));
		}
	}
	
	levelFile = readPascalString(opt, 9);
	
	for(int i = 0; i < 2; ++i)
	{
		if(wormSettings[i].name.empty())
			generateName(wormSettings[i]);
		else
			wormSettings[i].randomName = false;
	}
	
	fclose(opt);
	
	return true;
}

void Settings::save(std::string const& path)
{
	FILE* opt = fopen(path.c_str(), "wb");
	
	writeUint8(opt, maxBonuses);
	writeUint16(opt, loadingTime);
	writeUint16(opt, lives);
	writeUint16(opt, timeToLose);
	writeUint16(opt, flagsToWin);
	
	writeUint8(opt, screenSync);
	writeUint8(opt, map);
	writeUint8(opt, wormSettings[0].controller);
	writeUint8(opt, wormSettings[1].controller);
	writeUint8(opt, randomLevel);
	writeUint16(opt, blood);
	writeUint8(opt, gameMode);
	writeUint8(opt, namesOnBonuses);
	writeUint8(opt, regenerateLevel);
	writeUint8(opt, shadow);
	
	fwrite(weapTable, 1, 40, opt);
	
	for(int i = 0; i < 2; ++i)
	for(int j = 0; j < 3; ++j)
		writeUint8(opt, wormSettings[i].rgb[j]);
		
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			writeUint8(opt, wormSettings[i].weapons[j]);
		}
	}

	writeUint16(opt, wormSettings[0].health);
	writeUint16(opt, wormSettings[1].health);

	for(int i = 0; i < 2; ++i)
	{
		if(wormSettings[i].randomName)
			writePascalString(opt, "", 21);
		else
			writePascalString(opt, wormSettings[i].name, 21);
	}
	
	//fputc(0, opt); // What's this?
	
	writeUint8(opt, loadChange);
	
	char const lieroStr[] = "\x05LIERO\0\0";
	
	fwrite(lieroStr, 1, sizeof(lieroStr), opt);
	
	//fseek(opt, 0x84, SEEK_SET);
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 7; ++j)
		{
			writeUint8(opt, wormSettings[i].controls[j]);
		}
	}
	
	writePascalString(opt, levelFile, 9);
	
	fclose(opt);
}

void Settings::generateName(WormSettings& ws)
{
	FILE* f = fopen(joinPath(lieroDataRoot, "NAMES.DAT").c_str(), "rb");
	
	if(!f)
		return;
		
	std::vector<std::string> names;
	
	std::size_t len = fileLength(f);
	
	std::vector<char> chars(len);
	
	fread(&chars[0], 1, len, f);
	
	fclose(f);
	
	std::size_t begin = 0;
	for(std::size_t i = 0; i < len; ++i)
	{
		if(chars[i] == '\r'
		|| chars[i] == '\n')
		{
			if(i > begin)
			{
				names.push_back(std::string(chars.begin() + begin, chars.begin() + i));
			}
			
			begin = i + 1;
		}
	}
	
	if(!names.empty())
	{
		ws.name = names[gfx.rand(Uint32(names.size()))];
		ws.randomName = true;
	}
}
