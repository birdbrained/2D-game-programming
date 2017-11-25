/*#ifndef __AUDIO_WITH_FMOD__
#define __AUDIO_WITH_FMOD__


#include <SDL.h>
#include <fmod.h>
//#include <audio.h>
#include "simple_logger.h"
#include "entity_s.h"

#define MAX_FSOUND_FILEPATH_LENGTH 512

typedef struct fsound_s
{
	Uint64 id;
	Uint8 inUse;
	char filepath[MAX_FSOUND_FILEPATH_LENGTH];
	FMOD_SOUND *sound;
	float volume;
	int defaultChannel;
	Instrument instrument;
}FSound;

void fsound_system_init(Uint32 maxSounds, int maxChannels, FMOD_INITFLAGS flags, void * extraData);
FSound * fsound_new(Instrument instrument);
FSound * fsound_load(FMOD_SYSTEM * system, char filepath[MAX_FSOUND_FILEPATH_LENGTH], int mode, FMOD_CREATESOUNDEXINFO *exinfo, Instrument instr);
void fsound_play(FMOD_SYSTEM * system, FSound * sound);

#endif // !__AUDIO_WITH_FMOD__
*/