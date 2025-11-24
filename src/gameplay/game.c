#include "game.h"

Enemy enemies[MAX_ENEMIES];

void game(Texture2D room_texture, Texture2D aang_texture, Player *player, Map *map) {
    float deltaTime = GetFrameTime();

    draw_empty_room(room_texture);
    // Atualiza player
    UpdatePlayer(player);
    
    if (map->grid[map->currentRoomX][map->currentRoomY].doors_state == DOORS_OPEN){
    opened_door chosen_door = door_colision_verifier(*player);

        if (chosen_door){
            bool avaiable_room = next_room_available(chosen_door, *map);

            if (avaiable_room) {
                if (chosen_door == TOP) map->currentRoomY--;
                else if (chosen_door == BOTTOM) map->currentRoomY++;
                else if (chosen_door == LEFT) map->currentRoomX--;
                else if (chosen_door == RIGHT) map->currentRoomX++;
                
                room_changer(player);
            }
        }
    }
    // Desenhar tudo
    DrawPlayer(player);
    
    switch (map->grid[map->currentRoomX][map->currentRoomY].type) {
        case (ROOM_START):
        case (ROOM_EMPTY):
        break;

        case (ROOM_COMBAT):
            if (!map->grid[map->currentRoomX][map->currentRoomY].aang_entered) initialize_combat_room(room_texture, map, player);
            else combat_room(map, player, deltaTime);

        break;
        case (ROOM_BOSS):
        

    }

    DrawMiniMap(*map);
    
}