#include "blocks.h"
#include "render.h"
#include "collisions.h"

int checkLeftCollision(SHAPE activeShape, GameBoard *gameBoard) {
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            if (activeShape.matrix[i][j]) {
                int boardX = activeShape.x + i - 1;
                int boardY = activeShape.y + j;
                if (boardX < 0 || getCell(gameBoard, boardX, BOARD_HEIGHT - boardY - 1)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int checkRightCollision(SHAPE activeShape, int playWidth, GameBoard *gameBoard) {
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            if (activeShape.matrix[i][j]) {
                int boardX = activeShape.x + i + 1;
                int boardY = activeShape.y + j;
                if (boardX >= playWidth || getCell(gameBoard, boardX, BOARD_HEIGHT - boardY - 1)) {
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

                if (getCell(gameBoard, boardX, BOARD_HEIGHT - boardY - 1)) {
                    return 1;
                }

                if (boardY >= gameBoard->height - 1) {
                    return 2;
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
                int invertedY = BOARD_HEIGHT - boardY - 1;

                if (activeShape.matrix[i][j]) {
                    setCell(gameBoard, boardX, invertedY, 1);
                }
            }
        }
    }
}

void clearLines(GameBoard *gameBoard) {
    for (int i = gameBoard->height - 1; i >= 0; i--) {
        int isLineFull = 1;
        for (int j = 0; j < gameBoard->width; j++) {
            if (!getCell(gameBoard, j, i)) {
                isLineFull = 0;
                break;
            }
        }
        if (isLineFull) {

            for (int j = 0; j < gameBoard->width; j++) {
                setCell(gameBoard, j, i, 0);
            }

            // Shift cells above the cleared line down by one unit
            for (int k = i; k < BOARD_HEIGHT-1; k++) {
                for (int j = 0; j < gameBoard->width; j++) {
                    setCell(gameBoard, j, k, getCell(gameBoard, j, k + 1));
                }
            }

            // Set the top row to empty
            for (int j = 0; j < gameBoard->width; j++) {
                setCell(gameBoard, j, BOARD_HEIGHT-1, 0);
            }

            gameBoard->score++;
        }
    }
}
