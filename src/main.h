#ifndef MAIN_H
#define MAIN_H

// importing the required libraries
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stddef.h>

// defining screen dimensions and font size
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500
#define FONT_SIZE 32

// defining game screens
typedef enum GameScreen {QUIT = -1, LOGO, TITLE, CREDITS, SETTINGS, GAMEPLAY, ENDING} GameScreen;

// declaring functions
void credits (Font font, Texture2D default_texture);
int title (Font font, Texture2D default_texture);
void draw_logo (Font font, Texture2D default_texture);

#endif // MAIN_H