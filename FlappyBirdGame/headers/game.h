#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "Bird.h"
#include "Pipe.h"
#include "Base.h"
#include "Land.h"
#include "Sound.h"


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
    sound sound;
    land land;

public:
    game();
    ~game();
    bool initGraphic();
    void releaseGraphic();
    bool isQuit() { return BaseTexture::quit; }
    bool isDie() { return BaseTexture::die; }
    int getPipeWidth() { return pipe.width(); }
    int getPipeHeight() { return pipe.height(); }
    void takeInput();
    void display();
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
    void birdImage();
    void nextButton();
    bool changeTheme();
    void renderGameOver();
    void replay();
    bool checkReplay();
    void Restart();

private:
    void loadBestScore();
    void saveBestScore();
    const double scaleNumberS;
    int bestScore;
};