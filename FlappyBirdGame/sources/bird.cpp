#include "../headers/bird.h"
#include <stdio.h>
#include <iostream>

bool Bird::init(bool isDark)
{
    string bird_path = "../resources/image/bird.png";
    if (Load(bird_path.c_str(), 1.0))
    {
        posBird.getPos(75, SCREEN_HEIGHT / 2 - BIRD_HEIGHT / 2);
        angle = 0;
        velocityY = 0;
        return true;
    }
    else
    {
        printf("Failed to load bird texture!\n");
        return false;
    }
}

void Bird::Free()
{
    free();
}

void Bird::render()
{
    Render(posBird.x, posBird.y, angle);
}

void Bird::applyGravity()
{
    const double GRAVITY = 0.3;
    const double MAX_FALL_SPEED = 8.0;

    if (!BaseTexture::die)
    {
        velocityY += GRAVITY;
        if (velocityY > MAX_FALL_SPEED) {
            velocityY = MAX_FALL_SPEED;
        }
        posBird.y += (int)velocityY;


        if (posBird.y + getHeight() >= SCREEN_HEIGHT - LAND_HEIGHT) {
            posBird.y = SCREEN_HEIGHT - LAND_HEIGHT - getHeight();
            velocityY = 0;

        }

        if (posBird.y < 0) {
            posBird.y = 0;
            velocityY = 0;
        }



        if (velocityY > 1.0) {
            angle += 3;
            if (angle > 70) angle = 70;
        }
        else if (angle > 0) {
            angle -= 2;
            if (angle < 0) angle = 0;
        }
    }
}

void Bird::update(int pipeWidth, int pipeHeight)
{
    applyGravity();

}


void Bird::flap()
{
    if (!BaseTexture::die)
    {
        velocityY = -6.5;
        angle = -25;
    }
}