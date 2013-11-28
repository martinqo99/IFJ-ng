#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

#include <math.h>
#include "errors.h"
#include "stack.h"

//#include "syntactic.h"
#include "symbol_table.h"
//#include "library.h"

//#include "quicksort.h"
//#include "kmp.h"


// --- API ---
ERROR interpret(SYMBOL_TABLE*);
ERROR recursive_interpret(FUNCTIONPTR, TStack*);

/*bool isConstant(tSymbol *);
ERROR copySymbolData(tSymbolData *,tSymbolData *);
void reallocSymbol(tSymbol *,tFunction *);
tSymbolData* getData(tSymbol*,tFunction *);
*/

#endif