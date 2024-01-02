#include "render.h"
#include "collisions.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TOP_SCORES_FILE "topScores.txt"
#define NUM_TOP_SCORES 10

int getCell(GameBoard *board, int x, int y)
{
    return board->data[y * board->width + x];
}

void setCell(GameBoard *board, int x, int y, int value)
{
    board->data[y * board->width + x] = value;
}

void draw(SHAPE activeShape, SHAPE nextShape, SDL_Renderer *renderer, GameBoard *gameBoard, TTF_Font *font)
{
    int ones = 0;
    SDL_Rect rect;
    rect.w = rect.h = TILE_SIZE;

    for (int i = 0; i < gameBoard->width; i++)
    {
        for (int j = 0; j < gameBoard->height; j++)
        {
            if (getCell(gameBoard, i, j))
            {
                ones++;
                rect.x = TILE_SIZE * i;
                rect.y = TILE_SIZE * (BOARD_HEIGHT - j - 1); 
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    for (int i = 0; i < activeShape.size; i++)
    {
        for (int j = 0; j < activeShape.size; j++)
        {
            if (activeShape.matrix[i][j])
            {
                rect.x = (activeShape.x + i) * TILE_SIZE;
                rect.y = (activeShape.y + j) * TILE_SIZE;
                SDL_SetRenderDrawColor(renderer, activeShape.color.r, activeShape.color.g, activeShape.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    for (int i = 0; i < nextShape.size; i++)
    {
        for (int j = 0; j < nextShape.size; j++)
        {
            if (nextShape.matrix[i][j])
            {
                rect.x = (BOARD_WIDTH + 3 + i) * TILE_SIZE;
                rect.y = (5 + j) * TILE_SIZE;
                SDL_SetRenderDrawColor(renderer, nextShape.color.r, nextShape.color.g, nextShape.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    char str2[10];
    sprintf(str2, "%d", ones);

    char str[10];
    sprintf(str, "%d", gameBoard->score);

    renderText(renderer, font, "Score : ", TILE_SIZE * (BOARD_WIDTH + 1), 1);
    renderText(renderer, font, str, TILE_SIZE * (BOARD_WIDTH + 5), 1);
    renderText(renderer, font, str2, TILE_SIZE * (BOARD_WIDTH + 5),350);
    renderText(renderer, font, "Next shape", TILE_SIZE * (BOARD_WIDTH + 1), 2 * TILE_SIZE);
}

void renderGrid(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    for (int i = 0; i <= TILE_SIZE * BOARD_WIDTH; i += TILE_SIZE)
    {
        SDL_RenderDrawLine(renderer, i, 0, i, TILE_SIZE * BOARD_HEIGHT);
    }

    for (int j = 0; j <= TILE_SIZE * BOARD_HEIGHT; j += TILE_SIZE)
    {
        SDL_RenderDrawLine(renderer, 0, j, TILE_SIZE * BOARD_WIDTH, j);
    }
}

void renderMenu(SDL_Renderer *renderer, TTF_Font *font, int selectedMenuItem,GameBoard *gameBoard)
{
    if(gameBoard->lastScore != -1){
    char str[10];
    sprintf(str, "%d", gameBoard->lastScore);
    renderText(renderer, font, "Last Score :", 120, 20);
    renderText(renderer, font, str, 250, 20);
    }

    renderText(renderer, font, "Play", 200, 100);
    renderText(renderer, font, "Top scores", 170, 160);
    renderText(renderer, font, "Settings", 180, 220);
    renderText(renderer, font, "Quit", 200, 280);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect highlightRect = {150, 100 + selectedMenuItem * 60, 150, 50};
    SDL_RenderDrawRect(renderer, &highlightRect);
}

void renderSettings(SDL_Renderer *renderer, TTF_Font *font, GameSettings *settings, int selectedInput)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    if(selectedInput == 0){
    SDL_Rect nameRect = {155, 50, 100, 40};
    SDL_RenderDrawRect(renderer, &nameRect);
    }
    renderText(renderer, font, "Name:", 10, 50);

    char speedStr[20];
    sprintf(speedStr, "%d", settings->gameSpeed);

    if (strlen(settings->name) > 0) {
        renderText(renderer, font, settings->name, 170, 50);
    }

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    if(selectedInput == 1){
    SDL_Rect speedRect = {155, 120, 100, 40};
    SDL_RenderDrawRect(renderer, &speedRect);
    }
    renderText(renderer, font, "Game Speed:", 10, 120);

    if (strlen(speedStr) > 0) {
        renderText(renderer, font, speedStr, 170, 125);
    }

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    if(selectedInput == 2){
    SDL_Rect backRect = {155, 190, 100, 40};
    SDL_RenderDrawRect(renderer, &backRect);
    }
    renderText(renderer, font, "Back", 170, 195);

    SDL_RenderPresent(renderer);
}

void renderTopScores(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    

    FILE *file = fopen(TOP_SCORES_FILE, "r");
    if (file != NULL)
    {
        int topScores[NUM_TOP_SCORES];
        for (int i = 0; i < NUM_TOP_SCORES; i++)
        {
            if (fscanf(file, "%d", &topScores[i]) != 1)
            {
                topScores[i] = 0;
            }
        }
        fclose(file);

        int y = 50;
        for (int i = 0; i < NUM_TOP_SCORES; i++)
        {
            char scoreText[20];
            sprintf(scoreText, "%d", topScores[i]);
            renderText(renderer, font, scoreText, 195, y);
            y += 30;
        }
    }
    else
    {
        perror("Error opening top scores file for reading");
    }

    SDL_Rect backRect = {158, 380, 100, 50};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &backRect);

    renderText(renderer, font, "Top 10 scores", 135, 10);
    renderText(renderer, font, "Back", 180, 385);

    SDL_RenderPresent(renderer);
}



void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, (SDL_Color){255, 255, 255});
    if (!surface)
    {
        fprintf(stderr, "Error rendering text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    int textWidth = surface->w;
    int textHeight = surface->h;

    if (textWidth > 0 && textHeight > 0)  // Check for non-zero width and height
    {
        SDL_Rect destRect = {x, y, textWidth, textHeight};
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void render(SDL_Renderer *renderer, SDL_Window *window, SHAPE activeShape, SHAPE nextShape, GameBoard *gameBoard, GameState gameState, int selectedMenuItem, TTF_Font *font, GameSettings *settings, int selectedSettingsItem)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (gameState)
    {
    case MENU:
        renderMenu(renderer, font, selectedMenuItem, gameBoard);
        break;
    case PLAYING:
        renderGrid(renderer);
        draw(activeShape, nextShape, renderer, gameBoard, font);
        break;
    case SETTINGS:
        renderSettings(renderer, font, settings, selectedSettingsItem);
        break;
    case TOP_SCORES_MENU:
        renderTopScores(renderer,font);
        break;
    case QUIT_GAME:
        exit(0);
    }

    SDL_RenderPresent(renderer);
}
