#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h> // Para memset
#include <stdlib.h> // Para abs
#include <time.h>   // Para a semente aleatória real

//----------------------------------------------------------------------------------
// ESTRUTURAS E DEFINIÇÕES GLOBAIS
//----------------------------------------------------------------------------------

// --- Mapa ---
#define GRID_SIZE 8
#define MAX_ROOMS 16

typedef enum {
    ROOM_EMPTY,
    ROOM_START,
    ROOM_COMBAT,
    ROOM_BOSS
} RoomType;

typedef struct Room {
    bool exists;
    RoomType type;
} Room;

static Room map[GRID_SIZE][GRID_SIZE] = { 0 };
static int playerRoomX = 0;
static int playerRoomY = 0;
static int bossRoomX = 0;
static int bossRoomY = 0;

// --- Jogo ---
typedef enum {
    STATE_PLAYING,
    STATE_TRANSITIONING
} GameState;

// --- Entidades ---
#define MAX_PROJECTILES 50
typedef struct Projectile {
    Rectangle rect;      
    Vector2 velocity;
    bool active;
    Color color;
} Projectile;

#define MAX_ENEMIES 20
typedef struct Enemy {
    Rectangle rect;     
    bool active;
    int generation;    // 0 = Grande, 1 = Pequeno
    Color color;
    
    // ### NOVO: Para o "burst" da divisão ###
    Vector2 velocity;   // Velocidade atual (para IA ou para o burst)
    float spawnTimer;  // Timer para o estado de "spawn"
} Enemy;

// "Pools" globais
static Projectile projectiles[MAX_PROJECTILES] = { 0 };
static Enemy enemies[MAX_ENEMIES] = { 0 };


//----------------------------------------------------------------------------------
// FUNÇÕES AUXILIARES (Helpers)
//----------------------------------------------------------------------------------

void ClearAllEntities()
{
    for (int i = 0; i < MAX_PROJECTILES; i++) projectiles[i].active = false;
    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].active = false;
}

// ### CORREÇÃO: SpawnEnemy agora aceita uma velocity inicial ###
void SpawnEnemy(Vector2 position, float size, int generation, Vector2 initialVelocity)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].rect = (Rectangle){ position.x, position.y, size, size };
            enemies[i].active = true;
            enemies[i].generation = generation;
            enemies[i].color = (generation == 0) ? YELLOW : ORANGE;
            
            // Define a velocidade e o timer de spawn
            enemies[i].velocity = initialVelocity;
            enemies[i].spawnTimer = 0.3f; // 0.3 segundos de "burst"
            break; 
        }
    }
}

void SpawnProjectile(Vector2 position, Vector2 velocity)
{
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].active)
        {
            projectiles[i].rect = (Rectangle){ position.x, position.y, 10, 10 };
            projectiles[i].velocity = velocity;
            projectiles[i].active = true;
            projectiles[i].color = BLACK;
            break;
        }
    }
}

