#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include "strings.h"
#include "mmu.h"
#include "errors.h"

typedef enum mmutreeitemtype{
    MMU_UNDEFINED,
    MMU_MEMORY,
    MMU_FILE
} tMMUTreeItemType;


typedef struct struct_BTree_Node
{
    struct struct_BTree_Node* left;
	struct struct_BTree_Node* right;
	tMMUTreeItemType type;

	void *data;
	intptr_t key;
	unsigned long allocated;
} *struct_BTree_Node;


typedef struct struct_BTree
{
	struct_BTree_Node root;
	struct_BTree_Node last;
} struct_BTree;

// API
void 				BT_Init(struct_BTree*);
void 				BT_Free(struct_BTree*);
struct_BTree_Node	BT_Insert(struct_BTree*, intptr_t);
struct_BTree_Node 	BT_Search(struct_BTree*, intptr_t);

// library functions
struct_BTree_Node 	recursive_Node_Search(struct_BTree_Node, intptr_t);
void 				recursive_Node_Delete(struct_BTree_Node);


#endif
