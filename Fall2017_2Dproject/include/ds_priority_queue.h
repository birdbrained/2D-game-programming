#ifndef __DS_PRIORITY_QUEUE__
#define __DS_PRIORITY_QUEUE__

/*
typedef struct node {
void * data;
int priority;
struct node * prev;
struct node * next;
}Node;

Node * pqInit(void * data, int priority);
void pqEnqueue(Node * pq, void * data, int priority);
Node * pqDequeue(Node * pq);
*/

#include <SDL.h>

/**
* @brief A priority queue data structure
*/
typedef struct priorityqueue
{
	void *data;						/**<The data the node will hold*/
	int priority;					/**<Positive or zero, negative numbers will become zero*/
	struct priorityqueue * next;	/**<The next node in the pq*/
	struct priorityqueue * prev;	/**<The previous node in the pq*/
	size_t elementSize;				/**<Size of the data*/
}PriorityQueue;

/**
* @brief Creates a new priority queue
* @param elementSize The size of the data the pq will hold
*/
PriorityQueue * pq_new(size_t elementSize);

/**
* @brief Dequeues the first (oldest) element in the pq
* @param pq_head Head node of pq (most recent in)
* @param pq_tail Tail node of pq (first in)
* @returns The data that the first-in node holds
*/
void * pq_delete(PriorityQueue ** pq_head, PriorityQueue ** pq_tail);

/**
* @brief Dequeues the element in the pq with the highest priority
* @param pq_head Head node of pq (most recent in)
* @param pq_tail Tail node of pq (first in)
* @returns The data within the highest priority node
*/
void * pq_delete_max(PriorityQueue ** pq_head, PriorityQueue ** pq_tail);

/**
* @brief Inserts a new node into the priority queue, tail points to first in, head points to most recently added
* @param pq_head The head node that will have a node inserted into it
* @param pq_tail The tail node, required if inserting first node
* @param data The data that will be inserted
* @param elementSize Size of the data
* @param priority The data's priority, larger numbers have greater priority
* @returns 0 if successful, -1 if data could not be allocated
*/
int pq_insert(PriorityQueue ** pq_head, PriorityQueue ** pq_tail, void *data, size_t elementSize, int priority);

/**
* @brief Gets the length of the Priority Queue
* @param pq_tail Pointer to the tail (first element added to pq)
* @returns Length of priority queue, 0 if no elements in queue
*/
int pq_length(PriorityQueue ** pq_tail);

#endif // !__DS_PRIORITY_QUEUE__
