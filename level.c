#include "level.h"

Level* currentLev;
Enemy* activeEnemies;
GameObject* activeObjects;
int numObjs;

byte initLevel(Level* l, char* path){
    FILE* file = fopen(path, "rb");
    if (file == NULL) return 1;
    fread(&(l->mode), 1, 1, file);
    byte tileLen;
    fread(&tileLen, 1, 1, file);
    if (tileLen == 0) return 1;
    byte* tileSheet = (byte*)malloc(2 * tileLen);
    if (tileSheet > 0){}
    fread(tileSheet, 1, 2 * tileLen, file);
    fread(&(l->numOfTiles), sizeof(int), 1, file);
    l->tileMap = (SolidObject*)malloc(l->numOfTiles * sizeof(SolidObject));
    byte instruction[2];
    int index = 0;
    int cx = 0;
    int cy = 0;
    while (index < l->numOfTiles){
        fread(instruction, 1, 2, file);
        if (instruction[0] == 0xFF){
            if (l->mode == 0){
                cy += instruction[1] * 16;
                cx += cy / 368 * 16;
                cy %= 368;
            } else {
                cx += instruction[1] * 16;
                cy += cx / 640 * 16;
                cx %= 640;
            }
        } else {
            for (int i = 0; i < instruction[1] && index < l->numOfTiles; i++){
                l->tileMap[index].x = cx;
                l->tileMap[index].y = cy;
                l->tileMap[index].xlen = 2;
                l->tileMap[index].ylen = 2;
                l->tileMap[index].sprIndex = tileSheet[2 * instruction[0]];
                l->tileMap[index].palIndex = tileSheet[2 * instruction[0] + 1];
                if (l->mode == 0){
                    cy += 16;
                    if (cy == 368){
                        cx += 16;
                        cy = 0;
                    }
                } else {
                    cx += 16;
                    if (cx == 640){
                        cy += 16;
                        cx = 0;
                    }
                }
                index++;
            }

        }
    }
    l->enemies = NULL;
    l->numOfEnemies = 0;
    return 0;
}

void updateLevel(Level* l, Player* p){
    for (int i = 0; i < l->numOfTiles; i++){
        updateSolidObject(&(l->tileMap[i]));
    }
}