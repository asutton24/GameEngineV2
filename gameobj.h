#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "sprite.h"
#include "management.h"

typedef byte rectangle[4];
typedef struct cam {
    byte mode;
    int low;
    int highest;
} Cam;
typedef struct hb {
    byte num;
    rectangle *boxes;
} Hitbox;
typedef struct wo {
    Hitbox hitbox;
    Animation *spr;
    byte current;
    float x;
    float y;
    float xv;
    float yv;
} WorldObject;
typedef struct so {
    float x;
    float y;
    dbyte sprIndex;
    dbyte palIndex;
    byte xlen;
    byte ylen;
} SolidObject;
typedef struct en {
    WorldObject w;
    byte onGround;
    byte facing;
    float adg;
    int hp;
    float speed;
    float kx;
    float ky;
    int kt; 
} Entity;
typedef struct pl {
    Entity e;
    float maxSpeed;
    int maxHealth;
    byte isMoving;
} Player;
typedef struct eni {
    Entity e;
    byte isMoving;
    int damage;
    dbyte moveType;
    dbyte attackType;
} Enemy;
typedef struct it {
    WorldObject w;
    int id;
    int amount;
    byte active;
} Item;
typedef struct pr {
    WorldObject w;
    int lifetime;
    int damage;
} Projectile;
typedef struct inter {
    WorldObject w;
    Interrupt act;
} Interactable;
typedef struct go {
    void* loc;
    byte type;
} GameObject;
extern Cam mainCamera;

void initCamera(byte m, int l, int h);
byte rectangleCollision(int x1, int xlen1, int y1, int ylen1, int x2, int xlen2, int y2, int ylen2, byte pass);
byte solidWorldCollide(WorldObject* w, SolidObject* s);
byte worldCollide(WorldObject* a, WorldObject* b);
void updateSolidObject(SolidObject* s);
byte updateWorldObject(WorldObject* w, SolidObject* solids, int numOfObjs);
void updateEntity(Entity* e, SolidObject* solids, int numSolids);
void updatePlayer(Player* p, SolidObject* solids, int numSolids, int moveDir, int jump);
void playerInit(Player* p);
#endif