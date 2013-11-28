/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   symbol_table.h
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

#ifndef SYMBOL_TABLE_H_INCLUDED
#define SYMBOL_TABLE_H_INCLUDED

#include <stdbool.h>

#include "strings.h"
#include "list.h"
#include "btree.h"

typedef union VALUE{
    bool valBool;
    int valInt;
    double valDouble;
    STRING valString;    
} VALUE;

typedef enum TYPE{
    TYPE_BOOL,
    TYPE_DIGIT,
    TYPE_STRING,
    TYPE_CALLABLE,
    TYPE_OTHER
} TYPE;

typedef enum INSTRUCTION_TYPE{
    INSTRUCTION_RETURN,
    //Tuxi dopln si instrukce, ktere potrebujes
} INSTRUCTION_TYPE;

typedef struct ITEM{
    TYPE type;
    VALUE value;
} ITEM, *ITEMPTR;

typedef struct SYMBOL{
    STRING id;
    TYPE type;    
    ITEMPTR items;    
} SYMBOL;

typedef struct INSTRUCTION{
    INSTRUCTION_TYPE type;
    PTR destionation;
    PTR operand1;
    PTR operand2;
} INSTRUCTION;

typedef struct FUNCTION{
    STRING id;
    
    struct_BTree symbols;
    LIST instructions;
    LIST staticValues;
    
    int calledCount;
    bool defined;
} FUNCTION, *FUNCTIONPTR;

typedef struct SYMBOL_TABLE{
    struct_BTree functions;
    
    FUNCTION start;
    FUNCTIONPTR curr;    
} SYMBOL_TABLE;


// ---------- PREPSAT!!!! --------
typedef enum {
    I_RETURN,
//stack
    I_POP,
    I_PUSH,
    I_SEMPTY,
//presun
    I_MOV,
//aritmetika
    I_ADD, //= LEX_ADDITION,
    I_SUB, //= LEX_SUBSTRACTION,
    I_MUL, //= LEX_MULTIPLICATION,
    I_DIV, //= LEX_DIVISION,
    I_POW, //= LEX_POWER,
//logika
    I_EQUAL, //= LEX_EQUAL,		// ==
    I_NEQUAL, //= LEX_UNEQUAL,	// !=
    I_LESS, //= LEX_LESSER, //<
    I_ELESS, //= LEX_LESSER_EQUAL,		// <=
    I_MORE, //= LEX_GREATER,		// >
    I_EMORE, //= LEX_GREATER_EQUAL,		// >=
//skoky - snad nebudou potreba vsechny
    I_LABEL,		// navesti
    I_FJUMP,		//false jump
    I_JUMP,		//nepodmineny jump

    I_CALL,
//vestavene fce
    I_INPUT,
    I_NUMERIC,
    I_PRINT,
    I_TYPEOF,
    I_LEN,
    I_FIND,
    I_SORT,
    I_STRING, // dest od do, string je na stacku
    NOINSTR
}tItype;
// ---------- PREPSAT!!!! --------

#endif
