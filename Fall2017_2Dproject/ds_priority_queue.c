/*
#include <SDL.h>
#include "ds_priority_queue.h"

Node * head = NULL;

Node * pqInit(void * data, int priority)
{
Node * n = NULL;
n = malloc(sizeof(Node));
n->data = data;
n->priority = priority;
n->prev = NULL;
n->next = NULL;
return n;
}

void pqEnqueue(Node * pq, void * data, int priority)
{
int i = 0;
Node *newNode = pqInit(data, priority);

while (pq[i].next != NULL)
{
i++;
}

newNode->prev = &pq[i];
pq[i].next = newNode;
}

Node * pqDequeue(Node * pq)
{
Node * n;
int currMaxPriority = -1;
int i = 0;

while (pq[i] != NULL)
{

}

return NULL;
}
*/

#include <SDL.h>
#include "ds_priority_queue.h"
#include "simple_logger.h"

/**
* @brief Called by pq_delete, frees memory associated with a dataless node
* @param node The node to free
* @returns -2 if node is NULL, -1 if data still in node, 0 if successful
*/
int pq_free_node(PriorityQueue * node)
{
	if (!node)
	{
		slog("Error: trying to free node that is already NULL");
		return -2;
	}
	if (node->data != NULL)
	{
		slog("Error: data still present in node");
		return -1;
	}
	free(node);
	return 0;
}

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
	pq = (PriorityQueue *)malloc(sizeof(PriorityQueue) + elementSize);
	if (pq == NULL)
	{
		slog("Error: could not allocate memory for new pq node");
		return NULL;
	}
	memset(pq, 0, sizeof(PriorityQueue) + elementSize);
	pq->elementSize = elementSize;
	return pq;
}

void * pq_delete(PriorityQueue ** pq_head, PriorityQueue ** pq_tail)
{
	PriorityQueue * temp = NULL;
	void * endData = NULL;

	if ((*pq_head) == NULL)
	{
		slog("Error: cannot delete from an empty priority queue!");
		return NULL;
	}
	else if ((*pq_head) == (*pq_tail))
	{
		temp = (*pq_tail);
		(*pq_head) = NULL;
		(*pq_tail) = NULL;
	}
	else
	{
		temp = (*pq_tail);
		(*pq_tail) = (*pq_tail)->next;
		(*pq_tail)->prev = NULL;
		temp->next = NULL;
	}

	endData = temp->data;
	temp->data = NULL;
	pq_free_node(temp);
	return endData;
}

void * pq_delete_max(PriorityQueue ** pq_head, PriorityQueue ** pq_tail)
{
	PriorityQueue * maxPriorityNode = NULL;
	int currMaxPriority = -1;
	PriorityQueue * temp = (*pq_tail);
	void * endData = NULL;

	if ((*pq_head) == NULL)
	{
		slog("Error: cannot delete max from an empty priority queue!");
		return NULL;
	}
	else if ((*pq_head) == (*pq_tail))
	{
		maxPriorityNode = (*pq_tail);
		(*pq_head) = NULL;
		(*pq_tail) = NULL;
	}
	else
	{
		while (temp != NULL)
		{
			if (temp->priority > currMaxPriority)
			{
				currMaxPriority = temp->priority;
				maxPriorityNode = temp;
			}
			temp = temp->next;
		}
		if (maxPriorityNode == (*pq_tail))
		{
			maxPriorityNode->next->prev = NULL;
			(*pq_tail) = maxPriorityNode->next;
			maxPriorityNode->next = NULL;
		}
		else if (maxPriorityNode == (*pq_head))
		{
			maxPriorityNode->prev->next = NULL;
			(*pq_head) = maxPriorityNode->prev;
			maxPriorityNode->prev = NULL;

		}
		else
		{
			maxPriorityNode->prev->next = maxPriorityNode->next;
			maxPriorityNode->next->prev = maxPriorityNode->prev;
			maxPriorityNode->next = NULL;
			maxPriorityNode->prev = NULL;
		}
	}

	endData = maxPriorityNode->data;
	maxPriorityNode->data = NULL;
	pq_free_node(maxPriorityNode);
	return endData;
}

int pq_insert(PriorityQueue ** pq_head, PriorityQueue ** pq_tail, void *data, size_t elementSize, int priority)
{
	PriorityQueue * n = pq_new(elementSize);

	if (n == NULL)
	{
		return -1;
	}
	n->data = data;
	n->elementSize = elementSize;
	if (priority < 0)
	{
		priority = 0;
	}
	n->priority = priority;

	//If the priority queue doesn't have anything in it yet
	if ((*pq_head) == NULL)
	{
		(*pq_head) = n;
		(*pq_tail) = n;
	}
	//But if it already does, enqueue something and change the head pointer to new node
	else
	{
		n->prev = (*pq_head);
		(*pq_head)->next = n;
		(*pq_head) = n;
	}
	return 0;
}

int pq_length(PriorityQueue ** pq_tail)
{
	int length = 0;
	PriorityQueue *iterator = (*pq_tail);
	while (iterator != NULL)
	{
		length++;
		iterator = iterator->next;
	}
	return length;
}
