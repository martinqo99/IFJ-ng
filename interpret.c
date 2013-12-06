#include "interpret.h"

ERROR op_check(SYMBOL_PTR smb, char IO)
{
	if(smb == NULL)
		return E_COMPILATOR;
	else
	{
		if(smb->id.data == NULL)
		{
			if(smb->items == NULL)
				return 51;
			else
				return E_OK;
		}
		else
		{
			if(smb->items == NULL)
			{
				if(IO == 'o' || IO == 'O')
				{
					printf("ALOKuJu\n");
					smb->items = gcMalloc(sizeof(struct ITEM));
					return E_OK;
				}
				else
					return E_COMPILATOR;
			}
			else
				return E_OK;
		}
	}
	return E_OK;
}

#define data_copy(data_src, data_dst) \
{\
	if((data_dst) != NULL)\
	{\
		if((data_dst)->items != NULL)\
			if((data_dst)->items->type == TYPE_STRING) \
			{\
				gcFree(&(data_dst)->items->value.valString); \
			}\
	}\
	else\
	{\
		(data_dst) = gcMalloc(sizeof(struct SYMBOL));\
	}\
	ITEMPTR item;\
	item = gcMalloc(sizeof(struct ITEM));\
	(data_dst)->items = item;\
	\
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
		strInit(&(data_dst)->items->value.valString);\
		strCopy(&(data_dst)->items->value.valString, &(data_src)->items->value.valString);\
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
	printf("Zavolana funkce: %d\n",function);
	LIST_NODE_PTR instr_node = function->instructions.begin;
	ERROR err = E_OK;
	
	// ----- Priprava promenych ------
	INSTRUCTION_PTR instruction = NULL;
	int str_from,str_to;
	STRING tmp_string;
	
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
	
	FUNCTION_PTR tmp_function = NULL;
	
	while(instr_node != NULL && err == E_OK)
	{
		instruction = instr_node->value;
		
		op1 = (SYMBOL*) instruction->operand1;
		op2 = (SYMBOL*) instruction->operand2;
		op3 = (SYMBOL*) instruction->destionation;
		
		printf("while interpret : %d (%s)\n",instruction->type, debugInstruction(instruction->type));
		
		switch(instruction->type)
		{
			case INSTRUCTION_NOP: 			// FUNGUJE
			break;
			
			case INSTRUCTION_MOV: 			// FUNGUJE
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				
				data_copy(op2, op1);
			break;
			
			case INSTRUCTION_LOAD_NULL:	// FUNGUJE
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'O')) != E_OK)
					return err;
				if ((err = op_check(op3,'O')) != E_OK)
					return err;
				data_copy(null_symbol,op1); 	// from, to
				data_copy(null_symbol,op2); 	// from, to
				data_copy(null_symbol,op3); 	// from, to
			break;
			
			case INSTRUCTION_ADDITION:	// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
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
			
			case INSTRUCTION_PUSH:			// OTESTOVAT
				if ((err = op_check(op1,'I')) != E_OK)
					return err;
				stackPush(stack,op1);
			break;
			
			case INSTRUCTION_POP:			// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				op1 = stackPop(stack);
			break;
			
			case INSTRUCTION_CALL:		// OTESTOVAT
				if(op1 == NULL)
					return E_COMPILATOR;
				
				tmp_function = function;
				function = (FUNCTION_PTR)op1;
				
				err=recursive_interpret(function,stack);
				if (err != E_OK)
					return err;
				
				function = tmp_function;
			break;
			
			case INSTRUCTION_RETURN:		// OTESTOVAT
				return err;
			break;
			
			case INSTRUCTION_JUMP:		// OTESTOVAT
				if(op3 == NULL)
					return E_COMPILATOR;
				instruction = (INSTRUCTION_PTR)op3;
			break;
			
			case INSTRUCTION_IF_JUMP:		// OTESTOVAT
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if(op3 == NULL)
					return E_COMPILATOR;
				
				if(op2->items->type == TYPE_BOOL)
					if(op2->items->value.valBool)
						instruction = (INSTRUCTION_PTR)op3;
			break;
			
			case INSTRUCTION_LABEL:		// FUNGUJE
				
			break;
			
			case INSTRUCTION_BOOLVAL:		// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				tmp_symbol->items = boolval(*tmp_symbol->items);
				
				data_copy(tmp_symbol,op1);
			break;
			
			case INSTRUCTION_DOUBLEVAL:	// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				tmp_symbol->items = doubleval(*tmp_symbol->items);
				
				data_copy(tmp_symbol,op1);
			break;
			
			case INSTRUCTION_INTVAL:		// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				tmp_symbol->items = intval(*tmp_symbol->items);
				
				data_copy(tmp_symbol,op1);
			break;
			
			case INSTRUCTION_STRVAL: 		// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				tmp_symbol->items = strval(*tmp_symbol->items);
				
				data_copy(tmp_symbol,op1);
			break;
			
			case INSTRUCTION_GET_STRING: 	// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_string = get_string();
				
				tmp_symbol->items->type = TYPE_STRING;
				strInit(&tmp_symbol->items->value.valString);
				strCopy(&tmp_symbol->items->value.valString,&tmp_string);
				
				data_copy(tmp_symbol, op1);
			break;
			
			case INSTRUCTION_CONCATE: 		// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
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
			
			case INSTRUCTION_PUT_STRING: 		// OTESTOVAT
				// DODELAT
			break;
			
			case INSTRUCTION_STRLEN: 		// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				op1 = stackPop(stack);
				tmp_symbol->items->type = TYPE_DIGIT_INT;
				tmp_symbol->items->value.valInt = my_strlen(op1->items->value.valString);
				
				data_copy(tmp_symbol, op1);
			break;
			
			case INSTRUCTION_GET_SUBSTRING: 		// OTESTOVAT
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				// TO
				tmp_symbol = stackPop(stack);
				if ((err = op_check(tmp_symbol,'I')) != E_OK)
					return err;
				str_to = tmp_symbol->items->value.valInt;
				
				// FROM
				tmp_symbol = stackPop(stack);
				if ((err = op_check(tmp_symbol,'I')) != E_OK)
					return err;
				str_from = tmp_symbol->items->value.valInt;
				
				// STRING
				tmp_symbol = stackPop(stack);
				if ((err = op_check(tmp_symbol,'I')) != E_OK)
					return err;
				
				tmp_string = get_substring(tmp_symbol->items->value.valString,str_from,str_to,&err);
				if(err != E_OK)
					return err;
				tmp_symbol->items->type = TYPE_STRING;
				strCopy(&tmp_symbol->items->value.valString,&tmp_string);
				data_copy(tmp_symbol,op1);
			break;
			
			case INSTRUCTION_FIND_STRING: 		// OTESTOVAT
				if ((err = op_check(op1,'I')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_DIGIT_INT;
				tmp_symbol->items->value.valInt = find_string(op1->items->value.valString, op2->items->value.valString);
				
				data_copy(tmp_symbol,op1);
			break;
			
			case INSTRUCTION_SORT_STRING: 		// OTESTOVAT
				if ((err = op_check(op1,'I')) != E_OK)
					return err;
				
				sort_string(op1->items->value.valString);
			break;
			
			default:
				return E_OK;
			break;
			
		}
		instr_node = instr_node->next;
	}
	return err;
}
