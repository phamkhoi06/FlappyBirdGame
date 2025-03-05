#define SDL_MAIN_HANDLED 

#include "../headers/base.h" 
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


        SDL_SetRenderDrawColor(BaseTexture::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(BaseTexture::gRenderer);




        SDL_RenderPresent(BaseTexture::gRenderer);


        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }


    BaseTexture::releaseGraphic();

    return 0;
}