#include "raylib.h"
#include "sprite.h"
#include "gameobj.h"
#include "level.h"
#include <stdio.h>

int main(){
    InitWindow(640, 360, "Test");
    loadAssets();
    Player* p = (Player*)malloc(sizeof(Player));
    playerInit(p);
    Level* l = (Level*)malloc(sizeof(Level));
    initLevel(l, "level.bin");
    initCamera(0, 0, 100);
    resizeScreen(2);
    int input;
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
        updateLevel(l, p);
        updatePlayer(p, l->tileMap, l->numOfTiles, input, jump);
        displayText(TextFormat("%f\n%f\n%d", (*p).e.w.yv, (*p).e.w.y, l->numOfTiles), 1, 400, 50, 2);
        EndDrawing();
    }
    freeAssets();
    return 0;
}