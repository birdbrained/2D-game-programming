#include <SDL.h>
#include "shape.h"
#include "simple_logger.h"

Rect * rect_new(float x, float y, float width, float height)
{
	Rect * new_rect = NULL;
	new_rect = malloc(sizeof(Rect));
	if (!new_rect)
	{
		slog("Could not allocate memory for a new Rect");
		return NULL;
	}
	memset(new_rect, 0, sizeof(Rect));
	new_rect->x = x;
	new_rect->y = y;
	new_rect->w = width;
	new_rect->h = height;
	return new_rect;
}

int point_in_rect(float x, float y, Rect * rect)
{
	if (x >= rect->x)
		if (x <= rect->x + rect->w)
			if (y >= rect->y)
				if (y <= rect->y + rect->h)
					return 1;
	return 0;
}
