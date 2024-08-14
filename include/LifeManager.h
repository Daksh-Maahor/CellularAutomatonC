#ifndef LIFE_MANAGER_H_
#define LIFE_MANAGER_H_

#include "Window.h"
#include "InputManager.h"
#include <stdbool.h>

typedef struct
{
    Window* window;
    
    int extraTimes;
    int width, height;
    int scale;

    bool* cells;
    bool* next;

    int xOffset, yOffset;

    int scrollSpeed;

    int speedDivider;

    int tickCounter;
    SDL_Texture* liveCell;
} LifeManager;

LifeManager* CreateManager(Window* window, int width, int height, int scale, int speedDivider);
void Clear(LifeManager* manager);
void Tick(LifeManager* manager, InputManager* inputManager);
void CheckInput(LifeManager* manager, InputManager* inputManager);
void RenderLifeManager(LifeManager* manager);
void CleanUp(LifeManager* manager);

#endif
