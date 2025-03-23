#define SDL_MAIN_HANDLED

#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <Windows.h>

#include "../headers/Game.h"
#include "../headers/Base.h"


const int FPS = 60;
const int frameDelay = 1000 / FPS;

extern "C"

int main(int argc, char* argv[])
{
    Uint32 frameStart;
    int frameTime;
    game game_instance;
    bool isMenu = false;
    bool isPause = false;
    bool isSound = true;

    while (!game_instance.isQuit())
    {
        frameStart = SDL_GetTicks();
        if (game_instance.isDie())
        {
            if (isMenu)
            {
                if (isSound)
                    game_instance.sound.playHit();
            }
            game_instance.userInput.Type = game::input::NONE;
            while (game_instance.isDie() && !game_instance.isQuit())
            {
                Uint32 innerFrameStart = SDL_GetTicks();
                game_instance.takeInput();

                if (isMenu == true && game_instance.userInput.Type == game::input::PLAY)
                {
                    if (game_instance.checkReplay())
                    {
                        isMenu = false;
                        break;
                    }
                    game_instance.userInput.Type = game::input::NONE;
                }
                else if (isMenu == false && game_instance.userInput.Type == game::input::PLAY)
                {
                    game_instance.Restart();
                    isMenu = true;
                    break;
                }

                game_instance.renderBackground();
                game_instance.pipe.render();
                game_instance.land.render();

                if (isMenu)
                {
                    game_instance.bird.render();
                    game_instance.bird.fall();
                    game_instance.renderGameOver();
                    game_instance.renderScoreSmall();
                    game_instance.renderBestScore();
                    game_instance.replay();
                }
                else
                {
                    game_instance.bird.init();
                    game_instance.bird.render();
                    game_instance.renderMessage();
                    game_instance.land.update();
                }
                game_instance.display();

                int innerFrameTime = SDL_GetTicks() - innerFrameStart;
                if (frameDelay > innerFrameTime)
                    SDL_Delay(frameDelay - innerFrameTime);
            }

            if (!isMenu)
            {
                game_instance.Restart();
            }
        }
        else
        {
            isMenu = true;
            game_instance.takeInput();

            if (game_instance.userInput.Type == game::input::PAUSE)
            {
                isPause = abs(1 - isPause);
                game_instance.userInput.Type = game::input::NONE;
            }

            if (isPause && game_instance.userInput.Type == game::input::PLAY)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x > BaseTexture::SCREEN_WIDTH - 50 && x < BaseTexture::SCREEN_WIDTH - 20 &&
                    y > 20 && y < 50)
                {
                    isPause = false;
                    game_instance.userInput.Type = game::input::NONE;
                }
            }

            if (isPause == false && game_instance.userInput.Type == game::input::PLAY)
            {
                if (isSound)
                    game_instance.sound.playBreath();
                game_instance.bird.resetTime();
                game_instance.bird.flap();
                game_instance.userInput.Type = game::input::NONE;
            }

            game_instance.renderBackground();
            game_instance.pipe.render();
            game_instance.land.render();
            game_instance.bird.render();
            game_instance.renderScoreLarge();

            if (!isPause)
            {
                game_instance.bird.update(game_instance.getPipeWidth(), game_instance.getPipeHeight());
                game_instance.pipe.update();
                game_instance.land.update();
                game_instance.pause();
            }
            else
            {
                game_instance.resume();
                game_instance.renderPauseTab();
                game_instance.renderScoreSmall();
                game_instance.renderBestScore();
                game_instance.replay();
                game_instance.sound.renderSound();

                if (game_instance.userInput.Type == game::input::PLAY)
                {
                    if (game_instance.checkReplay())
                    {
                        isPause = false;
                        game_instance.Restart();
                    }
                    else if (game_instance.sound.checkSound())
                    {
                        isSound = abs(1 - isSound);
                        game_instance.sound.setPlay(isSound);
                    }
                    game_instance.userInput.Type = game::input::NONE;
                }
            }
            game_instance.display();
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}