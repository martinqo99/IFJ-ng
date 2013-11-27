#include "mmu.h"

btMMU mmuTree;

void recursive_mmuGlobalFree(struct_BTree_Node node);

void mmuTreeInit()
{
    signal(SIGINT, mmuGlobalFree);
    signal(SIGABRT, mmuGlobalFree);

    //Inicializace pametovych pocitatel
    mmuTree.mallocs = 0;
    mmuTree.reallocs = 0;
    mmuTree.callocs = 0;
    mmuTree.fopens = 0;
    mmuTree.frees = 0;
    mmuTree.fcloses = 0;
    mmuTree.allocated = 0;
    
	mmuTree.tree = malloc(sizeof(void*));
    BT_Init(mmuTree.tree);
}

void* mmuMalloc(size_t size)
{	
    mmuTree.mallocs++;
    mmuTree.allocated += size;

    void* newPtr = malloc(size);
	
    if (newPtr == NULL)
		exit (E_COMPILATOR);

    assert(newPtr);

    struct_BTree_Node item = BT_Insert(mmuTree.tree,(intptr_t)newPtr);

    assert(item);

    item->data = newPtr;
    item->allocated = size;
    item->type = MMU_MEMORY;

    return newPtr;
}

void* mmuRealloc(void* ptr, size_t size)
{

    mmuTree.reallocs++;
    mmuTree.allocated += size;

    void* newPtr = realloc(ptr, size);

    if (newPtr == NULL)
		exit (E_COMPILATOR);

    assert(newPtr);

    struct_BTree_Node item = NULL;

    if(newPtr != ptr)
	{
        item = BT_Insert(mmuTree.tree,(intptr_t)ptr);
        
		assert(item);
		
        item->allocated = 0;
        item->data = NULL;
    }

    item = BT_Insert(mmuTree.tree,(intptr_t)ptr);

    assert(item);

    item->data = newPtr;
    item->allocated = size;
    item->type = MMU_MEMORY;

    return newPtr;
}

void* mmuCalloc(size_t num, size_t size)
{
    mmuTree.callocs++;
    mmuTree.allocated += size;

    void* newPtr = calloc(num, size);

    if (newPtr == NULL)
		exit (E_COMPILATOR);

    assert(newPtr);

    struct_BTree_Node item = BT_Insert(mmuTree.tree,(intptr_t)newPtr);

    assert(item);

    item->data = newPtr;
    item->allocated = size;
    item->type = MMU_MEMORY;

    return newPtr;
}

void* mmuFopen(const char* fileName, const char* mode)
{
    mmuTree.fopens++;
    mmuTree.allocated += sizeof(FILE*);

    void* newPtr = (FILE*)fopen(fileName, mode);

    if(!newPtr)
        return NULL;

    struct_BTree_Node item = BT_Insert(mmuTree.tree,(intptr_t)newPtr);

    assert(item);

    item->data = newPtr;
    item->allocated = sizeof(FILE*);
    item->type = MMU_FILE;

    return newPtr;
}

void mmuFree(void* ptr)
{
    mmuTree.frees++;

    if(!ptr)
        return;

    struct_BTree_Node item = BT_Search(mmuTree.tree,(intptr_t)ptr);

    if(!item)
        return;

    if(item->allocated != 0)
	{
        free(ptr);
        item->allocated = 0;
        item->data = NULL;
    }
}

void mmuFclose(void* ptr)
{
    mmuTree.fcloses++;

    if(!ptr)
        return;

    struct_BTree_Node item = BT_Search(mmuTree.tree,(intptr_t)ptr);

    if(!item)
        return;

    if(item->allocated != 0){
        fclose(ptr);
        item->allocated = 0;
        item->data = NULL;
    }

}

void mmuGlobalFree()
{
//     recursive_mmuGlobalFree(mmuTree.tree->root);
	BT_Free(mmuTree.tree);
// 	free(mmuTree.tree);
}

void recursive_mmuGlobalFree(struct_BTree_Node node)
{
	if(node->left != NULL)
		recursive_Node_Delete(node->left);
	if(node->right != NULL)
		recursive_Node_Delete(node->right);
	
	mmuFree(node);
}

