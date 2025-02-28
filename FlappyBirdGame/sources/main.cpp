#define SDL_MAIN_HANDLED

#include "../headers/base.h"
#include "../headers/bird.h" 
#include <SDL2/SDL.h>
#include <iostream>

const int FPS = 60;
const int frameDelay = 1000 / FPS;

int main(int argc, char* argv[])
{
    if (!BaseTexture::initGraphic()) {
        printf("Failed to initialize graphics!\n");
        return -1;
    }

    Bird gameBird;
    if (!gameBird.init()) {
        printf("Failed to initialize bird!\n");
        BaseTexture::releaseGraphic();
        return -1;
    }

    Uint32 frameStart;
    int frameTime;

    while (!BaseTexture::quit)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&BaseTexture::event) != 0)
        {
            if (BaseTexture::event.type == SDL_QUIT)
            {
                BaseTexture::quit = true;
            }

        }


        gameBird.update();


        SDL_SetRenderDrawColor(BaseTexture::gRenderer, 135, 206, 235, 0xFF);
        SDL_RenderClear(BaseTexture::gRenderer);

        gameBird.render();

        SDL_RenderPresent(BaseTexture::gRenderer);


        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    gameBird.Free();
    BaseTexture::releaseGraphic();

    return 0;
}