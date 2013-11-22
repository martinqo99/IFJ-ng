/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   strings.h
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

#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_STEP 8

typedef struct string{
	char* data;
	unsigned int size;
	unsigned int used;
} STRING, *STRINGPTR;

void strInit(STRING*);
void strInitRaw(STRING*, char*);
void strInitString(STRING*, STRING*);

void strAppend(STRING*, char);
void strClear(STRING*);
void strFree(STRING*);

int strCompare(STRING, char*);

unsigned int strSize(STRING*);
unsigned int strUsed(STRING*);



#endif