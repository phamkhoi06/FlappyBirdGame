#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

#include "../headers/Base.h"
#include "../headers/Pipe.h"


std::vector<position> posPipe;

bool pipe::init()
{
    if (isNULL())
    {
        if (!Load("resources/sprites/pipe.png", 1))
        {
            printf("Failed to load pipe texture in init!\n");
            return false;
        }
    }

    posPipe.clear();
    const int playableHeight = BaseTexture::SCREEN_HEIGHT - BaseTexture::LAND_HEIGHT;
    const int pipeTextureHeight = getHeight();

    const int randMin_actual = (int)(0.20 * playableHeight) - pipeTextureHeight;
    const int randMax_actual = (int)(0.80 * playableHeight) - pipeTextureHeight - BaseTexture::PIPE_SPACE;

    for (signed char i = 0; i < BaseTexture::TOTAL_PIPE; i++)
    {
        position temp;
        int yPos = (randMax_actual > randMin_actual)
            ? (rand() % (randMax_actual - randMin_actual + 1)) + randMin_actual
            : (BaseTexture::SCREEN_HEIGHT / 2 - BaseTexture::PIPE_SPACE / 2 - pipeTextureHeight);
        temp.setPos(BaseTexture::SCREEN_WIDTH + i * BaseTexture::PIPE_DISTANCE + 350, yPos);
        posPipe.push_back(temp);
    }
    return !isNULL();
}
void pipe::Free() { free(); }
void pipe::render()
{
    if (isNULL())
        return;
    for (signed char i = 0; i < BaseTexture::TOTAL_PIPE; i++)
    {
        if (posPipe[i].x <= BaseTexture::SCREEN_WIDTH && posPipe[i].x > -getWidth())
            Render(posPipe[i].x, posPipe[i].y);
        Render(posPipe[i].x, posPipe[i].y + getHeight() + BaseTexture::PIPE_SPACE, 180);
    }
}
void pipe::update()
{
    if (isNULL())
        return;
    if (!BaseTexture::die)
    {
        const int playableHeight = BaseTexture::SCREEN_HEIGHT - BaseTexture::LAND_HEIGHT;
        const int pipeTextureHeight = getHeight();
        const int randMin_actual = (int)(0.20 * playableHeight) - pipeTextureHeight;
        const int randMax_actual = (int)(0.80 * playableHeight) - pipeTextureHeight - BaseTexture::PIPE_SPACE;

        for (signed char i = 0; i < BaseTexture::TOTAL_PIPE; i++)
        {
            if (posPipe[i].x < -getWidth())
            {
                int yPos = (randMax_actual > randMin_actual)
                    ? (rand() % (randMax_actual - randMin_actual + 1)) + randMin_actual
                    : (BaseTexture::SCREEN_HEIGHT / 2 - BaseTexture::PIPE_SPACE / 2 - pipeTextureHeight);
                posPipe[i].y = yPos;
                posPipe[i].x = posPipe[(i + BaseTexture::TOTAL_PIPE - 1) % BaseTexture::TOTAL_PIPE].x + BaseTexture::PIPE_DISTANCE;
            }
            else
            {
                posPipe[i].x -= 3;
            }
        }
    }
}