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
    posPipe.clear();
    for (signed char i = 0; i < TOTAL_PIPE; i++)
    {
        position temp;
        temp.getPos(SCREEN_WIDTH + i * PIPE_DISTANCE + 350, (rand() % (randMax - randMin + 1)) + randMin);
        posPipe.push_back(temp);
    }
    if (isNULL())
    {
        if (!Load("resources/image/pipe.png", 1))
            return false;
    }
    return !isNULL();
}
void pipe::Free() { free(); }
void pipe::render()
{
    if (isNULL())
        return;
    for (signed char i = 0; i < TOTAL_PIPE; i++)
    {
        if (posPipe[i].x <= SCREEN_WIDTH && posPipe[i].x > -getWidth())
            Render(posPipe[i].x, posPipe[i].y);
        Render(posPipe[i].x, posPipe[i].y + getHeight() + PIPE_SPACE, 180);
    }
}
void pipe::update()
{
    if (isNULL())
        return;
    if (!die)
    {
        const int randMin_actual = -getHeight() + 30;
        const int randMax_actual = SCREEN_HEIGHT - LAND_HEIGHT - getHeight() - PIPE_SPACE - 30;
        for (signed char i = 0; i < TOTAL_PIPE; i++)
        {
            if (posPipe[i].x < -getWidth())
            {
                posPipe[i].y = (rand() % (randMax_actual - randMin_actual + 1)) + randMin_actual;
                posPipe[i].x = posPipe[(i + TOTAL_PIPE - 1) % TOTAL_PIPE].x + PIPE_DISTANCE;
            }
            else
            {
                posPipe[i].x -= 3;
            }
        }
    }
}