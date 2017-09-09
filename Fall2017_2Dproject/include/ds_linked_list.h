#ifndef __DS_LINKED_LIST__
#define __DS_LINKED_LIST__

/**
 * @brief A single node that holds and int and a pointer to the next IntNode
 * @param data The data the node holds
 * @param next Pointer to the next IntNode
 */
struct intnode{
	int data;
	struct intnode * next;
};
typedef struct intnode IntNode;

/**
 * @brief A single node that holds a float and a pointer to the next FloatNode
 * @param data The data the node holds
 * @param next Pointer to the next FloatNode
 */
struct floatnode{
	float data;
	struct floatnode * next;
};
typedef struct floatnode FloatNode;

/**
 * @brief Initializes an IntNode with a given int
 * @param i The int required it construct an IntNode
 */
IntNode * IntNode_init(int i);

#endif // !__DS_LINKED_LIST__
