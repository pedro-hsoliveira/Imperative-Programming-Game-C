#include "main.h"

Rectangle roomMain = { 
    .x = 190,      
    .y = 120,      
    .width = 650,  
    .height = 540  
};

Rectangle roomHori = { 
    .x = 80,       
    .y = 340,      
    .width = 880,  
    .height = 90   
};

Rectangle roomVert = { 
    .x = 470,      
    .y = 80,       
    .width = 80,       
    .height = 620
};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Bending of Aang");
 
    // Initialize audio
    InitAudioDevice();

    int quit = 0, first_iteration = 1;

    float delta_time = 0;

    Map current_map;

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


    // gameplay music 
    Music musicgame = LoadMusicStream("../assets/audio/musicgame.mp3");

    // MENU / TITLE / LOGO music 
    Music musicplay = LoadMusicStream("../assets/audio/musicplay.mp3");
    SetMusicVolume(musicplay, 0.2f);
    PlayMusicStream(musicplay);
    // defining the initial game screen
    GameScreen current_screen = LOGO;

    SetTargetFPS(60);   

    // game loop
    while (!quit)
    {

        delta_time = GetFrameTime();

        // update gameplay music 
        UpdateMusicStream(musicgame);

        // update menu music 
        UpdateMusicStream(musicplay);


        BeginDrawing();

            ClearBackground(RAYWHITE);

            // switch case to manage the screens
            switch (current_screen) {
                case QUIT:
                    quit = 1;

                case LOGO:

                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    draw_logo(default_font, background_texture);

                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                        current_screen = TITLE;

                    } else if (WindowShouldClose()) {
                        current_screen = QUIT;

                    }

                    break;
                
                case TITLE:

                    // ensure correct music
                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    current_screen = title(default_font, background_texture);

                    if (WindowShouldClose()) {
                        current_screen = QUIT;

                    }

                    break;

                case CREDITS:

                    // ensure correct music
                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }
                    
                    credits(default_font, background_texture);

                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)){
                        current_screen = TITLE;

                    } else if (WindowShouldClose()) {
                        quit = 1;

                    }

                    break;

                case SETTINGS:
                // ensure correct music
                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }
                    //code
                
                    break;

                case GAMEPLAY:

                    SetMusicVolume(musicgame, 0.1f);
                    // switch to gameplay music
                    if (!IsMusicStreamPlaying(musicgame)) {
                        StopMusicStream(musicplay);
                        PlayMusicStream(musicgame);
                    }
                    if (first_iteration) {
                        current_map = GenerateMap();
                        first_iteration = 0;
                    }

                    game(room_texture, room_texture, &player, &current_map);

                    if (!player.alive){
                        current_screen = ENDING;   
                        break;
                    
                    }


                    // Voltar ao menu
                    if (IsKeyPressed(KEY_ESCAPE))
                        current_screen = TITLE;

                    break;

                
                case ENDING:
                    // ensure correct music
                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    // ending screen logic

                    break;
            }

        EndDrawing();

    }

    // unload gameplay music 
    UnloadMusicStream(musicgame);
    UnloadMusicStream(musicplay);
    CloseAudioDevice();

    CloseWindow();

    return 0;
}