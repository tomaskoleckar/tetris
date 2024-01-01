#include "render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


void draw(SHAPE activeShape, SHAPE nextShape, SDL_Renderer *renderer, int (*gameBoard)[BOARD_WIDTH], TTF_Font *font)
 {
    SDL_Rect rect;
    rect.w = rect.h = TILE_SIZE;

    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (gameBoard[j * BOARD_WIDTH + i]) {  // Adjusted indexing for 1D array
                rect.x = TILE_SIZE * i;
                rect.y = TILE_SIZE * j;
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

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

    for (int i = 0; i < nextShape.size; i++) {
        for (int j = 0; j < nextShape.size; j++) {
            if (nextShape.matrix[i][j]) {
                rect.x = (BOARD_WIDTH + 3 + i) * TILE_SIZE;
                rect.y = (5 + j) * TILE_SIZE;
                SDL_SetRenderDrawColor(renderer, nextShape.color.r, nextShape.color.g, nextShape.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 219, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    renderText(renderer,font,"Score : ",TILE_SIZE*(BOARD_WIDTH+1),1);
    renderText(renderer,font,"0",TILE_SIZE*(BOARD_WIDTH+5),1);
    renderText(renderer,font,"Next shape",TILE_SIZE*(BOARD_WIDTH+1),2*TILE_SIZE);


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

    renderText(renderer, font, "Play", 120, 120);
    renderText(renderer, font, "Settings", 120, 220);
    renderText(renderer, font, "Quit", 120, 320);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect highlightRect = {100, 100 + selectedMenuItem * 100, 150, 50};
    SDL_RenderDrawRect(renderer, &highlightRect);
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color textColor = {255, 255, 255};

    surface = TTF_RenderText_Solid(font, text, textColor);
    if (!surface) {
        fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    int textWidth = surface->w;
    int textHeight = surface->h;

    SDL_Rect destRect = {x, y, textWidth, textHeight};

    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer *renderer, SDL_Window *window, SHAPE activeShape,SHAPE nextShape, int (*gameBoard)[BOARD_WIDTH], GameState gameState, int selectedMenuItem,TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (gameState) {
        case MENU:
            renderMenu(renderer, font, selectedMenuItem);
            break;
        case PLAYING:
            renderGrid(renderer);
            draw(activeShape, nextShape , renderer, gameBoard,font);
            break;
        case SETTINGS:
            break;
        case QUIT_GAME:
            exit(0);
    }

    SDL_RenderPresent(renderer);
}
