#pragma once
#include "base.h"
using namespace std;
class Bird : public BaseTexture
{
public:
    Bird() : angle(0), velocityY(0), ahead(0) {}
    ~Bird() { Free(); }
    bool init(bool isDark = false);
    void render();
    void Free();
    void flap();
    void applyGravity();
    void update(int pipeWidth, int pipeHeight);
    position posBird;

private:
    int angle;
    double velocityY;
    int ahead;
};