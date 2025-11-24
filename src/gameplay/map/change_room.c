#include "change_room.h"

Rectangle left_door = {74.67, 340.98, 66.69, 70.84};
Rectangle right_door = {892.05, 340.98, 71.25, 70.84};
Rectangle top_door = {490.18, 75.00, 40, 50};
Rectangle bottom_door = {498.18, 650.68, 40, 50};

static const float TELEPORT_CLEARANCE = 25.0f; // ajuste se precisar

bool next_room_available(opened_door door, Map map) {
    switch (door) {
        case (TOP):
            if (map.currentRoomY - 1 >= 0) {
                if (map.grid[map.currentRoomX][map.currentRoomY - 1].exists) return true;
            }
            break;

        case (BOTTOM):
            if (map.currentRoomY + 1 < GRID_SIZE) {
                if (map.grid[map.currentRoomX][map.currentRoomY + 1].exists) return true;
            }
            break;

        case (LEFT):
            if (map.currentRoomX - 1 >= 0) {
                if (map.grid[map.currentRoomX - 1][map.currentRoomY].exists) return true;
            }
            break;

        case (RIGHT):
            if (map.currentRoomX + 1 < GRID_SIZE) {
                if (map.grid[map.currentRoomX + 1][map.currentRoomY].exists) return true;
            }
            break;
    }

    return false;
}

opened_door door_colision_verifier(Player player) {

    // -----------------------------------------------------------
    // LEFT -> RIGHT
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(player.head.position, player.hitboxRadius, left_door))
    {
        return LEFT;

    }

    // -----------------------------------------------------------
    // RIGHT -> LEFT
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(player.head.position, player.hitboxRadius, right_door))
    {
        return RIGHT;
        
    }

    // -----------------------------------------------------------
    // TOP -> BOTTOM
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(player.head.position, player.hitboxRadius, top_door))
    {
        return TOP;
        
    }

    // -----------------------------------------------------------
    // BOTTOM -> TOP
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(player.head.position, player.hitboxRadius, bottom_door))
    {
        return BOTTOM;
        
    }
    
    return NONE;

}

void room_changer(Player *player)
{
    Vector2 p = player->head.position;

    // -----------------------------------------------------------
    // LEFT -> RIGHT
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(p, player->hitboxRadius, left_door))
    {
        // Nova posição: lado de fora (à direita) da porta oposta
        player->body.position.x = right_door.x - player->hitboxRadius - TELEPORT_CLEARANCE;

    }

    // -----------------------------------------------------------
    // RIGHT -> LEFT
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(p, player->hitboxRadius, right_door))
    {
        // Nova posição: lado de fora (à esquerda) da porta oposta
        player->body.position.x = left_door.x + left_door.width + player->hitboxRadius + TELEPORT_CLEARANCE + 20;
        
    }

    // -----------------------------------------------------------
    // TOP -> BOTTOM
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(p, player->hitboxRadius, top_door))
    {
        // Nova posição: lado de fora (abaixo) da porta oposta
        player->body.position.y = bottom_door.y - player->hitboxRadius - TELEPORT_CLEARANCE;
        
    }

    // -----------------------------------------------------------
    // BOTTOM -> TOP
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(p, player->hitboxRadius, bottom_door))
    {
        // Nova posição: lado de fora (acima) da porta oposta
        player->body.position.y = top_door.y + top_door.height + player->hitboxRadius + 2*TELEPORT_CLEARANCE;
        
    }
}