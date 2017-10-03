#include "think_functions.h"

void move(Entity *e, Vector2D vec)
{
	if (!e)
	{
		return;
	}
	e->position = vector2d(e->position.x + vec.x, e->position.y + vec.y);
	/*if (e->boundingBox != NULL)
	{
	e->boundingBox->x = e->position.x;
	e->boundingBox->y = e->position.y;
	}*/
}

void die_after_time(Entity * e)
{
	if (!e)
	{
		slog("Cannot run die_after_time function for an entity that does not exist");
		return;
	}
	if (e->inUse == 0)
	{
		slog("Cannot run die_after_time function for an entity that is not in use");
		return;
	}
	if (SDL_GetTicks() % 50 == 0)
	{
		e->inUse = 0;
		entityDelete(e);
		e = NULL;
	}
}

int mousePress(SDL_MouseButtonEvent *b)
{
	if (b->button == SDL_BUTTON_LEFT)
	{
		return 1;
	}
	return 0;
}
