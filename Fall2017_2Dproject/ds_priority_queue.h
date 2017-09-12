#ifndef __DS_PRIORITY_QUEUE__
#define __DS_PRIORITY_QUEUE__

#include <SDL.h>

/**
 * @brief A priority queue data structure
 */
typedef struct
{
	void *data;
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
void pq_insert(PriorityQueue *pq, void *data, int priority);

#endif // !__DS_PRIORITY_QUEUE__