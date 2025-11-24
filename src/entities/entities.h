#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include <math.h>
#include "raymath.h"
#include <stdbool.h>
#include "../define.h"

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
    Texture2D heartsSheet;
    Rectangle heartFrame;
    
    bool isMovingHead; 
    bool isMovingBody;
    bool isMoving;
    bool alive;

    float hp;   
    float maxHp;

    float fireCooldown;
    float fireRate;

    float hitboxRadius;    
    float spriteWidth;      
    float spriteHeight; 
      
} Player;

typedef struct {
    Texture2D sheet;
    Rectangle frame;      // Retângulo da animação
    int currentFrame;     // 0–7
    float timer;
    float animSpeed;      // Ex: 0.12
} EnemySpriteData;

typedef struct {
    Vector2 position;
    EnemySpriteData sprite;
    float hp;
    float maxHp;
    float radius;
    float speed;
    bool alive;

    float attackCooldown;   
    float attackRate;
} Enemy;


void InitPlayer(Player *p);
void UpdatePlayer(Player *p);
void PlayerTakeDamage(Player *p, float dmg);
void DrawPlayer(Player *p);
static void DrawHeartFrame(Texture2D sheet, Rectangle frame, Vector2 pos, float scale);
static void DrawHealth(Player *p);

void InitEnemy(Enemy *e, Vector2 pos);
void EnemyTakeDamage(Enemy *e, float dmg);
void UpdateEnemy(Enemy *e, Vector2 playerPos, float dt);
bool EnemyCheckCollisionWithPlayer(Enemy *e, Vector2 playerPos, float radius);
bool EnemyCheckCollisionWithProjectile(Enemy *e, Projectile *pr);
bool EnemyTryAttack(Enemy *e, Player *p, float dt); 

void DrawEnemy(Enemy *e);
void SpawnEnemiesRandom(Enemy enemies[], int count, Rectangle room, Vector2 playerPos);

#endif