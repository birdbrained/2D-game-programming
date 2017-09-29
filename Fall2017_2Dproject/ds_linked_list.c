#include <SDL.h>
#include "ds_linked_list.h"
#include "simple_logger.h"

LL_Node * linkedlist_new_node()
{
	LL_Node *node = NULL;
	node = (LL_Node *)malloc(sizeof(LL_Node));
	if (!node)
	{
		slog("Error: not enough memory to allocate");
		return NULL;
	}
	memset(node, 0, sizeof(LL_Node));
	return node;
}

int linkedlist_insert(LL_Node ** head, void * data)
{
	LL_Node *node = linkedlist_new_node();
	if (node == NULL)
	{
		return -1;
	}
	node->data = data;
	node->next = (*head);
	(*head) = node;
	return 0;
}

LL_Node * linkedlist_remove_front(LL_Node ** head)
{
	LL_Node * p = (*head);
	(*head) = (*head)->next;
	p->next = NULL;
	return p;
}

LL_Node * linkedlist_remove_back(LL_Node ** head)
{
	LL_Node * p = (*head);
	LL_Node * q = NULL;

	if (p && p->next)
	{
		while (p->next->next != NULL)
		{
			p = p->next;
		}
		q = p->next;
		p->next = NULL;
	}
	else
	{
		q = p;
		(*head) = NULL;
	}
	return q;
}

int linkedlist_free_node(LL_Node *node)
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
