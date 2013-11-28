/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   errors.c
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

typedef enum error{
	E_OK					= 0,
	E_INTERPRET			= 1,
	E_COMPILATOR			= 99	
} ERROR;

void ChuckSaidNo(ERROR ecode);

#endif