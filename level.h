#ifndef LEVEL_H
#define LEVEL_H

#include "gameobj.h"
#include "sprite.h"

typedef struct lev {
    byte mode;
    SolidObject* tileMap;
    int numOfTiles;
    Enemy* enemies;
    int numOfEnemies;
} Level;

extern Level* currentLev;
extern Enemy* activeEnemies;
extern GameObject* activeObjects;
extern int numObjs;

byte initLevel(Level* l, char* path);
void updateLevel(Level* l, Player* p);
#endif