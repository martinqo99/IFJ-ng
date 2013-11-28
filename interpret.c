#include "interpret.h"


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
	// ----- LIST? ------
	LISTNODE *help = function->instructions.begin;
	
	ERROR err = E_OK;
	
	// ----- Priprava promenych ------
	INSTRUCTION* instruction = NULL;
	
    SYMBOL* src_symbol = NULL;
    SYMBOL* src_symbol2 = NULL;
    SYMBOL* dest_symbol = NULL;
	
    FUNCTIONPTR tmp_function = NULL;
	
    ITEMPTR data1=NULL;
    ITEMPTR data2=NULL;
    ITEMPTR data3=NULL;
	
	
    int str_od,str_do;
    char* tmpstring;
	
	while(help != NULL && err == E_OK)
	{
		instruction = (INSTRUCTION*) help->value;
		
		src_symbol = (SYMBOL*) instruction->operand1;
		src_symbol2 = (SYMBOL*) instruction->operand2;
		dest_symbol = (SYMBOL*) instruction->destionation;
		
		switch(instruction->type)
		{
			default:
				return E_OK;
			break;
			
		}
	}
}