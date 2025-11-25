#include "screens.h"

void draw_victory (Texture2D victory_texture, Font font) {
    DrawTexture(victory_texture, 0, 0, RAYWHITE);

    char victory_text[] = "OBRIGADO POR JOGAR! :D";

    DrawTextEx(font, victory_text, (Vector2) {(SCREEN_WIDTH - MeasureTextEx(font, victory_text, FONT_SIZE + 20, 2).x) / 2, SCREEN_HEIGHT / 6}, FONT_SIZE + 20, 2, WHITE);
}