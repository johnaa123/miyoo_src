#include "reader.hpp"
#include "filesystem.hpp"
#include <SDL/SDL.h>
#include <map>
#include <stdexcept>

std::string lieroOPT;
std::string lieroDataRoot;
std::string lieroConfigRoot;

namespace
{

struct ReaderFile
{
	unsigned int lastTouch;
	FILE* f;
};

typedef std::map<std::string, ReaderFile> ReaderFileMap;

std::string lieroCHR;
std::string lieroSND;

ReaderFileMap readerFiles;

void closeReaderFile(ReaderFileMap::iterator i)
{
	fclose(i->second.f);
	readerFiles.erase(i);
}


}

FILE* openFile(std::string const& name)
{
	ReaderFileMap::iterator i = readerFiles.find(name);
	if(i != readerFiles.end())
	{
		i->second.lastTouch = SDL_GetTicks();
		return i->second.f;
	}

	FILE* f = tolerantFOpen(name.c_str(), "rb");
	if(!f)
		throw std::runtime_error("Could not open '" + name + '\'');
	ReaderFile& rf = readerFiles[name];
	rf.f = f;
	rf.lastTouch = SDL_GetTicks();
	return f;
}

FILE* openLieroSND()
{
	return openFile(lieroSND);
}

FILE* openLieroCHR()
{
	return openFile(lieroCHR);
}

void processReader()
{
	unsigned int now = SDL_GetTicks();
	for(ReaderFileMap::iterator i = readerFiles.begin(); i != readerFiles.end(); )
	{
		ReaderFileMap::iterator cur = i;
		++i;
		
		if((now - cur->second.lastTouch) > 5000)
		{
			closeReaderFile(cur);
		}
	}
}

void closeAllCachedFiles()
{
	for(ReaderFileMap::iterator i = readerFiles.begin(); i != readerFiles.end(); )
	{
		ReaderFileMap::iterator cur = i;
		++i;
		closeReaderFile(cur);
	}
}

void setLieroDataRoot(std::string const& path)
{
	//TODO: Close cached files

	lieroDataRoot = path;

	lieroCHR = joinPath(lieroDataRoot, "LIERO.CHR");
	lieroSND = joinPath(lieroDataRoot, "LIERO.SND");
	lieroOPT = joinPath(lieroDataRoot, "LIERO.OPT");
}

void setConfigRoot()
{
#if defined(HOME_DIR)
	lieroConfigRoot = getHome();
	if(lieroConfigRoot.empty())
	{
		lieroConfigRoot = lieroDataRoot;
	}
#else
	lieroConfigRoot = lieroDataRoot;
#endif
}
