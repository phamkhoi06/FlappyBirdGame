#pragma once

#include <vector>

#include "Base.h"


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