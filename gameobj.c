#include "gameobj.h"

Cam mainCamera;

void initCamera(byte m, int l, int h){
    mainCamera.mode = m;
    mainCamera.low = l;
    mainCamera.highest = h;
}

byte rectangleCollision(int x1, int xlen1, int y1, int ylen1, int x2, int xlen2, int y2, int ylen2, byte pass){
    byte temp = ((x2 >= x1 && x2 <= x1 + xlen1) || (x2 + xlen2 >= x1 && x2 + xlen2 <= x1 + xlen1)) && ((y2 >= y1 && y2 <= y1 + ylen1) || (y2 + ylen2 >= y1 && y2 + ylen2 <= y1 + ylen1));
    if (pass == 1){
        return temp;
    } 
    if (!temp){
        return rectangleCollision(x2, xlen2, y2, ylen2, x1, xlen1, y1, ylen1, 1);
    } 
    return temp;
}

byte solidWorldCollide(WorldObject* w, SolidObject* s){
    if (abs((int)((*s).x - (*w).x)) > 128 || abs((int)((*s).y - (*w).y)) > 128) return false;
    for (int i = 0; i < (*w).hitbox.num; i++){
        if (rectangleCollision((int)((*w).x + (*w).hitbox.boxes[i][0]), (*w).hitbox.boxes[i][2], (int)((*w).y + (*w).hitbox.boxes[i][1]), (*w).hitbox.boxes[i][3], (int)(*s).x, 8 * (*s).xlen - 1, (int)(*s).y, 8 * (*s).ylen - 1, 0)){
            return true;
        }
    }
    return false;
}

byte worldCollide(WorldObject* a, WorldObject* b){
    if (abs((int)((*a).x - (*b).x)) > 128 || abs((int)((*a).x - (*b).x)) > 128) return false;
    for (int i = 0; i < (*a).hitbox.num; i++){
        for (int j = 0; j < (*b).hitbox.num; j++){
            if (rectangleCollision((int)((*a).x + (*a).hitbox.boxes[i][0]), (*a).hitbox.boxes[i][2], (int)((*a).y + (*a).hitbox.boxes[i][1]), (*a).hitbox.boxes[i][3], (int)((*b).x + (*b).hitbox.boxes[j][0]), (*b).hitbox.boxes[j][2], (int)((*b).y + (*b).hitbox.boxes[j][1]), (*b).hitbox.boxes[j][3], 0)){
                return true;
            }
        }
    }
    return false;
}

void updateSolidObject(SolidObject* s){
    if (mainCamera.mode == 0){
        if ((*s).x - mainCamera.low >= -64 && (*s).x - mainCamera.low < 640){
            drawSprite((*s).sprIndex, (*s).palIndex, (*s).xlen, (*s).ylen, (int)((*s).x - mainCamera.low), (int)(360 - (*s).y - 8 * (*s).ylen));
        }
    } else {
        if ((*s).y - mainCamera.low >= -64 && (*s).y - mainCamera.low < 360){
            drawSprite((*s).sprIndex, (*s).palIndex, (*s).xlen, (*s).ylen, (int)((*s).x), (int)(360 - ((*s).y - mainCamera.low - 8 * (*s).ylen)));
        }
    }
}

byte updateWorldObject(WorldObject* w, SolidObject* solids, int numOfObjs){
    byte collision = 0;
    for (int i = 0; i < 2; i++){
        if (i == 0){
            (*w).x += (*w).xv;
        } else {
            (*w).y += (*w).yv;
        }
        for (int j = 0; j < numOfObjs; j++){
            if (solidWorldCollide(w, &(solids[j]))){
                collision = i + 1;
                break;
            }
        }
        if (collision){
            if (i == 0){
                (*w).x -= (*w).xv;
            } else {
                (*w).y -= (*w).yv;
            }
        }
    }
    if (mainCamera.mode == 0){
        if ((*w).x - mainCamera.low >= -64 && (*w).x - mainCamera.low < 640){
            drawAnimation(&((*w).spr[(*w).current]), (int)((*w).x - mainCamera.low), (int)(360 - ((int)(*w).y) - 8 * (*w).spr[(*w).current].ylen));
        }
    } else {
        if ((*w).y - mainCamera.low >= -64 && (*w).y - mainCamera.low < 360){
            drawAnimation(&((*w).spr[(*w).current]), (int)((*w).x), (int)(360 - (((int)(*w).y) - mainCamera.low - 8 * (*w).spr[(*w).current].ylen)));
        }
    }
    return collision;
}


