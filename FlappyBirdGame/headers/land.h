#pragma once
#include "base.h"
#include <vector>
using namespace std;
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