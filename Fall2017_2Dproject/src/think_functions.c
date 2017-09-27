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

int mousePress(SDL_MouseButtonEvent *b)
{
	if (b->button == SDL_BUTTON_LEFT)
	{
		return 1;
	}
	return 0;
}
