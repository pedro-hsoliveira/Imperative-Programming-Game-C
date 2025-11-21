#ifndef ROOM_H
#define ROOM_H

#include "raylib.h"
#include "raymath.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

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
    bool visited;
    float minimum_distance;
    RoomType type;
} Room;

// --- Jogo ---
typedef enum {
    STATE_PLAYING,
    STATE_TRANSITIONING
} GameState;

void GenerateMap();
void DrawMap();
void DrawMiniMap();

#endif