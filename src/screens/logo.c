#include "screens.h"

void draw_logo(Font font, Texture2D tex)
{
    // limpa a tela ANTES de desenhar
    ClearBackground(RAYWHITE);

    float scaleX = (float)SCREEN_WIDTH / tex.width;
    float scaleY = (float)SCREEN_HEIGHT / tex.height;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    DrawTextureEx(tex, (Vector2){0, 0}, 0, scale, WHITE);

    DrawTextEx(font, "The Bending of Aang",
        (Vector2){ (SCREEN_WIDTH - MeasureTextEx(font, "The Bending of Aang", FONT_SIZE + 10, 2).x) / 2,
                   SCREEN_HEIGHT - 200 },
        FONT_SIZE + 10, 2, WHITE
    );

    DrawTextEx(font, "Pressione espaco ou enter para continuar",
        (Vector2){ (SCREEN_WIDTH - MeasureTextEx(font, "Pressione espaco ou enter para continuar", FONT_SIZE, 2).x) / 2,
                   SCREEN_HEIGHT - 150 },
        FONT_SIZE, 2, WHITE
    );
}
