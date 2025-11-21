#include "room.h"

static Room map[GRID_SIZE][GRID_SIZE] = { 0 };
static int playerRoomX = 0;
static int playerRoomY = 0;
static int bossRoomX = 0;
static int bossRoomY = 0;

void dijkstra(Vector2 starting_room) {
    // Atribui todas as distancias iniciais do mapa para infinito 
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            map[i][j].minimum_distance = INFINITY;
            map[i][j].visited = false;

        }
    }

    // baixo, cima, direita e esquerda, respectivamente
    int dx[] = {1, -1, 0, 0,};
    int dy[] = {0, 0, 1, -1,};

    // distancia inicial = 0
    map[(int) starting_room.x][(int) starting_room.y].minimum_distance = 0;

    for (int _ = 0; _ < GRID_SIZE * GRID_SIZE; _++) {
        int min_distance = INFINITY;
        int u = -1, v = -1;

        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (map[i][j].minimum_distance < min_distance && !map[i][j].visited && map[i][j].exists){
                    min_distance = map[i][j].minimum_distance;
                    u = i;
                    v = j;
                }
            }
        }

        if (u != -1) {
            map[u][v].visited = true;
            for (int k = 0; k < 4; k++) {
                if (((0 <= u+ dx[k]) && (u+ dx[k] < GRID_SIZE)) && ((0 <= v+ dy[k]) && (v+ dy[k] < GRID_SIZE))) {
                    if (map[u+ dx[k]][v+ dy[k]].exists && !map[u+ dx[k]][v+ dy[k]].visited) {
                        int alt = 1 + map[u][v].minimum_distance;

                        if (alt < map[u+ dx[k]][v+ dy[k]].minimum_distance) {
                            map[u+ dx[k]][v+ dy[k]].minimum_distance = alt;

                        }
                    
                    }
                }
            }
        }
    }
}

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
                    if (!GetRandomValue(0, 2)) 
                    {
                        map[nx][ny].exists = true;
                        
                        if (!GetRandomValue(0, 1)) {
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

    dijkstra((Vector2) {playerRoomX, playerRoomY});

    // 4. Escolhe a sala do boss
    
    int bestBossX = -1;
    int bestBossY = -1;
    int maxDist = -1;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (map[i][j].minimum_distance > maxDist && map[i][j].minimum_distance != INFINITY) {
                bestBossX = i;
                bestBossY = j;
                maxDist = map[i][j].minimum_distance;

            }

        }
    }

    // Aplica o Boss na melhor sala encontrada
    if (bestBossX != -1) {
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
