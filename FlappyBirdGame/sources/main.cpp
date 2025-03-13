#define SDL_MAIN_HANDLED
#include "../headers/game.h"
#include <SDL2/SDL.h>
#include <iostream>
const int FPS = 60;
const int frameDelay = 1000 / FPS;
int main(int argc, char* argv[])
{
    game game_instance;
    if (game_instance.isQuit())
    {
        return -1;
    }
    Uint32 frameStart;
    int frameTime;
    while (!game_instance.isQuit())
    {
        frameStart = SDL_GetTicks();
        game_instance.takeInput();
        game_instance.update();
        game_instance.render();
        game_instance.display();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}
