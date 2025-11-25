#include "map.h"

void dijkstra(Vector2 starting_room, Map *map) {

    // Atribui todas as distancias iniciais do map.grida para infinito 
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            map->grid[i][j].minimum_distance = INFINITY; // Cast to int since struct uses int
            map->grid[i][j].visited = false;
        }
    }

    // baixo, cima, direita e esquerda, respectivamente
    int dx[] = {1, -1, 0, 0,};
    int dy[] = {0, 0, 1, -1,};

    // distancia inicial = 0
    map->grid[(int) starting_room.x][(int) starting_room.y].minimum_distance = 0;

    for (int _ = 0; _ < GRID_SIZE * GRID_SIZE; _++) {
        float min_distance = INFINITY;
        int u = -1, v = -1;

        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (map->grid[i][j].minimum_distance < min_distance && !map->grid[i][j].visited && map->grid[i][j].exists){
                    min_distance = map->grid[i][j].minimum_distance;
                    u = i;
                    v = j;
                }
            }
        }

        if (u != -1) {
            map->grid[u][v].visited = true;
            for (int k = 0; k < 4; k++) {
                // Bounds check
                if (((0 <= u+ dx[k]) && (u+ dx[k] < GRID_SIZE)) && ((0 <= v+ dy[k]) && (v+ dy[k] < GRID_SIZE))) {
                    // Check neighbor existence and visited status
                    if (map->grid[u+ dx[k]][v+ dy[k]].exists && !map->grid[u+ dx[k]][v+ dy[k]].visited) {
                        int alt = 1 + map->grid[u][v].minimum_distance;

                        if (alt < map->grid[u+ dx[k]][v+ dy[k]].minimum_distance) {
                            map->grid[u+ dx[k]][v+ dy[k]].minimum_distance = alt;
                        }
                    }
                }
            }
        }
    }

}

Map GenerateMap()
{
    int playerRoomX, playerRoomY;

    Map map;

    int roomsCreated = 0;

    do {
        memset(map.grid, 0, sizeof(map.grid));
        roomsCreated = 0;
        int frontierCount = 0;
        Vector2 frontier[GRID_SIZE * GRID_SIZE];
        
        playerRoomX = GetRandomValue(1, GRID_SIZE - 2);
        playerRoomY = GetRandomValue(1, GRID_SIZE - 2);
        map.grid[playerRoomX][playerRoomY].exists = true;
        map.grid[playerRoomX][playerRoomY].type = ROOM_START;
        
        frontier[frontierCount++] = (Vector2){ (float)playerRoomX, (float)playerRoomY };
        roomsCreated = 1;
        int currentFrontierIndex = 0;
        
        int dx[] = { 0, 0, 1, -1 };
        int dy[] = { -1, 1, 0, 0 };

        while (roomsCreated < MAX_ROOMS && currentFrontierIndex < frontierCount)
        {
            Vector2 current = frontier[currentFrontierIndex++];
            
            for (int i = 0; i < 4; i++)
            {
                int nx = (int)current.x + dx[i];
                int ny = (int)current.y + dy[i];
                
                if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE && !map.grid[nx][ny].exists)
                {
                    if (GetRandomValue(0, 1)) {
                        map.grid[nx][ny].exists = true;
                        map.grid[nx][ny].aang_entered = false;
                        
                        if (!GetRandomValue(0, 2)) {
                            map.grid[nx][ny].type = ROOM_COMBAT;
                            map.grid[nx][ny].doors_state = DOORS_CLOSED;
                        } else {
                            map.grid[nx][ny].type = ROOM_EMPTY;
                            map.grid[nx][ny].doors_state = DOORS_OPEN;
                        }
                        
                        frontier[frontierCount++] = (Vector2){ (float)nx, (float)ny };
                        roomsCreated++;
                        
                        if (roomsCreated >= MAX_ROOMS) break;
                    }
                }
            }
        }
        
    } while (roomsCreated < 8); // safety check

    dijkstra((Vector2) {(float)playerRoomX, (float)playerRoomY}, &map);

    // 4. Escolhe a sala do boss
    int bestBossX = -1;
    int bestBossY = -1;
    float maxDist = -1;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            // Check if exists so we don't pick a non-existent room
            if (map.grid[i][j].exists && (map.grid[i][j].minimum_distance > maxDist) && map.grid[i][j].minimum_distance != INFINITY) {
                bestBossX = i;
                bestBossY = j;
                maxDist = map.grid[i][j].minimum_distance;
            }
        }
    }

    // Aplica o Boss na melhor sala encontrada
    if (bestBossX != -1) {
        map.grid[bestBossX][bestBossY].type = ROOM_BOSS;
        map.grid[bestBossX][bestBossY].doors_state = DOORS_OPEN;

    } else {
        exit(1);
        printf("Error: Could not place boss.\n");
    }

    map.currentRoomX = playerRoomX;
    map.currentRoomY = playerRoomY;

    return map;
}

void DrawMiniMap(Map map)
{
    int cellSize = 20;       // Smaller cells for the HUD
    int padding = 20;        // Distance from the screen edges
    
    int startX = SCREEN_WIDTH - (GRID_SIZE * cellSize) - padding;
    int startY = padding;    // Top of the screen + padding

    // 3. Draw Background (Optional: semi-transparent black box behind map)
    DrawRectangle(startX - 5, startY - 5, (GRID_SIZE * cellSize) + 10, (GRID_SIZE * cellSize) + 10, Fade(BLACK, 0.5f));

    // 4. Draw Grid
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            int drawX = startX + x * cellSize;
            int drawY = startY + y * cellSize;
            
            if (map.grid[x][y].exists)
            {
                Color roomColor = LIGHTGRAY; // Default color for existing rooms
                
                // Determine Color based on Room Type
                if (x == map.currentRoomX && y == map.currentRoomY) {
                    roomColor = GREEN;  // Player is here
                }
                else if (map.grid[x][y].type == ROOM_BOSS) {
                    roomColor = RED;    // Boss Room
                }
                else if (map.grid[x][y].type == ROOM_COMBAT) {
                    roomColor = GRAY;
                }
                
                // Draw the room
                DrawRectangle(drawX, drawY, cellSize - 2, cellSize - 2, roomColor);

                // Optional: Draw a white border on the current player room
                if (x == map.currentRoomX && y == map.currentRoomY) {
                    DrawRectangleLines(drawX, drawY, cellSize - 2, cellSize - 2, WHITE);
                }
            }
            else
            {
                // Optional: Faint outline for empty grid slots (helps visualize the grid)
                DrawRectangleLines(drawX, drawY, cellSize - 2, cellSize - 2, Fade(GRAY, 0.2f));
            }
        }
    }
}