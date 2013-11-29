/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   exp.h
 *
 * Popis:
 *
 *
 * Datum:    29.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#ifndef EXP_H_INCLUDED
#define EXP_H_INCLUDED

#include "errors.h"
#include "gc.h"
#include "symbol_table.h"
#include "scanner.h"
#include "stack.h"

#define EXPRESSION_TABLE_SIZE TTYPE_SEMICOLON+1

typedef struct EXPRESSION{
	enum_RetVal retval;
	SYMBOL_PTR symbol;
} EXPRESSION, *EXPRESSION_PTR;

ERROR parserExpression(SYMBOL_TABLE_PTR st, enum_RetVal retval, SYMBOL_PTR symbol);

//helpers
EXPRESSION_PTR makeExpression(enum_RetVal retval, SYMBOL_PTR symbol);
enum_RetVal getTerm(STACK_PTR stack);

#endif