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
    TOP_SCORES_MENU,
    QUIT_GAME
} GameState;

typedef struct {
    int width;
    int height;
    int *data;
    int score;
    int lastScore;
} GameBoard;

typedef enum
{
    PLAY,
    TOP_SCORES,
    SETTINGS_MENU,
    QUIT
} MenuOption;

typedef struct
{
    int gameSpeed;
    char name[50];
} GameSettings;

void renderGrid(SDL_Renderer *renderer);
void renderMenu(SDL_Renderer *renderer, TTF_Font *font, int selectedMenuItem,GameBoard *gameBoard);
void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);
void render(SDL_Renderer *renderer, SDL_Window *window, SHAPE activeShape, SHAPE nextShape, GameBoard *gameBoard, GameState gameState, int selectedMenuItem, TTF_Font *font, GameSettings *settings, int selectedSettingsItem);
void draw(SHAPE activeShape, SHAPE nextShape, SDL_Renderer *renderer, GameBoard* gameBoard, TTF_Font *font);
int getCell(GameBoard *board, int x, int y);
void setCell(GameBoard *board, int x, int y, int value);

#endif
