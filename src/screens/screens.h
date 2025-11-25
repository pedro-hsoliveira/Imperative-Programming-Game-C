#ifndef SCREENS_H
#define SCREENS_H

// importing the required libraries
#include "raylib.h"
#include "raymath.h"
#include <stddef.h>

#include "../define.h"

// defining game screens
typedef enum GameScreen {QUIT = -1, LOGO, TITLE, CREDITS, SETTINGS, GAMEPLAY, DEFEAT, VICTORY} GameScreen;

// declaring functions
void credits (Font font);
int title (Font font, Texture2D default_texture);
void draw_logo (Font font, Texture2D default_texture);
void draw_victory (Texture2D victory_texture, Font font);
void draw_defeat (Texture2D defeat_texture, Font font);

#endif