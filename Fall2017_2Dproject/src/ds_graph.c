#include "ds_graph.h"

/**
 * @brief Called by graph_delete_node, frees a graph node from memory
 * @param nodeThatDies The node that will be freed
 * @param elementSize The size of the node
 * @returns 0 if successful, -1 if data still in node, -2 if node is NULL
 */
int graph_free_node(GraphNode * nodeThatDies, size_t elementSize)
{
	if (!nodeThatDies)
	{
		slog("Error: trying to free a graph node that is null");
		return -2;
	}
	if (nodeThatDies->data != NULL)
	{
		slog("Error: trying to free a graph node that still has data in it");
		return -1;
	}
	memset(nodeThatDies, 0, sizeof(GraphNode) + elementSize);
	free(nodeThatDies);
	return 0;
}

GraphNode * graph_new_node(size_t elementSize)
{
	GraphNode * new_node;
	new_node = (GraphNode *)malloc(sizeof(GraphNode) + elementSize);
	if (!new_node)
	{
		slog("Error: could not allocate space for a new graph node");
		return NULL;
	}
	memset(new_node, 0, sizeof(GraphNode) + elementSize);
	new_node->elementSize = elementSize;
	return new_node;
}

Graph * graph_init(int width, size_t elementSize)
{
	Graph * new_graph;
	new_graph = (Graph *)malloc(sizeof(Graph));
	if (!new_graph)
	{
		slog("Error: could not allocate space for a new graph");
		return NULL;
	}
	memset(new_graph, 0, sizeof(Graph));
	new_graph->head = graph_new_node(elementSize);
	new_graph->tail = graph_new_node(elementSize);
	new_graph->width = width;
	//atexit(graph_clear);
	return new_graph;
}

void * graph_delete_node(GraphNode ** nodeThatDies)
{
	void * data = NULL;
	if (!nodeThatDies)
	{
		slog("Error: cannot delete a null graph node");
		return NULL;
	}
	if (!(*nodeThatDies) || (*nodeThatDies) < 5)
	{
		slog("Error: cannot delete a null graph node");
		return NULL;
	}

	if ((*nodeThatDies)->up_node != NULL)
	{
		if ((*nodeThatDies)->down_node != NULL)
		{
			(*nodeThatDies)->up_node->down_node = (*nodeThatDies)->down_node;
		}
		else
		{
			(*nodeThatDies)->up_node->down_node = NULL;
		}
	}
	if ((*nodeThatDies)->right_node != NULL)
	{
		if ((*nodeThatDies)->left_node != NULL)
		{
			(*nodeThatDies)->right_node->left_node = (*nodeThatDies)->left_node;
		}
		else
		{
			(*nodeThatDies)->right_node->left_node = NULL;
		}
	}
	if ((*nodeThatDies)->down_node != NULL)
	{
		if ((*nodeThatDies)->up_node != NULL)
		{
			(*nodeThatDies)->down_node->up_node = (*nodeThatDies)->up_node;
		}
		else
		{
			(*nodeThatDies)->down_node->up_node = NULL;
		}
	}
	if ((*nodeThatDies)->left_node != NULL)
	{
		if ((*nodeThatDies)->right_node != NULL)
		{
			(*nodeThatDies)->left_node->right_node = (*nodeThatDies)->right_node;
		}
		else
		{
			(*nodeThatDies)->left_node->right_node = NULL;
		}
	}

	data = (*nodeThatDies)->data;
	(*nodeThatDies)->data = NULL;
	graph_free_node((*nodeThatDies), (*nodeThatDies)->elementSize);
	return data;
}

int graph_clear(Graph ** graphThatDies)
{
	GraphNode * horizontal_iterator = NULL;
	GraphNode * vertical_iterator = NULL;
	GraphNode * temp = NULL;

	if (!graphThatDies)
	{
		slog("Error: trying to delete a graph that is null");
		return -1;
	}
	if (!(*graphThatDies))
	{
		slog("Error: bad graph pointer");
		return -1;
	}

	horizontal_iterator = (*graphThatDies)->head;
	vertical_iterator = (*graphThatDies)->head;

	while (vertical_iterator != NULL)
	{
		horizontal_iterator = vertical_iterator;
		vertical_iterator = vertical_iterator->down_node;
		while (horizontal_iterator != NULL)
		{
			temp = horizontal_iterator;
			horizontal_iterator = horizontal_iterator->right_node;
			graph_delete_node(&temp);
		}
	}

	return 0;
}

