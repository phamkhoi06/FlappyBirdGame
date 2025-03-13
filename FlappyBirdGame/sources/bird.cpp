#include "../headers/bird.h" // Updated include path
#include "../headers/pipe.h" // Updated include path
#include <stdio.h>
#include <iostream>

bool
Bird::init(bool isDark)
{
    string bird_path = "resources/image/bird.png";
    if (Load(bird_path.c_str(), 1.0))
    {
        posBird.getPos(75, BaseTexture::SCREEN_HEIGHT / 2 - BaseTexture::BIRD_HEIGHT / 2);
        angle = 0;
        velocityY = 0;
        ahead = 0;
        return true;
    }
    else
    {
        printf("Failed to load bird texture!\n");
        return false;
    }
}
void Bird::Free() { free(); }
void Bird::render() { Render(posBird.x, posBird.y, angle); }
void Bird::applyGravity()
{
    const double GRAVITY = 0.3;
    const double MAX_FALL_SPEED = 8.0;
    if (!BaseTexture::die)
    {
        velocityY += GRAVITY;
        if (velocityY > MAX_FALL_SPEED)
            velocityY = MAX_FALL_SPEED;
        posBird.y += (int)velocityY;
        if (posBird.y + getHeight() >= BaseTexture::SCREEN_HEIGHT - BaseTexture::LAND_HEIGHT)
        {
            posBird.y = BaseTexture::SCREEN_HEIGHT - BaseTexture::LAND_HEIGHT - getHeight();
            velocityY = 0;
            BaseTexture::die = true;
        }
        if (posBird.y < 0)
        {
            posBird.y = 0;
            velocityY = 0;
            BaseTexture::die = true;
        }
        if (velocityY > 1.0)
        {
            angle += 3;
            if (angle > 70)
                angle = 70;
        }
        else if (angle > 0)
        {
            angle -= 2;
            if (angle < 0)
                angle = 0;
        }
    }
    else
    {
        velocityY += GRAVITY;
        if (velocityY > MAX_FALL_SPEED)
            velocityY = MAX_FALL_SPEED;
        posBird.y += (int)velocityY;
        if (posBird.y + getHeight() > BaseTexture::SCREEN_HEIGHT - BaseTexture::LAND_HEIGHT)
        {
            posBird.y = BaseTexture::SCREEN_HEIGHT - BaseTexture::LAND_HEIGHT - getHeight();
            velocityY = 0;
        }
        if (velocityY > 1.0)
        {
            angle += 3;
            if (angle > 70)
                angle = 70;
        }
    }
}
// Includes collision detection
void Bird::update(int pipeWidth, int pipeHeight)
{
    if (!BaseTexture::die)
    {
        applyGravity();
        if (!BaseTexture::die && !posPipe.empty())
        {
            position currentPipe = posPipe[ahead];
            int birdRight = posBird.x + getWidth();
            int birdBottom = posBird.y + getHeight();
            int pipeRight = currentPipe.x + pipeWidth;
            int tolerance = 5;
            if (birdRight > currentPipe.x + tolerance && posBird.x < pipeRight - tolerance)
            {
                bool hitTop = posBird.y < currentPipe.y + pipeHeight - tolerance;
                bool hitBottom = birdBottom > currentPipe.y + pipeHeight + BaseTexture::PIPE_SPACE + tolerance;
                if (hitTop || hitBottom)
                {
                    BaseTexture::die = true;
                    return;
                }
            }
        }
    }
    else
    {
        applyGravity();
    }
}
void Bird::flap()
{
    if (!BaseTexture::die)
    {
        velocityY = -6.5;
        angle = -25;
    }
}