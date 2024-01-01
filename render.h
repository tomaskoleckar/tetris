#ifndef RENDER_H
#define RENDER_H

#define TILE_SIZE 22
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#include "blocks.h"
#include "SDL2/SDL_ttf.h"

typedef enum {
    MENU,
    PLAYING,
    SETTINGS,
    QUIT_GAME
} GameState;

void renderGrid(SDL_Renderer *renderer);
void renderMenu(SDL_Renderer *renderer, TTF_Font *font, int selectedMenuItem);
void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);
void render(SDL_Renderer *renderer, SDL_Window *window, SHAPE activeShape,SHAPE nextShape, int gameBoard[BOARD_HEIGHT][BOARD_WIDTH], GameState gameState, int selectedMenuItem,TTF_Font *font);
void draw(SHAPE activeShape,SHAPE nextShape, SDL_Renderer *renderer, int gameBoard[BOARD_HEIGHT][BOARD_WIDTH],TTF_Font *font);

#endif
