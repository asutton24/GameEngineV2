#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "gameobj.h"

typedef struct ir {
    void* loc;
    dbyte type;
} Interrupt;

extern Interrupt* irStack;
#endif