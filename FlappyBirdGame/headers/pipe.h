#pragma once
#include <iostream>
#include <vector>
#include "base.h"
using namespace std;
extern vector<position> posPipe;
class pipe : public BaseTexture
{
private:
    const int randMin = -320 + 30;
    const int randMax = SCREEN_HEIGHT - LAND_HEIGHT - 320 - PIPE_SPACE - 30;

public:
    bool init();
    void Free();
    void render();
    void update();
    int width() { return getWidth(); }
    int height() { return getHeight(); }
};