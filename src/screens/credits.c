#include "screens.h"

void credits(Font default_font, Texture2D default_image)
{
    int x_names = SCREEN_WIDTH * 0.1;

    ClearBackground(BLACK);
    DrawTextEx(default_font, "Créditos:", (Vector2) {SCREEN_WIDTH * 0.01 , 0}, FONT_SIZE + 20, 2, WHITE);

    DrawTextEx(default_font, "Lucas de Moura <lmps3>", (Vector2) {x_names , SCREEN_HEIGHT * 0.15}, FONT_SIZE, 2, WHITE);
    DrawTextEx(default_font, "Pedro Henrique <phso>", (Vector2) {x_names , SCREEN_HEIGHT * 0.25}, FONT_SIZE, 2, WHITE);
    DrawTextEx(default_font, "Lucas David <ldlf>", (Vector2) {x_names , SCREEN_HEIGHT * 0.35}, FONT_SIZE, 2, WHITE);
    DrawTextEx(default_font, "Joyce Gabriele <jgsp3>", (Vector2) {x_names , SCREEN_HEIGHT * 0.45}, FONT_SIZE, 2, WHITE);

}