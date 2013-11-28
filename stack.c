/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   stack.c
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

#include "stack.h"

void stackInit(STACK_PTR stack){
	if(!stack)
		return;
	
	stack->begin = NULL;
	stack->count = 0;
}

void stackPush(STACK_PTR stack, PTR value){
	if(!stack)
		return;	
	
	STACK_NODE_PTR tmp = malloc(sizeof(STACK_NODE));
	
	tmp->value = value;
	tmp->next = stack->begin;
	
	stack->begin = tmp;
}

PTR stackPop(STACK_PTR stack){
	if(!stack)
		return NULL;
	
	if(!stack->begin)
		return NULL;
	
	STACK_NODE_PTR tmp = stack->begin;
	PTR value = tmp->value;
	stack->begin = tmp->next;

	free(tmp);	
	
	return value;
}

PTR stackBegin(STACK_PTR stack){
	if(!stack)
		return NULL;
	
	if(!stack->begin)
		return NULL;

	return stack->begin->value;
}

int stackCount(STACK_PTR stack){
	if(!stack)
		return 0;	
	
	return stack->count;
}

void stackFree(STACK_PTR stack){
	if(!stack)
		return;	

    STACK_NODE_PTR tmp;
	
	while((tmp = stack->begin)){
		stack->begin = tmp->next;
		
		free(tmp);
	}	
	
	stackInit(stack);
}