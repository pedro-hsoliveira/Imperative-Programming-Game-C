#include "door_manager.h"
#include "raymath.h"
#include <stdio.h>

void CheckDoorTransition(Player *player, Map *map)
{
    Vector2 pPos = player->body.position;
    bool changedRoom = false;
    int nextX = map->currentRoomX;
    int nextY = map->currentRoomY;

    // --- Verifica Saída SUPERIOR ---
    if (pPos.y < DOOR_TOP_Y - 10) // -10 de tolerância
    {
        // Tenta ir para sala de CIMA (Y diminui)
        if (nextY > 0 && map->grid[nextY - 1][nextX].exists)
        {
            map->currentRoomY--;
            // Teleporta para a parte DE BAIXO da nova sala
            player->body.position.y = DOOR_BOTTOM_Y - SPAWN_OFFSET;
            changedRoom = true;
        }
        else
        {
            // Bloqueia se não tiver sala
            player->body.position.y = DOOR_TOP_Y;
        }
    }
    // --- Verifica Saída INFERIOR ---
    else if (pPos.y > DOOR_BOTTOM_Y + 10)
    {
        // Tenta ir para sala de BAIXO (Y aumenta)
        if (nextY < MAP_HEIGHT - 1 && map->grid[nextY + 1][nextX].exists)
        {
            map->currentRoomY++;
            // Teleporta para a parte DE CIMA da nova sala
            player->body.position.y = DOOR_TOP_Y + SPAWN_OFFSET;
            changedRoom = true;
        }
        else
        {
            player->body.position.y = DOOR_BOTTOM_Y;
        }
    }
    // --- Verifica Saída ESQUERDA ---
    else if (pPos.x < DOOR_LEFT_X - 10)
    {
        // Tenta ir para sala da ESQUERDA (X diminui)
        if (nextX > 0 && map->grid[nextY][nextX - 1].exists)
        {
            map->currentRoomX--;
            // Teleporta para a DIREITA da nova sala
            player->body.position.x = DOOR_RIGHT_X - SPAWN_OFFSET;
            changedRoom = true;
        }
        else
        {
            player->body.position.x = DOOR_LEFT_X;
        }
    }
    // --- Verifica Saída DIREITA ---
    else if (pPos.x > DOOR_RIGHT_X + 10)
    {
        // Tenta ir para sala da DIREITA (X aumenta)
        if (nextX < MAP_WIDTH - 1 && map->grid[nextY][nextX + 1].exists)
        {
            map->currentRoomX++;
            // Teleporta para a ESQUERDA da nova sala
            player->body.position.x = DOOR_LEFT_X + SPAWN_OFFSET;
            changedRoom = true;
        }
        else
        {
            player->body.position.x = DOOR_RIGHT_X;
        }
    }

    if (changedRoom)
    {
        // Opcional: Limpar projéteis ao mudar de sala
        for (int i = 0; i < MAX_PROJECTILES; i++) player->projectiles[i].active = false;
        
        // Opcional: Resetar inimigos ou estado da sala
        printf("Mudou para Sala [%d][%d]\n", map->currentRoomX, map->currentRoomY);
    }
}