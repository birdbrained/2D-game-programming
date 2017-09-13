#include <SDL.h>
#include "ds_priority_queue.h"

PriorityQueue * pq_new(size_t elementSize)
{
	/*PriorityQueue *head = NULL;
	head = malloc(sizeof(PriorityQueue) + elementSize);
	head->elementSize = elementSize;
	head->numElements = 0;
	head->priority = 0;
	return head;*/

	int i;
	PriorityQueue *pq;
	pq = malloc(sizeof(PriorityQueue));
	for (i = 0; i < elementSize; i++)
	{
		pq[i].data = NULL;
		pq[i].priority = 0;
	}
}

void pq_delete(PriorityQueue *pq)
{
	return NULL;
}

void *pq_delete_max(PriorityQueue *pq)
{
	int currentMaxPriority, currentMaxIndex, i;
	currentMaxPriority = 0;
	currentMaxIndex = 0;
	i = 0;

	while (pq[i].data != NULL)
	{
		if (pq[i].priority > currentMaxPriority)
		{
			currentMaxPriority = pq[i].priority;
			currentMaxIndex = i;
		}
		i++;
	}


}

void pq_insert(PriorityQueue *pq, void *data, int priority)
{
	int i;
	i = 0;

	while (pq[i].data != NULL)
	{
		i++;
	}
	pq[i].data = data;
	pq[i].priority = priority;
}