#include "audio.h"

typedef struct audio_manager 
{
	Uint32 maxSounds;
	Sound * soundList;
}SoundManager;

static SoundManager soundManager = { 0, NULL };

void audioSystemClose()
{
	int i;
	if (soundManager.soundList != NULL)
	{
		for (i = 0; i < soundManager.maxSounds; i++)
		{
			audioFree(&soundManager.soundList[i]);
		}
		free(soundManager.soundList);
	}
	memset(&soundManager, 0, sizeof(SoundManager));
	slog("Audio system closed.");
}

//Still need to actually malloc and memset space for the audio shits
void audioSystemInit(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8 enableMP3, Uint8 enableOGG)
{
	int flags = 0;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		slog("Error: failed to open audio (%s)", SDL_GetError());
		return;
	}
	atexit(Mix_CloseAudio);

	if (enableMP3)
	{
		flags |= MIX_INIT_MP3;
	}
	if (enableOGG)
	{
		flags |= MIX_INIT_OGG;
	}
	if (!(Mix_Init(flags) & flags))
	{
		slog("Error: failed to initialize some audio support (%s)", SDL_GetError());
	}
	atexit(Mix_Quit);
	atexit(audioSystemClose);
}

void soundSystemInit(Uint32 maxSounds)
{
	if (maxSounds <= 0)
	{
		slog("Error: cannot initialize a sound manager for zero sounds!");
		return;
	}
	memset(&soundManager, 0, sizeof(SoundManager));

	soundManager.soundList = (Sound *)malloc(sizeof(Sound) * maxSounds);
	if (!soundManager.soundList)
	{
		slog("Could not allocate memory for the sound list");
		audioSystemClose();
		return;
	}
	memset(soundManager.soundList, 0, sizeof(Sound) * maxSounds);
	soundManager.maxSounds = maxSounds;

	slog("Sound system initialized");
	atexit(audioSystemClose);
}

void soundFree(Sound * sound)
{
	if (!sound)
	{
		slog("Error: can't free a sound that doesn't exist!");
		return;
	}

	sound->inUse = 0;
	memset(sound, 0, sizeof(Sound));
}

void soundDelete(Sound * soundToKill)
{
	if (!soundToKill)
	{
		slog("Error: cannot delete a sound that does not exist");
		return;
	}

	if (soundToKill->inUse)
	{
		soundToKill->inUse = 0;
	}
	memset(soundToKill, 0, sizeof(Sound));
	audioFree(soundToKill);
}

void soundDeleteAll()
{
	int i;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		soundDelete(&soundManager.soundList[i]);
	}
}
