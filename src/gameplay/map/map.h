#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include "raymath.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../../define.h"

typedef enum {
    ROOM_EMPTY,
    ROOM_START,
    ROOM_COMBAT,
    ROOM_BOSS
} RoomType;

typedef struct Room {
    bool exists;
    bool visited;
    float minimum_distance;
    RoomType type;
} Room;

// --- Jogo ---
typedef enum {
    STATE_PLAYING,
    STATE_TRANSITIONING
} GameState;

static Room map[GRID_SIZE][GRID_SIZE] = { 0 };
static int playerRoomX = 0;
static int playerRoomY = 0;
static int bossRoomX = 0;
static int bossRoomY = 0;

void GenerateMap();
void DrawMap();
void DrawMiniMap();

#endif