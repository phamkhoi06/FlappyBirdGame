#include "../headers/base.h"
#include <iostream>
#include <string>

// Static definitions
bool BaseTexture::quit = false;
bool BaseTexture::die = true;
int BaseTexture::score = 0;
SDL_Window* BaseTexture::gWindow = NULL;
SDL_Renderer* BaseTexture::gRenderer = NULL;
SDL_Event BaseTexture::event;
const int BaseTexture::SCREEN_WIDTH = 350;
const int BaseTexture::SCREEN_HEIGHT = 625;
const int BaseTexture::PIPE_SPACE = 160;
const int BaseTexture::TOTAL_PIPE = 4;
const int BaseTexture::PIPE_DISTANCE = 220;
const int BaseTexture::LAND_HEIGHT = 140;
const int BaseTexture::BIRD_WIDTH = 50;
const int BaseTexture::BIRD_HEIGHT = 35;

BaseTexture::BaseTexture() { Texture = NULL; }
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
bool BaseTexture::Load(std::string path, double scale)
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