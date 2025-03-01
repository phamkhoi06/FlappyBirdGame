#pragma once
#include "base.h" // Include from same directory
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