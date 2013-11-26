#include "btree.h"

void BT_Init(struct_BTree* tree)
{
	tree->root = NULL;
}

void BT_Free(struct_BTree* tree)
{
	recursive_Node_Delete(tree->root);
	BT_Init(tree);
}

void recursive_Node_Delete(struct_BTree_Node* node)
{
	if(node->left != NULL)
		recursive_Node_Delete(node->left);
	if(node->right != NULL)
		recursive_Node_Delete(node->right);
	mmuFree(node);
}


struct_BTree_Node BT_Search(struct_BTree* tree, STRING* key)
{
	return recursive_Node_Search(tree->root, key);
}

struct_BTree_Node recursive_Node_Search(struct_BTree_Node node, STRING* key)
{
	if(node == NULL)
		return NULL;
	else if(strCompare(node->key, key))
		return node;
	else if(strCompare(node->key, key) < 0)
		return recursive_Node_Search(node->left, key);
	else
		return recursive_Node_Search(node->right, key);
}


ERROR BT_Insert(struct_BTree* tree, STRING* key, void* data)
{
	if (tree->root == NULL)
	{
	    tree->root = mmuMalloc(sizeof(struct struct_BTree_Node));
        tree->root->key = key;
        tree->root->data = data;
        tree->root->left = NULL;
		tree->root->right = NULL;
        //tree->root->height = 0;
        tree->last = tree->root;
        return ERROR_OK;
	}
	
	
    struct_BTree_Node temp_tree = tree->root;

	while(temp_tree != NULL)
	{
        if (strCompare(key, temp_tree->key) < 0)
		{
            if(temp_tree->left == NULL)
			{
				temp_tree->left = mmuMalloc(sizeof(struct struct_BTree_Node));
				temp_tree->left->key = key;
				temp_tree->left->data = data;
				temp_tree->left->left = NULL;
				temp_tree->left->right = NULL;
				//temp_tree->left->height = (temp_tree->height) + 1;
				tree->last = temp_tree->left;
				return ERROR_OK;
            }
            else temp_tree=temp_tree->left;
        }
        else if (strCompare(key, temp_tree->key) > 0)
		{
            if (temp_tree->right == NULL)
			{
				temp_tree->right = mmuMalloc(sizeof(struct struct_BTree_Node));
				temp_tree->right->key = key;
				temp_tree->right->data = data;
				temp_tree->right->left = NULL;
				temp_tree->right->right = NULL;
				//temp_tree->right->height = (temp_tree->height) + 1;
				tree->last = temp_tree->right;
				return ERROR_OK;
            }
            else temp_tree = temp_tree->right;
        }
        else
		{ 
			tree->last = temp_tree;
			return ERROR_INS_EXIST; 
		}
    }
    return ERROR_OK;
}
