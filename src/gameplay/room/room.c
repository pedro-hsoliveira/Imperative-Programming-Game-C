#include "room.h"

void draw_empty_room(Texture2D room_sprite) {
    DrawTextureEx(room_sprite, (Vector2) {0, 0}, 0, 1, GRAY);

}

void is_combat_over(Map *map) {
    if (map->grid[map->currentRoomX][map->currentRoomY].type == ROOM_COMBAT) {
        map->grid[map->currentRoomX][map->currentRoomY].doors_state = DOORS_OPEN;

        for (int i = 0; i < MAX_ENEMIES; i++) map->grid[map->currentRoomX][map->currentRoomY].doors_state = (map->grid[map->currentRoomX][map->currentRoomY].enemies[i].alive) ? DOORS_CLOSED : map->grid[map->currentRoomX][map->currentRoomY].doors_state;

    }

}
    

void initialize_combat_room(Texture2D room_sprite, Map *map, Player *player) {
    map->grid[map->currentRoomX][map->currentRoomY].enemies_number = MAX_ENEMIES;
    SpawnEnemiesRandom(map->grid[map->currentRoomX][map->currentRoomY].enemies, MAX_ENEMIES, roomMain, player->body.position);
    map->grid[map->currentRoomX][map->currentRoomY].aang_entered = true;

}

void combat_room(Map *map, Player *player, float deltaTime) {
    // Pointer shortcut to the enemies in this specific room
    Enemy *roomEnemies = map->grid[map->currentRoomX][map->currentRoomY].enemies;

    // -----------------------------------------------------------
    // 1. UPDATE & DRAW ENEMIES
    // -----------------------------------------------------------
    for (int i = 0; i < MAX_ENEMIES; i++) {
        // Only update if alive (DrawEnemy/UpdateEnemy usually handle this check too, 
        // but it is good to be explicit)
        if (roomEnemies[i].alive) {
            UpdateEnemy(&roomEnemies[i], player->body.position, deltaTime);
            DrawEnemy(&roomEnemies[i]);

            // Check if Enemy attacks Player
            EnemyTryAttack(&roomEnemies[i], player, deltaTime);
        }
    }

    // -----------------------------------------------------------
    // 2. CHECK COLLISIONS: PLAYER PROJECTILES vs ENEMIES
    // -----------------------------------------------------------
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        // Skip inactive bullets
        if (!player->projectiles[i].active) continue;

        for (int j = 0; j < MAX_ENEMIES; j++) {
            // Skip dead enemies
            if (!roomEnemies[j].alive) continue;

            // Use the helper function from enemy.c
            if (EnemyCheckCollisionWithProjectile(&roomEnemies[j], &player->projectiles[i])) {
                
                // 1. Deal Damage
                EnemyTakeDamage(&roomEnemies[j], 1.0f);

                // 2. Destroy the bullet (so it doesn't pass through)
                player->projectiles[i].active = false;

                // 3. Break inner loop (one bullet hits one enemy)
                break;
            }
        }
    }
    is_combat_over(map);
}

void initialize_boss_room(Texture2D room_sprite, Map *map) {
    map->grid[map->currentRoomX][map->currentRoomY].enemies_number = 1;    

}