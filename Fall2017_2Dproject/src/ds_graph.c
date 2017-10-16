#include "ds_graph.h"

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

void * graph_delete(GraphNode ** nodeThatDies)
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
	//free the node
	return data;
}

int graph_clear(Graph ** graphThatDies)
{
	return 0;
}

int graph_insert(Graph ** graph, void * data, int width, size_t elementSize)
{
	return 0;
}
