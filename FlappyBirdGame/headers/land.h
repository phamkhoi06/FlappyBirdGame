#pragma once
#include "base.h"
using namespace std;
class land : public BaseTexture
{
public:
    land() {}
    ~land() { Free(); }
    bool init();
    void Free();
    void render();
    void update();

private:
    position posLand;
};