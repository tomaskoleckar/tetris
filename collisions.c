#include "blocks.h"
#include "render.h"
#include "collisions.h"

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

int checkCollision(SHAPE activeShape, GameBoard *gameBoard) {
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            if (activeShape.matrix[i][j]) {
                int boardX = activeShape.x + i;
                int boardY = activeShape.y + j;

                // Check collision with the occupied cells in the game board
                /*if (getCell(gameBoard, boardX, boardY)) {
                    return 1;
                }*/

                // Check collision with the bottom of the screen
                if (boardY >= gameBoard->height - 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void updateGameBoard(SHAPE activeShape, GameBoard *gameBoard) {
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            int boardX = activeShape.x + i;
            int boardY = activeShape.y + j;

            if (boardX >= 0 && boardY >= 0 && boardX < gameBoard->width && boardY < gameBoard->height) {
                // Calculate the correct Y position from bottom to top
                int invertedY = BOARD_HEIGHT - boardY - 1;

                if (activeShape.matrix[i][j]) {
                    setCell(gameBoard, boardX, invertedY, 1);
                }
            }
        }
    }
}


void clearLines(GameBoard *gameBoard,GameSettings *gameSettings) {
    for (int i = gameBoard->height - 1; i >= 0; i--) {
        int isLineFull = 1;
        for (int j = 0; j < gameBoard->width; j++) {
            if (!getCell(gameBoard, j, i)) {
                isLineFull = 0;
                break;
            }
        }
        if (isLineFull) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < gameBoard->width; j++) {
                    setCell(gameBoard, j, k, getCell(gameBoard, j, k - 1));
                }
            }
            for (int j = 0; j < gameBoard->width; j++) {
                setCell(gameBoard, j, 0, 0);
            }
            i++;
            gameSettings->score++;
        }
    }
}
