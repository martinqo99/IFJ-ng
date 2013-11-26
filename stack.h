#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>


#define MAX_SIZE 255


typedef struct{
    void **SPole;
    void* Top;
}TStack;



void Sinit(TStack* s);
void SPush(TStack* s,void* El);
void* SPop(TStack* s);
int SEmpty(TStack* s);




#endif // STACK_H_INCLUDED
