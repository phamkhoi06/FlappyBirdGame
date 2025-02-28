#include "../headers/game.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

game::game()
{
    if (!BaseTexture::initGraphic())
    {
        printf("FATAL: Graphics initialization failed!\n");
        BaseTexture::quit = true;
    }
    else
    {
        // Initialize bird
        if (!bird.init())
        {
            printf("FATAL: Bird initialization failed!\n");
            BaseTexture::quit = true;
        }
    }
    userInput.Type = input::NONE;
}

game::~game()
{
    bird.Free();
    BaseTexture::releaseGraphic();
}

void game::takeInput()
{
    userInput.Type = input::NONE;
    while (SDL_PollEvent(&BaseTexture::event) != 0)
    {
        if (BaseTexture::event.type == SDL_QUIT)
        {
            userInput.Type = input::QUIT;
            BaseTexture::quit = true;
        }
        else if (BaseTexture::event.type == SDL_MOUSEBUTTONDOWN ||
            (BaseTexture::event.type == SDL_KEYDOWN &&
                (BaseTexture::event.key.keysym.sym == SDLK_SPACE || BaseTexture::event.key.keysym.sym == SDLK_UP) &&
                BaseTexture::event.key.repeat == 0))
        {
            userInput.Type = input::PLAY;
        }
    }
}

void game::update()
{
    if (!isDie())
    {
        if (userInput.Type == input::PLAY)
        {
            bird.flap();
        }
        bird.update();
    }
    else
    {
        bird.applyGravity();
    }
}

void game::render()
{
    SDL_SetRenderDrawColor(BaseTexture::gRenderer, 135, 206, 235, 0xFF);
    SDL_RenderClear(BaseTexture::gRenderer);

    bird.render();
}

void game::display()
{
    SDL_RenderPresent(BaseTexture::gRenderer);
}

bool game::isQuit()
{
    return BaseTexture::quit;
}

bool game::isDie()
{
    return BaseTexture::die;
}

void game::Restart() { /* To be implemented */ }
int game::getPipeWidth() { return 0; }
int game::getPipeHeight() { return 0; }