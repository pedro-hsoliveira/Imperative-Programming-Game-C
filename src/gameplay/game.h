#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "../screens/screens.h"
#include "../entities/entities.h"
#include "map/map.h"

typedef struct {
    Texture2D roomTexture;
    Texture2D aangTexture;
    Texture2D doorsTextures[4];
    Player player;
    Map map;
    bool isGameOver;

} Game;

void is_game_over(Game *game, Map map);
void game(Texture2D room_texture, Texture2D doors_textures[4], Player *player, Map *map, Game game, int *currentScreen);

#endif