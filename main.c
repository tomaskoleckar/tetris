#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "blocks.h"
#include "collisions.h"
#include <SDL2/SDL_ttf.h>
#include "render.h"

#define MS_PER_UPDATE 16
#define TILE_SIZE 22
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define NUM_MENU_ITEMS 3
#define BOARD_SIZE (BOARD_HEIGHT * BOARD_WIDTH)

typedef enum
{
    PLAY,
    SETTINGS_MENU,
    QUIT
} MenuOption;

int selectedMenuItem = 0;
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


GameBoard createGameBoard(int width, int height) {
    GameBoard board;
    board.width = width;
    board.height = height;
    board.data = malloc(sizeof(int) * width * height);
    return board;
}

void freeGameBoard(GameBoard *board) {
    free(board->data);
}

SHAPE generateNewShape()
{
    srand(time(NULL));
    return blocks[rand() % 7];
}

void rotate() {
    SHAPE originalShape = activeShape;  

    int newSize = reverseCols(activeShape.matrix, activeShape.size);
    activeShape.size = newSize;
    transpose(activeShape.matrix, activeShape.size);

    if (checkLeftCollision(activeShape)) {
        activeShape.x++;
    } else if (checkRightCollision(activeShape, BOARD_WIDTH)) {
        activeShape.x--;
    }

    if (checkCollision(activeShape, &gameBoard)) {
        activeShape = originalShape; 
    }
}

void lockShape()
{
    srand(time(NULL));
    updateGameBoard(activeShape, &gameBoard);
    clearLines(&gameBoard, &gameSettings);
    activeShape = nextShape;
    nextShape = generateNewShape();
    nextShape.x = rand() % (10 - nextShape.size);
    activeShape.y = 0;
    if (checkCollision(activeShape, &gameBoard))
    {
        exit(0);
    }
}

void update(int frameCount)
{
    if (frameCount % 20 == 0)
    {
        if (!checkCollision(activeShape, &gameBoard))
        {
            activeShape.y++;
        }
        else
        {
            lockShape();
        }
    }

    if (left && !checkLeftCollision(activeShape))
    {
        activeShape.x--;
    }
    if (right && !checkRightCollision(activeShape, BOARD_WIDTH))
    {
        activeShape.x++;
    }
    if (down)
    {
        if (!checkCollision(activeShape, &gameBoard))
        {
            if (activeShape.y + activeShape.size < TILE_SIZE * BOARD_HEIGHT)
            {
                activeShape.y++;
            }
            else
            {
                activeShape.y = 5;
            }
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

void handleMenuInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            gameState = QUIT_GAME;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
                selectedMenuItem = (selectedMenuItem - 1 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS;
                break;
            case SDLK_DOWN:
                selectedMenuItem = (selectedMenuItem + 1) % NUM_MENU_ITEMS;
                break;
            case SDLK_RETURN:
                switch (selectedMenuItem)
                {
                case PLAY:
                    gameState = PLAYING;
                    break;
                case SETTINGS_MENU:
                    gameState = SETTINGS;
                    break;
                case QUIT:
                    gameState = QUIT_GAME;
                    break;
                }
                break;
            case SDLK_ESCAPE:
                exit(0);
                break;
            }
        }
    }
}

void input()
{
    up = down = left = right = 0;
    SDL_Event e;

    switch (gameState)
    {
    case MENU:
        handleMenuInput();
        break;
    case PLAYING:
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                exit(0);
            switch (e.type)
            {
            case SDL_KEYUP:
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    left = 1;
                    break;
                case SDLK_RIGHT:
                    right = 1;
                    break;
                case SDLK_UP:
                    up = 1;
                    break;
                case SDLK_DOWN:
                    down = 0;
                    break;
                case SDLK_ESCAPE:
                    exit(0);
                    break;
                }
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_DOWN:
                    down = 1;
                    break;
                }
            }
        }
        break;
    case SETTINGS:
        // Handle input for settings state if needed
        break;
    case QUIT_GAME:
        break; // No input handling for QUIT state
    }
}

int main()
{
    activeShape = generateNewShape();
    activeShape.x = rand() % (10 - activeShape.size);
    nextShape = activeShape = generateNewShape();
    nextShape.x = rand() % (10 - nextShape.size);
    gameSettings.gameSpeed = 150;
    gameSettings.score = 0;
    SDL_Rect rect;
    rect.w = rect.h = TILE_SIZE;
    gameBoard = createGameBoard(BOARD_WIDTH, BOARD_HEIGHT); 

    left = right = up = down = 0;
    int frameCount = 0;
    lastFrame = 0;
    fps = 0;

    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            setCell(&gameBoard,i,j,0);
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
            update(frameCount);
            input();
            render(renderer, window, activeShape,nextShape, &gameBoard, gameState, selectedMenuItem, font,gameSettings);
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
