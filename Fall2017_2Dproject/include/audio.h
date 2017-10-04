#ifndef __AUDIO__
#define __AUDIO__

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "simple_logger.h"

typedef struct audio_s {
	short unsigned int inUse;
	char filepath[512];
	Mix_Chunk *sound;
	float volume;
	int defaultChannel;
}Audio;

void audio_init(unsigned int maxSounds, unsigned int channels, unsigned int channelGroups, unsigned int maxMusic);
Audio * audio_load(char * filename, float volume, int defaultChannel);
void audio_play(Audio *aud, int numLoops, float volume, int channel, int group);
void audio_free(Audio *aud);
void audio_clear_all();

#endif // !__AUDIO__
