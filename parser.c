/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   parser.c
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

#include "parser.h"

/* LEX
enum_RetVal - kw
//typedef struct struct_Token
{
	unsigned int row;
	unsigned int column;

	STRING data;
} struct_Token;

extern struct_Token glob_Token;

void init_Token();
void clear_Token();

enum_RetVal get_Token();
enum_RetVal get_Next_Token();
*/

ERROR parser(SYMBOL_TABLE_PTR st){
	ERROR err;	
	
	err = parserFindFunctions(st);
	
	if(err != E_OK)
		return err;
	
}

ERROR parserFindFunctions(SYMBOL_TABLE_PTR st){
	enum_RetVal retval;
	
	init_Token();
	
	while((retval = get_Token()) != TTYPE_EOF){
		//printf("Token: %s\n", glob_Token.data.data);
		if(retval == TTYPE_KEYWORD && strCompare(glob_Token.data, "function")){
			retval = get_Token();
			
			if(retval == TTYPE_FUNCTION){
				printf("Funkce: %s\n", glob_Token.data.data);
				stInsertFunction(st, glob_Token.data);
			}
			else
				return E_SYNTAX;
		}
		else if(retval == TTYPE_ERROR)
			return E_SYNTAX;
	}
	
	return E_OK;
}