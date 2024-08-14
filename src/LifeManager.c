#include "../include/LifeManager.h"

LifeManager* CreateManager(Window* window, int width, int height, int scale, int speedDivider)
{
    LifeManager* manager = (LifeManager*)(malloc(sizeof(LifeManager)));

    manager->extraTimes = 3;
    manager->scrollSpeed = 20;

    manager->window = window;
    manager->scale = scale;

    manager->width = (width * manager->extraTimes) / manager->scale;
    manager->height = (height * manager->extraTimes) / manager->scale;

    manager->cells = (bool*)(malloc(sizeof(bool) * manager->width * manager->height));
    manager->next = (bool*)(malloc(sizeof(bool) * manager->width * manager->height));

    manager->xOffset = 0;
    manager->yOffset = 0;

    manager->speedDivider = speedDivider;
    manager->tickCounter = 0;

    manager->liveCell = LoadTexture(window, "res/liveCell.png");

    Clear(manager);

    return manager;
}

void Clear(LifeManager* manager)
{
    for (int x=0; x<manager->width; x++)
    {
        for (int y=0; y<manager->height; y++)
        {
            manager->cells[x + y * manager->width] = false;
            manager->next[x + y * manager->width] = false;
        }
    }
}

void Tick(LifeManager* manager, InputManager* inputManager)
{
    CheckInput(manager, inputManager);
    if (!inputManager->keys[SDLK_SPACE])
    {
        (manager->tickCounter)++;
        if (manager->tickCounter >= manager->speedDivider)
        {
            for (int x=0; x < manager->width; x++)
            {
                for (int y=0; y < manager->height; y++)
                {
                    manager->next[x + y * manager->width] = manager->cells[x + y * manager->width];
                }
            }

            for (int x=0; x<manager->width; x++)
            {
                for (int y=0; y<manager->height; y++)
                {
                    bool currentCell = manager->cells[x + y * manager->width];
                    int aliveNeighbours = 0;

                    for (int dx = -1; dx <= 1; dx++)
                    {
                        int tx = x + dx;
                        for (int dy = -1; dy <= 1; dy++)
                        {
                            if (dx == 0 && dy == 0)
                            {
                                continue;
                            }

                            int ty = y + dy;

                            if (tx < 0 || ty < 0)
                            {
                                continue;
                            }

                            if (tx >= manager->width || ty >= manager->height)
                            {
                                continue;
                            }

                            aliveNeighbours += manager->cells[tx + ty * manager->width] ? 1 : 0;
                        }
                    }

                    if (currentCell && (aliveNeighbours < 2 || aliveNeighbours > 3))
                    {
                        manager->next[x + y * manager->width] = !currentCell;
                    }

                    if (!currentCell && aliveNeighbours == 3)
                    {
                        manager->next[x + y * manager->width] = !currentCell;
                    }
                }
            }

            for (int x=0; x < manager->width; x++)
            {
                for (int y=0; y < manager->height; y++)
                {
                    manager->cells[x + y * manager->width] = manager->next[x + y * manager->width];
                }
            }
            manager->tickCounter = 1;
        }
    }
}

void CheckInput(LifeManager* manager, InputManager* inputManager)
{
    if (inputManager->keys[SDLK_SPACE])
    {
        if (inputManager->leftDown)
        {
            int x = (inputManager->mouseX + manager->xOffset) / manager->scale;
            int y = (inputManager->mouseY + manager->yOffset) / manager->scale;

            manager->cells[x + y * manager->width] = true;
        }
        if (inputManager->rightDown)
        {
            int x = (inputManager->mouseX + manager->xOffset) / manager->scale;
            int y = (inputManager->mouseY + manager->yOffset) / manager->scale;

            manager->cells[x + y * manager->width] = false;
        }
        if (inputManager->keys[SDLK_c])
        {
            Clear(manager);
        }
    }
    if (inputManager->keys[SDLK_w])
    {
        manager->yOffset -= manager->scrollSpeed;
    }
    if (inputManager->keys[SDLK_s])
    {
        manager->yOffset += manager->scrollSpeed;
    }
    if (inputManager->keys[SDLK_a])
    {
        manager->xOffset -= manager->scrollSpeed;
    }
    if (inputManager->keys[SDLK_d])
    {
        manager->xOffset += manager->scrollSpeed;
    }
    if (manager->xOffset < 0)
    {
        manager->xOffset = 0;
    }
    if (manager->xOffset >= manager->width * manager->scale - manager->window->width)
    {
        manager->xOffset = manager->width * manager->scale - manager->window->width;
    }
    if (manager->yOffset < 0)
    {
        manager->yOffset = 0;
    }
    if (manager->yOffset >= manager->height * manager->scale - manager->window->height)
    {
        manager->yOffset = manager->height * manager->scale - manager->window->height;
    }
    if (inputManager->keys[SDLK_o])
    {
        manager->xOffset = manager->width * manager->scale / 2;
        manager->yOffset = manager->height * manager->scale / 2;
    }
    if (inputManager->keys[SDLK_i])
    {
        manager->xOffset = 0;
        manager->yOffset = 0;
    }
}

void RenderLifeManager(LifeManager* manager)
{
    for (int x=manager->xOffset / manager->scale; x<(manager->xOffset + manager->window->width) / manager->scale; x++)
    {
        for (int y=manager->yOffset / manager->scale; y<(manager->yOffset + manager->window->height) / manager->scale; y++)
        {
            bool cell = manager->cells[x + y * manager->width];
            if (cell)
            {
                RenderTexture(manager->window, manager->liveCell, x * manager->scale - manager->xOffset, y * manager->scale - manager->yOffset, manager->scale, manager->scale);
            }
        }
    }
}

void CleanUp(LifeManager* manager)
{
    free(manager->liveCell);
    free(manager->cells);
    free(manager->next);

    free(manager);
}
