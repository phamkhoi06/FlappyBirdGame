#pragma once
#include "base.h"
#include "pipe.h"
using namespace std;
class Bird : public BaseTexture
{
public:
    bool init(bool isDark);
    void render();
    void Free();
    void resetTime();
    void fall();
    void update(int pileWidth, int pileHeight);
    void flap();

private:
    int angle, time, x0;
    int ahead = 0;
    string saved_path = "";
    position posBird;
};
