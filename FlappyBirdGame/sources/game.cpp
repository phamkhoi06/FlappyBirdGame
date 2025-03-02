/* file: sources/game.cpp */
#include "../headers/game.h"
#include "../headers/base.h" // Included for static member access
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h> // For abs
#include <time.h>
// No 'using namespace std;'

// --- Constructor / Destructor ---
game::game() : scaleNumberS(0.75), bestScore(0) {
    // No srand in original? ok.
    initGraphic(); // Call member function
    pipe.init();
    land.init();
    sound.init();
    loadBestScore(); // Call private member function
}

game::~game() {
    // Call private member function first
    saveBestScore();
    // Free resources
    bird.Free();
    pipe.Free();
    land.Free();
    sound.Free();
    // Release graphics context
    releaseGraphic(); // Call member function
}

// --- Member Function Definitions ---

// Use BaseTexture::event, BaseTexture::quit
void game::takeInput() {
    // Reset input type each frame? Original main loop suggests this happens before polling.
    // userInput.Type = game::input::NONE;
    while (SDL_PollEvent(&BaseTexture::event) != 0) {
        if (BaseTexture::event.type == SDL_QUIT) {
            userInput.Type = game::input::QUIT; // Use nested enum type
            BaseTexture::quit = true;
        }
        else if (BaseTexture::event.type == SDL_MOUSEBUTTONDOWN || (BaseTexture::event.type == SDL_KEYDOWN &&
            (BaseTexture::event.key.keysym.sym == SDLK_SPACE || BaseTexture::event.key.keysym.sym == SDLK_UP) &&
            BaseTexture::event.key.repeat == 0)) {
            userInput.Type = game::input::PLAY; // Use nested enum type
        }
        else if (BaseTexture::event.type == SDL_KEYDOWN && BaseTexture::event.key.keysym.sym == SDLK_ESCAPE && BaseTexture::event.key.repeat == 0) {
            userInput.Type = game::input::PAUSE; // Use nested enum type
        }
    }
}

// Use BaseTexture::gWindow, BaseTexture::gRenderer, BaseTexture::SCREEN_WIDTH, BaseTexture::SCREEN_HEIGHT
bool game::initGraphic() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        BaseTexture::gWindow = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BaseTexture::SCREEN_WIDTH, BaseTexture::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (BaseTexture::gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            BaseTexture::gRenderer = SDL_CreateRenderer(BaseTexture::gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (BaseTexture::gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                SDL_SetRenderDrawColor(BaseTexture::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
                // Mixer is initialized in sound.init()
            }
        }
    }
    return success;
}

// Use BaseTexture::gWindow, BaseTexture::gRenderer
void game::releaseGraphic() {
    if (BaseTexture::gRenderer) { SDL_DestroyRenderer(BaseTexture::gRenderer); BaseTexture::gRenderer = NULL; }
    if (BaseTexture::gWindow) { SDL_DestroyWindow(BaseTexture::gWindow); BaseTexture::gWindow = NULL; }
    // Sound::Free calls Mix_Quit
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// Use BaseTexture::gRenderer
void game::display() {
    SDL_RenderPresent(BaseTexture::gRenderer);
    SDL_RenderClear(BaseTexture::gRenderer);
}

// Use BaseTexture::score, BaseTexture::SCREEN_WIDTH, std::to_string, std::string
void game::renderScoreSmall() {
    std::string s = std::to_string(BaseTexture::score);
    signed char len = s.length();
    BaseTexture image;
    for (signed char i = len - 1; i >= 0; i--) {
        signed char number = s[i] - '0';
        std::string path = "resources/number/small/";
        path += std::to_string(number) + ".png";
        image.Load(path, scaleNumberS); // Use member scaleNumberS
        image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 268);
        image.free();
    }
}

// Use BaseTexture::score, BaseTexture::SCREEN_WIDTH, std::to_string, std::string
void game::renderScoreLarge() {
    std::string s = std::to_string(BaseTexture::score);
    signed char len = s.length();
    BaseTexture image;
    for (signed char i = 0; i < len; i++) {
        signed char number = s[i] - '0';
        std::string path = "resources/number/large/";
        path += std::to_string(number) + ".png";
        image.Load(path, 1);
        // Using corrected centering logic based on original attempt
        image.Render((BaseTexture::SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i * (image.getWidth() + 10)), 100);
        image.free();
    }
}

