#ifndef __ENTITY_S__
#define __ENTITY_S__

#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"


/**
 * @brief entity_s Struct that represents a generic entity 
 * @param inUse If 0, memory not in use; if non-zero (1), memory is in use
 * @param position Vector of position
 * @param velocity Vector of velocity
 * @param mySprite The sprite the entity will use
 * @param scale Vector of scale
 * @param currentFrame The index of the current frame to load
 */
typedef struct entity_s {
	short unsigned int inUse;
	Vector2D position;
	Vector2D velocity;
	Sprite *mySprite;
	Vector2D scale;
	unsigned int currentFrame;
	
	void(*update)(struct entity_s *self);
}Entity;

void entitySystemInit();
Entity *entityNew();
void entityDelete(Entity *thingThatDies);
void entityUpdate();
void entityDraw();

#endif // ! __ENTITY_S__
