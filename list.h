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

typedef struct tListNode{
    PTR value;
    struct tListNode* next;    
} tListNode, *tListNodePTR;

typedef struct tList{
    tListNodePTR begin;
    tListNodePTR end;
    tListNodePTR curr;
} tList, *tListPTR;


void listInit(tListPTR list);
void listInsertBegin(tListPTR list, PTR value);
void listInsertEnd(tListPTR list, PTR value);
void listInsertPost(tListPTR list, PTR value);

void listBegin(tListPTR list);
void listSucc(tListPTR list);
void listEnd(tListPTR list);

void listFree(tListPTR list);

#endif
