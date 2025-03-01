#pragma once
#include "bird.h"
#include "pipe.h"
#include "land.h"
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
    pipe pipe;
    land land;

public:
    game();
    ~game();
    bool initGraphic();
    void releaseGraphic();
    bool isQuit();
    bool isDie();
    int getPipeWidth();
    int getPipeHeight();
    void takeInput();
    void display();

public:
    void renderScoreSmall();
    void renderScoreLarge();
    void renderBestScore();
    void renderMessage();
    void renderBackground();
    void renderBackgroundNight();
    void renderLand();
    void resume();
    void pause();
    void renderPauseTab();
    void lightTheme();
    void darkTheme();
    void nextButton();
    bool changeTheme();
    void renderGameOver();
    void renderMedal();
    void replay();
    bool checkReplay();
    void Restart();

private:
};
