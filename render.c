#include "render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


void draw(SHAPE activeShape, SDL_Renderer *renderer, int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]) {
    SDL_Rect rect;
    rect.w = rect.h = TILE_SIZE;

    // Draw game board
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (gameBoard[i][j]) {
                rect.x = i * TILE_SIZE;
                rect.y = j * TILE_SIZE;
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    // Draw active shape
    for (int i = 0; i < activeShape.size; i++) {
        for (int j = 0; j < activeShape.size; j++) {
            if (activeShape.matrix[i][j]) {
                rect.x = (activeShape.x + i) * TILE_SIZE;
                rect.y = (activeShape.y + j) * TILE_SIZE;
                SDL_SetRenderDrawColor(renderer, activeShape.color.r, activeShape.color.g, activeShape.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void renderGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    for (int i = 0; i <= TILE_SIZE * BOARD_WIDTH; i += TILE_SIZE) {
        SDL_RenderDrawLine(renderer, i, 0, i, TILE_SIZE * BOARD_HEIGHT);
    }

    for (int j = 0; j <= TILE_SIZE * BOARD_HEIGHT; j += TILE_SIZE) {
        SDL_RenderDrawLine(renderer, 0, j, TILE_SIZE * BOARD_WIDTH, j);
    }
}

void renderMenu(SDL_Renderer *renderer, TTF_Font *font, int selectedMenuItem) {
    SDL_Rect playRect = {100, 100, 100, 50};
    SDL_Rect settingsRect = {100, 200, 150, 50};
    SDL_Rect quitRect = {100, 300, 100, 50};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &playRect);
    SDL_RenderFillRect(renderer, &settingsRect);
    SDL_RenderFillRect(renderer, &quitRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &playRect);
    SDL_RenderDrawRect(renderer, &settingsRect);
    SDL_RenderDrawRect(renderer, &quitRect);

    // Render text
    renderText(renderer, font, "Play", 120, 120);
    renderText(renderer, font, "Settings", 120, 220);
    renderText(renderer, font, "Quit", 120, 320);

    // Highlight the selected item
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect highlightRect = {100, 100 + selectedMenuItem * 100, 150, 50};
    SDL_RenderDrawRect(renderer, &highlightRect);
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color textColor = {255, 255, 255};  // You can change the text color

    // Render text to surface
    surface = TTF_RenderText_Solid(font, text, textColor);
    if (!surface) {
        fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());
        return;
    }

    // Create texture from surface
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    // Get the width and height of the text surface
    int textWidth = surface->w;
    int textHeight = surface->h;

    // Set the destination rectangle for rendering the text
    SDL_Rect destRect = {x, y, textWidth, textHeight};

    // Render the text to the screen
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    // Clean up
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer *renderer, SDL_Window *window, SHAPE activeShape, int gameBoard[BOARD_HEIGHT][BOARD_WIDTH], GameState gameState, int selectedMenuItem,TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (gameState) {
        case MENU:
            renderMenu(renderer, font, selectedMenuItem);
            break;
        case PLAYING:
            renderGrid(renderer);
            draw(activeShape, renderer, gameBoard);
            break;
        case SETTINGS:
            // Render settings if needed
            break;
        case QUIT_GAME:
            exit(0);
    }

    SDL_RenderPresent(renderer);
}