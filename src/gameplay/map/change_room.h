#ifndef CHANGE_ROOM_H
#define CHANGE_ROOM_H

#include "../../entities/entities.h"
#include "../../define.h"
#include "raylib.h"
#include "map.h"
#include <stdio.h>

typedef enum {
    NONE = 0,
    TOP,
    LEFT,
    BOTTOM,
    RIGHT
} opened_door;

bool next_room_available(opened_door door, Map map);
opened_door door_colision_verifier(Player player);
void room_changer(Player *player);

#endif