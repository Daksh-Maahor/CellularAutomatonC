#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/Utils.h"
#include "../include/Window.h"
#include "../include/InputManager.h"
#include "../include/LifeManager.h"

void Render(Window* window, LifeManager* manager)
{
    ClearScreen(window);
        
    RenderLifeManager(manager);

    Display(window);
}

void Update(Window* window, LifeManager* manager, InputManager* inputManager)
{
    Tick(manager, inputManager);
    Render(window, manager);
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL failed to init. Error : %s\n", SDL_GetError());
    }

    if (!IMG_Init(IMG_INIT_PNG))
    {
        printf("IMG failed to init. Error : %s\n", SDL_GetError());
    }

    int width = 1280;
    int height = 720;

    int scale = 10;

    int speedDivider = 3;

    Window* window = CreateWindow("Game of Life V2", width, height);
    InputManager* inputManager = CreateInputManager();
    LifeManager* manager = CreateManager(window, width, height, scale, speedDivider);

    bool gameRunning = true;

    SDL_Event event;

    const float timeStep = 0.01f;
    float accumulator = 0.0f;
    float currentTime = HireTimeInSeconds();

    // for my clock
    int fps = 30;
    double timePerTick = 1000/fps;
    double delta = 0;
    long now;
    long lastTime = SDL_GetTicks();
    long timer = 0;
    int ticks = 0;

    while (gameRunning)
    {
        int startTicks = SDL_GetTicks();

        float newTime = HireTimeInSeconds();
        float frameTime = newTime - currentTime;

        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= timeStep)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    gameRunning = false;
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        inputManager->leftDown = true;
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        inputManager->rightDown = true;
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONUP)
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        inputManager->leftDown = false;
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        inputManager->rightDown = false;
                    }
                }
                else if (event.type == SDL_MOUSEMOTION)
                {
                    inputManager->mouseX = event.motion.x;
                    inputManager->mouseY = event.motion.y;
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    if (!(event.key.keysym.sym == SDLK_SPACE))
                    {
                        inputManager->keys[event.key.keysym.sym] = true;
                    }
                }
                else if (event.type == SDL_KEYUP)
                {
                    if (!(event.key.keysym.sym == SDLK_SPACE))
                    {
                        inputManager->keys[event.key.keysym.sym] = false;
                    }
                    else
                    {
                        inputManager->keys[event.key.keysym.sym] = !inputManager->keys[event.key.keysym.sym];
                    }
                }
            }

            accumulator -= timeStep;
        }
        //my clock below

        now = SDL_GetTicks();
        delta += (now - lastTime) / timePerTick;
        timer += (now - lastTime);
        lastTime = now;

        if (delta >= 1)
        {
            //update here
            Update(window, manager, inputManager);
            //update above
            ticks++;
            delta--;
        }

        if (timer >= 1000)
        {
            printf("Ticks and Frames : %d\n", ticks);
            ticks = 0;
            timer = 0;
        }

        //my clock above

        int frameTicks = SDL_GetTicks() - startTicks;

        if (frameTicks < 1000 / GetRefreshRate(window))
        {
            SDL_Delay(1000 / GetRefreshRate(window) - frameTicks);
        }
    }

    DestroyInputManager(inputManager);
    CleanUp(manager);
    DestroyWindow(window);

    SDL_Quit();

    return 0;
}
