#include "interpret.h"

#define data_copy(data_src, data_dst) \
{\
	if((data_dst)->items->type == TYPE_STRING) \
	{\
		gcFree(&(data_dst)->items->value.valString); \
	}\
	if((data_src)->items->type == TYPE_DIGIT_INT)\
	{\
		((data_dst)->items->type) = TYPE_DIGIT_INT; \
		((data_dst)->items->value.valInt) = ((data_src)->items->value.valInt); \
	}\
	else if((data_src)->items->type == TYPE_DIGIT_DOUBLE)\
	{\
		((data_dst)->items->type) = TYPE_DIGIT_DOUBLE; \
		((data_dst)->items->value.valDouble) = ((data_src)->items->value.valDouble); \
	}\
	else if((data_src)->items->type == TYPE_BOOL)\
	{\
		((data_dst)->items->type) = TYPE_BOOL; \
		((data_dst)->items->value.valBool) = ((data_src)->items->value.valBool); \
	}\
	else if((data_src)->items->type == TYPE_STRING)\
	{\
		(data_dst)->items->type = TYPE_STRING;\
		strInitString(&(data_dst)->items->value.valString,&(data_src)->items->value.valString);\
	}\
	else \
	{\
		((data_dst)->items->type) = ((data_src)->items->type); \
	}\
}

ERROR interpret(SYMBOL_TABLE* table)
{
	STACK_PTR stack;
	ERROR err;
	stack = gcMalloc(sizeof(struct STACK));
	stackInit(stack);
	
	/// ----- POZNAMKA ----- 
	err = recursive_interpret(table->curr,stack);

	stackFree(stack);
	gcFree(stack);
	return err;
}

