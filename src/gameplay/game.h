#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "raymath.h"
#include "../screens/screens.h"
#include "room/room.h"
#include "../entities/entities.h"
#include "map/map.h"
#include "map/change_room.h"

void game(Texture2D room_texture, Texture2D aang_texture, Player *player, Map *map);

#endif