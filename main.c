/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   main.c
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

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "strings.h"
#include "scanner.h"

int main(int argc, char* argv[]){	
	
	if(argc != 2){
		fprintf(stderr, "\n");
		return E_COMPILATOR;
	}
	
	
	ERROR ecode = E_OK;

	STRING tmp1;
	STRING tmp2;
	
 	strInitRaw(&tmp1, "This is only for test purpose");
 	strInitRaw(&tmp2, "This is only for");
	
	printf("%s\n", tmp1.data);
	printf("%s\n", tmp2.data);
	
	strInitString(&tmp1, &tmp2);
	printf("%s\n", tmp1.data);

	strFree(&tmp1);
	strFree(&tmp2);
	
	// ------ FILE OPEN ------
	
	glob_FileHandler = fopen(argv[1],"r");
	
	// -------- LEX  --------
	
	init_Token();
	
	/*int i=0;
	int pom;
	while((pom = get_Token()) != TTYPE_EOF)
	{
		i++;
		printf("Token: %d\tTyp: %d\tData:\t%s\n",i,pom,glob_Token.data.data);
	}
	*/
	
	
	return ecode;
}