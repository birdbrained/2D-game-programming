#ifndef __DS_BINARY_TREE__
#define __DS_BINARY_TREE__

#include <stdlib.h>
#include "simple_logger.h"

typedef struct bitreenode_s
{
	void * data;
	struct bitreenode_s * left_child;
	struct bitreenode_s * right_child;
	size_t elementSize;
}BinaryTreeNode;

BinaryTreeNode * bt_new_node(size_t elementSize);
int bt_insert(BinaryTreeNode ** bt, void * data, size_t elementSize);
BinaryTreeNode * bt_find(BinaryTreeNode * bt, void * data);
BinaryTreeNode * bt_find_min(BinaryTreeNode * bt);
BinaryTreeNode * bt_find_max(BinaryTreeNode * bt);

#endif // !__DS_BINARY_TREE__
