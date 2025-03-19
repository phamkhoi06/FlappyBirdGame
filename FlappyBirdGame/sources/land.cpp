#include "../headers/land.h"
#include "../headers/base.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
bool
land::init()
{
    string land_path = "resources/image/land.png";
    if (!Load(land_path.c_str(), 1.0))
    {
        printf("Failed to load land texture!\n");
        return false;
    }
    posLand.getPos(0, BaseTexture::SCREEN_HEIGHT - getHeight());
    return true;
}
void land::Free() { free(); }
void land::render()
{
    int currentX = posLand.x;
    Render(currentX, posLand.y);
    Render(currentX + getWidth(), posLand.y);
}
void land::update()
{
    if (!BaseTexture::die)
    {
        const int LAND_SPEED = 3;
        posLand.x -= LAND_SPEED;
        if (posLand.x <= -getWidth())
        {
            posLand.x = 0;
        }
    }
}