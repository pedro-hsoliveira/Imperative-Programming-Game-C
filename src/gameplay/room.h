#ifndef ROOM_H
#define ROOM_H

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h> // Para memset
#include <stdlib.h> // Para abs
#include <time.h>   // Para a semente aleatória real

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

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
    bool visited;
    int minimum_distance;
    RoomType type;
} Room;

// --- Jogo ---
typedef enum {
    STATE_PLAYING,
    STATE_TRANSITIONING
} GameState;

void InitRoom(void);
void DrawRoom(void);
void GenerateMap();
void DrawMiniMap();

#endif