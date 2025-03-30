#pragma 

#include <SDL2/SDL_mixer.h>
#include <string>

#include "Base.h"


class sound : public BaseTexture
{
public:
    bool init();
    void Free();
    void playBreath();
    void playHit();
    void renderSound();
    bool checkSound();
    void setPlay(bool playing);

private:
    const int POS_X = 107;
    const int POS_Y = 267;
    bool isPlay = true;
    Mix_Chunk* breath = NULL;
    Mix_Chunk* hit = NULL;
    Mix_Chunk* drop = NULL;
    SDL_Rect Mute;
    SDL_Rect Active;
};