#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width, height;
} Window;

Window* CreateWindow(const char* title, int width, int height);
int GetRefreshRate(Window* window);
void DestroyWindow(Window* window);
void ClearScreen(Window* window);
void Display(Window* window);
void FillRect(Window* window, int x, int y, int w, int h, int r, int g, int b);
void RenderTexture(Window* window, SDL_Texture* texture, int x, int y, int w, int h);
SDL_Texture* LoadTexture(Window* window, const char* path);

#endif
