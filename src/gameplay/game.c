#include "game.h"

Enemy enemies[MAX_ENEMIES];

void is_game_over(Game *game, Map map);

void game(Texture2D room_texture, Texture2D doors_textures[4], Player *player, Map *map, Game game, int *currentScreen) {
    is_game_over(&game, *map);

    float deltaTime = GetFrameTime();

    draw_empty_room(room_texture);
    // Atualiza player
    UpdatePlayer(player);
    
    if (map->grid[map->currentRoomX][map->currentRoomY].doors_state == DOORS_OPEN){
        draw_doors(*map, doors_textures);
        opened_door chosen_door = door_colision_verifier(*player);

        if (chosen_door){
            bool avaiable_room = next_room_available(chosen_door, *map);

            if (avaiable_room) {
                if (chosen_door == TOP) map->currentRoomY--;
                else if (chosen_door == BOTTOM) map->currentRoomY++;
                else if (chosen_door == LEFT) map->currentRoomX--;
                else if (chosen_door == RIGHT) map->currentRoomX++;
                
                clear_projectiles(player);
                room_changer(player);
            }
        }
    } else {
        DrawTexture(doors_textures[0], 453, 44.74, RAYWHITE);
        DrawTexture(doors_textures[1], 55, 313.5, RAYWHITE);
        DrawTexture(doors_textures[2], 453, 640.71, RAYWHITE);
        DrawTexture(doors_textures[3], 882, 313.5, RAYWHITE);

    }
    DrawPlayer(player);
    
    switch (map->grid[map->currentRoomX][map->currentRoomY].type) {
        case (ROOM_START):
        case (ROOM_EMPTY):
        break;

        case (ROOM_COMBAT):
            if (!map->grid[map->currentRoomX][map->currentRoomY].aang_entered) initialize_combat_room(map, player);
            else combat_room(map, player, deltaTime);

        break;
        case (ROOM_BOSS):
            if (game.isGameOver) {
                *currentScreen = VICTORY;

            }
        

    }

    DrawMiniMap(*map);
    
}

void is_game_over(Game *game, Map map) {
    game->isGameOver = true;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++){
            if (map.grid[i][j].type == ROOM_COMBAT && map.grid[i][j].aang_entered == false) {
                game->isGameOver = false;

            }
        }
    }
}