#include "../headers/pipe.h"
#include "../headers/base.h"
#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
vector<position>
posPipe;
bool pipe::init()
{
    if (!Load("resources/image/pipe.png", 1.0))
    {
        printf("Failed to load pipe texture!\n");
        return false;
    }
    posPipe.clear();
    const int randMin = -getHeight() + 60;
    const int randMax = BaseTexture::SCREEN_HEIGHT - BaseTexture::LAND_HEIGHT - BaseTexture::PIPE_SPACE - getHeight() - 60;
    for (signed char i = 0; i < BaseTexture::TOTAL_PIPE; i++)
    {
        position temp;
        int initialX = BaseTexture::SCREEN_WIDTH + i * BaseTexture::PIPE_DISTANCE + 100;
        int randomY = (rand() % (randMax - randMin + 1)) + randMin;
        temp.getPos(initialX, randomY);
        posPipe.push_back(temp);
    }
    return true;
}
void pipe::Free()
{
    free();
    posPipe.clear();
}
void pipe::render()
{
    for (signed char i = 0; i < BaseTexture::TOTAL_PIPE; i++)
    {
        if (posPipe[i].x < BaseTexture::SCREEN_WIDTH && posPipe[i].x > -getWidth())
        {
            Render(posPipe[i].x, posPipe[i].y);
            Render(posPipe[i].x, posPipe[i].y + getHeight() + BaseTexture::PIPE_SPACE, 0, NULL, SDL_FLIP_VERTICAL);
        }
    }
}
void pipe::update()
{
    if (!BaseTexture::die)
    {
        const int PIPE_SPEED = 3;
        const int randMin = -getHeight() + 60;
        const int randMax = BaseTexture::SCREEN_HEIGHT - BaseTexture::LAND_HEIGHT - BaseTexture::PIPE_SPACE - getHeight() - 60;
        for (signed char i = 0; i < BaseTexture::TOTAL_PIPE; i++)
        {
            posPipe[i].x -= PIPE_SPEED;
            if (posPipe[i].x < -getWidth())
            {
                int max_x = -getWidth();
                for (int j = 0; j < BaseTexture::TOTAL_PIPE; ++j)
                {
                    if (posPipe[j].x > max_x)
                        max_x = posPipe[j].x;
                }
                posPipe[i].x = max_x + BaseTexture::PIPE_DISTANCE;
                posPipe[i].y = (rand() % (randMax - randMin + 1)) + randMin;
            }
        }
    }
}