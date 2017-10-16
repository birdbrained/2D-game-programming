#include "ds_binary_tree.h"

BinaryTreeNode * bt_new_node(size_t elementSize)
{
	BinaryTreeNode * bi_node;
	bi_node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode) + elementSize);
	if (!bi_node)
	{
		slog("Error: node is not bi, it is null");
		return NULL;
	}
	memset(bi_node, 0, sizeof(BinaryTreeNode) + elementSize);
	bi_node->elementSize = elementSize;
	return bi_node;
}

int bt_insert(BinaryTreeNode ** bt, void * data, size_t elementSize)
{
	if (!(*bt))
	{
		(*bt) = bt_new_node(elementSize);
		if (!(*bt))
		{
			return -1;
		}
		(*bt)->data = data;
		(*bt)->elementSize = elementSize;
		(*bt)->left_child = NULL;
		(*bt)->right_child = NULL;
	}
	else if (data <= (*bt)->data)
	{
		bt_insert(&(*bt)->left_child, data, elementSize);
	}
	else if (data > (*bt)->data)
	{
		bt_insert(&(*bt)->right_child, data, elementSize);
	}
}

BinaryTreeNode * bt_find(BinaryTreeNode * bt, void * data)
{
	if (bt != NULL)
	{
		if (bt->data == data)
		{
			return bt;
		}
		else if (bt->data < data)
		{
			return bt_find(bt->left_child, data);
		}
		else
		{
			return bt_find(bt->right_child, data);
		}
	}
	return 0;
}

BinaryTreeNode * bt_find_min(BinaryTreeNode * bt)
{
	BinaryTreeNode * iter = bt;

	while (iter->left_child != NULL)
		iter = iter->left_child;

	return iter;
}

BinaryTreeNode * bt_find_max(BinaryTreeNode * bt)
{
	BinaryTreeNode * iter = bt;

	while (iter->right_child != NULL)
		iter = iter->right_child;

	return iter;
}
