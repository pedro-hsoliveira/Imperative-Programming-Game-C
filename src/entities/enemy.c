#include "entities.h"
#include "raymath.h"

// ======================================================
// Inicializa um inimigo em uma posição específica
// ======================================================
void InitEnemy(Enemy *e, Vector2 pos, EnemyType type) {
    e->type = type;

    switch (type) {
        case ENEMY_MELEE:
            e->attackCooldown = 0.9f;
            e->attackRate = 0.8f;
            e->position = pos;
            e->hp = 3.0f;
            e->maxHp = 3.0f;
            e->radius = 24.0f;
            e->speed = 70.0f;
            e->alive = true;
            e->sprite.sheet = LoadTexture("../assets/sprites/enemy/algovermelho_teste.png");
            break;

        case ENEMY_SLOW_TANK:
            e->attackCooldown = 0.9f;
            e->attackRate = 0.8f;
            e->position = pos;
            e->hp = 7.0f;
            e->maxHp = 7.0f;
            e->radius = 24.0f;
            e->speed = 40.0f;
            e->alive = true;
            e->sprite.sheet = LoadTexture("../assets/sprites/enemy/monstro.png");
            break;
    }

    TraceLog(LOG_INFO, "ENEMY SPAWN: Tipo %d em {%.0f, %.0f}", type, pos.x, pos.y);

    e->sprite.frame = (Rectangle){0, 0, e->sprite.sheet.width / 3, e->sprite.sheet.height / 3};
    e->sprite.currentFrame = 0;
    e->sprite.timer = 0.0f;
    e->sprite.animSpeed = 0.12f;
}

static void UpdateEnemyAnimation(Enemy *e, float dt) {
    e->sprite.timer += dt;

    if (e->sprite.timer >= e->sprite.animSpeed) {
        e->sprite.timer = 0;
        e->sprite.currentFrame++;

        if (e->sprite.currentFrame >= 8)
            e->sprite.currentFrame = 0;

        int frame = e->sprite.currentFrame;
        int col = frame % 3;
        int row = frame / 3;

        if (col > 0 && row == 2) {
            col = 0;
            row = 0;
            e->sprite.currentFrame = 0;
        }

        e->sprite.frame.x = col * e->sprite.frame.width;
        e->sprite.frame.y = row * e->sprite.frame.height;
    }
}

void EnemyTakeDamage(Enemy *e, float dmg) {
    if (!e->alive) return;

    e->hp -= dmg;
    if (e->hp <= 0) {
        e->hp = 0;
        e->alive = false;
    }
}

// ======================================================
// Movimento básico: perseguir o player
// ======================================================
bool EnemyTryAttack(Enemy *e, Player *p, float dt) {
    if (!e->alive) return false;
    if (!p->alive) return false;

    // Atualiza cooldown do inimigo
    if (e->attackCooldown > 0)
        e->attackCooldown -= dt;

    if (e->attackCooldown > 0)
        return false;

    // Verificar distância para ataque
    float dist = Vector2Distance(e->position, p->body.position);

    if (dist <= e->radius + p->hitboxRadius) {
        PlayerTakeDamage(p, 1);
        e->attackCooldown = e->attackRate;
        return true;
    }

    return false;
}

// POSIÇÃO
void SpawnEnemiesQuadrants(Enemy enemies[], int count, Rectangle room) {
    int perQuadrant = count / 4;
    int idx = 0;

    for (int q = 0; q < 4; q++) {
        float qx = room.x + (q % 2) * (room.width / 2);
        float qy = room.y + (q / 2) * (room.height / 2);

        for (int i = 0; i < perQuadrant; i++) {
            Vector2 pos = {
                qx + GetRandomValue(40, room.width / 2 - 40),
                qy + GetRandomValue(40, room.height / 2 - 40)
            };

            EnemyType newType = (i == 0) ? ENEMY_SLOW_TANK : ENEMY_MELEE;
            InitEnemy(&enemies[idx++], pos, newType);
        }
    }
}

void SpawnEnemiesRandom(Enemy enemies[], int count, Rectangle room, Vector2 playerPos) {
    float minDistance = 120.0f;

    for (int i = 0; i < count; i++) {
        Vector2 pos;
        int safety = 0;

        do {
            pos.x = room.x + GetRandomValue(50, room.width - 50);
            pos.y = room.y + GetRandomValue(50, room.height - 50);
            safety++;
            if (safety > 100) break;

        } while (Vector2Distance(pos, playerPos) < minDistance);

        EnemyType newType = (GetRandomValue(1, 10) <= 5) ? ENEMY_SLOW_TANK : ENEMY_MELEE;
        InitEnemy(&enemies[i], pos, newType);
    }
}

bool EnemyCheckCollisionWithProjectile(Enemy *e, Projectile *pr) {
    if (!e->alive || !pr->active)
        return false;

    float dist = Vector2Distance(e->position, pr->position);
    return dist <= e->radius + 6;
}

bool EnemyCheckCollisionWithPlayer(Enemy *e, Vector2 playerPos, float radius) {
    if (!e->alive)
        return false;

    float dist = Vector2Distance(e->position, playerPos);
    return dist <= e->radius + radius;
}

// ======================================================
// Checa se inimigo encostou no player
// ======================================================
bool EnemyHitsPlayer(Enemy *e, Vector2 playerPos, float playerRadius) {
    float dist = Vector2Distance(e->position, playerPos);
    return (dist <= e->radius + playerRadius);
}

void UpdateEnemy(Enemy *e, Vector2 playerPos, float dt) {
    if (!e->alive) return;

    Vector2 dir = Vector2Subtract(playerPos, e->position);
    float dist = Vector2Length(dir);

    if (dist > 1.0f)
        dir = Vector2Normalize(dir);

    e->position.x += dir.x * e->speed * dt;
    e->position.y += dir.y * e->speed * dt;

    UpdateEnemyAnimation(e, dt);
}

// ======================================================
// Dano no inimigo
// ======================================================
void DamageEnemy(Enemy *e, float dmg) {
    e->hp -= dmg;

    if (e->hp <= 0.0f) {
        e->alive = false;
        e->hp = 0.0f;
    }
}

// ======================================================
// Desenhar o inimigo
// ======================================================
void DrawEnemy(Enemy *e) {
    if (!e->alive) return;

    float scale = 2.0f;

    Rectangle dest = {
        e->position.x - (e->sprite.frame.width * scale) / 2,
        e->position.y - (e->sprite.frame.height * scale) / 2,
        e->sprite.frame.width * scale,
        e->sprite.frame.height * scale
    };

    DrawTexturePro(
        e->sprite.sheet,
        e->sprite.frame,
        dest,
        (Vector2){0, 0},
        0,
        WHITE
    );
}