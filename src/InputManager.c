#include "../include/InputManager.h"

InputManager* CreateInputManager()
{
    InputManager* manager = (InputManager*)(malloc(sizeof(InputManager)));
    manager->leftDown = false;
    manager->rightDown = false;

    for (int i=0; i<256; i++)
    {
        manager->keys[i] = false;
    }
    manager->keys[SDLK_SPACE] = true;

    return manager;
}

void DestroyInputManager(InputManager* manager)
{
    free(manager);
}
