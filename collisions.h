#ifndef COLLISIONS_H
#define COLLISIONS_H

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#include "blocks.h"

int checkLeftCollision(SHAPE activeShape);
int checkRightCollision(SHAPE activeShape, int playWidth);
int checkDownCollision(SHAPE activeShape, int playWidth);
int checkCollision(SHAPE activeShape,int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]);
void updateGameBoard(SHAPE activeShape,int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]);
void clearLines(int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]);

#endif
