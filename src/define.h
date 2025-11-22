#ifndef DEFINE_H
#define DEFINE_H

// screen dimentions
#define SCREEN_WIDTH 1028
#define SCREEN_HEIGHT 768

// font size
#define FONT_SIZE 32

// map dimension
#define GRID_SIZE 8

// max amount of rooms at the map
#define MAX_ROOMS 16

// Coordenadas baseadas nas suas paredes (ajuste fino se necessário)
#define DOOR_TOP_Y    120.0f
#define DOOR_BOTTOM_Y 636.0f // 120 + 516
#define DOOR_LEFT_X   188.0f
#define DOOR_RIGHT_X  799.0f // 188 + 611

// Margem de segurança para o player não ficar preso na parede ao trocar de sala
#define SPAWN_OFFSET  60.0f 

#define MAP_WIDTH  10
#define MAP_HEIGHT 10


#endif