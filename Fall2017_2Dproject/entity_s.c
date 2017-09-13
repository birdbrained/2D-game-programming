#include "entity_s.h"
#include "simple_logger.h"

typedef struct
{
	Uint32 maxEntities;
	Entity * entityList;
}EntityManager;

static EntityManager entityManager;

/**
 * @brief Entity system clean-up function
 */
void entitySystemClose()
{
	entityDeleteAll();
	if (entityManager.entityList != NULL)
	{
		free(entityManager.entityList);
	}
	entityManager.entityList = NULL;
	entityManager.maxEntities = 0;
	slog("Entity system closed.");
}

void entitySystemInit(Uint32 max)
{
	if (max <= 0)
	{
		slog("Cannot initialize an entity manager for zero or negative entities!");
		return;
	}
	entityManager.maxEntities = max;
	entityManager.entityList = (Entity *)malloc(sizeof(Entity) * max);
	memset(entityManager.entityList, 0, sizeof(Entity) * max);

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
			entityManager.entityList[i].inUse = 1;
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
	if (thingThatDies->mySprite->texture != NULL)
	{
		SDL_DestroyTexture(thingThatDies->mySprite->texture);
	}
	if (thingThatDies->inUse)
	{
		thingThatDies->inUse = 0;
	}
	memset(thingThatDies, 0, sizeof(Entity));
}

void entityFree(Entity * e)
{
	if (!e)
	{
		return;
	}
	e->inUse = 0;
}

void entityDeleteAll()
{
	int i;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		entityDelete(&entityManager.entityList[i]);
	}
}
