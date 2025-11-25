#include "screens.h"

void draw_defeat (Texture2D defeat_texture, Font font) {
    DrawTexture(defeat_texture, 0, 0, RAYWHITE);

    char defeat_text[] = "VOCÊ PERDEU :(";

    DrawTextEx(font, defeat_text, (Vector2) {(SCREEN_WIDTH - MeasureTextEx(font, defeat_text, FONT_SIZE + 20, 2).x) / 2, SCREEN_HEIGHT / 6}, FONT_SIZE + 20, 2, RED);
}