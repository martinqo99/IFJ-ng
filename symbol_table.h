/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   symbol_table.h
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
 
#ifndef TYPE_PTR_DEFINED
#define TYPE_PTR_DEFINED
typedef void* PTR;
#endif

#ifndef SYMBOL_TABLE_H_INCLUDED
#define SYMBOL_TABLE_H_INCLUDED

#include <stdbool.h>

#include "strings.h"
#include "list.h"
#include "btree.h"


typedef union VALUE{
    bool valBool;
    int valInt;
    double valDouble;
    STRING valString;    
} VALUE;

typedef enum TYPE{
	TYPE_NULL,
    TYPE_BOOL,
    TYPE_DIGIT_INT,
	TYPE_DIGIT_DOUBLE,
    TYPE_STRING,
    TYPE_CALLABLE,
    TYPE_OTHER
} TYPE;

typedef enum INSTRUCTION_TYPE{
    INSTRUCTION_RETURN,
	INSTRUCTION_MOV,         	//	A B		R(A) := R(B)  		(konstantu move nepresouva)
    INSTRUCTION_LOADK,   	// 	A Bx    R(A) := Kst(Bx)    indexujeme konstanty od 0     
	INSTRUCTION_LOAD_NULL	//	A B     R(A) := ... := R(B) := nil       local a;
    
    //Tuxi dopln si instrukce, ktere potrebujes
} INSTRUCTION_TYPE;

typedef struct ITEM{
    TYPE type;
    VALUE value;
} ITEM, *ITEMPTR;

typedef struct SYMBOL{
    STRING id;
    TYPE type;    
    ITEMPTR items;    
} SYMBOL;

/*
  typedef struct tsymbol{
  tString key;
  tSymbolData *data; //pole, pro vice instanci stejne promenne
  tDataType type;
} tSymbol;
*/
typedef struct INSTRUCTION{
    INSTRUCTION_TYPE type;
    PTR operand1;
    PTR operand2;
	PTR destionation;
} INSTRUCTION;

typedef struct FUNCTION{
    STRING id;
    
    struct_BTree symbols;
    LIST instructions;
    LIST staticValues;
    
    int calledCount;
    bool defined;
} FUNCTION, *FUNCTIONPTR;

typedef struct SYMBOL_TABLE{
    struct_BTree functions;
    
    FUNCTION start;
    FUNCTIONPTR curr;    
} SYMBOL_TABLE, *SYMBOL_TABLE_PTR;

void stInit(SYMBOL_TABLE_PTR st);
void stFree(SYMBOL_TABLE_PTR st);

/*
enum enum_instruction
{// R = registr,  kst -pole konstant, load nil -  RK - registr nebo konstanta
	
	I_MOVE = 0,    		A B     R(A) := R(B)  (konstantu move nepresouva)         
	I_LOADK = 1, 		A Bx    R(A) := Kst(Bx)    indexujeme konstanty od 0     
	I_LOADNIL = 2,	A B     R(A) := ... := R(B) := nil       local a;
	
// konstanta je posunuta o MAX_STACK_SIZE  oproti registrum 
	I_ADD = 3,        	A B C   R(A) := RK(B) + RK(C)                           
	I_SUB = 4,         	A B C   R(A) := RK(B) - RK(C)                           
	I_MUL = 5,         	A B C   R(A) := RK(B) * RK(C)                           
	I_DIV = 6,        	A B C   R(A) := RK(B) / RK(C)                           
	I_MOD = 7,       	A B C   R(A) := RK(B) % RK(C)                           
	I_POW = 8,       	A B C   R(A) := RK(B) ^ RK(C)                           
	I_UNM = 9,       	A B     R(A) := -RK(B)                                   
	I_NOT = 10,      	A B     R(A) := not RK(B)                                
	I_CONCAT = 7,     	A B C   R(A) := RK(B) .. RK(C)                            
 	I_AND = 13,        A B C   R(A) := RK(B) and RK(C)                           
 	I_OR = 14,         	A B C   R(A) := RK(B) or RK(C)                            
	I_LE = 8,         	A B C   R(A) := RK(B) <  RK(C)                             
	I_GR = 9,         	A B C   R(A) := RK(B) >  RK(C)                             
	I_EL = 10,        	A B C   R(A) := RK(B) <= RK(C)                             
	I_EG = 11,        	A B C   R(A) := RK(B) >= RK(C)                             
	I_EQ = 12,         	A B C   R(A) := RK(B) == RK(C)                             
	I_NEQ = 13,        	A B C   R(A) := RK(B) ~= RK(C)                             
	I_JMP = 14,         B     pc=B                                    
	I_JT = 15,         	A B     if(RK(A) == true)   pc=B                
	I_JF = 16,         	A B     if(RK(A) == false)  pc=B  
	
	I_CALL = 17,      	A B C   R(A) <= funkce C (parametry R(A) * B)
				      neboli R(A) = call(R(A) = prvni parametr, B = pocet parametru, C = id funkce),
				      obdobne write - neni konstanta nic
				      
	I_RETURN = 18,     	A       return RK(A) 
 	I_READ = 26,       	A B     R(A) = read(RK(B))
 	I_WRITE = 27,   	A B     write(R(A) * R(B)) - neni konstanta nic

	I_INPUT = 19,	// embedd function input - syntax = input() - returns stdin input
			// R(A) := stdin = input()
			
	I_PRINT = 20, 		R(A) := print(R(B) - prvni parametr, C - pocet parametru)
	I_LEN = 21,   		R(A) := typeof(RK(B))
	I_TYPEOF = 22,		R(A) := len(RK(B))
	I_FIND = 23,		R(A) := find(RK(B),RK(C))
    I_SORT = 24,		R(A) := sort(RK(B))
    I_NUMERIC = 25,	R(A) := numeric(RK(B))
			
	I_STOP = 26 //- konec
};

*/

#endif
