#pragma once
#include <iostream>
#include <vector>
#include "base.h"
#include <stdlib.h>
#include <time.h>
using namespace std;
extern vector<position> posPipe;
class pipe : public BaseTexture
{
public:
    pipe() {}
    ~pipe() { Free(); }
    bool init();
    void Free();
    void render();
    void update();
    int width() { return getWidth(); }
    int height() { return getHeight(); }
};