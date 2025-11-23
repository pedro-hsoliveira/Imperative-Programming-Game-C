#ifndef ENEMY_H
#define ENEMY_H
#define MAX_ENEMIES 6

#include "raylib.h"
#include <stdbool.h>
#include "raymath.h"
#include "entidades/player/player.h"


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




extern Enemy enemy1; 

void InitEnemy(Enemy *e, Vector2 pos);
void EnemyTakeDamage(Enemy *e, float dmg);
void UpdateEnemy(Enemy *e, Vector2 playerPos, float dt);
bool EnemyCheckCollisionWithPlayer(Enemy *e, Vector2 playerPos, float radius);
bool EnemyCheckCollisionWithProjectile(Enemy *e, Projectile *pr);
bool EnemyTryAttack(Enemy *e, Player *p, float dt); 

void DrawEnemy(Enemy *e);
void SpawnEnemiesRandom(Enemy enemies[], int count, Rectangle room, Vector2 playerPos);

#endif