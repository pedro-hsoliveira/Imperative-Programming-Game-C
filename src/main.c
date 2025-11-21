#include "main.h"

int main(void)
{
    // Inicializa janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Bending of Aang");
    SetTargetFPS(60);

    int quit = 0;

    // Carrega fonte
    Font default_font = LoadFontEx("assets/upheavtt.ttf", FONT_SIZE, NULL, 256);

    // Carrega imagem de fundo
    Image background_image = LoadImage("assets/fundo.png");

    // Debug simples
    printf("IMG WIDTH  = %d\n", background_image.width);
    printf("IMG HEIGHT = %d\n", background_image.height);

    // Converte para textura
    Texture2D background_texture = LoadTextureFromImage(background_image);

    // Libera imagem original da RAM
    UnloadImage(background_image);

    // Sistema de telas
    GameScreen current_screen = LOGO;

    // Player para usar na tela de gameplay
    Player player;
    InitPlayer(&player);

    // Loop principal
    while (!WindowShouldClose() && !quit)
    {
        BeginDrawing();

            switch (current_screen)
            {
                case QUIT:
                    quit = 1;
                    break;

                case LOGO:
                    draw_logo(default_font, background_texture);
                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                        current_screen = TITLE;
                    break;

                case TITLE:
                    current_screen = title(default_font, background_texture);
                    break;

                case CREDITS:
                    credits(default_font, background_texture);
                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE))
                        current_screen = TITLE;
                    break;

                case SETTINGS:
                    // futura tela de settings
                    break;

                case GAMEPLAY:
                    // Atualiza player
                    UpdatePlayer(&player);

                    // Desenha fundo e player
                    ClearBackground(RAYWHITE);
                    DrawPlayer(&player);

                    // Voltar ao menu
                    if (IsKeyPressed(KEY_ESCAPE))
                        current_screen = TITLE;
                    break;

                case ENDING:
                    // futura tela de ending
                    break;
            }

        EndDrawing();
    }

    // Libera recursos
    UnloadTexture(background_texture);
    UnloadFont(default_font);

    CloseWindow();
    return 0;
}
