/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   string.h
 *
 * Popis:
 *
 *
 * Datum:    12.9.2013
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
#include "mmu.h"
#include "errors.h"

#define SIZE 8 // zacatecni velikost pole, pri realokaci se alokuje +SIZE

//Pozustatek...
typedef unsigned int uint;

typedef struct tstring {
  char *data;
  unsigned int len;
  unsigned int alloc;
} tString;

E_CODE strInit (tString *str);
//int strCmpRaw (tString *str, const char x);
tString strCreate (const char *str);
E_CODE strFree (tString *str);
E_CODE strClear (tString *str);
E_CODE strAdd (tString *str, char x);
E_CODE strCopy (tString *str, char *array);
E_CODE strCopyString (tString *strl, tString *strr);
int strCmp (tString *strl, tString *strr);
int strCmpRaw(tString *strl, const char* strr);
char *strRaw (tString *str);
uint strLen (tString *str);
uint strSize (tString *str);

#endif