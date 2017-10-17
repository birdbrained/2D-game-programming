#ifndef __ENTITY_S__
#define __ENTITY_S__

#define MAX_CHARS 32
#define MAX_FILEPATH_CHARS 256

#include <stdio.h>
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "shape.h"

typedef enum statusAliment
{
	Status_None,
	Status_Offstep,
	Status_Flat,
	Status_Sharp,
	Status_Exhausted,
	Status_Adrenaline_Rush
}StatusAliment;

typedef enum entityStates
{
	ES_Idle,
	ES_Moving,
	ES_Music_Time
}EntityState;

typedef enum instrument
{
	Instrument_Unassigned,
	Instrument_Flute,
	Instrument_Clarinet,
	Instrument_Alto_Saxophone,
	Instrument_Tenor_Saxophone,
	Instrument_Trumpet,
	Instrument_Baritone,
	Instrument_Tuba,
	Instrument_Snare_Drum,
	Instrument_Bass_Drum,
	Instrument_Pit_Marimba,
	Instrument_Pit_Xylophone,
	Instrument_Pit_Guitar,
	Instrument_Pit_Synthesizer,
	Instrument_Pit_Gong,
	Instrument_Colorguard_Flag,
	Instrument_Colorguard_Rifle
}Instrument;

typedef enum section
{
	Section_Band,
	Section_Drumline,
	Section_Colorguard,
	Section_Pit
}Section;

/**
 * @brief entity_s Struct that represents a generic entity 
 */
typedef struct entity_s 
{
	short unsigned int inUse;		/**<Don't ever touch this. 1 if in use, 0 if not*/
	Uint64 id;						/**<Auto increment id for this entity*/
	char name[MAX_CHARS];			/**<name of the object*/
	char favoriteThing[MAX_CHARS];	/**<guy's favorite thing, just for funsies*/
	char instrumentSpriteFilePath[MAX_FILEPATH_CHARS];

	//physics
	Vector2D position;				/**<Draw position*/
	Vector2D velocity;				/**<Desired movement direction*/
	Vector2D acceleration;			/**<Acceleration*/

	//collision
	Rect *boundingBox;

	//grahpics
	Sprite *mySprite;				/**<The entity's sprite, might get changed later*/
	Vector2D scale;					/**<Scale to draw the sprite at, will default to 1,1*/
	Vector2D scaleCenter;			/**<Center of sprite to scale at*/
	Vector2D rotation;				/**<Rotation*/
	Vector2D flip;					/**<Flip horizontal or vertical*/
	float currentFrame;				/**<Current frame of animation*/
	float minFrame;
	float maxFrame;
	
	//draw, think, update, touch, damage, die, free
	void(*update)(struct entity_s *self);	/**<Pointer to an update function that will get called every frame*/
	void(*on_click)(struct entity_s *self);
	void(*die)(struct entity_s *self);

	//game specific data
	StatusAliment currentStatus;
	EntityState currentState;
	Instrument myInstrument;
	Section mySection;
	Sprite *instrumentSprite;
	int statMarching;
	int statMusic;
	int statMorale;
	int statMotivation;
	short unsigned int isSectionLeader;
	int currentPosition;
}Entity;

/**
 * @brief Start of think function look-up table
 */
struct think_function
{
	void(*my_think_function)(Entity * self);
	char *name;
};

/**
 * @brief Initializes the Entity Manager
 * @param max The maximum number of entities that can be handled at once
 */
void entitySystemInit(Uint32 max);

/**
 * @brief Finds a memory address in the Entity Manager that is not in use, and returns that address
 * @returns An address in memory for the new entity if successful; NULL if failed
 */
Entity *entityNew();

/**
 * @brief Deletes a specified Entity from memory
 * @param thingThatDies Pointer to entity that will be deleted
 */
void entityDelete(Entity * thingThatDies);

/**
 * @brief Clears an entity from the entity manager, opening the spot back up
 * @param e The entity to free
 */
void entityFree(Entity * e);

/**
 * @brief Deletes all loaded enities from memory, but does not close the entity system
 */
void entityDeleteAll();

/**
 * @brief Performs generic entity updates (velocity/position updating, collision box movement, etc.)
 * @param self Entity to update
 * @returns Early return if error
 */
void entityUpdate(Entity * self);

/**
 * @brief Updates every entity currently existing
 */
void entityUpdateAll();

/**
 * @brief Draws the specified entity with it's parameters
 * @param e The entity to draw
 */
void entityDraw(Entity * e);

/**
 * @brief Draws every valid entity in the entity manager
 */
void entityDrawAll();

void entityIncrementCurrentFrame(Entity * self);
void entityIncrementCurrentFrameAll();

/**
 * @brief Inits an entity's parameters from a specified file
 * @param file File to read through
 * @param new_entity Entity that will be inited
 * @returns Populates new_entity parameter and returns it, NULL if invalid file pointer
 */
Entity * entityLoadFromFile(FILE * file, Entity * new_entity);

#endif // ! __ENTITY_S__
