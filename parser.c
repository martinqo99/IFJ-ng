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
	
	init_Token();
	
	return parserParse(st);	
}

ERROR parserFindFunctions(SYMBOL_TABLE_PTR st){
	printf("Finding function definitions:\n");
	enum_RetVal retval;
	
	init_Token();
	
	while((retval = get_Token()) != TTYPE_EOF){
		//printf("Token: %s\n", glob_Token.data.data);
		if(retval == TTYPE_KEYWORD && strCompare(glob_Token.data, "function")){
			retval = get_Token();
			
			if(retval == TTYPE_FUNCTION){
				printf("- %s\n", glob_Token.data.data);
				stInsertFunction(st, glob_Token.data);
			}
			else
				return E_SYNTAX;
		}
		else if(retval == TTYPE_ERROR)
			return E_SYNTAX;
	}
	
	free_Token();
	
	return E_OK;
}

ERROR parserParse(SYMBOL_TABLE_PTR st){
	ERROR err = E_OK;

	st->curr = &st->start;
	
	enum_RetVal retval = get_Token();
	
	if(retval == TTYPE_EOF)
		return err;
	else if(retval == TTYPE_KEYWORD && strCompare(glob_Token.data, "function")){
		err = parserParseFunction(st);
		
		if(err != E_OK)
			return err;
		
		return parserParse(st);
	}
	else{
		err = parserParseCode(st, retval);
		
		if(err != E_OK)
			return err;
		
		return parserParse(st);
	}
}
//function id (<params>){ <stat_list> }
ERROR parserParseFunction(SYMBOL_TABLE_PTR st){
	printf("Parsing function\n");
	ERROR err = E_OK;
	
	if(get_Token() != TTYPE_FUNCTION)
		return E_SYNTAX;
	
	st->curr = stSearchFunction(st, glob_Token.data);
	
	if(!st->curr)
		return E_COMPILATOR;
	
	printf("Current function %s\n", st->curr->id.data);
	
	if(get_Token() != TTYPE_L_BRACKET)
		return E_SYNTAX;
	
	err = parserParseFunctionParam(st);
	
	if(err != E_OK)
		return err;
		
	INSTRUCTION_PTR i = NULL;
	//INSTRUKCE
	
	listInsertEnd(&st->curr->instructions, i);
		
	if(get_Token() != TTYPE_L_BRACE)
		return E_SYNTAX;
	
	err = parserParseFunctionCode(st);
		
	if(get_Token() != TTYPE_R_BRACE)
		return E_SYNTAX;	

	return err;
}

// <params> - id <params_n>
// <params> - eps
ERROR parserParseFunctionParam(SYMBOL_TABLE_PTR st){
	printf("Parsing function param\n");
	
	enum_RetVal retval = get_Token();
	
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	else if(retval == TTYPE_VARIABLE){
		//Stejny nazev parametru a funkce - ale nemozne v PHP
		//if(stSearchFunction(st, glob_Token.data))
		//	return E_SEMANTIC;
		
		stInsertSymbol(st->curr, glob_Token.data);
			
		INSTRUCTION_PTR i;
		//INSTRUKCE
		
		return parserParseFunctionParams(st);
	}
	else
		return E_SYNTAX;
}

// <params_n> - , id <params_n>
// <params_n> - eps
ERROR parserParseFunctionParams(SYMBOL_TABLE_PTR st){
	printf("Parsing function params\n");
	
	enum_RetVal retval = get_Token();
	
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	else if(retval == TTYPE_COMMA){
		
		retval = get_Token();
		
		if(retval == TTYPE_VARIABLE){
			//Stejny nazev parametru a funkce - ale nemozne v PHP
			//if(stSearchFunction(st, glob_Token.data))
			//	return E_SEMANTIC;
			
			//Stejny nazev dvou parametru
			if(stSearchSymbol(st->curr, glob_Token.data))
 				return E_SEMANTIC;
			
			stInsertSymbol(st->curr, glob_Token.data);
			
			INSTRUCTION_PTR i;
			//INSTRUKCE
			
			return parserParseFunctionParams(st);
		}
		else
			return E_SYNTAX;
	}
	else
		return E_SYNTAX;	
}

ERROR parserParseFunctionCode(SYMBOL_TABLE_PTR st){
	printf("Parsing function code\n");
	return E_OK;
}

ERROR parserParseCode(SYMBOL_TABLE_PTR st, enum_RetVal retval){
	ERROR err = E_OK;
	
	return err;
}
