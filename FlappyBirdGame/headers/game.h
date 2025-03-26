#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "bird.h"
#include "pipe.h"
#include "base.h"
#include "land.h"
#include "sound.h"


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
    void lightTheme();
    void darkTheme();
    void nextButton();
    bool changeTheme();
    void renderGameOver();
    void replay();
    bool checkReplay();
    void Restart(bool isDark);

private:
    void loadBestScore();
    void saveBestScore();
    const double scaleNumberS;
    int bestScore;
};