#ifndef MMU_H_INCLUDED
#define MMU_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <assert.h>

#include "btree.h"
#include "strings.h"
#include "errors.h"

#define MMU_SIZE 32

typedef struct mmu
{
    struct struct_BTree* tree;

    //Pocitadla
    unsigned long mallocs;
    unsigned long reallocs;
    unsigned long callocs;
    unsigned long fopens;
    unsigned long frees;
    unsigned long fcloses;
    unsigned long allocated;
} btMMU;

extern btMMU mmuTable;

//API pro pouzivani MMU
void mmuTreeInit();
void* mmuMalloc(size_t);
void* mmuRealloc(void*, size_t);
void* mmuCalloc(size_t, size_t);
void* mmuFopen(const char* fileName, const char* mode);
void mmuFree(void*);
void mmuFclose(void*);
void mmuGlobalFree();


#endif
