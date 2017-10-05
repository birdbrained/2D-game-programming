#ifndef __AUDIO__
#define __AUDIO__

#define FILEPATH_CHAR_LEN 512

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "simple_logger.h"

typedef struct sound_s 
{
	Uint64 id;
	short unsigned int inUse;
	char filepath[FILEPATH_CHAR_LEN];
	Mix_Chunk *sound;
	float volume;
	int defaultChannel;
}Sound;

void audioSystemInit(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8 enableMP3, Uint8 enableOGG);
void soundSystemInit(Uint32 maxSounds);
Sound * soundNew(char * filepath);
Sound * soundGetByFilename(char * filename);
Sound * soundLoad(char * filename, float volume, int defaultChannel);
void soundPlay(Sound *sound, int numLoops, float volume, int channel, int group);
void soundFree(Sound *sound);
void soundDelete(Sound *soundToKill);
void soundDeleteAll();

#endif // !__AUDIO__
