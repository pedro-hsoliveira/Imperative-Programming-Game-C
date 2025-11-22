#include "room.h"

void draw_room(Texture2D room_sprite) {
    float scale = ((float) SCREEN_WIDTH)/((float) room_sprite.width);

    DrawTextureEx(room_sprite, (Vector2) {0, 0}, 0, scale, GRAY);
}