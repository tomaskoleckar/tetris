#include "input.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>

#define NUM_MENU_ITEMS 4
#define NUM_SETTINGS_FIELDS 3

extern SHAPE activeShape;
extern SHAPE nextShape;
extern int selectedMenuItem;
extern int selectedInput;
extern GameState gameState;
extern int left, right, up, down;
extern GameBoard gameBoard;
extern TTF_Font *font;
extern GameSettings gameSettings;


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
                case TOP_SCORES:
                    gameState = TOP_SCORES_MENU;
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

void handlePlayingInput()
{
    SDL_Event e;
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
                gameState = MENU;
                break;
            }
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_DOWN:
                down = 1;
                break;
            }
            break;
        }
    }
}

void handleTopScoresInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            exit(0);

        switch (e.type)
        {
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_RETURN:
                gameState = MENU;
                break;
            case SDLK_ESCAPE:
                exit(0);
                break;
            }
            break;
        }
    }
}

void handleSettingsInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            exit(0);

        switch (e.type)
        {
        case SDL_KEYUP:

            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
                selectedInput = (selectedInput - 1 + NUM_SETTINGS_FIELDS) % NUM_SETTINGS_FIELDS;
                break;
            case SDLK_DOWN:
                selectedInput = (selectedInput + 1) % NUM_SETTINGS_FIELDS;
                break;
            case SDLK_LEFT:
                if (selectedInput == 0 && strlen(gameSettings.name) > 0)
                {
                    gameSettings.name[strlen(gameSettings.name) - 1] = '\0'; // Backspace to delete the last character
                }
                else if (selectedInput == 1)
                {
                    gameSettings.gameSpeed = (gameSettings.gameSpeed - 10 > 0) ? gameSettings.gameSpeed - 1 : 0;
                }
                break;
            case SDLK_RIGHT:
                if (selectedInput == 1)
                {
                    if (gameSettings.gameSpeed < 40)
                    {
                        gameSettings.gameSpeed += 1;
                    }
                }
                break;
            case SDLK_RETURN:
                if (selectedInput == NUM_SETTINGS_FIELDS - 1)
                {
                    gameState = MENU;
                }
                break;
            case SDLK_ESCAPE:
                exit(0);
                break;
            }
            break;
        }
    }
}

void input()
{
    up = down = left = right = 0;

    switch (gameState)
    {
    case MENU:
        handleMenuInput();
        break;
    case PLAYING:
        handlePlayingInput();
        break;
    case TOP_SCORES_MENU:
        handleTopScoresInput();
        break;
    case SETTINGS:
        handleSettingsInput();
        break;
    case QUIT_GAME:
        break;
    }
}
