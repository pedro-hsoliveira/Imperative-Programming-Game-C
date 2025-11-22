#ifndef ENEMY_H
#define ENEMY_H


#include "raylib.h"
#include <stdbool.h>



typedef struct {
    Vector2 position;
    float hp;
    float maxHp;
    float radius;
    float speed;
    bool alive;
} Enemy;


#endif