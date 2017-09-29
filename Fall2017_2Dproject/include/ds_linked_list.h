#ifndef __DS_LINKED_LIST__
#define __DS_LINKED_LIST__

/**
* @brief Node_S LL_Node a node of a linked list
* @param data Void pointer to data the node will hold
* @param next Pointer to next node in linked list, NULL if end of linked list
*/
typedef struct Node_S
{
	void *data;
	struct Node_S *next;
}LL_Node;

/**
* @brief Allocates memory for a new node
* @returns A pointer to the newly created node, NULL if not enough memory
*/
LL_Node * linkedlist_new_node();

/**
* @brief Pushes a new element onto the linked list
* @param head The current head node
* @param void pointer to data the node will hold
* @returns 0 if successful, -1 if memory could not be allocated
*/
int linkedlist_insert(LL_Node ** head, void * data);

/**
* @brief Removes a node from the front of the linked list (most recent added), O(1)
* @param head The current head node
* @returns Pointer to the removed node
*/
LL_Node * linkedlist_remove_front(LL_Node ** head);

/**
* @brief Removes a node from the back of the linked list (first added), O(n)
* @param head The current head node
* @returns Pointer to the removed node
*/
LL_Node * linkedlist_remove_back(LL_Node ** head);

/**
* @brief Frees a null node from memory
* @param node The node to free
* @returns 0 if successful, -1 if node still has data, -2 if node is already null
*/
int linkedlist_free_node(LL_Node *node);

#endif // !__DS_LINKED_LIST__
