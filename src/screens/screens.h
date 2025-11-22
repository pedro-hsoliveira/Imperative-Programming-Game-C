#ifndef SCREENS_H
#define SCREENS_H

// importing the required libraries
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stddef.h>

#include "../define.h"

// defining game screens
typedef enum GameScreen {QUIT = -1, LOGO, TITLE, CREDITS, SETTINGS, GAMEPLAY, ENDING} GameScreen;

// declaring functions
void credits (Font font, Texture2D default_texture);
int title (Font font, Texture2D default_texture);
void draw_logo (Font font, Texture2D default_texture);

#endif