int graph_insert(Graph ** graph, void * data, int width, size_t elementSize, int firstInsert)
{
	GraphNode * n = graph_new_node(elementSize);
	GraphNode * temp = NULL;

	if (!n)
	{
		//slog in graph_new_node
		return -1;
	}
	if (!graph)
	{
		slog("Error: cannot insert into a null graph");
		return -2;
	}
	n->data = data;
	n->elementSize = elementSize;

	if ((*graph)->head == NULL || firstInsert == 0)						//if the node is the first one in the graph
	{
		(*graph)->head = n;
		(*graph)->tail = n;
		(*graph)->head->x = 0;
		(*graph)->head->y = 0;
	}
	else
	{
		if ((*graph)->tail->x + 1 < width)		//if x + 1 < width...
		{
			n->x = (*graph)->tail->x + 1;		//new node has x++
			n->y = (*graph)->tail->y;			//new node's y is still the same
			(*graph)->tail->right_node = n;		//iterator's right = new node
			n->left_node = (*graph)->tail;		//new node's left = iterator
			(*graph)->tail = n;					//iterator = new node

			if (n->y != 0)						//if we aren't on the first row
			{
				temp = (*graph)->head;			//temp = start
				while (temp->y < n->y - 1)		//go to new node's y - 1
				{
					temp = temp->down_node;
				}
				while (temp->x < n->x)			//go to new node's x
				{
					temp = temp->right_node;
				}
				temp->down_node = n;			//temp's bottom = new node
				n->up_node = temp;				//new node's top = temp
			}
		}
		else									//now it's time to move to the next row
		{
			n->x = 0;							//new node has x = 0
			(*graph)->tail = (*graph)->head;	//set iterator = start
			while ((*graph)->tail->down_node != NULL) //go down until next down pointer is NULL
			{
				(*graph)->tail = (*graph)->tail->down_node;
			}
			n->y = (*graph)->tail->y + 1;		//new node has y++
			(*graph)->tail->down_node = n;		//iterator's bottom = new node
			n->up_node = (*graph)->tail;		//new node's top = iterator
			(*graph)->tail = n;					//iterator = new node
		}
	}

	if ((*graph)->tail == NULL)
	{
		(*graph)->tail = n;
	}

	return 0;
}

int graph_update_node(Graph ** graph, int x, int y, void * data, size_t elementSize)
{
	GraphNode * iter = NULL;
	int currX = 0;
	int currY = 0;
	if (!graph)
	{
		slog("Error: cannot update node from a null graph");
		return -1;
	}
	iter = (*graph)->head;
	if (!iter)
	{
		slog("Error: graph's head was null!");
		return -3;
	}

	while (currX < x)
	{
		iter = iter->right_node;
		currX++;
		if (iter == NULL)
		{
			slog("Error: could not find node with x (%i) y (%i)", x, y);
			return -2;
		}
	}
	while (currY < y)
	{
		iter = iter->down_node;
		currY++;
		if (iter == NULL)
		{
			slog("Error: could not find node with x (%i) y (%i)", x, y);
			return -2;
		}
	}

	iter->data = data;
	return 0;
}

int graph_zero_all(Graph ** graph)
{
	GraphNode * hor_iter = NULL;
	GraphNode * ver_iter = NULL;
	if (!graph)
	{
		slog("Error: cannot zero data from a null graph");
		return -1;
	}
	hor_iter = (*graph)->head;
	ver_iter = (*graph)->head;
	while (ver_iter != NULL)
	{
		while (hor_iter != NULL)
		{
			hor_iter->data = 0;
			hor_iter->traversed = 0;
			hor_iter->hasEvent = 0;
			hor_iter = hor_iter->right_node;
		}
		ver_iter = ver_iter->down_node;
		hor_iter = ver_iter;
	}
	(*graph)->numAltoSaxes = 0;
	(*graph)->numBaritones = 0;
	(*graph)->numFlutes = 0;
	(*graph)->numOtherInstruments = 0;
	(*graph)->numSnareDrums = 0;
	(*graph)->numTrumpets = 0;
	return 0;
}

Graph * graph_init_from_tilemap(TileMap * map, size_t elementSize)
{
	Graph * grape = graph_init(map->width, elementSize);
	int i = 0;
	int max = 0;
	if (!grape)
	{
		//error! could not allocate memeory for a new graph
		return NULL;
	}
	if (!map)
	{
		slog("Error: cannot load a graph from a null tile map");
		return NULL;
	}
	max = map->width * map->height;

	for (i = 0; i < max; i++)
	{
		graph_insert(&grape, map->space[i], map->width, elementSize, i);
	}

	return grape;
}

