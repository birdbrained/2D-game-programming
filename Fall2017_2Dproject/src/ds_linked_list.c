#include "ds_linked_list.h"
#include <stdlib.h>

IntNode * IntNode_init(int i)
{
	IntNode * head = NULL;
	head = malloc(sizeof(IntNode));
	head->data = i;
	head->next = NULL;
	return head;
}