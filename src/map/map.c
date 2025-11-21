#include "map.h"

static Map gameMap;

void InitMap(void)
{
    gameMap.width = GetScreenWidth();
    gameMap.height = GetScreenHeight();
    gameMap.backgroundColor = RAYWHITE;   // mapa branco
}

void DrawMap(void)
{
    ClearBackground(gameMap.backgroundColor);
}
