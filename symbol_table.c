/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   symbol_table.c
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

#include "symbol_table.h"

void stInit(SYMBOL_TABLE_PTR st){
	if(!st)
		return;
	
	BT_Init(&st->start.symbols);
	BT_Init(&st->functions);
	
	listInit(&st->start.staticValues);
	listInit(&st->start.instructions);
	
	st->start.calledCount = 0;
	st->start.argumentsCount = 0;
	st->start.argumentsCalled = 0;
	
	st->curr = &(st->start);
}

ERROR stInsertFunction(SYMBOL_TABLE_PTR st, STRING id){
	FUNCTION_PTR function = gcMalloc(sizeof(FUNCTION));
	
	BT_Init(&function->symbols);
	listInit(&function->staticValues);
	listInit(&function->instructions);
	
	function->calledCount = 0;
	function->argumentsCount = 0;
	function->argumentsCalled = 0;
	
	strCopy(&id, &function->id);
	
	ERROR err = BT_Insert(&st->functions, &function->id, function);
	
	if(err != E_OK){
		
		
	}
	
	return err;	
}

ERROR stInsertSymbol(FUNCTION_PTR function, STRING id){
	SYMBOL_PTR symbol = gcMalloc(sizeof(SYMBOL));
	
	strCopy(&id, &symbol->id);
	symbol->type = TYPE_OTHER;
	symbol->items = NULL;
	
	ERROR err = BT_Insert(&function->symbols, &symbol->id, symbol);
	
	if(err != E_OK){
		
		
	}
	
	return err;
}

FUNCTION_PTR stSearchFunction(SYMBOL_TABLE_PTR st, STRING id){
	struct_BTree_Node node = BT_Search(&st->functions, &id);
	
	if(!node)
		return NULL;
	
	if(!node->data)
		return NULL;
	
	return (FUNCTION_PTR)node->data;	
}

SYMBOL_PTR stInsertStaticValue(FUNCTION_PTR function, STRING id, enum_RetVal retval, SYMBOL_PTR* symbol){
		
	printf("|| Static val in %s : %s\n", id.data, debugRetval(retval));
	*symbol = gcMalloc(sizeof(SYMBOL));
	
	(*symbol)->items = gcMalloc(sizeof(ITEM));
	//strCopy(&id, &(*symbol)->id); //mozno ne?
	
	
	switch(retval){
		case TTYPE_NULL:
			(*symbol)->items->type = TYPE_NULL;
			break;
		case TTYPE_TRUE:
			(*symbol)->items->type = TYPE_BOOL;
			(*symbol)->items->value.valBool = true;
			break;
		case TTYPE_FALSE:
			(*symbol)->items->type = TYPE_BOOL;
			(*symbol)->items->value.valBool = false;
			break;
		case TTYPE_STRING:
			printf("-| stringuju: %s\n", id);
			(*symbol)->items->type = TYPE_STRING;
			strCopy(&((*symbol)->items->value.valString), &id);
			break;
		case TTYPE_NUMBER:
			(*symbol)->items->type = TYPE_DIGIT_INT;
		
			sscanf(id.data, "%d", &((*symbol)->items->value.valInt));
			break;
		case TTYPE_DEC_NUMBER:
			(*symbol)->items->type = TYPE_DIGIT_DOUBLE;
			
			sscanf(id.data, "%lf", &((*symbol)->items->value.valDouble));
			break;
		case TTYPE_VARIABLE:
			(*symbol)->items->type = TYPE_CALLABLE;
			break;
		default:
			(*symbol)->items->type = TYPE_OTHER;
			break;
	}
	
	listInsertEnd(&function->staticValues, (*symbol));
	//printf("POKOKOT\n");
	return (*symbol);
}

SYMBOL_PTR stInsertStaticValueEmpty(FUNCTION_PTR function){
	SYMBOL_PTR symbol = gcMalloc(sizeof(SYMBOL));
	
	symbol->id.data = NULL;
	symbol->type = TTYPE_CONSTANT;
	symbol->items = NULL;
	
	listInsertEnd(&function->staticValues, symbol);
	
	return symbol;
}

SYMBOL_PTR stSearchSymbol(FUNCTION_PTR function, STRING id){
	struct_BTree_Node node = BT_Search(&function->symbols, &id);
	
	if(!node)
		return NULL;
	
	if(!node->data)
		return NULL;
	
	return (SYMBOL_PTR)node->data;	
}

SYMBOL_PTR stGetLastSymbol(FUNCTION_PTR function){
	if(!function)
		return NULL;
		
	if(!function->symbols.last)
		return NULL;

	return function->symbols.last->data;
}

void stFree(SYMBOL_TABLE_PTR st){
	if(!st)
		return;
	
	BT_Free(&st->start.symbols);
	BT_Free(&st->functions);
	
	listFree(&st->start.staticValues);
	listFree(&st->start.instructions);
	
	st->start.calledCount = 0;
	
	st->curr = NULL;
}

// Helpers

INSTRUCTION_PTR makeInstruction(INSTRUCTION_TYPE type, PTR operand1, PTR operand2, PTR destionation){
	INSTRUCTION_PTR i = gcMalloc(sizeof(INSTRUCTION));
	
	i->type = type;
	i->operand1 = operand1;
	i->operand2 = operand2;
	i->destionation = destionation;
	
	return i;
}

const char* debugInstruction(INSTRUCTION_TYPE i){
	switch(i){
		case INSTRUCTION_RETURN:
			return "return";
		case INSTRUCTION_POP:
			return "pop";
		case INSTRUCTION_PUSH:
			return "push";
		case INSTRUCTION_MOV:
			return "mov";
		case INSTRUCTION_LOAD_NULL:
			return "loadnull";
		case INSTRUCTION_NOP:
			return "nop";
		case INSTRUCTION_ADDITION:
			return "addition";
		case INSTRUCTION_SUBSTRACTION:
			return "substraction";
		case INSTRUCTION_MULTIPLICATION:
			return "multiplication";
		case INSTRUCTION_DIVISION:
			return "division";
		case INSTRUCTION_CONCATE:
			return "concate";
		case INSTRUCTION_EQUAL:
			return "equal";
		case INSTRUCTION_NOT_EQUAL:
			return "not equal";
		case INSTRUCTION_LESS:
			return "less?";
		case INSTRUCTION_LESS_EQUAL:
			return "less_equal";
		case INSTRUCTION_GREATER:
			return "greater";
		case INSTRUCTION_GREATER_EQUAL:
			return "greater_equal";
		case INSTRUCTION_JUMP:
			return "jump";
		case INSTRUCTION_IF_JUMP:
			return "jump_if";
		case INSTRUCTION_LABEL:
			return "label";
		case INSTRUCTION_CALL:
			return "call";
		case INSTRUCTION_BOOLVAL:
			return "boolval";
		case INSTRUCTION_DOUBLEVAL:
			return "doubleval()";
		case INSTRUCTION_INTVAL:
			return "intval";
		case INSTRUCTION_STRVAL:
			return "strval";
		case INSTRUCTION_GET_STRING:
			return "get_string";		
		case INSTRUCTION_PUT_STRING:
			return "put_string";
		case INSTRUCTION_STRLEN:
			return "strlen";
		case INSTRUCTION_GET_SUBSTRING:
			return "get_substring";
		case INSTRUCTION_FIND_STRING:
			return "find_string";
		case INSTRUCTION_SORT_STRING:
			return "sort_string";
		default:
			return "undefined";
	}
}