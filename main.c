#include "raylib.h"
#include "sprite.h"
#include "gameobj.h"
#include <stdio.h>

int main(){
    InitWindow(640, 360, "Test");
    loadAssets();
    initCamera(0, 0, 100);
    resizeScreen(2);
    SolidObject* objects = (SolidObject*)malloc(150 * sizeof(SolidObject));
    Player* p = (Player*)malloc(sizeof(Player));
    playerInit(p);
    int input;
    for (int i = 0; i < 150; i++){
        objects[i].palIndex = 1;
        objects[i].sprIndex = 60;
        objects[i].xlen = 2;
        objects[i].ylen = 2;
        objects[i].y = 32;
        objects[i].x = 16 * i;
    }
    int jump;
    SetTargetFPS(60);
    while (!WindowShouldClose()){
        input = 0;
        if (IsKeyDown(KEY_LEFT)) input = -1;
        else if (IsKeyDown(KEY_RIGHT)) input = 1;
        else input = 0;
        if (IsKeyDown(KEY_SPACE)) jump = 1;
        else jump = 0;
        if (IsKeyDown(KEY_R)){
            (*p).e.w.x = 100.0;
            (*p).e.w.y = 100.0;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < 150; i++){
            updateSolidObject(&(objects[i]));
        }
        updatePlayer(p, objects, 150, input, jump);
        displayText(TextFormat("%f\n%f\n%d", (*p).e.w.xv, (*p).e.w.x, (*p).e.onGround), 1, 400, 50, 2);
        EndDrawing();
    }
    freeAssets();
    free(objects);
    return 0;
}