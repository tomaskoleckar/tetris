#include "blocks.h"
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20


int checkLeftCollision(SHAPE activeShape) {
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            if (activeShape.matrix[i][j]) {
                if (activeShape.x + i <= 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int checkRightCollision(SHAPE activeShape, int playWidth) {
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            if (activeShape.matrix[i][j]) {
                if (activeShape.x + i >= playWidth - 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}


int checkCollision(SHAPE activeShape, int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            if (activeShape.matrix[i][j] &&
                (activeShape.y + j >= BOARD_HEIGHT-1)) {
                    return 1;
            }
        }
    }
    return 0;
}

void updateGameBoard(SHAPE activeShape, int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            int boardX = activeShape.x + i;
            int boardY = activeShape.y + j;

            if (boardX >= 0 && boardY >= 0 && boardX < BOARD_WIDTH && boardY < BOARD_HEIGHT) {
                gameBoard[boardY][boardX] = 1;
            }
        }
    }
}



void clearLines(int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        int isLineFull = 1;
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (!gameBoard[i][j]) {
                isLineFull = 0;
                break;
            }
        }
        if (isLineFull) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    gameBoard[k][j] = gameBoard[k - 1][j];
                }
            }
            for (int j = 0; j < BOARD_WIDTH; j++) {
                gameBoard[0][j] = 0;
            }
            i++;
        }
    }
}