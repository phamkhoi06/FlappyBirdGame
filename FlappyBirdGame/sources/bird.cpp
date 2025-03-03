#include <stdio.h>
#include <iostream>
#include <string>

#include "../headers/bird.h"
#include "../headers/pipe.h"


bool Bird::init(bool isDark)
{
    std::string bird_path = isDark ? "resources/image/bird-dark.png" : "resources/image/bird.png";
    if (saved_path == bird_path && !isNULL())
    {
        posBird.getPos(75, SCREEN_HEIGHT / 2 - BIRD_HEIGHT / 2);
        ahead = 0;
        angle = 0;
        resetTime();
    }
    if (isNULL() || saved_path != bird_path)
    {
        saved_path = bird_path;
        if (Load(bird_path.c_str(), 1))
        {
            posBird.getPos(75, SCREEN_HEIGHT / 2 - BIRD_HEIGHT / 2);
            ahead = 0;
            angle = 0;
            resetTime();
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
void Bird::Free()
{
    free();
    saved_path = "";
}
void Bird::render() { Render(posBird.x, posBird.y, angle); }
void Bird::fall()
{
    if (die && posBird.y < SCREEN_HEIGHT - LAND_HEIGHT - BIRD_HEIGHT - 5)
    {
        if (time == 0)
        {
            x0 = posBird.y;
            angle = -25;
        }
        else if (angle < 70 && time > 30)
        {
            angle += 3;
        }
        if (time >= 0)
        {
            posBird.y = static_cast<int>(x0 + time * time * 0.18 - 7.3 * time);
            time++;
        }
    }
    else
        return;
}
void Bird::update(int pipeWidth, int pipeHeight)
{
    if (!die)
    {
        if (time == 0)
        {
            x0 = posBird.y;
            angle = -25;
        }
        else if (angle < 70 && time > 30)
        {
            angle += 3;
        }
        if (time >= 0)
        {
            posBird.y = static_cast<int>(x0 + time * time * 0.18 - 7.3 * time);
            time++;
        }
        if (!posPipe.empty() && (posBird.x + getWidth() > posPipe[ahead].x + 5) && (posBird.x + 5 < posPipe[ahead].x + pipeWidth) && (posBird.y + 5 < posPipe[ahead].y + pipeHeight || posBird.y + getHeight() > posPipe[ahead].y + pipeHeight + PIPE_SPACE + 5))
        {
            die = true;
        }
        else if (!posPipe.empty() && posBird.x > posPipe[ahead].x + pipeWidth)
        {
            ahead = (ahead + 1) % TOTAL_PIPE;
            score++;
        }
        if (posBird.y > SCREEN_HEIGHT - LAND_HEIGHT - BIRD_HEIGHT - 5 || posBird.y < -10)
        {
            die = true;
        }
    }
}
void Bird::resetTime() { time = 0; }
void Bird::flap()
{
    if (!die)
    {
        angle = -25;
        resetTime();
    }
}