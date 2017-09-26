#include "entity_s.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"

typedef struct
{
	Uint32 maxEntities;
	Entity * entityList;
	Uint64 increment;
}EntityManager;

//static variables prevents any other file from accessing the variable
//similar to private in this case
static EntityManager entityManager;

/**
 * @brief Entity system clean-up function
 */
void entitySystemClose()
{
	int i;
	//entityDeleteAll();
	if (entityManager.entityList != NULL)
	{
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			entityFree(&entityManager.entityList[i]);
		}
		free(entityManager.entityList);
	}
	//entityManager.entityList = NULL;
	//entityManager.maxEntities = 0;
	memset(&entityManager, 0, sizeof(EntityManager));
	slog("Entity system closed.");
}

void entitySystemInit(Uint32 max)
{
	if (max <= 0)
	{
		slog("Cannot initialize an entity manager for zero or negative entities!");
		return;
	}
	memset(&entityManager, 0, sizeof(EntityManager));

	entityManager.entityList = (Entity *)malloc(sizeof(Entity) * max);
	if (!entityManager.entityList)
	{
		slog("Could not allocate memory for the entity list");
		entitySystemClose();
		return;
	}
	memset(entityManager.entityList, 0, sizeof(Entity) * max);
	entityManager.maxEntities = max;

	slog("Entity system initialized");
	//on exit clean-up stuff goes here
	atexit(entitySystemClose);
}

Entity * entityNew()
{
	int i;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse == 0)
		{
			slog("Found a suitible spot at (%i)", i);
			memset(&entityManager.entityList[i], 0, sizeof(Entity));
			entityManager.entityList[i].id = entityManager.increment++;
			entityManager.entityList[i].inUse = 1;
			vector2d_set(entityManager.entityList[i].scale, 1, 1);
			//entityManager.entityList[i].actor.color = vector4d(1, 1, 1, 1);
			return &entityManager.entityList[i];
		}
	}

	slog("Error: Out of entity addresses!");
	return NULL;
}

void entityDelete(Entity * thingThatDies)
{
	if (!thingThatDies)
	{
		slog("Cannot delete an entity that does not exist!");
		return;
	}
	/*if (thingThatDies->mySprite != NULL)
	{
		if (thingThatDies->mySprite->texture != NULL)
		{
			slog("destroying texture");
			SDL_DestroyTexture(thingThatDies->mySprite->texture);
		}
	}*/
	if (thingThatDies->inUse)
	{
		thingThatDies->inUse = 0;
	}
	//slog("Deleting a thing...");
	memset(thingThatDies, 0, sizeof(Entity));
	entityFree(thingThatDies);
	//slog("Deleted!");
}

void entityFree(Entity * e)
{
	int i;
	if (!e)
	{
		slog("Error: Cannot free something that doesn't exist!");
		return;
	}

	//function pointer to a free functions
	//sound free
	//free actor
	//particless

	//slog("Freeing something from memory.");
	e->inUse = 0;
	memset(e, 0, sizeof(Entity));
}

void entityDeleteAll()
{
	int i;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		entityDelete(&entityManager.entityList[i]);
	}
}

void entityUpdate(Entity * self)
{
	if (!(self))
	{
		slog("Cannot update an entity that does not exist");
		return;
	}
	if (!(self)->inUse)
	{
		slog("Cannot update an entity that is not in use");
		return;
	}
	
	vector2d_add((self)->velocity, (self)->velocity, (self)->acceleration);

	if ((self)->boundingBox != NULL)
	{
		(self)->boundingBox->x = (self)->position.x;
		(self)->boundingBox->y = (self)->position.y;
	}

	if ((self)->update != NULL)
	{
		(self)->update((self));
	}
}

void entityUpdateAll()
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse == 1)
		{
			entityUpdate(&entityManager.entityList[i]);
		}
	}
}

void entityDraw(Entity * self)
{
	if (!self)
	{
		slog("Cannot draw an entity that does not exist");
		return;
	}
	if (!self->inUse)
	{
		slog("Cannot draw an entity that is not in use");
		return;
	}
	if (!self->mySprite)
	{
		slog("Cannot draw an entity with no sprite");
		return;
	}
	gf2d_sprite_draw(
		self->mySprite, 
		self->position, 
		&self->scale, 
		&self->scaleCenter, 
		&self->rotation, 
		&self->flip, 
		NULL, 
		self->currentFrame
	);

	if (self->instrumentSprite)
	{
		gf2d_sprite_draw(
			self->instrumentSprite,
			self->position,
			&self->scale,
			NULL,
			NULL,
			NULL,
			NULL,
			0
		);
	}
}

void entityDrawAll()
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse)
		{
			entityDraw(&entityManager.entityList[i]);
		}
	}
}
