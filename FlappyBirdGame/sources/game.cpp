#include "../headers/game.h"
#include "../headers/sound.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
game::game() : scaleNumberS(0.75), bestScore(0)
{
    srand(time(NULL));
    if (!initGraphic())
    {
        BaseTexture::quit = true;
        return;
    }
    pipe.init();
    land.init();
    if (!sound.init())
    {
        printf("Warning: Sound init failed!\n");
    }
    userInput.Type = input::NONE;
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
{ /* Same as commit 7 */
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
        else if (BaseTexture::event.type == SDL_KEYDOWN && BaseTexture::event.key.keysym.sym == SDLK_ESCAPE && BaseTexture::event.key.repeat == 0)
        {
            userInput.Type = input::PAUSE;
        }
    }
}
bool game::initGraphic()
{ /* Same as commit 7 */
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
{ /* Same as commit 7 */
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
void game::display()
{
    SDL_RenderPresent(BaseTexture::gRenderer);
    SDL_RenderClear(BaseTexture::gRenderer);
}
bool game::isQuit() { return BaseTexture::quit; }
bool game::isDie() { return BaseTexture::die; }
int game::getPipeWidth() { return pipe.width(); }
int game::getPipeHeight() { return pipe.height(); }
void game::Restart()
{ /* Same as commit 7 */
    BaseTexture::die = false;
    BaseTexture::score = 0;
    bird.resetTime();
}
// Score/UI functions same as commit 7
void game::renderScoreDigit(signed char digit, int& currentX, int yPos, bool large, double scale)
{
    BaseTexture image;
    string sizeStr = large ? "large" : "small";
    string digitPath = "resources/number/" + sizeStr + "/" + to_string(digit) + ".png";
    const int Kern = large ? 2 : 1;
    if (image.Load(digitPath, scale))
    {
        image.Render(currentX, yPos);
        currentX += image.getWidth() + Kern;
        image.free();
    }
    else
    {
        int phW = large ? 24 : 18;
        currentX += phW * scale + Kern;
    }
}
void game::renderScoreLarge()
{
    string s = to_string(BaseTexture::score);
    signed char len = s.length();
    BaseTexture image;
    for (signed char i = 0; i < len; i++)
    {
        signed char number = s[i] - '0';
        string path = "resources/number/large/" + to_string(number) + ".png";
        image.Load(path, 1);
        image.Render((BaseTexture::SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i * (image.getWidth() + 10)), 100);
        image.free();
    }
}
void game::renderScoreSmall()
{
    string s = to_string(BaseTexture::score);
    signed char len = s.length();
    BaseTexture image;
    for (signed char i = len - 1; i >= 0; i--)
    {
        signed char number = s[i] - '0';
        string path = "resources/number/small/" + to_string(number) + ".png";
        image.Load(path, scaleNumberS);
        image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 268);
        image.free();
    }
}
void game::loadBestScore()
{
    ifstream fileIn("resources/data/bestScore.txt");
    if (fileIn.is_open())
        fileIn >> bestScore;
    else
        bestScore = 0;
    fileIn.close();
}
void game::saveBestScore()
{
    ofstream fileOut("resources/data/bestScore.txt", ios::trunc);
    if (fileOut.is_open())
        fileOut << bestScore;
    fileOut.close();
}
void game::renderBestScore()
{
    loadBestScore();
    ofstream fileOut("resources/data/bestScore.txt", ios::trunc);
    if (BaseTexture::score > bestScore)
        bestScore = BaseTexture::score;
    string s = to_string(bestScore);
    signed char len = s.length();
    BaseTexture image;
    for (signed char i = len - 1; i >= 0; i--)
    {
        signed char number = s[i] - '0';
        string path = "resources/number/small/" + to_string(number) + ".png";
        image.Load(path, scaleNumberS);
        image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 315);
        image.free();
    }
    if (fileOut.is_open())
        fileOut << bestScore;
    fileOut.close();
}
void game::renderMessage()
{
    BaseTexture image;
    image.Load("resources/image/message.png", 1);
    image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 180);
    image.free();
}
void game::renderBackground()
{
    BaseTexture image;
    image.Load("resources/image/background.png", 1);
    image.Render(0, 0);
    image.free();
}
void game::renderBackgroundNight()
{
    BaseTexture image;
    image.Load("resources/image/background-night.png", 1);
    image.Render(0, 0);
    image.free();
}
void game::renderLand() {}
void game::resume()
{
    BaseTexture image;
    image.Load("resources/image/resume.png", 1);
    image.Render(BaseTexture::SCREEN_WIDTH - 50, 20);
    image.free();
}
void game::pause()
{
    BaseTexture image;
    image.Load("resources/image/pause.png", 1);
    image.Render(BaseTexture::SCREEN_WIDTH - 50, 20);
    image.free();
}
void game::renderPauseTab()
{
    BaseTexture image;
    image.Load("resources/image/pauseTab.png", 1);
    image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 230);
    image.free();
}
void game::lightTheme()
{
    BaseTexture image;
    image.Load("resources/image/bird.png", 0.8);
    image.Render(105, 315);
    image.free();
}
void game::darkTheme()
{
    BaseTexture image;
    image.Load("resources/image/bird-dark.png", 0.8);
    image.Render(105, 315);
    image.free();
}
void game::nextButton()
{
    BaseTexture image;
    image.Load("resources/image/nextRight.png", 1);
    image.Render(149, 322);
    image.Load("resources/image/nextLeft.png", 1);
    image.Render(88, 322);
    image.free();
}
bool game::changeTheme()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (((x > 149 && x < 149 + 13) || (x > 88 && x < 88 + 13)) && (y > 322 && y < 322 + 16))
        return true;
    return false;
}
void game::renderGameOver()
{
    BaseTexture image;
    image.Load("resources/image/gameOver.png", 1);
    image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 150);
    image.free();
}
void game::renderMedal()
{
    BaseTexture image;
    string medalPath = "resources/medal/";
    if (BaseTexture::score > 50)
        medalPath += "gold.png";
    else if (BaseTexture::score > 20)
        medalPath += "silver.png";
    else
        medalPath += "honor.png";
    image.Load(medalPath, scaleNumberS);
    image.Render(82, 275);
    image.free();
}
void game::replay()
{
    BaseTexture image;
    image.Load("resources/image/replay.png", 1);
    image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 380);
    image.free();
}
bool game::checkReplay()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    BaseTexture image;
    image.Load("resources/image/replay.png", 1);
    int w = image.getWidth();
    int h = image.getHeight();
    image.free();
    int btnX = (BaseTexture::SCREEN_WIDTH - w) / 2;
    int btnY = 380;
    if (x > btnX && x < btnX + w && y > btnY && y < btnY + h)
        return true;
    return false;
}
