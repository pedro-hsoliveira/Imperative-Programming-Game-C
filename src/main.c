#include "main.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Bending of Aang");

    int quit = 0;

    // declaring the font
    Font default_font = LoadFontEx("assets/upheavtt.ttf", FONT_SIZE, NULL, 256);
    
    // initializing the background image for the title 
    Image background_image = LoadImage("assets/the_bending_of_aang_logo.jpeg");

    // converting the title image to a texture
    Texture background_texture = LoadTextureFromImage(background_image);

    UnloadImage(background_image);

    // defining the initial game screen
    GameScreen current_screen = LOGO;

    SetTargetFPS(60);   

    // game loop
    while (!WindowShouldClose() && !quit)
    {

        BeginDrawing();

            // switch case to manage the screens
            switch (current_screen) {
                case QUIT:
                    quit = 1;

                case LOGO:
                    draw_logo(default_font, background_texture);

                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                        current_screen = TITLE;

                    } else if (WindowShouldClose()) {
                        current_screen = QUIT;

                    }

                    break;
                
                case TITLE:
                    current_screen = title(default_font, background_texture);

                    break;

                case CREDITS:
                    credits(default_font, background_texture);

                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)){
                        current_screen = TITLE;

                    } else if (WindowShouldClose()) {
                        quit = 1;

                    }

                    break;

                case SETTINGS:
                    //code
                
                    break;

                case GAMEPLAY:
                    // code

                    break;
                
                case ENDING:
                    // code

                    break;
            }

        EndDrawing();

    }

    CloseWindow();

    return 0;
}