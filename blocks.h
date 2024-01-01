#include <SDL2/SDL.h>
#include <stdio.h>

#ifndef BLOCKS_H
#define BLOCKS_H

typedef struct {
    Uint8 r, g, b;
} COLOR;

typedef struct {
    COLOR color;
    int matrix[4][4];
    int x, y;
    int size;
    int width;
} SHAPE;

extern SHAPE blocks[7];

int reverseCols(int matrix[4][4], int size);
void transpose(int matrix[4][4], int size);

#endif