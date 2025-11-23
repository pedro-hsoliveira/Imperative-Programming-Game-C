// door_manager.c
// Gerado pelo assistente — usa radius = 18.0f (TELEPORT_CLEARANCE = 25.0f) por padrão
// O código foi corrigido para prevenir teletransportes em loop, movendo
// o player para FORA da porta de destino.

#include "raylib.h"

// -----------------------------------------------------------------------------
// CONFIGURAÇÃO Padrão das portas laterais (valores que você passou)
// -----------------------------------------------------------------------------
static Rectangle left_door  = {126, 616, 131, 109};
static Rectangle right_door = {1560, 614, 164, 115};

// Retângulos de sala (preenchidos via DoorManager_SetRooms)
static Rectangle roomMain = {0};
static Rectangle roomHori = {0};
static Rectangle roomVert = {0};

// Portas verticais calculadas
static Rectangle topDoor = {0};
static Rectangle bottomDoor = {0};

// Distância extra para garantir que o player não reapareça colidindo na porta
static const float TELEPORT_CLEARANCE = 25.0f; // ajuste se precisar

// -----------------------------------------------------------------------------
// Funções utilitárias estáticas (Declarações antecipadas)
// -----------------------------------------------------------------------------
static void ComputeVerticalDoors(void);
static bool IsInsidePlayableArea(Vector2 pos, float radius);
static void ClampInsideRoomMain(Vector2 *pos, float radius);


// -----------------------------------------------------------------------------
// API pública: setar as salas (chame isso a partir do seu código principal)
// -----------------------------------------------------------------------------
void DoorManager_SetRooms(Rectangle mainRoom, Rectangle hori, Rectangle vert)
{
    roomMain = mainRoom;
    roomHori = hori;
    roomVert = vert;

    // calcula portas verticais imediatamente
    ComputeVerticalDoors();
}

// -----------------------------------------------------------------------------
// Funções utilitárias estáticas (Definições)
// -----------------------------------------------------------------------------
static void ComputeVerticalDoors(void)
{
    // recalcula com base nos retângulos atuais (segurança se as rooms mudarem)
    
    // TOP Door
    topDoor.x = roomVert.x;
    topDoor.width = roomVert.width;
    topDoor.y = roomMain.y;
    topDoor.height = roomHori.y - roomMain.y;
    if (topDoor.height < 0) topDoor.height = 0;

    // BOTTOM Door
    bottomDoor.x = roomVert.x;
    bottomDoor.width = roomVert.width;
    bottomDoor.y = roomHori.y + roomHori.height;
    bottomDoor.height = (roomMain.y + roomMain.height) - bottomDoor.y;
    if (bottomDoor.height < 0) bottomDoor.height = 0;
}

// Verifica se o círculo do player está totalmente dentro das áreas principais
// ou está em contato com alguma porta (permitindo a entrada)
static bool IsInsidePlayableArea(Vector2 pos, float radius)
{
    Rectangle* areas[] = { &roomMain, &roomHori, &roomVert };

    // 1. Checa se está inteiramente dentro de alguma das áreas principais
    for (int i = 0; i < 3; i++)
    {
        Rectangle r = *areas[i];

        if (pos.x - radius >= r.x &&
            pos.x + radius <= r.x + r.width &&
            pos.y - radius >= r.y &&
            pos.y + radius <= r.y + r.height)
        {
            return true;
        }
    }

    // 2. Permite colisão com as portas para que o player consiga entrar nelas
    if (CheckCollisionCircleRec(pos, radius, left_door))  return true;
    if (CheckCollisionCircleRec(pos, radius, right_door)) return true;
    if (CheckCollisionCircleRec(pos, radius, topDoor))    return true;
    if (CheckCollisionCircleRec(pos, radius, bottomDoor)) return true;

    return false;
}

// Proteção: garante que a posição esteja dentro do roomMain caso algo dê errado
static void ClampInsideRoomMain(Vector2 *pos, float radius)
{
    float minX = roomMain.x + radius;
    float maxX = roomMain.x + roomMain.width - radius;
    float minY = roomMain.y + radius;
    float maxY = roomMain.y + roomMain.height - radius;

    if (pos->x < minX) pos->x = minX;
    if (pos->x > maxX) pos->x = maxX;
    if (pos->y < minY) pos->y = minY;
    if (pos->y > maxY) pos->y = maxY;
}

