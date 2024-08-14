#include "../include/Window.h"

#include <stdio.h>
#include <SDL2/SDL_image.h>

Window* CreateWindow(const char* title, int w, int h)
{
    Window* window = (Window*)(malloc(sizeof(Window)));

    window->window = NULL;
    window->width = w;
    window->height = h;

    window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);

    if (window->window == NULL)
    {
        printf("Window failed to init. Error : %s", SDL_GetError());
    }

    window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    return window;
}

int GetRefreshRate(Window* window)
{
    int displayIndex = SDL_GetWindowDisplayIndex(window->window);

    SDL_DisplayMode mode;

    SDL_GetDisplayMode(displayIndex, 0, &mode);

    return mode.refresh_rate;
}

void DestroyWindow(Window* window)
{
    SDL_DestroyWindow(window->window);
    SDL_DestroyRenderer(window->renderer);

    free(window);
}

void ClearScreen(Window* window)
{
    SDL_RenderClear(window->renderer);
    FillRect(window, 0, 0, window->width, window->height, 0, 0, 0);
}

void Display(Window* window)
{
    SDL_RenderPresent(window->renderer);
}

void FillRect(Window* window, int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect fillRect = {x, y, w, h};

    SDL_SetRenderDrawColor(window->renderer, r, g, b, 1);

    SDL_RenderFillRect(window->renderer, &fillRect);
}

SDL_Texture* LoadTexture(Window* window, const char* path)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(window->renderer, path);

    if (texture == NULL)
    {
        printf("Failed to load texture, %s, Error : %s\n", path, SDL_GetError());
    }

    return texture;
}

void RenderTexture(Window* window, SDL_Texture* texture, int x, int y, int w, int h)
{
    SDL_Rect destRect = {x, y, w, h};

    SDL_RenderCopy(window->renderer, texture, NULL, &destRect);
}
