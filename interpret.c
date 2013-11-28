#include "interpret.h"

#define data_copy(data_src, data_dst) \
{\
	if((data_dst).type == TYPE_STRING) \
	{\
		mmuFree((data_dst).items.value.valString); \
	}\
	if((data_src).type == TYPE_DIGIT_INT)\
	{\
		((data_dst).type) = TYPE_DIGIT_INT; \
		((data_dst).items.value.valInt) = ((data_src).data.valInt); \
	}\
	else if((data_src).type == TYPE_DIGIT_DOUBLE)\
	{\
		((data_dst).type) = TYPE_DIGIT_DOUBLE; \
		((data_dst).items.value.valDouble) = ((data_src).data.valDouble); \
	}\
	else if((data_src).type == TYPE_OTHER)\
	{\
		((data_dst).type) = ((data_src).type); \
	}\
	else \
	{\
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
	
    SYMBOL* op1 = NULL;
    SYMBOL* op2 = NULL;
    SYMBOL* dest = NULL;
	
    FUNCTIONPTR tmp_function = NULL;
	
    ITEMPTR data1 = NULL;
    ITEMPTR data2 = NULL;
    ITEMPTR data3 = NULL;
	
	
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
				
// 				reallocSymbol(dest,function);
// 				data1=getData(dest,function);
// 				data2=getData(dest,function);
// 				copySymbolData(data1,data2);
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