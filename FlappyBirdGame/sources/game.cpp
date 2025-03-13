#include "../headers/game.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

game::game()
{
    srand(time(NULL));
    if (!BaseTexture::initGraphic())
    {
        BaseTexture::quit = true;
    }
    else
    {
        if (!bird.init())
        {
            BaseTexture::quit = true;
        }
        if (!pipe.init())
        {
            BaseTexture::quit = true;
        }
        if (!land.init())
        {
            BaseTexture::quit = true;
        }
    }
    userInput.Type = input::NONE;
} // Init land
game::~game()
{
    bird.Free();
    pipe.Free();
    land.Free();
    BaseTexture::releaseGraphic();
} // Free land
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
        else if (BaseTexture::event.type == SDL_MOUSEBUTTONDOWN || (BaseTexture::event.type == SDL_KEYDOWN && (BaseTexture::event.key.keysym.sym == SDLK_SPACE || BaseTexture::event.key.keysym.sym == SDLK_UP) && BaseTexture::event.key.repeat == 0))
        {
            userInput.Type = input::PLAY;
        }
    }
}
void game::update()
{
    bool wasAlive = !isDie();
    if (wasAlive)
    {
        if (userInput.Type == input::PLAY)
        {
            bird.flap();
        }
        pipe.update();
        land.update();
    }
    bird.update(getPipeWidth(), getPipeHeight());
    if (wasAlive && isDie())
        printf("--- GAME OVER ---\n");
}
void game::render()
{
    SDL_SetRenderDrawColor(BaseTexture::gRenderer, 135, 206, 235, 0xFF);
    SDL_RenderClear(BaseTexture::gRenderer);
    pipe.render();
    land.render();
    bird.render();
} // Render land
void game::display() { SDL_RenderPresent(BaseTexture::gRenderer); }
bool game::isQuit() { return BaseTexture::quit; }
bool game::isDie() { return BaseTexture::die; }
void game::Restart()
{
    BaseTexture::die = false;
    BaseTexture::score = 0;
    bird.init();
    pipe.init();
    userInput.Type = input::NONE;
}
int game::getPipeWidth() { return pipe.width(); }
int game::getPipeHeight() { return pipe.height(); }