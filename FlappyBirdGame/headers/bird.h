#pragma once

#include <string>

#include "base.h"
#include "pipe.h"


class Bird : public BaseTexture {
public:
    Bird() : angle(0), time(0), x0(0), ahead(0), saved_path("") {}

    bool init(bool isDark);
    void render();
    void Free();
    void resetTime();
    void fall();
    void update(int pileWidth, int pileHeight);
    void flap();

private:
    int angle, time, x0;
    int ahead;
    std::string saved_path;
    position posBird;
};