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

enum_RetVal getToken();
enum_RetVal getNextToken();
*/

//Hlavni funkce parseru
ERROR parser(SYMBOL_TABLE_PTR st){
	ERROR err;	
	
	if(getToken() != TTYPE_PHP_START)
		return E_SYNTAX;
	
	err = parserFindFunctions(st);
	
	if(err != E_OK)
		return err;
	
	init_Token();
	
	if(getToken() != TTYPE_PHP_START)
		return E_SYNTAX;
	
	return parserParse(st);	
}

//Vyhleda definice funkci
ERROR parserFindFunctions(SYMBOL_TABLE_PTR st){
	printf("Finding function definitions:\n");
	enum_RetVal retval;
	
	init_Token();

	while((retval = getToken()) != TTYPE_EOF){
		//printf("Token: %s\n", glob_Token.data.data);
		if(retval == TTYPE_KEYWORD && strCompare(glob_Token.data, "function")){
			retval = getToken();
			
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

//Funkce pro druhe parsovani
ERROR parserParse(SYMBOL_TABLE_PTR st){
	ERROR err = E_OK;

	st->curr = &st->start;
	
	enum_RetVal retval = getToken();
	
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

//Parsovani definice funkce
//function id (<params>){ <stat_list> }
ERROR parserParseFunction(SYMBOL_TABLE_PTR st){
	printf("Parsing definition function\n");
	ERROR err = E_OK;
	
	if(getToken() != TTYPE_FUNCTION)
		return E_SYNTAX;
	
	st->curr = stSearchFunction(st, glob_Token.data);
	
	if(!st->curr)
		return E_COMPILATOR;
	
	printf("Change current function to %s\n", st->curr->id.data);
	
	if(getToken() != TTYPE_L_BRACKET)
		return E_SYNTAX;
	
	err = parserParseFunctionParam(st);
	
	if(err != E_OK)
		return err;
		
	listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_NOP, NULL, NULL, NULL));

	if(getToken() != TTYPE_L_BRACE)
		return E_SYNTAX;
	
	err = parserParseFunctionCode(st);


	return err;
}

//Parsovani parametru definice funkce (0-1)
// <params> - id <params_n>
// <params> - eps
ERROR parserParseFunctionParam(SYMBOL_TABLE_PTR st){
	printf("Parsing function definition param\n");
	
	enum_RetVal retval = getToken();
	
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	else if(retval == TTYPE_VARIABLE){
		//Stejny nazev parametru a funkce - ale nemozne v PHP
		//if(stSearchFunction(st, glob_Token.data))
		//	return E_SEMANTIC;
		
		stInsertSymbol(st->curr, glob_Token.data);
			
		listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_POP, NULL, NULL, NULL));
		
		return parserParseFunctionParams(st);
	}
	else
		return E_SYNTAX;
}

