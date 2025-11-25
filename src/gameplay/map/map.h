#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include "../../entities/entities.h"

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

typedef enum {
    NONE = 0,
    TOP,
    LEFT,
    BOTTOM,
    RIGHT
} opened_door;

typedef enum {
    DOORS_OPEN,
    DOORS_CLOSED

} DoorsState;

typedef struct {
    bool exists;
    bool visited;
    bool aang_entered;
    float minimum_distance;
    Enemy enemies[MAX_ENEMIES];
    int enemies_number;
    RoomType type;
    DoorsState doors_state;
    
} Room;

typedef struct {
    Room grid[MAP_HEIGHT][MAP_WIDTH];
    int currentRoomX;
    int currentRoomY;
    // because of the way the map generator was designed, only for this struct, the X and Y coordinates are like cartesian coordinates, so, for example, if i want to go down a row, i need to decrement Y.
} Map;

void dijkstra(Vector2 starting_room, Map *map);
Map GenerateMap();
void DrawMap();
void DrawMiniMap(Map map);
void InitMap(Map *map);

bool next_room_available(opened_door door, Map map);
opened_door door_colision_verifier(Player player);
void room_changer(Player *player);

void draw_doors(Map map, Texture2D doors_textures[4]);

void draw_empty_room(Texture2D room_sprite);
void initialize_combat_room(Map *map, Player *player);
void combat_room(Map *map, Player *player, float deltaTime);

#endif