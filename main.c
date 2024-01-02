#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "blocks.h"
#include "render.h"
#include "collisions.h"
#include <SDL2/SDL_ttf.h>
#include "input.h"

#define TOP_SCORES_FILE "topScores.txt"
#define NUM_TOP_SCORES 10
#define MS_PER_UPDATE 16
#define TILE_SIZE 22
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define NUM_MENU_ITEMS 4
#define NUM_SETTINGS_FIELDS 3
#define MAX_GAME_SPEED 40
#define BOARD_SIZE (BOARD_HEIGHT * BOARD_WIDTH)


int selectedMenuItem = 0;
int selectedInput = 0;
SHAPE activeShape;
SHAPE nextShape;
SDL_Renderer *renderer;
SDL_Window *window;
GameState gameState = MENU;
int timerFPS, lastFrame, fps;
int left, right, up, down, active;
GameBoard gameBoard;
TTF_Font *font;
GameSettings gameSettings;

GameBoard createGameBoard(int width, int height)
{
    GameBoard board;
    board.width = width;
    board.height = height;
    board.data = malloc(sizeof(int) * width * height);

    if (board.data == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for game board\n");
        exit(1);
    }

    return board;
}

void freeGameBoard(GameBoard *board)
{
    free(board->data);
}

SHAPE generateNewShape()
{
    srand(time(NULL));
    return blocks[rand() % 7];
}

void rotate()
{
    SHAPE originalShape = activeShape;

    int newSize = reverseCols(activeShape.matrix, activeShape.size);
    activeShape.size = newSize;
    transpose(activeShape.matrix, activeShape.size);

    if (checkLeftCollision(activeShape, &gameBoard))
    {
        activeShape.x++;
    }
    else if (checkRightCollision(activeShape, BOARD_WIDTH, &gameBoard))
    {
        activeShape.x--;
    }

    if (checkCollision(activeShape, &gameBoard))
    {
        activeShape = originalShape;
    }
}

void checkForEnd(GameBoard *gameBoard)
{
    int currentScore = gameBoard->score;
    int topScores[NUM_TOP_SCORES];
    int gameOver = 0;

    FILE *file = fopen(TOP_SCORES_FILE, "a+");

    if (file != NULL)
    {
        fseek(file, 0, SEEK_SET);
        for (int i = 0; i < NUM_TOP_SCORES; i++)
        {
            if (fscanf(file, "%d", &topScores[i]) != 1)
            {
                topScores[i] = 0;
            }
        }

        fclose(file);
    }
    else
    {
        perror("Error opening top scores file");
        return;
    }
    for (int i = 0; i < gameBoard->width; i++)
    {
        if (getCell(gameBoard, i, BOARD_HEIGHT - 1) == 1)
        {
            gameOver = 1;
        }
        }

    if (gameOver)
    {
        int isNewTopScore = 0;
        for (int i = 0; i < NUM_TOP_SCORES; i++)
        {
            if (currentScore > topScores[i])
            {
                isNewTopScore = 1;
                break;
            }
        }
        if (isNewTopScore)
        {
            file = fopen(TOP_SCORES_FILE, "w");
            if (file != NULL)
            {
                for (int i = 0; i < NUM_TOP_SCORES; i++)
                {
                    if (currentScore > topScores[i])
                    {
                        fprintf(file, "%d\n", currentScore);
                        currentScore = topScores[i];
                    }
                    else
                    {
                        fprintf(file, "%d\n", topScores[i]);
                    }
                }

                fclose(file);
            }
            else
            {
                perror("Error opening top scores file for writing");
            }

        }
        gameBoard->lastScore = gameBoard->score;
            gameBoard->score = 0;

            for (int i = 0; i < BOARD_WIDTH; i++)
            {
                for (int j = 0; j < BOARD_HEIGHT; j++)
                {
                    setCell(gameBoard, i, j, 0);
                }
            }

            gameState = MENU;
    }
}

void lockShape()
{
    srand(time(NULL));
    updateGameBoard(activeShape, &gameBoard);
    clearLines(&gameBoard);
    activeShape = nextShape;
    nextShape = generateNewShape();
    nextShape.x = rand() % (10 - nextShape.size);
    activeShape.y = 0;
    checkForEnd(&gameBoard);
}

void update(int frameCount)
{
    SHAPE testShape = activeShape;
    testShape.y++;

    if (frameCount % (MAX_GAME_SPEED - gameSettings.gameSpeed + 1) == 0)
    {
        int collision = checkCollision(testShape, &gameBoard);
        if (!collision)
        {
            activeShape.y++;
        }
        else if (collision == 2)
        {
            activeShape.y++;
            lockShape();
        }
        else
        {
            lockShape();
        }
    }

    if (left && !checkLeftCollision(activeShape, &gameBoard))
    {
        activeShape.x--;
    }
    if (right && !checkRightCollision(activeShape, BOARD_WIDTH, &gameBoard))
    {
        activeShape.x++;
    }
    if (down)
    {
        int collision = checkCollision(testShape, &gameBoard);
        if (!collision)
        {
            activeShape.y++;
        }
        else if (collision == 2)
        {
            activeShape.y++;
            lockShape();
        }
        else
        {
            lockShape();
        }
    }
    if (up)
    {
        rotate();
    }
}

int main()
{
    activeShape = generateNewShape();
    activeShape.x = rand() % (10 - activeShape.size);
    nextShape = activeShape = generateNewShape();
    nextShape.x = rand() % (10 - nextShape.size);
    gameSettings.gameSpeed = 20;
    strcpy(gameSettings.name, "Player");
    SDL_Rect rect;
    rect.w = rect.h = TILE_SIZE;
    gameBoard = createGameBoard(BOARD_WIDTH, BOARD_HEIGHT);
    gameBoard.score = 0;
    gameBoard.lastScore = -1;
    left = right = up = down = 0;
    int frameCount = 0;
    lastFrame = 0;
    fps = 0;

    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            setCell(&gameBoard, i, j, 0);
        }
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    font = TTF_OpenFont("OpenSans-Regular.ttf", 24); // Replace with your font path and size
    if (!font)
    {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
        exit(1);
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed at SDL_Init()\n");
        exit(1);
    }

    if (SDL_CreateWindowAndRenderer((TILE_SIZE * 2) * BOARD_WIDTH, TILE_SIZE * BOARD_HEIGHT, 0, &window, &renderer) < 0)
    {
        printf("Failed at SDL_CreateWindowAndRenderer()\n");
        exit(1);
    }

    SDL_SetWindowTitle(window, "Tetris");

    Uint32 currentTime, deltaTime;
    Uint32 lastFrameTime = SDL_GetTicks();

    while (1)
    {
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastFrameTime;

        if (deltaTime >= MS_PER_UPDATE)
        {
            lastFrameTime = currentTime;
            frameCount++;
            if (gameState == PLAYING)
            {
                update(frameCount);
            }
            input();
            render(renderer, window, activeShape, nextShape, &gameBoard, gameState, selectedMenuItem, font, &gameSettings, selectedInput);
        }
    }

    freeGameBoard(&gameBoard);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