//Parsovani parametru definice funkce (2+)
// <params_n> - , id <params_n>
// <params_n> - eps
ERROR parserParseFunctionParams(SYMBOL_TABLE_PTR st){
	printf("Parsing function definition params\n");
	
	enum_RetVal retval = getToken();
	
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	else if(retval == TTYPE_COMMA){
		
		retval = getToken();
		
		if(retval == TTYPE_VARIABLE){
			//Stejny nazev parametru a funkce - ale nemozne v PHP
			//if(stSearchFunction(st, glob_Token.data))
			//	return E_SEMANTIC;
			
			//Stejny nazev dvou parametru
			if(stSearchSymbol(st->curr, glob_Token.data))
 				return E_SEMANTIC;
			
			stInsertSymbol(st->curr, glob_Token.data);
			
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_POP, NULL, NULL, NULL));
			
			return parserParseFunctionParams(st);
		}
		else
			return E_SYNTAX;
	}
	else
		return E_SYNTAX;	
}
//Parsovani tela definice funkce
// <stat_list> - eps
// <stat_list> - <command> <stat_list>
ERROR parserParseFunctionCode(SYMBOL_TABLE_PTR st){
	printf("Parsing function code\n");
	
	enum_RetVal retval = getToken();
	
	if(retval == TTYPE_R_BRACE)
		return E_OK;
	//else if(retval == TTYPE_ELSE) // why?
	//	return E_SYNTAX;
	else{
		printf("Parsing function body\n");
		ERROR err = parserParseCode(st, retval);
		
		if(err == E_OK)
			return parserParseFunctionCode(st);
		else
			return err;
	}
}
//Parsovani prikazu
ERROR parserParseCode(SYMBOL_TABLE_PTR st, enum_RetVal retval){
	printf("Parsing code [%d]: %s\n", retval, glob_Token.data.data);
	
	ERROR err = E_OK;	
	//INSTRUCTION_PTR operator1, operator2, i;
	SYMBOL_PTR symbol;
	
	switch(retval){
		case TTYPE_VARIABLE:
			printf("Found variable: %s\n", glob_Token.data.data);
			if(!stSearchSymbol(st->curr, glob_Token.data))
				stInsertSymbol(st->curr, glob_Token.data);

			//Stejny nazev parametru a funkce - ale nemozne v PHP
			//if(!stSearchFunction(st, glob_Token.data))
			//	return E_SEMANTIC;
				
			retval = getToken();
			
			//Prazdny prikaz
			if(retval == TTYPE_SEMICOLON)
				return E_OK;
			else if(retval != TTYPE_ASSIGN)
				return E_SYNTAX;
			else{
				printf("Found assign and last symbol was: %s\n", stGetLastSymbol(st->curr)->id.data);
				err = parserControlAssign(st, stGetLastSymbol(st->curr));
				
				if(err != E_OK)
					return err;
			}
			
			break;
		case TTYPE_FUNCTION:
			printf("Found function: %s\n", glob_Token.data.data);
			break;
		case TTYPE_KEYWORD:
			printf("Found keyword: %s\n", glob_Token.data.data);
			if(strCompare(glob_Token.data, "if")){
				retval = getToken();
			}
			//while(<expression>){ <stat_list> }
			else if(strCompare(glob_Token.data, "while")){
				retval = getToken();
			}
			//return expression;
			else if(strCompare(glob_Token.data, "return")){
				retval = getToken();
				
				//EDIT
				err = parserExpression(st, retval, &symbol);
				
				if(err != E_OK)
					return err;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_PUSH, symbol, NULL, NULL));
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_RETURN, NULL, NULL, NULL));
				
				//if(getToken() != TTYPE_SEMICOLON)
				//	return E_SYNTAX;
			}
			else
				return E_SYNTAX;
			break;
		default:
			printf("[debug] SYNTAX ERROR: [%s] %s\n", debugRetval(retval), glob_Token.data.data);
			return E_SYNTAX;
	}
	
	return err;
}

