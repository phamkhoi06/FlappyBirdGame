#include "../headers/game.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
game::game()
{
    srand(time(NULL));
    if (!initGraphic())
    {
        BaseTexture::quit = true;
        return;
    }
    pipe.init();
    land.init();
    userInput.Type = input::NONE;
}
game::~game()
{
    bird.Free();
    pipe.Free();
    land.Free();
    releaseGraphic();
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
        else if (BaseTexture::event.type == SDL_MOUSEBUTTONDOWN || (BaseTexture::event.type == SDL_KEYDOWN && (BaseTexture::event.key.keysym.sym == SDLK_SPACE || BaseTexture::event.key.keysym.sym == SDLK_UP) && BaseTexture::event.key.repeat == 0))
        {
            userInput.Type = input::PLAY;
        }
    }
}
bool game::initGraphic()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
            printf("Warning: Linear texture filtering not enabled!");
        BaseTexture::gWindow = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BaseTexture::SCREEN_WIDTH, BaseTexture::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (BaseTexture::gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            BaseTexture::gRenderer = SDL_CreateRenderer(BaseTexture::gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (BaseTexture::gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(BaseTexture::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}
void game::releaseGraphic()
{
    if (BaseTexture::gRenderer)
        SDL_DestroyRenderer(BaseTexture::gRenderer);
    BaseTexture::gRenderer = NULL;
    if (BaseTexture::gWindow)
        SDL_DestroyWindow(BaseTexture::gWindow);
    BaseTexture::gWindow = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
void game::update()
{
    bool wasAlive = !isDie();
    if (wasAlive)
    {
        if (userInput.Type == input::PLAY)
            bird.flap();
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
    if (!isDie())
    {
        renderScoreLarge();
    }
}
void game::display() { SDL_RenderPresent(BaseTexture::gRenderer); }
bool game::isQuit() { return BaseTexture::quit; }
bool game::isDie() { return BaseTexture::die; }
void game::Restart()
{
    BaseTexture::die = false;
    BaseTexture::score = 0;
    bird.init(false);
    pipe.init();
    userInput.Type = input::NONE;
}
int game::getPipeWidth() { return pipe.width(); }
int game::getPipeHeight() { return pipe.height(); }
void game::renderScoreLarge()
{
    string s = to_string(BaseTexture::score);
    signed char len = s.length();
    BaseTexture image;
    int digitWidth = 0, Kern = 2, totalWidth = 0;
    if (len > 0)
    {
        image.Load("resources/number/large/0.png", 1.0);
        digitWidth = image.getWidth();
        image.free();
    }
    if (digitWidth <= 0 && len > 0)
        return;
    totalWidth = (digitWidth * len) + (Kern * (len > 0 ? len - 1 : 0));
    int currentX = (BaseTexture::SCREEN_WIDTH - totalWidth) / 2;
    const int ScoreY = 100;
    for (signed char i = 0; i < len; i++)
    {
        signed char number = s[i] - '0';
        string digitPath = "resources/number/large/" + string(1, s[i]) + ".png";
        if (image.Load(digitPath, 1.0))
        {
            image.Render(currentX, ScoreY);
            currentX += image.getWidth() + Kern;
            image.free();
        }
        else
        {
            currentX += digitWidth + Kern;
        }
    }
}
void game::renderScoreSmall() {}
void game::renderBestScore() {}
void game::renderMessage() {}
void game::renderBackground() {}
void game::renderBackgroundNight() {}
void game::renderLand() {}
void game::resume() {}
void game::pause() {}
void game::renderPauseTab() {}
void game::lightTheme() {}
void game::darkTheme() {}
void game::nextButton() {}
bool game::changeTheme() { return false; }
void game::renderGameOver() {}
void game::renderMedal() {}
void game::replay() {}
bool game::checkReplay() { return false; }
