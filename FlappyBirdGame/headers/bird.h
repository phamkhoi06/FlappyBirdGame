#pragma once

#include "base.h"


using namespace std;

class Bird : public BaseTexture
{
public:
    Bird() : angle(0), velocityY(0) {}
    ~Bird() { Free(); }

    bool init(bool isDark = false);

    void render();

    void Free();

    void resetTime();

    void fall();

    void update(int pipeWidth = 0, int pipeHeight = 0);

private:
    int angle;
    double velocityY;



    position posBird;
};