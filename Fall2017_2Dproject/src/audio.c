#include "audio.h"

typedef struct audio_manager 
{
	Uint32 maxSounds;
	Sound * soundList;
	Uint64 increment;
}SoundManager;

static SoundManager soundManager = { 0, NULL, 0 };

void audioSystemClose()
{
	int i;
	if (soundManager.soundList != NULL)
	{
		for (i = 0; i < soundManager.maxSounds; i++)
		{
			soundFree(&soundManager.soundList[i]);
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

Sound * soundNew(/*char * filepath*/)
{
	int i;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		if (soundManager.soundList[i].inUse == 0)
		{
			slog("Found a spot for a sound at (%i)", i);
			memset(&soundManager.soundList[i], 0, sizeof(Sound));
			soundManager.soundList[i].id = soundManager.increment++;
			soundManager.soundList[i].inUse = 1;
			//strncpy(soundManager.soundList[i].filepath, filepath, FILEPATH_CHAR_LEN);
			return &soundManager.soundList[i];
		}
	}

	slog("Error: out of sound addresses!");
	return NULL;
}

Sound * soundGetByFilename(char * filename)
{
	int i;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		if (strncmp(soundManager.soundList[i].filepath, filename, FILEPATH_CHAR_LEN) == 0)
		{
			return &soundManager.soundList[i];
		}
	}

	slog("Could not find sound with filepath (%s)", filename);
	return NULL;
}

Sound * soundLoad(char * filename, float volume, int defaultChannel)
{
	Sound *sound;
	sound = soundGetByFilename(filename);
	if (sound)
	{
		sound->inUse = 1;
		return sound;
	}

	sound = soundNew(filename);
	if (!sound)
	{
		return NULL;
	}
	//slog("My song filepath (%s)", sound->filepath);
	sound->sound = Mix_LoadWAV(filename);
	if (!sound->sound)
	{
		slog("Failed to load sound file (%s)", Mix_GetError());
		soundFree(sound);
		return NULL;
	}

	sound->volume = volume;
	sound->defaultChannel = defaultChannel;
	strncpy(sound->filepath, filename, FILEPATH_CHAR_LEN);
	return sound;
}

void soundPlay(Sound * sound, int numLoops, float volume, int channel, int group)
{
	int actualChannel;
	float actualVolume = 1;
	if (!sound)
	{
		slog("Error: cannot play a sound that does not exist");
		return;
	}

	if (volume > 0)
	{
		actualVolume *= volume;
	}
	if (channel >= 0)
	{
		actualChannel = channel;
	}
	else
	{
		actualChannel = sound->defaultChannel;
	}

	Mix_VolumeChunk(sound->sound, (int)(actualVolume * MIX_MAX_VOLUME));
	Mix_PlayChannel(actualChannel, sound->sound, numLoops);
}

void soundFree(Sound * sound)
{
	if (!sound)
	{
		slog("Error: can't free a sound that doesn't exist!");
		return;
	}

	sound->inUse = 0;
	//memset(sound, 0, sizeof(Sound));
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
	if (soundToKill->sound != NULL)
	{
		Mix_FreeChunk(soundToKill->sound);
	}
	memset(soundToKill, 0, sizeof(Sound));
	soundFree(soundToKill);
}

void soundDeleteAll()
{
	int i;

	for (i = 0; i < soundManager.maxSounds; i++)
	{
		soundDelete(&soundManager.soundList[i]);
	}
}
