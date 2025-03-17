#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <stdio.h>


class position
{
public:
    int x, y, angle, state;
    void setPos(const int x, const int y);
};

class BaseTexture
{
public:
    BaseTexture();
    ~BaseTexture() {};
    bool isNULL();
    bool Load(std::string path, double scale = 1);
    int getWidth();
    int getHeight();
    void free();
    void Render(int x, int y, int angle = 0, SDL_Rect* clip = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

public:
    SDL_Texture* Texture;
    int tWidth;
    int tHeight;

public:
    static SDL_Window* gWindow;
    static SDL_Renderer* gRenderer;
    static SDL_Event event;
    static bool quit;
    static bool die;
    static int score;
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;
    static const int PIPE_SPACE;
    static const int TOTAL_PIPE;
    static const int PIPE_DISTANCE;
    static const int LAND_HEIGHT;
    static const int BIRD_WIDTH;
    static const int BIRD_HEIGHT;
};