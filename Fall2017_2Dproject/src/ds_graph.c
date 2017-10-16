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
	atexit(graph_clear);
	return new_graph;
}

void * graph_delete_node(GraphNode ** nodeThatDies)
{
	void * data = NULL;
	if (!(*nodeThatDies))
	{
		slog("Error: cannot delete a null graph node");
		return NULL;
	}

	if ((*nodeThatDies)->up_node != NULL)
		(*nodeThatDies)->up_node->down_node = (*nodeThatDies)->down_node;
	if ((*nodeThatDies)->right_node != NULL)
		(*nodeThatDies)->right_node->left_node = (*nodeThatDies)->left_node;
	if ((*nodeThatDies)->down_node != NULL)
		(*nodeThatDies)->down_node->up_node = (*nodeThatDies)->up_node;
	if ((*nodeThatDies)->left_node != NULL)
		(*nodeThatDies)->left_node->right_node = (*nodeThatDies)->right_node;

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
			graph_delete_node(temp);
		}
	}

	return 0;
}

int graph_insert(Graph ** graph, void * data, int width, size_t elementSize)
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

	if (!(*graph)->head)						//if the node is the first one in the graph
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

	return 0;
}
