#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "gc.h"
#include "scanner.h"

int main(int argc, char* argv[])
{

	if(argc != 2){
		fprintf(stderr, "Invalid arguments\n");
		return E_COMPILATOR;
	}
	
	gcInit();
	
	if(!(glob_FileHandler = gcFopen(argv[1], "r"))){
		fprintf(stderr, "Cannot open input file\n");
		return E_COMPILATOR;
	}
	
	// ------ LEX ------
	
	
	
	/*
	int* value = malloc(sizeof(int));
	printf("address: %d\n", value);
	int* value1 = malloc(sizeof(int));
	printf("address: %d\n", value1);
	int* value2 = malloc(sizeof(int));
	printf("address: %d\n", value2);
	int* value3 = mmuMalloc(sizeof(int));
 	int* value4 = malloc(sizeof(int));
	printf("address: %d\n", value4);
	int* value5 = mmuMalloc(sizeof(int));
	int* value6 = mmuMalloc(sizeof(int));

	value6 = mmuRealloc(value6, sizeof(long int));
	
	free(value4);
	free(value2);
	free(value1);
	free(value4);
	mmuFree(value5);
	mmuFree(value6);
*/
	
	gcFclose(glob_FileHandler);

	ERROR err = E_OK;

	gcDispose();
	
	return err;
}
