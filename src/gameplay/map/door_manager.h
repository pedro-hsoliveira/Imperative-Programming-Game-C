#ifndef DOOR_MANAGER_H
#define DOOR_MANAGER_H

#include "../../entidades/player/player.h"
#include "../../define.h"
#include "map.h"

// Verifica se o player tocou em alguma porta e realiza a troca de sala se possível
void CheckDoorTransition(Player *player, Map *map);

#endif