/* file: sources/main.cpp */
#define SDL_MAIN_HANDLED
#include "../headers/game.h"
#include "../headers/base.h"
#include <time.h>
#include <stdlib.h> // For abs
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream> // Kept

const int FPS = 60; const int frameDelay = 1000 / FPS;
// No 'using namespace std;'

int main(int argc, char* argv[]) {
    Uint32 frameStart; int frameTime; game game_instance;
    bool isMenu = false; bool isPause = false; bool isSound = true; bool isDark = false;
    // No quit check needed if constructor handles failure via flag

    while (!game_instance.isQuit()) {
        frameStart = SDL_GetTicks();
        if (game_instance.isDie()) {
            // Sound play logic
            if (isMenu) { if (isSound) game_instance.sound.playHit(); }
            // Reset input before polling *within* the loop for this state
            game_instance.userInput.Type = game::input::NONE;
            // Inner loop for Get Ready / Game Over state
            while (game_instance.isDie() && !game_instance.isQuit()) {
                Uint32 innerFrameStart = SDL_GetTicks();
                game_instance.takeInput(); // Poll events

                // Input Handling for Menu/Ready states
                if (isMenu == true && game_instance.userInput.Type == game::input::PLAY) { // Game Over input
                    if (game_instance.checkReplay()) {
                        isMenu = false; // Will trigger restart logic after loop break
                        break;
                    }
                    // Consume click otherwise
                    game_instance.userInput.Type = game::input::NONE;
                }
                else if (isMenu == false && game_instance.userInput.Type == game::input::PLAY) { // Get Ready input
                    // Restart resets die flag to false, score to 0, inits bird/pipes
                    game_instance.Restart(isDark); // Pass current theme
                    isMenu = true; // Set flag to indicate we're past initial screen
                    break; // Exit inner loop to start gameplay in outer loop
                }

                // --- Rendering based on state ---
                if (!isDark) game_instance.renderBackground(); else game_instance.renderBackgroundNight();
                game_instance.pipe.render(); // Render static pipes
                game_instance.land.render(); // Render static land (or scrolling if updated below)

                if (isMenu) { // Render Game Over Screen
                    game_instance.bird.render();
                    game_instance.bird.fall(); // Animate dead bird
                    game_instance.renderGameOver();
                    game_instance.renderMedal();
                    game_instance.renderScoreSmall();
                    game_instance.renderBestScore(); // This reads/writes the file
                    game_instance.replay();
                }
                else { // Render Get Ready Screen
                    game_instance.bird.init(isDark); // Ensure bird is reset visually
                    game_instance.bird.render();
                    game_instance.renderMessage();
                    game_instance.land.update(); // Keep land scrolling in Get Ready state
                }
                game_instance.display(); // Show frame

                // Inner frame limiting
                int innerFrameTime = SDL_GetTicks() - innerFrameStart;
                if (frameDelay > innerFrameTime) SDL_Delay(frameDelay - innerFrameTime);
            } // End inner while loop (isDie state)

            // If loop broken by Replay button (isMenu will be false)
            if (!isMenu) {
                game_instance.Restart(isDark); // Restart game state
            }
            // Pipe reset is now handled inside Restart
            // game_instance.pipe.init(); // Remove redundant call

        }
        else { // Gameplay State (isDie is false)
            isMenu = true; // Track that we are past the initial "Get Ready" state
            game_instance.takeInput(); // Poll input

            // Handle Pause Toggle
            if (game_instance.userInput.Type == game::input::PAUSE) {
                isPause = abs(1 - isPause); // Use abs() like original
                game_instance.userInput.Type = game::input::NONE; // Consume input
            }

            // Handle Flap Input (sound + reset time)
            if (isPause == false && game_instance.userInput.Type == game::input::PLAY) {
                if (isSound) game_instance.sound.playBreath();
                game_instance.bird.resetTime(); // Reset physics timer per original logic
                game_instance.userInput.Type = game::input::NONE; // Consume input
            }

            // --- Rendering ---
            if (!isDark) game_instance.renderBackground(); else game_instance.renderBackgroundNight();
            game_instance.pipe.render();
            game_instance.land.render();
            game_instance.bird.render();
            game_instance.renderScoreLarge();

            // --- Update / Pause Menu ---
            if (!isPause) {
                // Update game objects
                game_instance.bird.update(game_instance.getPipeWidth(), game_instance.getPipeHeight());
                game_instance.pipe.update();
                game_instance.land.update();
                // Render pause button
                game_instance.pause();
            }
            else {
                // Render Pause Menu UI
                game_instance.resume();
                game_instance.renderPauseTab();
                game_instance.renderScoreSmall();
                game_instance.renderBestScore(); // Reads/writes file
                game_instance.replay();
                game_instance.sound.renderSound();
                if (!isDark) game_instance.lightTheme(); else game_instance.darkTheme();
                game_instance.nextButton();

                // Handle Pause Menu Interactions
                if (game_instance.userInput.Type == game::input::PLAY) {
                    if (game_instance.checkReplay()) {
                        isPause = false;
                        game_instance.Restart(isDark); // Restart game
                        // Pipe init now handled by Restart
                        // game_instance.pipe.init();
                    }
                    else if (game_instance.sound.checkSound()) {
                        isSound = abs(1 - isSound); // Use abs()
                        game_instance.sound.setPlay(isSound);
                    }
                    else if (game_instance.changeTheme()) {
                        isDark = abs(1 - isDark); // Use abs()
                        game_instance.bird.init(isDark); // Reload bird texture
                    }
                    // No 'else resume' in original
                    game_instance.userInput.Type = game::input::NONE; // Consume input
                }
            } // End if (!isPause)
            game_instance.display(); // Display frame
        } // End if (isDie) / else

        // --- Frame Limiting ---
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    } // End main while loop
    return 0;
}