void updateEntity(Entity* e, SolidObject* solids, int numSolids){
    if ((*e).hp <= 0) return;
    byte collision = updateWorldObject(&((*e).w), solids, numSolids);
    (*e).onGround = (collision == 2 && (*e).w.yv < 0);
    if ((*e).kt > 0){
        (*e).kt--;
        (*e).w.xv = (*e).kx;
        (*e).w.yv = (*e).ky;
    }
    if (!(*e).onGround) (*e).w.yv += (*e).adg;
    else if ((*e).w.yv < -0.5) (*e).w.yv += 0.5;
    if ((*e).facing == 0) (*e).w.xv = (*e).speed * -1;
    else if ((*e).facing == 1) (*e).w.xv = (*e).speed;
}   

void updatePlayer(Player* p, SolidObject* solids, int numSolids, int moveDir, int jump){
    if (moveDir != 0){
        (*p).e.speed = (*p).maxSpeed;
        if (moveDir == 1){
            (*p).e.facing = 1;
        } else {
            (*p).e.facing = 0;
        }
    } else {
        (*p).e.speed = 0.0;
    }
    if (jump == 1 && (*p).e.onGround) (*p).e.w.yv = 3.0;
    updateEntity(&((*p).e), solids, numSolids);
    if (mainCamera.mode == 0){
        if ((*p).e.w.xv > 0 && (*p).e.w.x - mainCamera.low > 540 - 8 * (*p).e.w.spr[(*p).e.w.current].xlen) mainCamera.low = (int)((*p).e.w.x - 540 + 8 * (*p).e.w.spr[(*p).e.w.current].xlen);
        else if ((*p).e.w.xv < 0 && (*p).e.w.x - mainCamera.low < 100) mainCamera.low = (int)((*p).e.w.x - 100);
        if (mainCamera.low < 0) mainCamera.low = 0;
        else if (mainCamera.low > mainCamera.highest) mainCamera.low = mainCamera.highest;
    } else {
        if ((*p).e.w.yv > 0 && (*p).e.w.y - mainCamera.low > 300) mainCamera.low = (int)((*p).e.w.y - 300);
        else if ((*p).e.w.yv < 0 && (*p).e.w.y - mainCamera.low < 60 + 8 * (*p).e.w.spr[(*p).e.w.current].ylen) mainCamera.low = (int)((*p).e.w.y - 60 + (*p).e.w.spr[(*p).e.w.current].ylen);
    }
}

void playerInit(Player* p){
    (*p).e.w.hitbox.num = 1;
    (*p).e.w.hitbox.boxes = (rectangle*)malloc(sizeof(rectangle));
    (*p).e.w.hitbox.boxes[0][0] = 0;
    (*p).e.w.hitbox.boxes[0][1] = 0;
    (*p).e.w.hitbox.boxes[0][2] = 7;
    (*p).e.w.hitbox.boxes[0][3] = 7;
    (*p).e.w.spr = (Animation*)malloc(sizeof(Animation));
    (*p).e.w.spr[0].sprIndex = 56;
    (*p).e.w.spr[0].current = (*p).e.w.spr[0].sprIndex;
    (*p).e.w.spr[0].xlen = 1;
    (*p).e.w.spr[0].ylen = 1;
    (*p).e.w.spr[0].max = 1;
    (*p).e.w.spr[0].length = 1;
    (*p).e.w.spr[0].palIndex = 1;
    (*p).e.w.current = 0;
    (*p).e.w.x = 100.0;
    (*p).e.w.y = 100.0;
    (*p).e.w.xv = 0.0;
    (*p).e.w.yv = -1.0;
    (*p).e.facing = 0;
    (*p).e.adg = -0.1;
    (*p).e.hp = 100;
    (*p).e.kt = 0;
    (*p).e.kx = 0;
    (*p).e.ky = 0;
    (*p).e.onGround = 0;
    (*p).e.speed = 0.0;
    (*p).isMoving = 0;
    (*p).maxHealth = 100;
    (*p).maxSpeed = 1.0;
}


