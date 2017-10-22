#ifndef __DS_GRAPH__
#define __DS_GRAPH__

#include <stdlib.h>
#include "simple_logger.h"
#include "tilemap.h"

/**
 * @brief A data structure that represents a single node in a graph
 */
typedef struct graphnode_s
{
	void * data;						/**<The data this node will hold*/
	int x, y;							/**<The position in the graph of this node*/
	struct graphnode_s * up_node;		/**<Pointers to the surrounding nodes in the graph*/
	struct graphnode_s * right_node;
	struct graphnode_s * down_node;
	struct graphnode_s * left_node;
	size_t elementSize;					/**<The size of the data held*/
}GraphNode;

/**
 * @brief A data structure that represents an entire graph
 */
typedef struct graph_s
{
	GraphNode * head;					/**<Pointer to head node (start)*/
	GraphNode * tail;					/**<Pointer to tail node (most recently added)*/
	int width;							/**<How many nodes should be in one row*/
}Graph;

/**
 * @brief Creates one new graph node
 * @param elementSize The size of the data the node will hold
 * @returns Pointer to new node is successful; NULL if could not allocate memory
 */
GraphNode * graph_new_node(size_t elementSize);

/**
 * @brief Initializes a new graph
 * @param width How many nodes should be in one row
 * @param elementSize The size of the data each node will hold
 * @returns Pointer to new graph if successful; NULL if could not allocate memory
 */
Graph * graph_init(int width, size_t elementSize);

/**
 * @brief Deletes one node from its graph; pointers will be fixed to point to the nodes to the up, down, left, and right surrounding the node
 * @param nodeThatDies The node to delete from the graph
 * @returns The data held by the deleted node; NULL if could not delete node
 */
void * graph_delete_node(GraphNode ** nodeThatDies);

/**
 * @brief Completely deletes a graph
 * @param graphThatDies The graph to brutally murder from memory
 * @returns 0 if successful; -1 if error
 */
int graph_clear(Graph ** graphThatDies);

/**
 * @brief Inserts a new node into the requested graph
 * @param graph The graph to insert into
 * @param data The data the new node will hold
 * @param width How many nodes a graph should have in one row
 * @param elementSize The size of the data to hold
 * @returns 0 if successful; -1 if could not allocate memory for a new node; -2 if graph is null pointer
 */
int graph_insert(Graph ** graph, void * data, int width, size_t elementSize, int firstInsert);

/**
 * @brief Updates the value of a node in an already exisiting graph
 * @param graph The graph that will have a node updated
 * @param x X value of node to update
 * @param y Y value of node to update
 * @param data The data that the node will hold
 * @param elementSize Size of data to hold
 * @returns 0 if successful; -1 if graph is NULL; -2 if requested node could not be found; -3 if error finding graph's head
 */
int graph_update_node(Graph ** graph, int x, int y, void * data, size_t elementSize);

/**
 * @brief Sets all nodes' data to zero, but does not delete the graph or any nodes
 * @param graph The graph to zero out
 * @returns 0 if successful; -1 if graph is NULL
 */
int graph_zero_all(Graph ** graph);

/**
 * @brief Initializes a graph from a given tilemap
 * @param map The TileMap to reference
 * @param elementSize Size of data each node will hold
 * @returns Pointer to new Graph if successful; NULL if error allocating memory or if map was NULL
 */
Graph * graph_init_from_tilemap(TileMap * map, size_t elementSize);

/**
 * @brief Iterates through a graph and prints out each nodes' position and data (as int)
 * @param graph The graph to traverse
 */
void graph_print(Graph * graph);


#endif // !__DS_GRAPH__
