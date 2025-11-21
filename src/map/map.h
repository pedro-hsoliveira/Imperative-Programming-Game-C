#ifndef MAP_H
#define MAP_H

#include "raylib.h"

typedef struct {
    int width;
    int height;
    Color backgroundColor;
} Map;

void InitMap(void);
void DrawMap(void);

#endif