ERROR recursive_interpret(FUNCTION_PTR function, STACK_PTR stack)
{
	LIST_NODE_PTR instr_node = function->instructions.begin;
	ERROR err = E_OK;
	
	// ----- Priprava promenych ------
	INSTRUCTION* instruction = NULL;
	
	
	// NULL SYMBOL
	ITEMPTR null_item;
	null_item = gcMalloc(sizeof(struct ITEM));
	null_item->type = TYPE_NULL;
	
	SYMBOL* null_symbol;
	null_symbol = gcMalloc(sizeof(struct SYMBOL));
	null_symbol->type = TYPE_NULL;
	null_symbol->items = null_item;
	
	// TMP SYMBOL
	ITEMPTR tmp_item;
	tmp_item = gcMalloc(sizeof(struct ITEM));
	
	SYMBOL* tmp_symbol;
	tmp_symbol = gcMalloc(sizeof(struct SYMBOL));
	tmp_symbol->items = tmp_item;
	
	
    SYMBOL* op1 = NULL;
    SYMBOL* op2 = NULL;
    SYMBOL* op3 = NULL;
		
	while(instr_node != NULL && err == E_OK)
	{
		printf("while interpret\n");
		instruction = instr_node->value;
		
		op1 = (SYMBOL*) instruction->operand1;
		op2 = (SYMBOL*) instruction->operand2;
		op3 = (SYMBOL*) instruction->destionation;
// 		printf("typ: %d\n",((INSTRUCTION_PTR)instruction)->type);
		switch(instruction->type)
		{
			case INSTRUCTION_NOP: 			// FUNGUJE
			break;
			
			case INSTRUCTION_MOV: 			// FUNGUJE
				if (op1->items == NULL) 
					return E_COMPILATOR;
				data_copy(op1,op3);
			break;
			
			case INSTRUCTION_LOAD_NULL:	// FUNGUJE
				if (op1->items == NULL) 
					return E_COMPILATOR;
				
// 				printf("op1 type: %d\n",op1->items->type);
// 				printf("op2 type: %d\n",op1->items->type);
// 				printf("op3 type: %d\n",op1->items->type);
				
				data_copy(null_symbol,op1); // from, to
				data_copy(null_symbol,op2); // from, to
				data_copy(null_symbol,op3); // from, to
				
// 				printf("op1 type: %d\n",op1->items->type);
// 				printf("op2 type: %d\n",op1->items->type);
// 				printf("op3 type: %d\n",op1->items->type);
			break;
			
			case INSTRUCTION_LOADK:		// OTESTOVAT
				op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				data_copy(op2,op1); // from, to
			break;
			
			case INSTRUCTION_ADDITION:			// OTESTOVAT
				//	A B C	R(A) := RK(B) + RK(C) 
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_INT;
					tmp_symbol->items->value.valInt = op2->items->value.valInt + op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valInt + op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble + op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble + op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
// 				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_STRING)
// 				{
// 					STRING new_string;
// 					char* new_char_string = NULL;
// 					sprintf(new_char_string, "%d", op2->items->value.valInt);
// 					strInitRaw(&new_string, new_char_string);
// 					
// 					tmp_symbol->items->type = TYPE_STRING;
// 					
// // 					strConcatenate(tmp_symbol->items->value.valString,op2->items->value.valString);
// // 					strConcatenate(tmp_symbol->items->value.valString,op3->items->value.valString);
// 					
// 					data_copy(tmp_symbol,op1); // from, to
// 				}
// 				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_STRING)
// 				{
// 					STRING new_string;
// 					char* new_char_string = NULL;
// 					sprintf(new_char_string, "%lf", op2->items->value.valDouble);
// 					strInitRaw(&new_string, new_char_string);
// 					
// 					tmp_symbol->items->type = TYPE_STRING;
// 					
// // 					strConcatenate(tmp_symbol->items->value.valString,op2->items->value.valString);
// // 					strConcatenate(tmp_symbol->items->value.valString,op3->items->value.valString);
// 					
// 					data_copy(tmp_symbol,op1); // from, to
// 				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->type = TYPE_STRING;
					
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_DIGIT_INT)
				{
					STRING new_string;
					char* new_char_string = NULL;
					sprintf(new_char_string, "%d", op3->items->value.valInt);
					strInitRaw(&new_string, new_char_string);
					
					tmp_symbol->items->type = TYPE_STRING;
					
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					STRING new_string;
					char* new_char_string = NULL;
					sprintf(new_char_string, "%lf", op3->items->value.valDouble);
					strInitRaw(&new_string, new_char_string);
					
					tmp_symbol->items->type = TYPE_STRING;
					
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					
					data_copy(tmp_symbol,op1); // from, to
				}
			break;
			
			case INSTRUCTION_SUBSTRACTION:			// OTESTOVAT
				//	A B C	R(A) := RK(B) - RK(C)
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_INT;
					tmp_symbol->items->value.valInt = op2->items->value.valInt - op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valInt - op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble - op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble - op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				
			break;
			
			case INSTRUCTION_MULTIPLICATION:			// OTESTOVAT
				//	A B C	R(A) := RK(B) - RK(C)
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_INT;
					tmp_symbol->items->value.valInt = op2->items->value.valInt * op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = ((double)op2->items->value.valInt) * op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble * ((double)op3->items->value.valInt);
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble * op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				
			break;
			
			case INSTRUCTION_DIVISION:			// OTESTOVAT
				//	A B C	R(A) := RK(B) - RK(C)
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					if(op3->items->value.valInt == 0)
						return E_COMPILATOR;
					tmp_symbol->items->type = TYPE_DIGIT_INT;
					tmp_symbol->items->value.valInt = op2->items->value.valInt / op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					if(op3->items->value.valDouble == 0.0)
						return E_COMPILATOR;
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valInt / op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_INT)
				{
					if(op3->items->value.valInt == 0)
						return E_COMPILATOR;
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble / op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					if(op3->items->value.valDouble == 0.0)
						return E_COMPILATOR;
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble / op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
			break;
			
			case INSTRUCTION_LESS:		// OTESTOVAT
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt < op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble < op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used < op3->items->value.valString.used;
				}
				
				data_copy(tmp_symbol,op1); // from, to
				
			break;
			
			case INSTRUCTION_GREATER:		// OTESTOVAT
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt > op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble > op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used > op3->items->value.valString.used;
				}
				
				data_copy(tmp_symbol,op1); // from, to
			break;
			
			case INSTRUCTION_LESS_EQUAL:		// OTESTOVAT
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt <= op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble <= op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used <= op3->items->value.valString.used;
				}
				
				data_copy(tmp_symbol,op1); // from, to
			break;
			
			case INSTRUCTION_GREATER_EQUAL:		// OTESTOVAT
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt >= op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble >= op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used >= op3->items->value.valString.used;
				}
				
				data_copy(tmp_symbol,op1); // from, to
			break;
			
			case INSTRUCTION_EQUAL:		// OTESTOVAT
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt == op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble == op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used == op3->items->value.valString.used;
				}
				
				data_copy(tmp_symbol,op1); // from, to
			break;
			
			case INSTRUCTION_NOT_EQUAL:		// OTESTOVAT
				if(op2->items->type == TTYPE_CONSTANT)
				{
					op2 = (listAt(&function->staticValues,op2->items->value.valInt))->value;
				}
				if(op3->items->type == TTYPE_CONSTANT)
				{
					op3 = (listAt(&function->staticValues,op3->items->value.valInt))->value;
				}
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt != op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble != op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used != op3->items->value.valString.used;
				}
				
				data_copy(tmp_symbol,op1); // from, to
			break;
			
			default:
				return E_OK;
			break;
			
		}
		instr_node = instr_node->next;
	}
	return err;
}
