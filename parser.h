/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   parser.h
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

#include "errors.h"
#include "gc.h"
#include "symbol_table.h"
#include "scanner.h"

ERROR parser(SYMBOL_TABLE_PTR st);
ERROR parserFindFunctions(SYMBOL_TABLE_PTR st);
ERROR parserParse(SYMBOL_TABLE_PTR st);
ERROR parserParseFunction(SYMBOL_TABLE_PTR st);
ERROR parserParseFunctionParam(SYMBOL_TABLE_PTR st);
ERROR parserParseFunctionParams(SYMBOL_TABLE_PTR st);
ERROR parserParseFunctionCode(SYMBOL_TABLE_PTR st);
ERROR parserParseCode(SYMBOL_TABLE_PTR st, enum_RetVal retval);