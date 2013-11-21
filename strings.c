/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   strings.c
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

#include "strings.h"

void strInit(STRING* str){
	if(!str)
		return;
	
	str->data = NULL;
	str->size = 0;
	str->used = 0;
}

void strInitRaw(STRING* str, char* raw){
	if(!str)
		return;
	
	strClear(str);
		
	if(raw != NULL || *raw != '\0'){
		char* ptr = raw;
		
		do{
			strAppend(str, *ptr);

			ptr++;
		}while(ptr && *ptr != '\0');		
	}
}

void strInitString(STRING* strDst, STRING* strSrc){
	if(!strDst || !strSrc)
		return;
	
	strClear(strDst);
	
	if(!strSrc->data)
		return;
	
	if(strSrc->data != NULL || *strSrc->data != '\0'){
		char* ptr = strSrc->data;
		
		do{
			strAppend(strDst, *ptr);

			ptr++;
		}while(ptr && *ptr != '\0');		
	}	
}

void strAppend(STRING* str, char c){
	if(!str)
		return;

	if((str->data == NULL) || (str->size == (str->used + 1) && str->data == NULL)){
		str->data = (char*)malloc(sizeof(char) * SIZE_STEP);
		str->size = SIZE_STEP;		
	}
	else if(str->size == (str->used + 1)){
		str->data = (char*)realloc(str->data, sizeof(char) * (str->size + SIZE_STEP));
		str->size = str->size + SIZE_STEP;
	}

	str->data[str->used] = c;	
	str->used++;
	str->data[str->used] = '\0';
}

void strClear(STRING* str){
	if(!str)
		return;
	
	strFree(str);
	strInit(str);
}

void strFree(STRING* str){
	if(!str)
		return;
	
	if(str->data != NULL && str->used != 0)
		free(str->data);
	
	str->data = NULL;
	str->size = 0;
	str->used = 0;
	
	return;
}

unsigned int strSize(STRING* str){
	return str->size;
}

unsigned int strUsed(STRING* str){
	return str->used;	
}

unsigned int strCompare(STRING str, char* str2)
{
	return strcmp(str.data, str2);
}