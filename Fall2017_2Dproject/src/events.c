#include "events.h"

Vector2D scaleUp = { 2, 2 };
Vector4D trans = { 255, 255, 255, 150 };

Event event_decide()
{
	Event e = 0;
	e = random_int(0, Event_Max_Number);
	slog("event: (%i)", e);
	return e;
}

int event_assign_tiles(Graph ** graph, Event e, int height)
{
	int maxNumOfEventsToSpawn = 0;
	int i = 0;
	int x = 0;
	int y = 0;
	GraphNode * node = NULL;

	if (!graph || !(*graph))
	{
		return -1;
	}

	switch (e)
	{
	case Event_Football:
		maxNumOfEventsToSpawn = 7;
		break;
	case Event_Colorguard_Flags:
		maxNumOfEventsToSpawn = 4;
		break;
	default:
		break;
	}

	for (i = 0; i < maxNumOfEventsToSpawn; i++)
	{
		x = random_int(0, (*graph)->width);
		y = random_int(0, height);
		node = graph_find_node(graph, x, y);
		if (node)
		{
			node->hasEvent = 1;
		}
	}

	return 0;
}

void event_damage_entities(int damage, Graph * graph)
{
	GraphNode * hor_iter = NULL;
	GraphNode * ver_iter = NULL;

	if (!graph)
	{
		slog("Error: cannot damage entities in a null graph");
		return;
	}

	hor_iter = graph->head;
	ver_iter = graph->head;

	while (ver_iter != NULL)
	{
		while (hor_iter != NULL)
		{
			if (hor_iter->data != NULL && hor_iter->hasEvent)
			{
				entityDamage(hor_iter->data, damage);
			}
			hor_iter = hor_iter->right_node;
		}
		ver_iter = ver_iter->down_node;
		hor_iter = ver_iter;
	}
}

int event_execute(Event e, Graph * graph)
{
	if (!graph)
	{
		return -1;
	}

	switch (e)
	{
	case Event_Football:
		event_damage_entities(DAMAGE_FROM_FOOTBALL_PLAYER, graph);
		break;
	default:
		break;
	}
	return 0;
}

int event_draw_from_graph(Graph * graph, Sprite * sprite, int tileWidth, int tileHeight, int xPos, int yPos)
{
	GraphNode * hor_iter = NULL;
	GraphNode * ver_iter = NULL;
	if (!graph)
	{
		slog("Error: cannot draw where events are from a null graph");
		return -1;
	}

	hor_iter = graph->head;
	ver_iter = graph->head;
	while (ver_iter != NULL)
	{
		while (hor_iter != NULL)
		{
			if (hor_iter->hasEvent > 0)
			{
				gf2d_sprite_draw(
					sprite,
					vector2d(hor_iter->x * tileWidth + xPos, hor_iter->y * tileHeight + yPos),
					&scaleUp,
					NULL,
					NULL,
					NULL,
					&trans,
					0
				);
			}
			hor_iter = hor_iter->right_node;
		}
		ver_iter = ver_iter->down_node;
		hor_iter = ver_iter;
	}

	return 0;
}
