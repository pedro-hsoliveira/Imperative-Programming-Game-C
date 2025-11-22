#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <math.h>
#include "raymath.h"
#include <stdbool.h>
#include "../enemy/enemy.h"
#define MAX_PROJECTILES 200

// 4 direções 
typedef enum{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// Corpo
typedef struct {
    Vector2 position;
    float speed;
    Direction direction;
} Body;

// Cabeça
typedef struct {
    Vector2 position;
    Direction direction;
    int frame;
    float timer;
    float animSpeed;
} Head;

// Tiro
typedef struct {
    Vector2 position;
    Vector2 direction;
    float speed;
    bool active;
} Projectile;

// Sprites do corpo
typedef struct {
    Texture2D frente[6];
    Texture2D tras[6];
    Texture2D esquerda[6];
    Texture2D direita[6];
    Texture2D idle;
    int frame;
    float timer;
    float animSpeed;
} BodySprites;

// Sprites da cabeça
typedef struct {
    Texture2D frente[2];
    Texture2D tras[2];
    Texture2D esquerda[2];
    Texture2D direita[2];
    Texture2D idle;
} HeadSprites;

// Player completo
typedef struct {
    Body body;
    Head head;
    BodySprites bodySprites;
    HeadSprites headSprites;
    Projectile projectiles[MAX_PROJECTILES];
    
    
    bool isMovingHead; 
    bool isMovingBody;
    bool isMoving;

    float hp;   
    float maxHp;

    float fireCooldown;
    float fireRate;

    float hitboxRadius;    
    float spriteWidth;      
    float spriteHeight;     
} Player;

void InitPlayer(Player *p);
void UpdatePlayer(Player *p);
void DrawPlayer(Player *p);

#endif