// Geração do Mapa 16x16
void GenerateMap(void)
{
    int roomsCreated = 0;

    do {
        memset(map, 0, sizeof(map));
        roomsCreated = 0;
        int frontierCount = 0;
        Vector2 frontier[GRID_SIZE * GRID_SIZE];
        
        playerRoomX = GetRandomValue(1, GRID_SIZE - 2);
        playerRoomY = GetRandomValue(1, GRID_SIZE - 2);
        map[playerRoomX][playerRoomY].exists = true;
        map[playerRoomX][playerRoomY].type = ROOM_START;
        
        frontier[frontierCount++] = (Vector2){ playerRoomX, playerRoomY };
        roomsCreated = 1;
        int currentFrontierIndex = 0;
        
        int dx[] = { 0, 0, 1, -1 };
        int dy[] = { -1, 1, 0, 0 };

        while (roomsCreated < MAX_ROOMS && currentFrontierIndex < frontierCount)
        {
            Vector2 current = frontier[currentFrontierIndex++];
            
            for (int i = 0; i < 4; i++)
            {
                int nx = current.x + dx[i];
                int ny = current.y + dy[i];
                
                if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE && !map[nx][ny].exists)
                {
                    if (GetRandomValue(0, 2) == 0) 
                    {
                        map[nx][ny].exists = true;
                        
                        if (GetRandomValue(0, 1) == 0) {
                            map[nx][ny].type = ROOM_COMBAT;
                        } else {
                            map[nx][ny].type = ROOM_EMPTY;
                        }
                        
                        frontier[frontierCount++] = (Vector2){ nx, ny };
                        roomsCreated++;
                    }
                }
            }
        }
        
    } while (roomsCreated < 8); // <-- AQUI ESTÁ A SEGURANÇA. Garante no mínimo 8 salas.


    // 4. Escolhe a sala do boss
    
    int bestBossX = -1;
    int bestBossY = -1;
    int maxDist = -1;

    // Varre todo o grid procurando a sala mais distante
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (map[x][y].exists && map[x][y].type != ROOM_START)
            {
                int dist = abs(x - playerRoomX) + abs(y - playerRoomY);
                if (dist > maxDist)
                {
                    maxDist = dist;
                    bestBossX = x;
                    bestBossY = y;
                }
            }
        }
    }

    // Aplica o Boss na melhor sala encontrada
    if (bestBossX != -1)
    {
        map[bestBossX][bestBossY].type = ROOM_BOSS;
        bossRoomX = bestBossX;
        bossRoomY = bestBossY;
    }
}

void DrawMiniMap(void)
{
    int mapPadding = 10;
    int cellSize = 12;
    
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            int drawX = mapPadding + x * cellSize;
            int drawY = mapPadding + y * cellSize;
            
            if (map[x][y].exists)
            {
                Color roomColor = GRAY;
                
                if (map[x][y].type == ROOM_BOSS) roomColor = RED;
                if (x == playerRoomX && y == playerRoomY) roomColor = GREEN;
                
                DrawRectangle(drawX, drawY, cellSize - 1, cellSize - 1, roomColor);
                
                if (map[x][y].type == ROOM_BOSS)
                {
                    DrawText("B", drawX + 2, drawY + 1, 10, BLACK);
                }
            }
            else
            {
                DrawRectangleLines(drawX, drawY, cellSize - 1, cellSize - 1, LIGHTGRAY);
            }
        }
    }
}

void DrawRoom(int rx, int ry, float roomWorldX, float roomWorldY, float boundarySize, float boundaryThickness, float doorSize)
{
    float halfSize = boundarySize / 2.0f;
    Color roomColor = BLACK;

    bool hasTopDoor = (ry > 0 && map[rx][ry - 1].exists);
    bool hasBottomDoor = (ry < GRID_SIZE - 1 && map[rx][ry + 1].exists);
    bool hasLeftDoor = (rx > 0 && map[rx - 1][ry].exists);
    bool hasRightDoor = (rx < GRID_SIZE - 1 && map[rx + 1][ry].exists);
    
    float doorMinY = roomWorldY - doorSize / 2.0f;
    float doorMaxY = roomWorldY + doorSize / 2.0f;
    float doorMinX = roomWorldX - doorSize / 2.0f;
    float doorMaxX = roomWorldX + doorSize / 2.0f;

    float wallMinY = roomWorldY - halfSize;
    float wallMaxY = roomWorldY + halfSize;
    float wallMinX = roomWorldX - halfSize;
    float wallMaxX = roomWorldX + halfSize;
    
    // Parede Esquerda
    if (hasLeftDoor)
    {
        DrawRectangle(wallMinX, wallMinY, boundaryThickness, (wallMaxY - wallMinY - doorSize) / 2.0f, roomColor);
        DrawRectangle(wallMinX, doorMaxY, boundaryThickness, (wallMaxY - wallMinY - doorSize) / 2.0f, roomColor);
    }
    else
    {
        DrawRectangle(wallMinX, wallMinY, boundaryThickness, wallMaxY - wallMinY, roomColor);
    }
    
    // Parede Direita
    if (hasRightDoor)
    {
        DrawRectangle(wallMaxX - boundaryThickness, wallMinY, boundaryThickness, (wallMaxY - wallMinY - doorSize) / 2.0f, roomColor);
        DrawRectangle(wallMaxX - boundaryThickness, doorMaxY, boundaryThickness, (wallMaxY - wallMinY - doorSize) / 2.0f, roomColor);
    }
    else
    {
        DrawRectangle(wallMaxX - boundaryThickness, wallMinY, boundaryThickness, wallMaxY - wallMinY, roomColor);
    }

    // Parede Topo
    if (hasTopDoor)
    {
        DrawRectangle(wallMinX, wallMinY, (wallMaxX - wallMinX - doorSize) / 2.0f, boundaryThickness, roomColor);
        DrawRectangle(doorMaxX, wallMinY, (wallMaxX - wallMinX - doorSize) / 2.0f, boundaryThickness, roomColor);
    }
    else
    {
        DrawRectangle(wallMinX, wallMinY, wallMaxX - wallMinX, boundaryThickness, roomColor);
    }
    
    // Parede Fundo
    if (hasBottomDoor)
    {
        DrawRectangle(wallMinX, wallMaxY - boundaryThickness, (wallMaxX - wallMinX - doorSize) / 2.0f, boundaryThickness, roomColor);
        DrawRectangle(doorMaxX, wallMaxY - boundaryThickness, (wallMaxX - wallMinX - doorSize) / 2.0f, boundaryThickness, roomColor);
    }
    else
    {
        DrawRectangle(wallMinX, wallMaxY - boundaryThickness, wallMaxX - wallMinX, boundaryThickness, roomColor);
    }
    
    if (map[rx][ry].type == ROOM_BOSS)
    {
        DrawText("SALA DO BOSS", roomWorldX - 100, roomWorldY, 30, RED);
    }
}


