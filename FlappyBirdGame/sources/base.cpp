#include "../headers/base.h"
#include <iostream>

bool BaseTexture::quit = false;
bool BaseTexture::die = false;
int BaseTexture::score = 0;
SDL_Window* BaseTexture::gWindow = NULL;
SDL_Renderer* BaseTexture::gRenderer = NULL;
SDL_Event BaseTexture::event;

BaseTexture::BaseTexture()
{
    Texture = NULL;
    tWidth = 0;
    tHeight = 0;
}
int BaseTexture::getWidth() { return tWidth; }
int BaseTexture::getHeight() { return tHeight; }
void BaseTexture::free()
{
    if (Texture != NULL)
    {
        SDL_DestroyTexture(Texture);
        Texture = NULL;
        tWidth = 0;
        tHeight = 0;
    }
}
void BaseTexture::Render(int x, int y, int angle, SDL_Rect* clip, SDL_RendererFlip flip)
{
    SDL_Rect Rec_Render = { x, y, tWidth, tHeight };
    if (clip != NULL)
    {
        Rec_Render.w = clip->w;
        Rec_Render.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, Texture, clip, &Rec_Render, angle, NULL, flip);
}
bool BaseTexture::Load(string path, double scale)
{
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
        Texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (Texture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            tWidth = (loadedSurface->w) * scale;
            tHeight = (loadedSurface->h) * scale;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return Texture != NULL;
}
bool BaseTexture::isNULL()
{
    if (Texture == NULL)
        return true;
    return false;
}
void position::getPos(const int x, const int y)
{
    this->x = x;
    this->y = y;
}
bool BaseTexture::initGraphic()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
        gWindow = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
void BaseTexture::releaseGraphic()
{
    if (gRenderer)
    {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = NULL;
    }
    if (gWindow)
    {
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
    }
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
