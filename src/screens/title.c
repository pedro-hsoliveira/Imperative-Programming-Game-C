#include "screens.h"


void draw_title(Font default_font, Texture2D default_image, int selected_option) {
    float scale = ((float) SCREEN_WIDTH)/((float) default_image.width);

    DrawTextureEx(default_image, (Vector2) {0, 0}, 0, scale, GRAY);

    DrawTextEx(default_font, "Selecione uma opção", (Vector2) {SCREEN_WIDTH*0.02, SCREEN_HEIGHT*0.02}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.000075, 2.0f, RAYWHITE);

    DrawTextEx(default_font, "Jogar", (Vector2) {SCREEN_WIDTH*0.075, SCREEN_HEIGHT*0.25}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00007,2.0f, RAYWHITE);
    DrawTextEx(default_font, "Créditos", (Vector2) {SCREEN_WIDTH*0.075, SCREEN_HEIGHT*0.40}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00007,2.0f, RAYWHITE);
    DrawTextEx(default_font, "Opções", (Vector2) {SCREEN_WIDTH*0.075, SCREEN_HEIGHT*0.55}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00007,2.0f, RAYWHITE);
    DrawTextEx(default_font, "Sair", (Vector2) {SCREEN_WIDTH*0.075, SCREEN_HEIGHT*0.70}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00007,2.0f, RAYWHITE);

    switch (selected_option)
    {
    case 0:
        DrawTextEx(default_font, "Jogar", (Vector2) {SCREEN_WIDTH*0.075, SCREEN_HEIGHT*0.25}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00007, 2.0f, MAGENTA);

        break;

    case 1:
        DrawTextEx(default_font, "Créditos", (Vector2) {SCREEN_WIDTH*0.075, SCREEN_HEIGHT*0.40}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00007, 2.0f, MAGENTA);
        
        break;

    case 2:
        DrawTextEx(default_font, "Opções", (Vector2) {SCREEN_WIDTH*0.075, SCREEN_HEIGHT*0.55}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00007, 2.0f, MAGENTA);

        break;

    case 3:
        DrawTextEx(default_font, "Sair", (Vector2) {SCREEN_WIDTH*0.075, SCREEN_HEIGHT*0.70}, SCREEN_HEIGHT * SCREEN_WIDTH * 0.00007, 2.0f, MAGENTA);

        break;

    }
}

int title(Font default_font, Texture2D default_image)
{
    static int selected_option = 0;

    draw_title(default_font, default_image, selected_option);

    if (IsKeyPressed(KEY_DOWN))
    {
        selected_option ++;
        selected_option = selected_option % 4;

    }

    else if (IsKeyPressed(KEY_UP))
    {
        selected_option --;
    
        if (selected_option < 0)
        {
            selected_option = 3;
        
        }
    
    }

    else if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
    {
        switch (selected_option)
        {
        case 0: // Play button
            return GAMEPLAY;

        case 1: // Credits
            return CREDITS;

        case 2: // Settings
            return SETTINGS;

        case 3: // Quit
            return QUIT;
            break;
        
        }    

    }

    return TITLE;

}