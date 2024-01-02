#ifndef RENDER_H
#define RENDER_H

#define TILE_SIZE 22
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#include "blocks.h"
#include "SDL2/SDL_ttf.h"
#include "collisions.h"

typedef enum {
    MENU,
    PLAYING,
    SETTINGS,
    QUIT_GAME
} GameState;

typedef struct {
    int width;
    int height;
    int *data;
} GameBoard;

typedef struct
{
    int gameSpeed;
    int score;
} GameSettings;

void renderGrid(SDL_Renderer *renderer);
void renderMenu(SDL_Renderer *renderer, TTF_Font *font, int selectedMenuItem);
void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);
void render(SDL_Renderer *renderer, SDL_Window *window, SHAPE activeShape,SHAPE nextShape, GameBoard *gameBoard, GameState gameState, int selectedMenuItem,TTF_Font *font,GameSettings gameSettings);
void draw(SHAPE activeShape, SHAPE nextShape, SDL_Renderer *renderer, GameBoard* gameBoard, TTF_Font *font,GameSettings gameSettings);
int getCell(GameBoard *board, int x, int y);
void setCell(GameBoard *board, int x, int y, int value);

#endif
