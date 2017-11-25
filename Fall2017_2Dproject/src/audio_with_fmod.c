/*#include "audio_with_fmod.h"

typedef struct fsoundmanager_s
{
	Uint32 maxSounds;
	FSound * soundList;
	Uint64 increment;
}FSoundManager;

static FSoundManager soundManager = { 0, NULL, 0 };

void fsound_system_init(Uint32 maxSounds, int maxChannels, FMOD_INITFLAGS flags, void * extraData)
{
	FMOD_RESULT result = 0;
	if (maxSounds <= 0)
	{
		slog("Error: cannot initialize a sound manager for 0 sounds");
		return;
	}
	if (maxChannels <= 0)
	{
		slog("Error: cannot initialize a sound system with 0 channels");
		return;
	}

	//result = FMOD_System_Create(&system);
	//if (result != FMOD_OK)
	//{
		//slog("Error creating FMOD system: (%s)", PHYSFS_getLastError());
	//}
	//result = FMOD_System_Init(system, maxChannels, flags, extraData);
	//{
		//slog("Error initializing FMOD system: (%s)", PHYSFS_getLastError());
	//}

	memset(&soundManager, 0, sizeof(FSoundManager));

	soundManager.soundList = (FSound *)malloc(sizeof(FSound) * maxSounds);
	if (!soundManager.soundList)
	{
		slog("Could not allocate memory for the sound list");
		//audioSystemClose();
		return;
	}
	memset(soundManager.soundList, 0, sizeof(FSound) * maxSounds);
	soundManager.maxSounds = maxSounds;

	slog("Sound system initialized");
	//atexit(audioSystemClose);
}

FSound * fsound_new(Instrument instrument)
{
	int i;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		if (soundManager.soundList[i].inUse == 0)
		{
			slog("Found a spot for a sound at (%i)", i);
			memset(&soundManager.soundList[i], 0, sizeof(FSound));
			soundManager.soundList[i].id = soundManager.increment++;
			soundManager.soundList[i].inUse = 1;
			soundManager.soundList[i].instrument = instrument;
			//strncpy(soundManager.soundList[i].filepath, filepath, FILEPATH_CHAR_LEN);
			return &soundManager.soundList[i];
		}
	}

	slog("Error: out of sound addresses!");
	return NULL;
}

FSound * fsound_load(FMOD_SYSTEM * system, char filepath[MAX_FSOUND_FILEPATH_LENGTH], int mode, FMOD_CREATESOUNDEXINFO * exinfo, Instrument instr)
{
	FSound * sound = NULL;
	FMOD_SOUND * fsound = NULL;
	FMOD_RESULT result = 0;
	if (filepath[0] == '\0')
	{
		slog("Error: cannot load a sound from a null filepath");
		return NULL;
	}
	sound = fsound_new(instr);
	result = FMOD_System_CreateSound(system, filepath, mode, exinfo, &fsound);
	if (result != FMOD_OK)
	{
		slog("Error creating FMOD sound: (%s)", PHYSFS_getLastError());
	}
	sound->sound = fsound;

	return sound;
}

void fsound_play(FMOD_SYSTEM * system, FSound * sound)
{
	if (!sound)
	{
		slog("Error: cannot play a null sound");
		return;
	}

	FMOD_System_PlaySound(system, sound->sound, NULL, 0, 0);
}
*/