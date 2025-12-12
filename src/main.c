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

    Image dark_room_image = LoadImage("../assets/dark_room.png");
    Image green_room_image = LoadImage("../assets/green_room.png");

    Image dark_door_image = LoadImage("../assets/dark_door_sprite.png"); // will preserve the dimensions and the orientation
    Image green_door_image = LoadImage("../assets/green_door_sprite-removebg-preview2.png");

    Image top_door_dark_image = ImageCopy(dark_door_image);
    Image bottom_door_dark_image = ImageCopy(dark_door_image);
    Image right_door_dark_image = ImageCopy(dark_door_image);
    Image left_door_dark_image = ImageCopy(dark_door_image);

    Image top_door_green_image = ImageCopy(green_door_image);
    Image bottom_door_green_image = ImageCopy(green_door_image);
    Image right_door_green_image = ImageCopy(green_door_image);
    Image left_door_green_image = ImageCopy(green_door_image);

    Player player;

    Game current_game;
    current_game.match = 0;

    InitPlayer(&player);

    // resizing the images
    ImageResize(&dark_room_image, SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageResize(&green_room_image, SCREEN_WIDTH, SCREEN_HEIGHT);

    ImageResize(&logo_image, SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageResize(&victory_image, SCREEN_WIDTH, SCREEN_HEIGHT);
    ImageResize(&defeat_image, SCREEN_WIDTH, SCREEN_HEIGHT);

    ImageResize(&top_door_dark_image, 120, 90);
    
    ImageResize(&bottom_door_dark_image, 120, 90);
    ImageRotate(&bottom_door_dark_image, 180);

    ImageResize(&left_door_dark_image, 140, 90);
    ImageRotate(&left_door_dark_image, 90);

    ImageResize(&right_door_dark_image, 140, 90);
    ImageRotate(&right_door_dark_image, 270);

    ImageResize(&top_door_green_image, 130, 90);
    
    ImageResize(&bottom_door_green_image, 130, 90);
    ImageRotate(&bottom_door_green_image, 180);

    ImageResize(&left_door_green_image, 150, 90);
    ImageRotate(&left_door_green_image, 90);

    ImageResize(&right_door_green_image, 150, 90);
    ImageRotate(&right_door_green_image, 270);

    // converting the resized images to textures
    Texture logo_texture = LoadTextureFromImage(logo_image);
    Texture victory_texture = LoadTextureFromImage(victory_image);
    Texture defeat_texture = LoadTextureFromImage(defeat_image);

    Texture dark_room_texture = LoadTextureFromImage(dark_room_image);
    Texture green_room_texture = LoadTextureFromImage(green_room_image);

    Texture top_door_dark_texture = LoadTextureFromImage(top_door_dark_image);
    Texture right_door_dark_texture = LoadTextureFromImage(right_door_dark_image);
    Texture bottom_door_dark_texture = LoadTextureFromImage(bottom_door_dark_image);
    Texture left_door_dark_texture = LoadTextureFromImage(left_door_dark_image);

    Texture top_door_green_texture = LoadTextureFromImage(top_door_green_image);
    Texture right_door_green_texture = LoadTextureFromImage(right_door_green_image);
    Texture bottom_door_green_texture = LoadTextureFromImage(bottom_door_green_image);
    Texture left_door_green_texture = LoadTextureFromImage(left_door_green_image);

    UnloadImage(logo_image);

    UnloadImage(dark_room_image);
    UnloadImage(green_room_image);

    UnloadImage(dark_door_image);
    UnloadImage(green_door_image);

    UnloadImage(top_door_dark_image);
    UnloadImage(bottom_door_dark_image);
    UnloadImage(left_door_dark_image);
    UnloadImage(right_door_dark_image);
    
    UnloadImage(top_door_green_image);
    UnloadImage(bottom_door_green_image);
    UnloadImage(left_door_green_image);
    UnloadImage(right_door_green_image);

    Texture2D dark_doors_textures[4] = {
        top_door_dark_texture,
        right_door_dark_texture,
        bottom_door_dark_texture,
        left_door_dark_texture
    };

    Texture2D green_doors_textures[4] = {
        top_door_green_texture,
        right_door_green_texture,
        bottom_door_green_texture,
        left_door_green_texture
    };

    // gameplay music 
    Music musicgame = LoadMusicStream("../assets/audio/musicgame.mp3");

    // MENU / TITLE / LOGO music 
    Music musicplay = LoadMusicStream("../assets/audio/musicplay.mp3");

    Texture2D currentRoom;
    Texture2D currentDoors[4];

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
                        InitPlayer(&player);
                        first_iteration = 0;
                    }

                    current_screen = title(default_font, logo_texture);

                    if (current_screen == GAMEPLAY && current_game.isGameOver && !current_game.onGoingGame) {
                        current_map = GenerateMap();
                        current_game.onGoingGame = true;
                        current_game.match++;
                        
                        if (current_game.match % 2 == 0) {
                            currentDoors[0] = dark_doors_textures[0];
                            currentDoors[1] = dark_doors_textures[1];
                            currentDoors[2] = dark_doors_textures[2];
                            currentDoors[3] = dark_doors_textures[3];
                            currentRoom = dark_room_texture;
                        } else {
                            currentDoors[0] = green_doors_textures[0];
                            currentDoors[1] = green_doors_textures[1];
                            currentDoors[2] = green_doors_textures[2];
                            currentDoors[3] = green_doors_textures[3];
                            currentRoom = green_room_texture;
                        }
                    }

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

                    game(currentRoom, currentDoors, &player, &current_map, current_game, &current_screen);

                    if (!player.alive){
                        current_screen = DEFEAT;
                        break;
                    
                    }


                    // Voltar ao menu
                    if (IsKeyPressed(KEY_ESCAPE))
                        current_screen = TITLE;

                    break;

                case TRANSITION:
                    current_game.onGoingGame = false;

                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    transitionDraw(logo_texture, default_font);

                    if (IsKeyPressed(KEY_Y)) {
                        current_screen = GAMEPLAY;
                    } else if (IsKeyPressed(KEY_N)) {
                        current_screen = TITLE;
                    }

                    if (current_screen == GAMEPLAY && !current_game.onGoingGame) {
                        current_game.onGoingGame = true;
                        current_map = GenerateMap();
                        current_game.match++;
                        
                        if (current_game.match % 2 == 0) {
                            currentDoors[0] = dark_doors_textures[0];
                            currentDoors[1] = dark_doors_textures[1];
                            currentDoors[2] = dark_doors_textures[2];
                            currentDoors[3] = dark_doors_textures[3];
                            currentRoom = dark_room_texture;
                        } else {
                            currentDoors[0] = green_doors_textures[0];
                            currentDoors[1] = green_doors_textures[1];
                            currentDoors[2] = green_doors_textures[2];
                            currentDoors[3] = green_doors_textures[3];
                            currentRoom = green_room_texture;
                        }
                    }


                    break;

                case DEFEAT:
                    current_game.onGoingGame = false;
                    
                    // ensure correct music
                    first_iteration = 1;
                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    draw_defeat(defeat_texture, default_font);

                    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)){
                        current_game.match++;
                        current_screen = LOGO;
                    }

                    break;
                    
                case VICTORY:
                    current_game.onGoingGame = false;

                    first_iteration = 1;
                    
                    if (!IsMusicStreamPlaying(musicplay)) {
                        StopMusicStream(musicgame);
                        PlayMusicStream(musicplay);
                    }

                    draw_victory(victory_texture, default_font);

                    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)){
                        current_game.match++;
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
    UnloadTexture(dark_room_texture);
    UnloadTexture(green_room_texture);
    UnloadTexture(top_door_dark_texture);
    UnloadTexture(right_door_dark_texture);
    UnloadTexture(bottom_door_dark_texture);
    UnloadTexture(left_door_dark_texture);
    UnloadTexture(top_door_green_texture);
    UnloadTexture(right_door_green_texture);
    UnloadTexture(bottom_door_green_texture);
    UnloadTexture(left_door_green_texture);
    
    // unloading font
    UnloadFont(default_font);

    CloseWindow();

    return 0;
}