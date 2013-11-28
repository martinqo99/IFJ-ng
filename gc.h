/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   gc.h
 *
 * Popis:
 *
 *
 * Datum:    28.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#ifndef TYPE_PTR_DEFINED
#define TYPE_PTR_DEFINED
typedef void* PTR;
#endif

#ifndef GC_H_INCLUDED
#define GC_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#define GC_ITEM_MEMORY 1
#define GC_ITEM_FILE 2

typedef struct GC_ITEM{
	int type;
	PTR block;	
} GC_ITEM, *GC_ITEM_PTR;

typedef struct GC
{
    struct struct_BTree* tree;

    //Pocitadla
    long mallocs,reallocs,callocs,fopens,frees,fcloses;
    long allocated;
} GC;

void gcInit();
PTR gcMalloc(int size);
PTR gcRealloc(PTR block, int size);
PTR gcCalloc(int count, int size);
PTR gcFopen(const char* fileName, const char* mode);
void gcFree(PTR);
void gcFclose(PTR);
void gcDispose();
void gcAbort();

extern GC garbageCollector;

#endif
