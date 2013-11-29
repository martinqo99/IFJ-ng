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
	
	st->curr = &(st->start);
}

ERROR stInsertFunction(SYMBOL_TABLE_PTR st, STRING id){
	FUNCTION_PTR function = gcMalloc(sizeof(FUNCTION));
	
	BT_Init(&function->symbols);
	listInit(&function->staticValues);
	listInit(&function->instructions);
	
	function->calledCount = 0;
	
	strCopy(&id, &function->id);
	
	ERROR err;
	STRING_PTR t = malloc(sizeof(STRING));
	strInitRaw(t, "text");
	
	BT_Insert(&st->functions, t);
	
	if(err != E_OK){
		
		
	}
	
	return err;	
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