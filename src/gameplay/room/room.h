#ifndef ROOM_H
#define ROOM_H

#include "raylib.h"
#include "../../define.h"
#include "../map/map.h"
#include "../../entities/entities.h"

void draw_empty_room(Texture2D room_sprite);

void initialize_combat_room(Texture2D room_sprite, Map *map, Player *player);
void combat_room(Map *map, Player *player, float deltaTime);

void initialize_boss_room(Texture2D room_sprite, Map *map);

#endif