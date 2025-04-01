#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>

#include "../headers/Game.h"
#include "../headers/Base.h"


game::game() : scaleNumberS(0.75), bestScore(0)
{
    initGraphic();
    pipe.init();
    land.init();
    sound.init();
    loadBestScore();
}

game::~game()
{
    saveBestScore();
    bird.Free();
    pipe.Free();
    land.Free();
    sound.Free();
    releaseGraphic();
}

void game::takeInput()
{
    while (SDL_PollEvent(&BaseTexture::event) != 0)
    {
        if (BaseTexture::event.type == SDL_QUIT)
        {
            userInput.Type = game::input::QUIT;
            BaseTexture::quit = true;
        }
        else if (BaseTexture::event.type == SDL_MOUSEBUTTONDOWN || (BaseTexture::event.type == SDL_KEYDOWN &&
            (BaseTexture::event.key.keysym.sym == SDLK_SPACE || BaseTexture::event.key.keysym.sym == SDLK_UP) &&
            BaseTexture::event.key.repeat == 0))
        {
            userInput.Type = game::input::PLAY;
        }
        else if (BaseTexture::event.type == SDL_KEYDOWN && BaseTexture::event.key.keysym.sym == SDLK_ESCAPE && BaseTexture::event.key.repeat == 0)
        {
            userInput.Type = game::input::PAUSE;
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
		printf("SDL initialized!\n");
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
        else
        {
			printf("Linear texture filtering enabled!\n");
        }
        BaseTexture::gWindow = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BaseTexture::SCREEN_WIDTH, BaseTexture::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (BaseTexture::gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
			printf("Window created!\n");
            BaseTexture::gRenderer = SDL_CreateRenderer(BaseTexture::gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (BaseTexture::gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
				printf("Renderer created!\n");
                SDL_SetRenderDrawColor(BaseTexture::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                else
                {
					printf("SDL_image initialized!\n");
                }
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
                else
                {
					printf("SDL_ttf initialized!\n");
                }
            }
        }
    }
    return success;
}

void game::releaseGraphic()
{
    if (BaseTexture::gRenderer)
    {
        SDL_DestroyRenderer(BaseTexture::gRenderer);
        BaseTexture::gRenderer = NULL;
    }
    if (BaseTexture::gWindow)
    {
        SDL_DestroyWindow(BaseTexture::gWindow);
        BaseTexture::gWindow = NULL;
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void game::display()
{
    SDL_RenderPresent(BaseTexture::gRenderer);
    SDL_RenderClear(BaseTexture::gRenderer);
}

void game::renderScoreSmall()
{
    std::string s = std::to_string(BaseTexture::score);
    signed char len = s.length();
    BaseTexture image;

    // Calculate the width of all digits to center properly
    int totalWidth = 0;
    int* digitWidths = new int[len];

    for (signed char i = 0; i < len; i++) {
        signed char number = s[i] - '0';
        std::string path = "resources/sprites/number/small/";
        path += std::to_string(number) + ".png";
        image.Load(path, scaleNumberS);
        digitWidths[i] = image.getWidth();
        totalWidth += image.getWidth() * scaleNumberS;
        if (i < len - 1) totalWidth += 5; // spacing between digits
        image.free();
    }
    for (signed char i = len - 1; i >= 0; i--)
    {
        signed char number = s[i] - '0';
        std::string path = "resources/sprites/number/small/";
        path += std::to_string(number) + ".png";
        image.Load(path, scaleNumberS);
        //image.Render((BaseTexture::SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i + 30) * i, 100);
        image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 268);
        image.free();
    }
}

void game::renderScoreLarge()
{
    std::string s = std::to_string(BaseTexture::score);
    signed char len = s.length();
    BaseTexture image;
    for (signed char i = 0; i < len; i++)
    {
        signed char number = s[i] - '0';
        std::string path = "resources/sprites/number/large/";
        path += std::to_string(number) + ".png";
        image.Load(path, 1);
        //image.Render((BaseTexture::SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i * (image.getWidth() + 10)), 100);
        image.Render((BaseTexture::SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i + 30) * i, 100);
        image.free();
    }
}

void game::renderBestScore()
{
    loadBestScore();
    std::ofstream fileOut("resources/data/bestscore.txt", std::ios::trunc);
    if (BaseTexture::score > bestScore)
    {
        bestScore = BaseTexture::score;
    }
    std::string s = std::to_string(bestScore);
    signed char len = s.length();
    BaseTexture image;

    // Calculate the width of all digits to center properly
    int totalWidth = 0;
    int* digitWidths = new int[len];

    for (signed char i = 0; i < len; i++) {
        signed char number = s[i] - '0';
        std::string path = "resources/sprites/number/small/";
        path += std::to_string(number) + ".png";
        image.Load(path, scaleNumberS);
        digitWidths[i] = image.getWidth();
        totalWidth += image.getWidth() * scaleNumberS;
        if (i < len - 1) totalWidth += 5; // spacing between digits
        image.free();
    }

    for (signed char i = len - 1; i >= 0; i--)
    {
        signed char number = s[i] - '0';
        std::string path = "resources/sprites/number/small/";
        path += std::to_string(number) + ".png";
        image.Load(path, scaleNumberS);
        //image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 315);
        //image.Render((BaseTexture::SCREEN_WIDTH - totalWidth) / 2, 315);
        image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 315);
        image.free();
    }
    if (fileOut.is_open())
    {
        fileOut << bestScore;
        fileOut.close();
    }
}

void game::loadBestScore()
{
    std::ifstream fileIn("resources/saved/bestscore.txt");
    if (fileIn.is_open())
    {
        fileIn >> bestScore;
        fileIn.close();
    }
    else
    {
        bestScore = 0;
    }
}

void game::saveBestScore()
{
    std::ofstream fileOut("resources/saved/bestscore.txt", std::ios::trunc);
    if (fileOut.is_open())
    {
        fileOut << bestScore;
        fileOut.close();
    }
}

void game::renderMessage()
{
    BaseTexture image;
    image.Load("resources/sprites/message.png", 1);
    image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 100);
    image.free();
}
void game::renderBackground()
{
    BaseTexture image;
    image.Load("resources/sprites/background.png", 1);
    image.Render(0, 0);
    image.free();
}
void game::renderLand() {}
void game::resume()
{
    BaseTexture image;
    image.Load("resources/sprites/resume.png", 1);
    image.Render(BaseTexture::SCREEN_WIDTH - 50, 20);
    image.free();
}
void game::pause()
{
    BaseTexture image;
    image.Load("resources/sprites/pause.png", 1);
    image.Render(BaseTexture::SCREEN_WIDTH - 50, 20);
    image.free();
}
void game::renderPauseTab()
{
    BaseTexture image;
    image.Load("resources/sprites/pauseTab.png", 1);
    image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 230);
    image.free();
}
void game::birdImage()
{
    BaseTexture image;
    image.Load("resources/sprites/bird.png", 0.8);
    image.Render(105, 315);
    image.free();
}
void game::renderGameOver()
{
    BaseTexture image;
    image.Load("resources/sprites/gameOver.png", 1);
    image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 150);
    image.free();
}
void game::replay()
{
    BaseTexture image;
    image.Load("resources/sprites/replay.png", 1);
    image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 380);
    image.free();
}
bool game::checkReplay()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    BaseTexture image;
    image.Load("resources/sprites/replay.png", 1);
    int w = image.getWidth();
    int h = image.getHeight();
    image.free();
    int btnX = (BaseTexture::SCREEN_WIDTH - w) / 2;
    int btnY = 380;
    if (x > btnX && x < btnX + w && y > btnY && y < btnY + h)
        return true;
    return false;
}

void game::Restart()
{
    BaseTexture::die = false;
    BaseTexture::score = 0;
    bird.init();
    pipe.init();
}