// <assign> - expression
// <assign> - id(<params>)
// <assign> - boolval(<term>)
// <assign> - doubleval(<term>)
// <assign> - intval(<term>)
// <assign> - strval(<term>)
// <assign> - get_string()
// <assign> - put_string(<term>)
// <assign> - strlen(<term>)
// <assign> - get_substring(<term>,<num>,<num>)
// <assign> - find_string(string, string)
// <assign> - sort_string(string)
ERROR parserControlAssign(SYMBOL_TABLE_PTR st, SYMBOL_PTR symbol){
	printf("Control assign\n");
	ERROR err = E_OK;
	
	enum_RetVal retval = getToken();
	INSTRUCTION_PTR i = NULL;
	FUNCTION_PTR f = NULL;
	
	switch(retval){
		case TTYPE_RESERVED:
			printf(" - assign reserved (NOT IMPLEMENTED YET)\n");
			
			if(strCompare(glob_Token.data, "boolval")){
				printf(" - assign lib boolval\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_BOOLVAL, symbol, NULL, NULL));
			}
			else if(strCompare(glob_Token.data, "doubleval")){
				printf(" - assign lib boolval\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_BOOLVAL, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "intval")){
				printf(" - assign lib intval\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_INTVAL, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "strval")){
				printf(" - assign lib strval\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_STRVAL, symbol, NULL, NULL));				
			}			
			else if(strCompare(glob_Token.data, "get_string")){
				printf(" - assign lib get_string\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_GET_STRING, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "put_string")){
				printf(" - assign lib put_string\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_PUT_STRING, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "strlen")){
				printf(" - assign lib strlen\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_STRLEN, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "get_substring")){
				printf(" - assign lib get_substring\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_GET_SUBSTRING, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "find_string")){
				printf(" - assign lib find_string\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_FIND_STRING, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "sort_string")){
				printf(" - assign lib sort_string\n");
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_SORT_STRING, symbol, NULL, NULL));				
			}
			else
				return E_SYNTAX;
			break;
		// variable + 5
		case TTYPE_VARIABLE:
			printf(" - assign variable\n");
			
			SYMBOL_PTR tmp = NULL;
			err = parserExpression(st, retval, &symbol);

			if(err != E_OK)
				return err;
			
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_MOV, symbol, tmp, NULL));
			
			printf(" - assign variable completed\n");
			break;
		// funkce(x)
		case TTYPE_FUNCTION:
			printf(" - assign function\n");
			
			if(!(f = stSearchFunction(st, glob_Token.data)))
				return E_SEMANTIC_UNDECLARED;
			
			if(getToken() != TTYPE_L_BRACKET)
				return E_SYNTAX;
			
			err = parserParseCallParam(st);
			
			if(err != E_OK)
				return err;
				
			if(getToken() != TTYPE_SEMICOLON)
				return E_SYNTAX;			
			
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_CALL, f, NULL, NULL));
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_POP, symbol, NULL, NULL));
			
			printf(" - assign function completed\n");
			break;
		case TTYPE_STRING:
			printf(" - assign string\n");
			break;
		default:
			printf(" - assign default: %s\n", glob_Token.data.data);
			break;
	}	
	
	return err;	
}

ERROR parserParseCallParam(SYMBOL_TABLE_PTR st){	
	enum_RetVal retval = getToken();
	SYMBOL_PTR symbol = NULL;
	
	//Bez parametru
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	
	listEnd(&st->curr->instructions);
	
	if(retval == TTYPE_VARIABLE){
		if(!stSearchFunction(st, glob_Token.data))
			return E_SEMANTIC_UNDECLARED;
		
		symbol = stInsertStaticValue(st->curr, glob_Token.data, retval);
	}
	else if(
		retval == TTYPE_NUMBER ||
		retval == TTYPE_DEC_NUMBER ||
		retval == TTYPE_TRUE ||
		retval == TTYPE_FALSE || 
		retval == TTYPE_NULL ||
		retval == TTYPE_STRING
	)
		symbol = stInsertStaticValue(st->curr, glob_Token.data, retval);		
	else
		return E_SYNTAX;

	listInsertPost(&st->curr->instructions, makeInstruction(INSTRUCTION_PUSH, symbol, NULL, NULL));
	
	return parserParseCallParams(st);
}

ERROR parserParseCallParams(SYMBOL_TABLE_PTR st){
	enum_RetVal retval = getToken();
	SYMBOL_PTR symbol = NULL;
	
	listEnd(&st->curr->instructions);
	
	//Bez dalsich parametru
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	//Parametry oddelene carkou
	else if(retval != TTYPE_COMMA)
		return E_SYNTAX;
	else{
		if(retval == TTYPE_VARIABLE){
			if(!stSearchFunction(st, glob_Token.data))
				return E_SEMANTIC_UNDECLARED;
		
			symbol = stInsertStaticValue(st->curr, glob_Token.data, retval);			
		}
		else if(
			retval == TTYPE_NUMBER ||
			retval == TTYPE_DEC_NUMBER ||		
			retval == TTYPE_TRUE ||
			retval == TTYPE_FALSE || 
			retval == TTYPE_NULL ||
			retval == TTYPE_STRING
		)
			symbol = stInsertStaticValue(st->curr, glob_Token.data, retval);	
		else
			return E_SYNTAX;		
		
		listInsertPost(&st->curr->instructions, makeInstruction(INSTRUCTION_PUSH, symbol, NULL, NULL));
		
		return parserParseCallParams(st);
	}
}