// Use member bestScore, BaseTexture::score, std::ifstream, std::ofstream, std::ios::trunc, std::to_string, std::string
void game::renderBestScore() {
    // Load best score first (using private helper)
    loadBestScore();
    // Open output file (truncates)
    std::ofstream fileOut("resources/data/bestScore.txt", std::ios::trunc);
    // Update best score if current score is higher
    if (BaseTexture::score > bestScore) {
        bestScore = BaseTexture::score;
    }
    // Render the best score
    std::string s = std::to_string(bestScore);
    signed char len = s.length();
    BaseTexture image;
    for (signed char i = len - 1; i >= 0; i--) {
        signed char number = s[i] - '0';
        std::string path = "resources/number/small/";
        path += std::to_string(number) + ".png";
        image.Load(path, scaleNumberS);
        image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 315);
        image.free();
    }
    // Write the potentially updated best score back to the file
    if (fileOut.is_open()) {
        fileOut << bestScore;
        fileOut.close(); // Close output file stream
    }
    // fileIn is already closed by loadBestScore or implicitly by going out of scope if not called separately
}

// Use member bestScore, std::ifstream
void game::loadBestScore() {
    std::ifstream fileIn("resources/data/bestScore.txt");
    if (fileIn.is_open()) {
        fileIn >> bestScore;
        fileIn.close();
    }
    else {
        bestScore = 0; // Default if file doesn't exist
    }
}

// Use member bestScore, std::ofstream, std::ios::trunc
void game::saveBestScore() {
    // Note: Best score is updated in renderBestScore just before saving in the original logic.
    // This separate function just saves the current `bestScore` member value.
    std::ofstream fileOut("resources/data/bestScore.txt", std::ios::trunc);
    if (fileOut.is_open()) {
        fileOut << bestScore;
        fileOut.close();
    }
}

// Use BaseTexture::SCREEN_WIDTH
void game::renderMessage() { BaseTexture image; image.Load("resources/image/message.png", 1); image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 180); image.free(); }
void game::renderBackground() { BaseTexture image; image.Load("resources/image/background.png", 1); image.Render(0, 0); image.free(); }
void game::renderBackgroundNight() { BaseTexture image; image.Load("resources/image/background-night.png", 1); image.Render(0, 0); image.free(); }
void game::renderLand() { /* This function was empty in the original game.cpp */ }
void game::resume() { BaseTexture image; image.Load("resources/image/resume.png", 1); image.Render(BaseTexture::SCREEN_WIDTH - 50, 20); image.free(); }
void game::pause() { BaseTexture image; image.Load("resources/image/pause.png", 1); image.Render(BaseTexture::SCREEN_WIDTH - 50, 20); image.free(); }
void game::renderPauseTab() { BaseTexture image; image.Load("resources/image/pauseTab.png", 1); image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 230); image.free(); }
void game::lightTheme() { BaseTexture image; image.Load("resources/image/bird.png", 0.8); image.Render(105, 315); image.free(); }
void game::darkTheme() { BaseTexture image; image.Load("resources/image/bird-dark.png", 0.8); image.Render(105, 315); image.free(); }
void game::nextButton() { BaseTexture image; image.Load("resources/image/nextRight.png", 1); image.Render(149, 322); image.Load("resources/image/nextLeft.png", 1); image.Render(88, 322); image.free(); }
bool game::changeTheme() { int x, y; SDL_GetMouseState(&x, &y); if (((x > 149 && x < 149 + 13) || (x > 88 && x < 88 + 13)) && (y > 322 && y < 322 + 16)) return true; return false; }
void game::renderGameOver() { BaseTexture image; image.Load("resources/image/gameOver.png", 1); image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 150); image.free(); }
void game::renderMedal() { BaseTexture image; std::string medalPath = "resources/medal/"; if (BaseTexture::score > 50) medalPath += "gold.png"; else if (BaseTexture::score > 20) medalPath += "silver.png"; else medalPath += "honor.png"; image.Load(medalPath, scaleNumberS); image.Render(82, 275); image.free(); } // Use BaseTexture::score, std::string
void game::replay() { BaseTexture image; image.Load("resources/image/replay.png", 1); image.Render((BaseTexture::SCREEN_WIDTH - image.getWidth()) / 2, 380); image.free(); }
bool game::checkReplay() { int x, y; SDL_GetMouseState(&x, &y); BaseTexture image; image.Load("resources/image/replay.png", 1); int w = image.getWidth(); int h = image.getHeight(); image.free(); int btnX = (BaseTexture::SCREEN_WIDTH - w) / 2; int btnY = 380; if (x > btnX && x < btnX + w && y > btnY && y < btnY + h) return true; return false; }

// Modified Restart
void game::Restart(bool isDark) { // Add isDark parameter
    BaseTexture::die = false;
    BaseTexture::score = 0;
    // bird.resetTime(); // Not needed, init calls it
    bird.init(isDark); // Reset bird's position, physics, angle etc.
    pipe.init();      // Reset pipes
    // userInput.Type = input::NONE; // Optionally reset input state here too
}