void graph_print(Graph * graph)
{
	GraphNode * hor_iter = graph->head;
	GraphNode * ver_iter = graph->head;
	int data = 0;

	while (ver_iter != NULL)
	{
		while (hor_iter != NULL)
		{
			slog("x: (%i), y: (%i), data: (%i)", hor_iter->x, hor_iter->y, hor_iter->data);
			hor_iter = hor_iter->right_node;
		}
		ver_iter = ver_iter->down_node;
		hor_iter = ver_iter;
	}
}

GraphNode * graph_find_node(Graph ** graph, unsigned int x, unsigned int y)
{
	GraphNode * iter = NULL;

	if (graph == NULL)
	{
		slog("Error: cannot find node from an empty graph");
		return NULL;
	}

	iter = graph_new_node((*graph)->head->elementSize);
	iter = (*graph)->head;

	while (iter != NULL && iter->x < x)
	{
		iter = iter->right_node;
	}
	if (iter == NULL || iter->x != x)
	{
		slog("Error: node with x value (%i) does not exist in graph", x);
		return NULL;
	}
	while (iter != NULL && iter->y < y)
	{
		iter = iter->down_node;
	}
	if (iter == NULL || iter->y != y)
	{
		slog("Error: node with y value (%i) does not exist in graph", y);
		return NULL;
	}

	//slog("Found node x (%i) y (%i) data (%i)", iter->x, iter->y, iter->data);
	return iter;
}

void graph_draw_walkable_tiles(GraphNode * node, int radius, int tileWidth, int tileHeight, int xPos, int yPos, Vector4D color, Uint8 forceAll)
{
	GraphNode * start = node;
	GraphNode * hor_iter = node;
	GraphNode * ver_iter = node;
	SDL_Rect rect = { 0, 0, tileWidth, tileHeight };
	int x = radius;
	int i = 0;

	if (!node)
	{
		slog("Error: node was null");
		return;
	}

	//draw the right side
	while (x >= 0)
	{
		rect.x = hor_iter->x * tileWidth + xPos;
		rect.y = hor_iter->y * tileHeight + yPos;
		draw_filled_rect(rect, color);
		for (i = 0; i < x; i++)
		{
			ver_iter = ver_iter->up_node;
			if (ver_iter == NULL)
			{
				break;
			}
			else
			{
				rect.x = ver_iter->x * tileWidth + xPos;
				rect.y = ver_iter->y * tileHeight + yPos;
				draw_filled_rect(rect, color);
			}
		}
		ver_iter = hor_iter;
		for (i = 0; i < x; i++)
		{
			ver_iter = ver_iter->down_node;
			if (ver_iter == NULL)
			{
				break;
			}
			else
			{
				rect.x = ver_iter->x * tileWidth + xPos;
				rect.y = ver_iter->y * tileHeight + yPos;
				draw_filled_rect(rect, color);
			}
		}
		hor_iter = hor_iter->right_node;
		if (hor_iter == NULL)
		{
			break;
		}
		ver_iter = hor_iter;
		x--;
	}

	//draw the left side
	x = radius - 1;
	ver_iter = start->left_node;
	hor_iter = start->left_node;
	if (ver_iter != NULL)
	{
		while (x >= 0)
		{
			rect.x = hor_iter->x * tileWidth + xPos;
			rect.y = hor_iter->y * tileHeight + yPos;
			draw_filled_rect(rect, color);
			for (i = 0; i < x; i++)
			{
				ver_iter = ver_iter->up_node;
				if (ver_iter == NULL)
				{
					break;
				}
				else
				{
					rect.x = ver_iter->x * tileWidth + xPos;
					rect.y = ver_iter->y * tileHeight + yPos;
					draw_filled_rect(rect, color);
				}
			}
			ver_iter = hor_iter;
			for (i = 0; i < x; i++)
			{
				ver_iter = ver_iter->down_node;
				if (ver_iter == NULL)
				{
					break;
				}
				else
				{
					rect.x = ver_iter->x * tileWidth + xPos;
					rect.y = ver_iter->y * tileHeight + yPos;
					draw_filled_rect(rect, color);
				}
			}
			hor_iter = hor_iter->left_node;
			if (hor_iter == NULL)
			{
				break;
			}
			ver_iter = hor_iter;
			x--;
		}
	}
}
