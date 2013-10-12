/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   errors.h
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

#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#include <stdlib.h>

typedef enum e_code{
    ERROR_OK                			= 0,
    ERROR_LEX               			= 1,
    ERROR_SYNTAX            			= 2,
    ERROR_SEMANTIC_FUNCTION_UNDEFINED	= 3,
    ERROR_SEMANTIC_FUNCTION_ARGUMENT	= 4,
    ERROR_SEMANTIC_UNDECLARED_VARIABLE	= 5,
    ERROR_ZERO_DIVISION     			= 10,
    ERROR_NUMERIC_CONVERSION			= 11,
	ERROR_INCOMPATIBLE_TYPE				= 12,
    ERROR_RUNTIME           = 13,
    ERROR_COMPILATOR        = 99,

} E_CODE;

extern E_CODE gErrorCode;

void programAbort(E_CODE);

#endif