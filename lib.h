/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: a/1/I
 * Soubor:   lib.h
 *
 * Popis:
 *
 *
 * Datum:    21.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "strings.h"
#include "symbol_table.h"

/*

typedef struct ITEM{
    TYPE type;
    VALUE value;
} ITEM, *ITEMPTR;

*/

/*

typedef union VALUE{
    bool valBool;
    int valInt;
    double valDouble;
    STRING valString;    
} VALUE;

typedef enum TYPE{
	TYPE_NULL,
    TYPE_BOOL,
    TYPE_DIGIT_INT,
	TYPE_DIGIT_DOUBLE,
    TYPE_STRING,
    TYPE_CALLABLE,
    TYPE_OTHER
} TYPE;

*/

// ChuckSaid(E_LEXICAL, "Neplatny lexem");

ITEMPTR boolval(ITEM);
ITEMPTR doubleval(ITEM);
ITEMPTR intval(ITEM);
ITEMPTR strval(ITEM);

STRING get_string();
int put_string(int count,  ...);
int my_strlen(STRING);
STRING get_substring(STRING, int, int, ERROR*);



#endif