//----------------------------------------------------------------------------------
// PONTO DE ENTRADA PRINCIPAL (main)
//----------------------------------------------------------------------------------
int main(void)
{
    // 1. Configuração da Janela
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Isaac de Pobre 2.0");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL)); // Semente para o mapa

    // 2. Definir o Jogo
    float boundarySize = 400.0f;
    float boundaryThickness = 10.0f; 
    float doorSize = 80.0f;
    
    GenerateMap();
    
    // 3. Definir o Jogador
    float playerSize = 50.0f;
    float playerSpeed = 5.0f;
    Vector2 playerPosition = { 
        (playerRoomX * screenWidth) - playerSize / 2.0f, 
        (playerRoomY * screenHeight) - playerSize / 2.0f 
    };
    
    // I-Frames (Invencibilidade)
    float playerInvincibilityTimer = 0.0f;
    const float playerInvincibilityDuration = 2.0f;
    
    // 4. Controle de Visibilidade e Posição
    bool isPlayerVisible = true;
    int transitionDirection = 0; // 0=Nenhum, 1=Cima, 2=Baixo, 3=Esquerda, 4=Direita

    // 5. Controle de Mira
    Vector2 aimDirection = { 0.0f, 1.0f };
    float miraSize = 15.0f;
    float miraOffset = 40.0f;

    // 6. Controle de Tiro
    float shootTimer = 0.0f;
    const float shootCooldown = 0.4f;
    const float projectileSpeed = 8.0f;
    
    // 7. Configuração da Câmera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ playerRoomX * screenWidth, playerRoomY * screenHeight }; 
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; 
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // 8. Controle de Transição
    GameState gameState = STATE_PLAYING;
    Vector2 cameraTarget = camera.target; 
    float transitionSpeed = 500.0f; 

    char windowTitle[100];

    // 9. Loop Principal do Jogo
    while (!WindowShouldClose())
    {
        // 10. Atualizar (Update)
        float moveDelta = transitionSpeed * GetFrameTime();
        shootTimer -= GetFrameTime();
        
        if (playerInvincibilityTimer > 0.0f) playerInvincibilityTimer -= GetFrameTime();

        switch (gameState)
        {
            case STATE_PLAYING:
            {
                // --- Lógica de Mira e Movimento Combinada ---
                bool isAiming = false;
                if (IsKeyDown(KEY_UP)) { aimDirection = (Vector2){ 0.0f, -1.0f }; isAiming = true; }
                else if (IsKeyDown(KEY_DOWN)) { aimDirection = (Vector2){ 0.0f, 1.0f }; isAiming = true; }
                else if (IsKeyDown(KEY_LEFT)) { aimDirection = (Vector2){ -1.0f, 0.0f }; isAiming = true; }
                else if (IsKeyDown(KEY_RIGHT)) { aimDirection = (Vector2){ 1.0f, 0.0f }; isAiming = true; }
                
                bool isMoving = false;
                Vector2 moveInput = { 0.0f, 0.0f }; 
                if (IsKeyDown(KEY_W)) { playerPosition.y -= playerSpeed; moveInput.y = -1.0f; isMoving = true; }
                if (IsKeyDown(KEY_S)) { playerPosition.y += playerSpeed; moveInput.y = 1.0f; isMoving = true; }
                if (IsKeyDown(KEY_A)) { playerPosition.x -= playerSpeed; moveInput.x = -1.0f; isMoving = true; }
                if (IsKeyDown(KEY_D)) { playerPosition.x += playerSpeed; moveInput.x = 1.0f; isMoving = true; }

                if (!isAiming && isMoving)
                {
                    if (moveInput.y == -1.0f) { aimDirection = (Vector2){ 0.0f, -1.0f }; }
                    else if (moveInput.y == 1.0f) { aimDirection = (Vector2){ 0.0f, 1.0f }; }
                    else if (moveInput.x == -1.0f) { aimDirection = (Vector2){ -1.0f, 0.0f }; }
                    else if (moveInput.x == 1.0f) { aimDirection = (Vector2){ 1.0f, 0.0f }; }
                }
                
                // --- Lógica de Tiro ---
                if (IsKeyDown(KEY_SPACE) && shootTimer <= 0.0f)
                {
                    Vector2 playerCenter = { playerPosition.x + playerSize / 2.0f, playerPosition.y + playerSize / 2.0f };
                    Vector2 miraCenter = Vector2Add(playerCenter, Vector2Scale(aimDirection, miraOffset));
                    Vector2 projVelocity = Vector2Scale(aimDirection, projectileSpeed);
                    
                    SpawnProjectile(miraCenter, projVelocity);
                    shootTimer = shootCooldown;
                }

                // --- Lógica de Colisão de Transição (4 DIREÇÕES) ---
                float roomWorldX = playerRoomX * screenWidth;
                float roomWorldY = playerRoomY * screenHeight;
                float halfSize = boundarySize / 2.0f;
                
                float roomMinX = roomWorldX - halfSize + boundaryThickness;
                float roomMaxX = roomWorldX + halfSize - boundaryThickness;
                float roomMinY = roomWorldY - halfSize + boundaryThickness;
                float roomMaxY = roomWorldY + halfSize - boundaryThickness;
                
                float doorH_MinY = roomWorldY - doorSize / 2.0f;
                float doorH_MaxY = roomWorldY + doorSize / 2.0f;
                float doorV_MinX = roomWorldX - doorSize / 2.0f;
                float doorV_MaxX = roomWorldX + doorSize / 2.0f;

                bool alignedH = (playerPosition.y > doorH_MinY && playerPosition.y + playerSize < doorH_MaxY);
                bool hasLeftDoor = (playerRoomX > 0 && map[playerRoomX - 1][playerRoomY].exists);
                bool hasRightDoor = (playerRoomX < GRID_SIZE - 1 && map[playerRoomX + 1][playerRoomY].exists);
                
                bool alignedV = (playerPosition.x > doorV_MinX && playerPosition.x + playerSize < doorV_MaxX);
                bool hasTopDoor = (playerRoomY > 0 && map[playerRoomX][playerRoomY - 1].exists);
                bool hasBottomDoor = (playerRoomY < GRID_SIZE - 1 && map[playerRoomX][playerRoomY + 1].exists);
                
                // Colisão Cima
                if (playerPosition.y < roomMinY) {
                    if (hasTopDoor && alignedV) {
                        ClearAllEntities(); playerRoomY--; transitionDirection = 1; gameState = STATE_TRANSITIONING;
                        cameraTarget = (Vector2){ playerRoomX * screenWidth, playerRoomY * screenHeight };
                        isPlayerVisible = false; 
                    } else {
                        playerPosition.y = roomMinY;
                    }
                }
                // Colisão Fundo
                if (playerPosition.y + playerSize > roomMaxY) {
                    if (hasBottomDoor && alignedV) {
                        ClearAllEntities(); playerRoomY++; transitionDirection = 2; gameState = STATE_TRANSITIONING;
                        cameraTarget = (Vector2){ playerRoomX * screenWidth, playerRoomY * screenHeight };
                        isPlayerVisible = false; 
                    } else {
                        playerPosition.y = roomMaxY - playerSize;
                    }
                }
                // Colisão Esquerda
                if (playerPosition.x < roomMinX) {
                    if (hasLeftDoor && alignedH) {
                        ClearAllEntities(); playerRoomX--; transitionDirection = 3; gameState = STATE_TRANSITIONING;
                        cameraTarget = (Vector2){ playerRoomX * screenWidth, playerRoomY * screenHeight };
                        isPlayerVisible = false; 
                    } else {
                        playerPosition.x = roomMinX;
                    }
                }
                // Colisão Direita
                if (playerPosition.x + playerSize > roomMaxX) {
                    if (hasRightDoor && alignedH) {
                        ClearAllEntities(); playerRoomX++; transitionDirection = 4; gameState = STATE_TRANSITIONING;
                        cameraTarget = (Vector2){ playerRoomX * screenWidth, playerRoomY * screenHeight };
                        isPlayerVisible = false; 
                    } else {
                        playerPosition.x = roomMaxX - playerSize;
                    }
                }
                
                // --- Update Projéteis ---
                for (int i = 0; i < MAX_PROJECTILES; i++)
                {
                    if (projectiles[i].active)
                    {
                        projectiles[i].rect.x += projectiles[i].velocity.x;
                        projectiles[i].rect.y += projectiles[i].velocity.y;
                        
                        if (projectiles[i].rect.x < roomMinX || projectiles[i].rect.x > roomMaxX ||
                            projectiles[i].rect.y < roomMinY || projectiles[i].rect.y > roomMaxY)
                        {
                            projectiles[i].active = false;
                        }
                    }
                }
                
                // --- Update Inimigos ---
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemies[i].active)
                    {
                        // ### CORREÇÃO: Lógica de Estado do Inimigo (Spawn vs. IA) ###
                        
                        if (enemies[i].spawnTimer > 0.0f)
                        {
                            // --- ESTADO DE SPAWN (Burst) ---
                            enemies[i].spawnTimer -= GetFrameTime();
                            
                            // Move-se pela velocidade de "burst"
                            enemies[i].rect.x += enemies[i].velocity.x;
                            enemies[i].rect.y += enemies[i].velocity.y;
                            
                            // Aplica atrito (desaceleração)
                            enemies[i].velocity = Vector2Scale(enemies[i].velocity, 0.95f);
                        }
                        else
                        {
                            // --- ESTADO DE IA (Seguir o Player) ---
                            Vector2 enemyCenter = { enemies[i].rect.x + enemies[i].rect.width/2, enemies[i].rect.y + enemies[i].rect.height/2 };
                            Vector2 playerCenter = { playerPosition.x + playerSize/2, playerPosition.y + playerSize/2 };
                            Vector2 direction = Vector2Normalize(Vector2Subtract(playerCenter, enemyCenter));
                            float enemySpeed = (enemies[i].generation == 0) ? 1.0f : 1.5f;
                            
                            // Define a velocidade da IA
                            enemies[i].velocity = Vector2Scale(direction, enemySpeed);
                            
                            // Move o inimigo
                            enemies[i].rect.x += enemies[i].velocity.x;
                            enemies[i].rect.y += enemies[i].velocity.y;
                        }

                        
                        // Colisão do Inimigo com Paredes (sempre ativa)
                        if (enemies[i].rect.x < roomMinX) enemies[i].rect.x = roomMinX;
                        if (enemies[i].rect.x + enemies[i].rect.width > roomMaxX) enemies[i].rect.x = roomMaxX - enemies[i].rect.width;
                        if (enemies[i].rect.y < roomMinY) enemies[i].rect.y = roomMinY;
                        if (enemies[i].rect.y + enemies[i].rect.height > roomMaxY) enemies[i].rect.y = roomMaxY - enemies[i].rect.height;

                        // Colisão Inimigo -> Inimigo
                        for (int j = 0; j < MAX_ENEMIES; j++)
                        {
                            if (i == j || !enemies[j].active) continue; 

                            if (CheckCollisionRecs(enemies[i].rect, enemies[j].rect))
                            {
                                Vector2 enemyCenter_i = { enemies[i].rect.x + enemies[i].rect.width/2, enemies[i].rect.y + enemies[i].rect.height/2 };
                                Vector2 enemyCenter_j = { enemies[j].rect.x + enemies[j].rect.width/2, enemies[j].rect.y + enemies[j].rect.height/2 };
                                Vector2 pushDirection = Vector2Normalize(Vector2Subtract(enemyCenter_i, enemyCenter_j));

                                if (pushDirection.x == 0 && pushDirection.y == 0)
                                {
                                    pushDirection = (Vector2){ 1.0f, 0.0f };
                                }
                                
                                float pushAmount = 0.5f; 
                                enemies[i].rect.x += pushDirection.x * pushAmount;
                                enemies[i].rect.y += pushDirection.y * pushAmount;
                                enemies[j].rect.x -= pushDirection.x * pushAmount;
                                enemies[j].rect.y -= pushDirection.y * pushAmount;
                            }
                        }

                        // Colisão Inimigo -> Player (com Knockback)
                        Rectangle playerRect = { playerPosition.x, playerPosition.y, playerSize, playerSize };
                        if (CheckCollisionRecs(enemies[i].rect, playerRect) && playerInvincibilityTimer <= 0.0f)
                        {
                            playerInvincibilityTimer = playerInvincibilityDuration; 
                            
                            // Pega a direção do inimigo para o player
                            Vector2 playerCenter = { playerPosition.x + playerSize/2, playerPosition.y + playerSize/2 };
                            Vector2 enemyCenter = { enemies[i].rect.x + enemies[i].rect.width/2, enemies[i].rect.y + enemies[i].rect.height/2 };
                            Vector2 knockbackDir = Vector2Normalize(Vector2Subtract(playerCenter, enemyCenter));

                            float knockbackStrength = 35.0f;
                            playerPosition.x += knockbackDir.x * knockbackStrength;
                            playerPosition.y += knockbackDir.y * knockbackStrength;
                            
                            enemies[i].rect.x -= knockbackDir.x * 20.0f;
                            enemies[i].rect.y -= knockbackDir.y * 20.0f;
                        }
                        
                        // Colisão Projétil -> Inimigo
                        for (int j = 0; j < MAX_PROJECTILES; j++)
                        {
                            if (projectiles[j].active && CheckCollisionRecs(enemies[i].rect, projectiles[j].rect))
                            {
                                projectiles[j].active = false; 
                                enemies[i].active = false;     

                                // ### CORREÇÃO: Slimes se espalham com velocidade ###
                                if (enemies[i].generation == 0)
                                {
                                    float parentWidth = enemies[i].rect.width;
                                    Vector2 pos1 = { enemies[i].rect.x - (parentWidth / 4.0f), enemies[i].rect.y };
                                    Vector2 pos2 = { enemies[i].rect.x + (parentWidth / 4.0f), enemies[i].rect.y };

                                    // Define as velocidades de "burst" (diagonal, para longe)
                                    Vector2 vel1 = { -4.0f, -2.0f }; // Cima-Esquerda
                                    Vector2 vel2 = { 4.0f, -2.0f };  // Cima-Direita

                                    SpawnEnemy(pos1, 25.0f, 1, vel1);
                                    SpawnEnemy(pos2, 25.0f, 1, vel2);
                                }
                            }
                        }
                    }
                }
                
            } break;
            
            case STATE_TRANSITIONING:
            {
                camera.target = Vector2MoveTowards(camera.target, cameraTarget, moveDelta);
                
                if (Vector2Distance(camera.target, cameraTarget) < 1.0f)
                {
                    camera.target = cameraTarget;
                    gameState = STATE_PLAYING;
                    isPlayerVisible = true; 
                    
                    float roomWorldX = playerRoomX * screenWidth;
                    float roomWorldY = playerRoomY * screenHeight;
                    float halfSize = boundarySize / 2.0f;
                    
                    if (transitionDirection == 1) // Veio de Baixo
                    {
                        playerPosition.x = roomWorldX - playerSize / 2.0f;
                        playerPosition.y = (roomWorldY + halfSize - boundaryThickness) - playerSize - 5.0f;
                    }
                    else if (transitionDirection == 2) // Veio de Cima
                    {
                        playerPosition.x = roomWorldX - playerSize / 2.0f;
                        playerPosition.y = (roomWorldY - halfSize + boundaryThickness) + 5.0f;
                    }
                    else if (transitionDirection == 3) // Veio da Direita
                    {
                        playerPosition.x = (roomWorldX + halfSize - boundaryThickness) - playerSize - 5.0f;
                        playerPosition.y = roomWorldY - playerSize / 2.0f;
                    }
                    else if (transitionDirection == 4) // Veio da Esquerda
                    {
                        playerPosition.x = (roomWorldX - halfSize + boundaryThickness) + 5.0f;
                        playerPosition.y = roomWorldY - playerSize / 2.0f;
                    }
                    
                    transitionDirection = 0; 
                    
                    if (map[playerRoomX][playerRoomY].type == ROOM_COMBAT)
                    {
                        // ### CORREÇÃO: Spawna com velocidade inicial 0 ###
                        SpawnEnemy(cameraTarget, 50.0f, 0, (Vector2){0,0}); 
                    }
                }
            } break;
        }
        
        sprintf(windowTitle, "Isaac de Pobre [Sala %d, %d]", playerRoomX, playerRoomY);
        SetWindowTitle(windowTitle);

        // 11. Desenhar (Draw)
        BeginDrawing();
            ClearBackground(RAYWHITE); 

            BeginMode2D(camera);
            
                // Desenha as salas
                for (int y = 0; y < GRID_SIZE; y++)
                {
                    for (int x = 0; x < GRID_SIZE; x++)
                    {
                        if (map[x][y].exists)
                        {
                            float roomWorldX = x * screenWidth;
                            float roomWorldY = y * screenHeight;
                            DrawRoom(x, y, roomWorldX, roomWorldY, boundarySize, boundaryThickness, doorSize);
                        }
                    }
                }
                
                // Desenha Inimigos
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemies[i].active)
                    {
                        DrawRectangleRec(enemies[i].rect, enemies[i].color);
                    }
                }
                
                // Desenha Projéteis
                for (int i = 0; i < MAX_PROJECTILES; i++)
                {
                    if (projectiles[i].active)
                    {
                        DrawRectangleRec(projectiles[i].rect, projectiles[i].color);
                    }
                }
                
                // Desenha o Player e a Mira
                if (isPlayerVisible)
                {
                    bool showPlayer = true;
                    if (playerInvincibilityTimer > 0.0f)
                    {
                        if ((int)(playerInvincibilityTimer * 10) % 2 != 0)
                        {
                            showPlayer = false;
                        }
                    }

                    if (showPlayer)
                    {
                        DrawRectangleV(playerPosition, (Vector2){ playerSize, playerSize }, MAROON); 
                    }
                    
                    Vector2 playerCenter = { playerPosition.x + playerSize / 2.0f, playerPosition.y + playerSize / 2.0f };
                    Vector2 miraCenter = Vector2Add(playerCenter, Vector2Scale(aimDirection, miraOffset));
                    Vector2 miraPosition = { miraCenter.x - miraSize / 2.0f, miraCenter.y - miraSize / 2.0f };
                    DrawRectangleV(miraPosition, (Vector2){ miraSize, miraSize }, BLUE);
                }

            EndMode2D();
            
            // DESENHA A UI (MINI-MAPA)
            DrawMiniMap();

        EndDrawing();
    }

    // 12. Encerrar
    CloseWindow();
    return 0;
}