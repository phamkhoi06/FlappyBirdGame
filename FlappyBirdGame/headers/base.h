#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

class BaseTexture;

class position
{
public:
    int x = 0, y = 0, angle = 0, state = 0;
    void getPos(const int x, const int y);
};

class BaseTexture
{
public:
    BaseTexture();
    virtual ~BaseTexture() { free(); }

    virtual bool Load(string path, double scale = 1);
    virtual void Render(int x, int y, int angle = 0, SDL_Rect* clip = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    virtual void free();
    virtual int getWidth();
    virtual int getHeight();
    virtual bool isNULL();

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


    enum InputType { INPUT_QUIT, INPUT_PLAY, INPUT_NONE };


    static const int SCREEN_WIDTH = 350;
    static const int SCREEN_HEIGHT = 625;
    static const int PIPE_SPACE = 160;
    static const int TOTAL_PIPE = 4;
    static const int PIPE_DISTANCE = 220;
    static const int LAND_HEIGHT = 140;
    static const int BIRD_WIDTH = 50;
    static const int BIRD_HEIGHT = 35;

    static bool initGraphic();
    static void releaseGraphic();
};