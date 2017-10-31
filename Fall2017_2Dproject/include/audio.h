#ifndef __AUDIO__
#define __AUDIO__

#define FILEPATH_CHAR_LEN 512


#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <fmod.h>
#include "simple_logger.h"
#include "entity_s.h"

/**
 * @brief A sound structure that represents...a sound
 */
typedef struct sound_s 
{
	Uint64 id;							/**<id of the sound, unique*/
	short unsigned int inUse;			/**<1 if memory space in use, 0 if free*/
	char filepath[FILEPATH_CHAR_LEN];	/**<filepath to the sound file, wav or ogg*/
	Mix_Chunk *sound;					/**<space for the sound chunk*/
	float volume;						/**<earrape adjuster*/
	int defaultChannel;					/**<default channel to play on*/
	Instrument instrument;				/**<the instrument that is playing*/
	FMOD_SOUND *fsound;
}Sound;

/**
 * @brief Initializes the audio system (for actually playing audio)
 * @param maxSounds Max number of sounds that can be loaded into memory at once
 * @param channels Max number of channels to play sounds on
 * @param channelGroups Currently unimplemented
 * @param maxMusic Currently unimplemented
 * @param enableMP3 Currently unimplemented
 * @param enableOGG Currently unimplemented
 */
void audioSystemInit(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8 enableMP3, Uint8 enableOGG);

/**
 * @brief Initializes the sound system (space in memory to store sounds)
 * @param maxSounds Max number of sounds that can be loaded into memory at once
 */
void soundSystemInit(Uint32 maxSounds);

/**
 * @brief Allocates spot in sound manager for a new sound
 * @returns An address in memory for the new sound if successful; NULL if failed
 */
Sound * soundNew(Instrument instrument/*char * filepath*/);

/**
 * @brief Searches to see if a certain sound is already loaded in memory
 * @param filename File location
 * @returns The address in memory of the requested sound; NULL if that sound is not loaded in memory
 */
Sound * soundGetByFilename(char * filename);

/**
 * @brief Will load the actual sound
 * @param filename File location
 * @param volume Earrape adjuster
 * @param defaultChannel Default channel to play on
 * @returns The sound that has been loaded; NULL if error
 */
Sound * soundLoad(char * filename, float volume, int defaultChannel, Instrument instrument);

/**
 * @brief Plays a loaded sound
 * @param sound The sound to play
 * @param numLoops Number of times to loop, -1 to loop infinitely
 * @param volume Earrape adjuster
 * @param channel The channel to play on
 * @param group Currently unimplemented
 */
void soundPlay(Sound *sound, int numLoops, float volume, int channel, int group);

Sound * soundFindByInstrument(Instrument instrument);
void soundAdjustVolume(Sound * sound, float volume);
void soundAdjustVolumeAll(float volume);

/**
 * @brief Clears a sound from the sound manager, opening the spot back up
 * @param sound The sound to free
 */
void soundFree(Sound *sound);

/**
 * @brief Deletes a specified sound from memory
 * @param soundToKill The sound to delete
 */
void soundDelete(Sound *soundToKill);

/**
 * @brief Deletes all loaded sounds from memory, but does not close the sound system
 */
void soundDeleteAll();

#endif // !__AUDIO__
