#include <stdio.h>
#include <iostream>
#include <string>

#include "../headers/sound.h"


bool sound::init()
{
    std::string breath_path = "resources/sound/sfx_breath.wav";
    std::string hit_path = "resources/sound/sfx_bonk.wav";
    std::string sound_path = "resources/image/sound.png";
    bool success = true;
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    else
    {
        breath = Mix_LoadWAV(breath_path.c_str());
        if (breath == NULL)
            printf("Failed to load sound! SDL_mixer Error: %s\n", Mix_GetError());
        hit = Mix_LoadWAV(hit_path.c_str());
        if (hit == NULL)
            printf("Failed to load chord! SDL_mixer Error: %s\n", Mix_GetError());
        if (!Load(sound_path))
        {
        }
        else
        {
            Active.x = 0;
            Active.y = 0;
            Active.w = getWidth();
            Active.h = getHeight() / 2;
            Mute.x = 0;
            Mute.y = getHeight() / 2;
            Mute.w = getWidth();
            Mute.h = getHeight() / 2;
        }
    }
    isPlay = true;
    return success;
}
void sound::Free()
{
    free();
    Mix_FreeChunk(breath);
    breath = NULL;
    Mix_FreeChunk(hit);
    hit = NULL;
    Mix_FreeChunk(drop);
    drop = NULL;
    Mix_CloseAudio();
    Mix_Quit();
}
void sound::playBreath()
{
    if (isPlay)
        Mix_PlayChannel(-1, breath, 0);
}
void sound::playHit()
{
    if (isPlay)
        Mix_PlayChannel(-1, hit, 0);
}
void sound::renderSound()
{
    if (isNULL())
        return;
    if (isPlay)
        Render(POS_X, POS_Y, 0, &Active);
    else
        Render(POS_X, POS_Y, 0, &Mute);
}
bool sound::checkSound()
{
    if (isNULL())
        return false;
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (x > POS_X && x < POS_X + getWidth() && y > POS_Y && y < POS_Y + getHeight())
    {
        return true;
    }
    return false;
}
void sound::setPlay(bool playing) { isPlay = playing; }