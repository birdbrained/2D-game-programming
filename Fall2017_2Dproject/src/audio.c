#include "audio.h"

typedef struct audio_manager 
{
	Uint32 maxSounds;
	Audio * audio_list;
}AudioManager;

static AudioManager audio_manager = { 0, NULL };

void audio_init(unsigned int maxSounds, unsigned int channels, unsigned int channelGroups, unsigned int maxMusic)
{
	int flags = 0;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		slog("Error: failed to open audio (%s)", SDL_GetError());
		return;
	}
	atexit(Mix_CloseAudio);

	if (!(Mix_Init(flags) & flags))
	{
		slog("Error: failed to initialize some audio support (%s)", SDL_GetError());
	}
	atexit(Mix_Quit);
	//audio close
}
