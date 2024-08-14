#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <SDL2/SDL.h>

#include <stdbool.h>

typedef struct
{
    bool keys[256];
    bool leftDown;
    bool rightDown;
    int mouseX, mouseY;
} InputManager;

InputManager* CreateInputManager();
void DestroyInputManager(InputManager* manager);

#endif
