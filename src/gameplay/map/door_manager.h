#ifndef DOOR_MANAGER_H
#define DOOR_MANAGER_H

#include "../../entidades/player/player.h"
#include "../../define.h"
#include "raylib.h"
#include "map.h"
#include <stdio.h>

extern Rectangle leftDoor;
extern Rectangle rightDoor;
extern Rectangle roomVert;
extern Rectangle roomHori;
// Verifica se o player tocou em alguma porta e realiza a troca de sala se possível
void DoorManager_UpdateTransition(Vector2 *playerPos, float radius);

#endif