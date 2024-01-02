#ifndef COLLISIONS_H
#define COLLISIONS_H

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#include "blocks.h"
#include "render.h"

int checkLeftCollision(SHAPE activeShape);
int checkRightCollision(SHAPE activeShape, int playWidth);
int checkDownCollision(SHAPE activeShape, int playWidth);
int checkCollision(SHAPE activeShape,GameBoard *gameBoard);
void updateGameBoard(SHAPE activeShape,GameBoard *gameBoard);
void clearLines(GameBoard *gameBoard, GameSettings *gameSettings);

#endif
