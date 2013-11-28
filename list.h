/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   list.h
 *
 * Popis:
 *
 *
 * Datum:    20.11.2013
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

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "mmu.h"

typedef struct LISTNODE{
    PTR value;
    struct LISTNODE* next;    
} LISTNODE, *LISTNODEPTR;

typedef struct LIST{
    LISTNODEPTR begin;
    LISTNODEPTR end;
    LISTNODEPTR curr;
} LIST, *LISTPTR;


void listInit(LISTPTR list);
void listInsertBegin(LISTPTR list, PTR value);
void listInsertEnd(LISTPTR list, PTR value);
void listInsertPost(LISTPTR list, PTR value);

void listBegin(LISTPTR list);
void listSucc(LISTPTR list);
void listEnd(LISTPTR list);

void listFree(LISTPTR list);

#endif
