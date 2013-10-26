/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   main.c
 *
 * Popis:
 *
 *
 * Datum:    12.9.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "mmu.h"
#include "scanner.h"

int main(int argc, char* argv[]){
		
	if(argc != 2){
		fprintf(stderr, "Invalid arguments\n");
		return ERROR_COMPILATOR;		
	}
	
	mmuInit();
	
	if(!(gFileHandler = mmuFopen(argv[1], "r"))){
		fprintf(stderr, "Cannot open input file\n");
		return ERROR_COMPILATOR;		
	}
	
	E_CODE err = ERROR_OK;
	
	mmuGlobalFree();
	
	return err;
}