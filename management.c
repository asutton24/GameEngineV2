#include "management.h"

Interrupt* irStack;

void stackSetUp(dbyte size){
    irStack = (Interrupt*)malloc(size * sizeof(Interrupt));
}

void freeStack(){
    free(irStack);
}