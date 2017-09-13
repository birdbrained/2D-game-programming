#ifndef __DS_PRIORITY_QUEUE__
#define __DS_PRIORITY_QUEUE__

#include <SDL.h>

/**
* @brief A priority queue data structure
*/
typedef struct
{
	void *data;
	int priority;
	size_t numElements;
	size_t elementSize;
}PriorityQueue;

/**
* @brief Creates a new priority queue
* @param elementSize
*/
PriorityQueue *pq_new(size_t elementSize);
void pq_delete(PriorityQueue *pq);
void *pq_delete_max(PriorityQueue *pq);

/**
* @brief Inserts a new node into the priority queue passed into the function
* @param pq The priority queue node that will have a node inserted into it
* @param data The data that will be inserted
* @param priority The data's priority, larger numbers have greater priority
*/
void pq_insert(PriorityQueue *pq, void *data, int priority);

#endif // !__DS_PRIORITY_QUEUE__