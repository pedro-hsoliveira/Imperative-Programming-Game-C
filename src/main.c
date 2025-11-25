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

int main(void) {
    ChangeDirectory(GetApplicationDirectory());

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Bending of Aang");
 
    // Initialize audio
    InitAudioDevice();

    int quit = 0, first_iteration = 1;

    Map current_map;

    // declaring the font
    Font default_font = LoadFontEx("../assets/upheavtt.ttf", FONT_SIZE, NULL, 256);
    
    // initializing the images 
    Image logo_image = LoadImage("../assets/the_bending_of_aang_logo.png");
    Image victory_image = LoadImage("../assets/victory_screen.png");
    Image defeat_image = LoadImage("../assets/defeat_screen.png");

    Image room_image = LoadImage("../assets/default_room.png");

    Image original_door_image = LoadImage("../assets/locked_door_sprite.png"); // will preserve the dimensions and the orientation
    
    Image top_door_image = ImageCopy(original_door_image);
    Image bottom_door_image = ImageCopy(original_door_image);
    Image right_door_image = ImageCopy(original_door_image);
    Image left_door_image = ImageCopy(original_door_image);

    Player player;

    Game current_game;

    InitPlayer(&player);

    // resizing the images
    ImageResize(&room_image, SCREEN_WIDTH, SCREEN_HEIGHT);

    ImageResize(&logo_image, SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageResize(&victory_image, SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageResize(&defeat_image, SCREEN_WIDTH, SCREEN_HEIGHT);

    ImageResize(&top_door_image, 120, 90);
    
    ImageResize(&bottom_door_image, 120, 90);
    ImageRotate(&bottom_door_image, 180);

    ImageResize(&left_door_image, 140, 90);
    ImageRotate(&left_door_image, 270);

    ImageResize(&right_door_image, 140, 90);
    ImageRotate(&right_door_image, 90);

    // converting the resized images to textures
    Texture logo_texture = LoadTextureFromImage(logo_image);
    Texture victory_texture = LoadTextureFromImage(victory_image);
    Texture defeat_texture = LoadTextureFromImage(defeat_image);

    Texture room_texture = LoadTextureFromImage(room_image);

    Texture top_door_texture = LoadTextureFromImage(top_door_image);
    Texture right_door_texture = LoadTextureFromImage(right_door_image);
    Texture bottom_door_texture = LoadTextureFromImage(bottom_door_image);
    Texture left_door_texture = LoadTextureFromImage(left_door_image);

    UnloadImage(logo_image);

    UnloadImage(room_image);

    UnloadImage(original_door_image);
    UnloadImage(top_door_image);
    UnloadImage(bottom_door_image);
    UnloadImage(left_door_image);
    UnloadImage(right_door_image);

    Texture2D doors_textures[4] = {top_door_texture, left_door_texture, bottom_door_texture, right_door_texture};

    // gameplay music 
    Music musicgame = LoadMusicStream("../assets/audio/musicgame.mp3");

    // MENU / TITLE / LOGO music 
    Music musicplay = LoadMusicStream("../assets/audio/musicplay.mp3");


    SetMusicVolume(musicplay, 0.05f);
    PlayMusicStream(musicplay);
    // defining the initial game screen
    GameScreen current_screen = LOGO;

    SetTargetFPS(60);   

    // game loop
    while (!quit)
    {
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
                    break;

                case LOGO:

                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    draw_logo(default_font, logo_texture);

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

                    if (first_iteration) {
                        current_map = GenerateMap();
                        InitPlayer(&player);
                        first_iteration = 0;
                    }

                    current_screen = title(default_font, logo_texture);

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
                    
                    credits(default_font);

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

                    SetMusicVolume(musicgame, 0.05f);
                    // switch to gameplay music
                    if (!IsMusicStreamPlaying(musicgame)) {
                        StopMusicStream(musicplay);
                        PlayMusicStream(musicgame);
                    }

                    game(room_texture, doors_textures, &player, &current_map, current_game, &current_screen);

                    if (!player.alive){
                        current_screen = DEFEAT;
                        break;
                    
                    }


                    // Voltar ao menu
                    if (IsKeyPressed(KEY_ESCAPE))
                        current_screen = TITLE;

                    break;

                case DEFEAT:
                    // ensure correct music
                    first_iteration = 1;
                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    draw_defeat(defeat_texture, default_font);

                    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)){
                        current_screen = LOGO;
                    }

                    break;
                    
                    case VICTORY:
                    first_iteration = 1;
                    
                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    draw_victory(victory_texture, default_font);

                    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)){
                        current_screen = LOGO;
                    }

                    break;
            }

        EndDrawing();

    }

    // unloading gameplay music 
    UnloadMusicStream(musicgame);
    UnloadMusicStream(musicplay);
    CloseAudioDevice();

    // unloading textures
    UnloadTexture(logo_texture);
    UnloadTexture(victory_texture);
    UnloadTexture(defeat_texture);
    UnloadTexture(room_texture);
    UnloadTexture(top_door_texture);
    UnloadTexture(right_door_texture);
    UnloadTexture(bottom_door_texture);
    UnloadTexture(left_door_texture);
    
    // unloading font
    UnloadFont(default_font);

    CloseWindow();

    return 0;
}