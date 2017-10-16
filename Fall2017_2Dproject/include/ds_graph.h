#ifndef __DS_GRAPH__
#define __DS_GRAPH__

#include <stdlib.h>
#include "simple_logger.h"
#include "ds_priority_queue.h"

/**
 * @brief A data structure that represents a single node in a graph
 */
typedef struct graphnode_s
{
	void * data;
	int x, y;
	struct graphnode_s * up_node;
	struct graphnode_s * right_node;
	struct graphnode_s * down_node;
	struct graphnode_s * left_node;
	size_t elementSize;
}GraphNode;

/**
 * @brief A data structure that represents an entire graph
 */
typedef struct graph_s
{
	GraphNode * head;
	GraphNode * tail;
	int width;
}Graph;

GraphNode * graph_new_node(size_t elementSize);
Graph * graph_init(int width, size_t elementSize);
void * graph_delete(GraphNode ** nodeThatDies);
int graph_clear(Graph ** graphThatDies);
int graph_insert(Graph ** graph, void * data, int width, size_t elementSize);


#endif // !__DS_GRAPH__
