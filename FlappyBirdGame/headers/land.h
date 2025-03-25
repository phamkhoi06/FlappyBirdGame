#pragma once

#include <vector>

#include "base.h"


class land : public BaseTexture
{
public:
    bool init();
    void Free();
    void render();
    void update();

private:
    position posLand;
};