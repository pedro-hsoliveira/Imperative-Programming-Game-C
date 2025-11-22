#include "main.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Bending of Aang");
 
    
    int quit = 0, first_iteration = 1;

    float delta_time = 0;

    // declaring the font
    Font default_font = LoadFontEx("../assets/upheavtt.ttf", FONT_SIZE, NULL, 256);
    
    // initializing the images 
    Image background_image = LoadImage("../assets/the_bending_of_aang_logo.png");

    Image room_image = LoadImage("../assets/dafult_room.png");

    // resizing the images

    Player player;
    InitPlayer(&player);


    ImageResize(&room_image, SCREEN_WIDTH, SCREEN_HEIGHT);

    ImageResize(&background_image, SCREEN_WIDTH, SCREEN_HEIGHT);

    // converting the resized images to textures
    Texture background_texture = LoadTextureFromImage(background_image);

    Texture room_texture = LoadTextureFromImage(room_image);

    UnloadImage(background_image);

    UnloadImage(room_image);

    // defining the initial game screen
    GameScreen current_screen = LOGO;

    SetTargetFPS(60);   

    // game loop
    while (!quit)
    {

        delta_time = GetFrameTime();


        BeginDrawing();

            ClearBackground(RAYWHITE);

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

                    if (!first_iteration) first_iteration = 1;

                    if (WindowShouldClose()) {
                        current_screen = QUIT;

                    }

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
                    draw_room(room_texture);
                    UpdatePlayer(&player);

                    // Desenha fundo e player
                    DrawPlayer(&player);
                    
                    // Voltar ao menu
                    if (IsKeyPressed(KEY_ESCAPE))
                        current_screen = TITLE;
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