// -----------------------------------------------------------------------------
// Função principal: atualiza teleporte entre portas SEM CRIAR LOOPS
// -----------------------------------------------------------------------------
void DoorManager_UpdateTransition(Vector2 *playerPos, float radius)
{
    // Garante que portas verticais estejam atualizadas
    ComputeVerticalDoors();

    Vector2 p = *playerPos;

    // -----------------------------------------------------------
    // LEFT -> RIGHT
    // (Player toca na porta esquerda, reaparece À DIREITA da porta direita)
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(p, radius, left_door))
    {
        // Nova posição: lado de fora (à direita) da porta oposta
        playerPos->x = right_door.x + right_door.width + radius + TELEPORT_CLEARANCE;

        if (!IsInsidePlayableArea(*playerPos, radius)) ClampInsideRoomMain(playerPos, radius);
        return;
    }

    // -----------------------------------------------------------
    // RIGHT -> LEFT
    // (Player toca na porta direita, reaparece À ESQUERDA da porta esquerda)
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(p, radius, right_door))
    {
        // Nova posição: lado de fora (à esquerda) da porta oposta
        playerPos->x = left_door.x - radius - TELEPORT_CLEARANCE;
        
        if (!IsInsidePlayableArea(*playerPos, radius)) ClampInsideRoomMain(playerPos, radius);
        return;
    }

    // -----------------------------------------------------------
    // TOP -> BOTTOM
    // (Player toca na porta superior, reaparece ABAIXO da porta inferior)
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(p, radius, topDoor))
    {
        // Nova posição: lado de fora (abaixo) da porta oposta
        playerPos->y = bottomDoor.y + bottomDoor.height + radius + TELEPORT_CLEARANCE;
        
        if (!IsInsidePlayableArea(*playerPos, radius)) ClampInsideRoomMain(playerPos, radius);
        return;
    }

    // -----------------------------------------------------------
    // BOTTOM -> TOP
    // (Player toca na porta inferior, reaparece ACIMA da porta superior)
    // -----------------------------------------------------------
    if (CheckCollisionCircleRec(p, radius, bottomDoor))
    {
        // Nova posição: lado de fora (acima) da porta oposta
        playerPos->y = topDoor.y - radius - TELEPORT_CLEARANCE;
        
        if (!IsInsidePlayableArea(*playerPos, radius)) ClampInsideRoomMain(playerPos, radius);
        return;
    }
}

// -----------------------------------------------------------------------------
// Função de debug para desenhar as portas na tela (chame dentro do Draw loop)
// -----------------------------------------------------------------------------
void DoorManager_DrawDebug(void)
{
    // desenha portas laterais
    DrawRectangleLinesEx(left_door, 2.0f, (Color){255, 0, 0, 255});
    DrawRectangleLinesEx(right_door, 2.0f, (Color){255, 0, 0, 255});

    // desenha portas verticais
    DrawRectangleLinesEx(topDoor, 2.0f, (Color){0, 255, 0, 255});
    DrawRectangleLinesEx(bottomDoor, 2.0f, (Color){0, 255, 0, 255});

    // desenha limites das rooms (caso tenha setado)
    DrawRectangleLinesEx(roomMain, 2.0f, (Color){200, 50, 50, 200});
    DrawRectangleLinesEx(roomHori, 2.0f, (Color){50, 50, 200, 200});
    DrawRectangleLinesEx(roomVert, 2.0f, (Color){50, 200, 50, 200});
}

// -----------------------------------------------------------------------------
// Opcional: funções para ajustar portas laterais em tempo de execução
// -----------------------------------------------------------------------------
void DoorManager_SetLeftDoor(Rectangle r)  { left_door = r; }
void DoorManager_SetRightDoor(Rectangle r) { right_door = r; }

// -----------------------------------------------------------------------------
// FIM do arquivo
// -----------------------------------------------------------------------------