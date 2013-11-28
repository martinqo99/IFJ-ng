#include "interpret.h"

#define data_copy(data_src, data_dst) \
{\
	if((data_dst)->items->type == TYPE_STRING) \
	{\
		mmuFree(&(data_dst)->items->value.valString); \
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
	else if((data_src)->items->type == TYPE_OTHER)\
	{\
		((data_dst)->items->type) = ((data_src)->items->type); \
	}\
	else \
	{\
		strInitString(&(data_dst)->items->value.valString,&(data_src)->items->value.valString);\
		(data_dst)->items->type = TYPE_STRING;\
	}\
}

/*
	(data_dst).data.string = *((s_string*)mem_alloc(sizeof(struct s_string),id_interpret));\
	(data_dst).data.string.input = mem_alloc((data_src).data.string.size,id_interpret);\
	memcpy((data_dst).data.string.input, (data_src).data.string.input, (data_src).data.string.length);\
	(data_dst).data.string.length = (data_src).data.string.length;\
	(data_dst).type = TYPE_STRING;\
*/

ERROR interpret(SYMBOL_TABLE* table)
{
  TStack* stack;
  ERROR err;
  Sinit(stack);
  
  /// ----- POZNAMKA ----- 
  err = recursive_interpret(&(table->start),stack);
  
  // ------ STACK FREE? ------
//   stackDispose(stack);
//   stackDestroy(stack);
  return err;
}

ERROR recursive_interpret(FUNCTIONPTR function, TStack* stack)
{
	
	LISTNODE *help = function->instructions.begin;
	
	ERROR err = E_OK;
	
	// ----- Priprava promenych ------
	INSTRUCTION* instruction = NULL;
	
	ITEMPTR null_item;
	null_item = mmuMalloc(sizeof(struct ITEM));
	null_item->type = TYPE_NULL;
	
	SYMBOL* null_symbol;
	null_symbol = mmuMalloc(sizeof(struct SYMBOL));
	null_symbol->type = TYPE_NULL;
	null_symbol->items = null_item;
	
    SYMBOL* op1 = NULL;
    SYMBOL* op2 = NULL;
    SYMBOL* dest = NULL;
	
    FUNCTIONPTR tmp_function = NULL;
		
	while(help != NULL && err == E_OK)
	{
		instruction = (INSTRUCTION*) help->value;
		
		op1 = (SYMBOL*) instruction->operand1;
		op2 = (SYMBOL*) instruction->operand2;
		dest = (SYMBOL*) instruction->destionation;

		switch(instruction->type)
		{
			case INSTRUCTION_MOV:
				if (op1->items == NULL) 
					return E_INTERPRET;
				
				//data_copy(act_func.registr[instruction.address_2], act_func.registr[instruction.address_1]);
				data_copy(op1,dest);
			break;
			case INSTRUCTION_LOADK:
				if (op1->items == NULL) 
					return E_INTERPRET;
				
				//data_copy(konst->konst[instruction.address_2], act_func.registr[instruction.address_1]);
				data_copy(op1,dest);
			break;
			case INSTRUCTION_LOAD_NULL:
				if (op1->items == NULL) 
					return E_INTERPRET;
				
				//v_pom.type = DATA_NULL;
				//data_copy(v_pom, act_func.registr[instruction.address_1]);
				//data_copy(v_pom, act_func.registr[instruction.address_2]);
		  
				data_copy(null_symbol,op1);
				data_copy(null_symbol,op2);
				data_copy(null_symbol,dest);
			break;
	      
			/*case I_EQUAL:
				if(src1SymbPtr->data==NULL ||src2SymbPtr->data==NULL)
					return ERROR_SEMANTIC_VARIABLE;
				reallocSymbol(destSymbPtr,function);
				data1=getData(src1SymbPtr,function);
				data2=getData(src2SymbPtr,function);
				data3=getData(destSymbPtr,function);
				
				data3->type=DT_BOOL;
				if (data1->type == DT_NUMBER && data2->type == DT_NUMBER)
				{
					data3->data.bData = (data1->data.dData == data2->data.dData);
				}
				else if (data1->type == DT_STRING && data2->type == DT_STRING)
				{
					if (strcmp(data1->data.sData.data, data2->data.sData.data) == 0)
						data3->data.bData = true;
					else
						data3->data.bData = false;
				}
				else if (data1->type == DT_BOOL && data2->type == DT_BOOL)
				{
					data3->data.bData = (data1->data.dData == data2->data.dData);
				}
				else if(data1->type==DT_NIL && data2->type==DT_NIL)
					data3->data.bData = true;
				else
					data3->data.bData = false;
			break;
			*/
			default:
				return E_OK;
			break;
			
		}
	}
}