#include "stack.h"


void Sinit(TStack* s){
    s->SPole  = malloc( MAX_SIZE * sizeof(void *) );
    s->Top = s->SPole;
}

void SPush(TStack* s, void* EL){
        s->Top  = EL;
        s->Top++1;
}

void* SPop(TStack* s){
    void* tmp;
    if(s->Top > 0){
      tmp = s->Top;
      s->Top--;
    }
    return tmp;
}

int SEmpty(TStack* s){

    s->Top = NULL;

    return 1;

}
