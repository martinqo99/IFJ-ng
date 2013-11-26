#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include "strings.h"
#include "mmu.h"
#include "errors.h"


typedef struct
{
	struct struct_BTree_Node *left;
	struct struct_BTree_Node *right;
	
	void *data;
	STRING *key;
	//int height;
} struct_BTree_Node;


typedef struct
{
	struct_BTree_Node *root;
	struct_BTree_Node *last;
} struct_BTree;

// API
void 				BT_Init(struct_BTree*);
void 				BT_Free(struct_BTree*);
ERROR 				BT_Insert(struct_BTree*, STRING*, void*);
struct_BTree_Node 	BT_Search(struct_BTree*, STRING*);

// library functions
struct_BTree_Node 	recursive_Node_Search(struct_BTree_Node, STRING*);
void 				recursive_Node_Delete(struct_BTree_Node*);


//void BT_Delete_Nodes(struct_BTree*);
//insert, search, 

#endif
