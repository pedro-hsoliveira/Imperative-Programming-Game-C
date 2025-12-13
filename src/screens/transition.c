#include "screens.h"

void transitionDraw (Texture transitionImage, Font defaultFont) {
    float scale = ((float) SCREEN_WIDTH)/((float) transitionImage.width);

    DrawTextureEx(transitionImage, (Vector2) {0, 0}, 0, scale, GRAY);

    DrawTextEx(defaultFont, "Você gostaria de continuar?", (Vector2) {SCREEN_WIDTH*0.25, SCREEN_HEIGHT*0.45}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00005, 2.0f, RAYWHITE);
    DrawTextEx(defaultFont, "Y/ Sim", (Vector2) {SCREEN_WIDTH*0.25, SCREEN_HEIGHT*0.55}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00005, 2.0f, RAYWHITE);
    DrawTextEx(defaultFont, "N/ Não", (Vector2) {SCREEN_WIDTH*0.25, SCREEN_HEIGHT*0.65}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00005, 2.0f, RAYWHITE);
}