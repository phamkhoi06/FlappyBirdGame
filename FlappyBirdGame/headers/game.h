#pragma once

#include "bird.h"
#include "base.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class game
{
public:
    struct input
    {
        enum type
        {
            QUIT,
            PLAY,
            NONE,
            PAUSE
        };
        type Type;
    };

    input userInput;
    Bird bird;

public:
    game();
    ~game();

    void takeInput();
    void update();
    void render();
    void display();

    bool isQuit();
    bool isDie();

    int getPipeWidth();
    int getPipeHeight();
    void Restart();

private:
};