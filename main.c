#include "raylib.h"
#include "sprite.h"
#include <stdio.h>

int main(){
    InitWindow(640, 360, "Test");
    loadAssets();
    SetTargetFPS(60);
    float yvel = 0;
    float x = 0;
    float y = 100;
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        drawSprite(0, 1, 1, 1, (int)x, (int)y);
        if (IsKeyDown(KEY_RIGHT))x += 1;
        if (IsKeyDown(KEY_LEFT))x -= 1;
        if (IsKeyDown(KEY_Z) && y == 200)yvel = -3;
        y += yvel;
        if (y < 200){
            yvel += 0.1;
        } else if (y > 200){
            y = 200;
        }
        EndDrawing();
    }
    freeAssets();
    return 0;
}