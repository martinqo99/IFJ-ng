#include "btree.h"

void BT_Init(struct_BTree* tree)
{
	tree = gcMalloc(sizeof(struct struct_BTree));
// 	tree->root = malloc(sizeof(void*));
	tree->root = NULL;
// 	tree->last = malloc(sizeof(void*));
	tree->last = NULL;
}

void BT_Free(struct_BTree* tree)
{
	if(tree->root == NULL)
	{
		gcFree(tree);
		return;
	}
	recursive_Node_Delete(tree->root);
	gcFree(tree);
}

void recursive_Node_Delete(struct_BTree_Node node)
{
	if(node->left != NULL)
		recursive_Node_Delete(node->left);
	if(node->right != NULL)
		recursive_Node_Delete(node->right);
	gcFree(node->data);
	gcFree(node);
}


struct_BTree_Node BT_Search(struct_BTree* tree, STRING_PTR key)
{
	return recursive_Node_Search(tree->root, key);
}

struct_BTree_Node recursive_Node_Search(struct_BTree_Node node, STRING_PTR key)
{
	if(node == NULL)
        return NULL;
	else if(strCompareStrings(*node->key,*key))
		return node;
	else if (strSize(node->key) < strSize(key))
		return recursive_Node_Search(node->left, key);
	else
		return recursive_Node_Search(node->right, key);
}


struct_BTree_Node BT_Insert(struct_BTree* tree, STRING_PTR key)
{
	if (tree->root == NULL)
	{
	    tree->root = malloc(sizeof(struct struct_BTree_Node));
		tree->root->key = key;
		tree->root->left = NULL;
		tree->root->right = NULL;
		tree->last = tree->root;
		return tree->root;
	}

    struct_BTree_Node temp_node = tree->root;

	while(temp_node != NULL)
	{
        if ( key < temp_node->key)
		{
            if(temp_node->left == NULL)
			{
				temp_node->left = malloc(sizeof(struct struct_BTree_Node));
				temp_node->left->key = key;
				temp_node->left->left = NULL;
				temp_node->left->right = NULL;
				tree->last = temp_node->left;
				return temp_node;
            }
            else temp_node=temp_node->left;
        }
        else if ( key > temp_node->key)
		{
            if (temp_node->right == NULL)
			{
				temp_node->right = malloc(sizeof(struct struct_BTree_Node));
				temp_node->right->key = key;
				temp_node->right->left = NULL;
				temp_node->right->right = NULL;
				tree->last = temp_node->right;
				return temp_node;
            }
            else temp_node = temp_node->right;
        }
        else
		{
			tree->last = temp_node;
			return temp_node;
		}
    }
    return temp_node;
}
