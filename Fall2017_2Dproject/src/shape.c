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
	if (!rect)
	{
		return -1;
	}
	if (x >= rect->x)
		if (x <= rect->x + rect->w)
			if (y >= rect->y)
				if (y <= rect->y + rect->h)
					return 1;
	return 0;
}

int rect_in_rect(Rect * a, Rect * b)
{
	if ((a->x + a->w) < b->x) return 0;
	if ((b->x + b->w) < a->x) return 0;
	if ((a->y + a->h) < b->y) return 0;
	if ((b->y + b->h) < a->y) return 0;
	return 1;
}

Circle * circle_new(float x, float y, float radius)
{
	Circle * c;
	c = (Circle *)malloc(sizeof(Circle));
	if (!c)
	{
		slog("Error: could not allocate memory for a new circle");
		return NULL;
	}
	memset(c, 0, sizeof(Circle));
	c->x = x;
	c->y = y;
	c->r = radius;
	return c;
}

int point_in_circle(float x, float y, Circle * c)
{
	if (!c)
	{
		slog("Error: circle was null");
		return 0;
	}

	float delta_x = x - c->x;
	float delta_y = y - c->y;
	
	if ((delta_x * delta_x) + (delta_y * delta_y) <= (c->r * c->r)) return 1;
	return 0;
}

void draw_line(Vector2D pointA, Vector2D pointB, Vector4D color)
{
	SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), color.x, color.y, color.z, color.w);
	SDL_RenderDrawLine(gf2d_graphics_get_renderer(), pointA.x, pointA.y, pointB.x, pointB.y);
	SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 255, 255, 255, 255);
}
