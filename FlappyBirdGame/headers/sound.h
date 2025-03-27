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
    void renderSoundBGM();
    bool checkSoundBGM();
    void setPlay(bool playing);
    void setPlayBGM(bool playing);
    void playBackgroundMusic();
    void stopBackgroundMusic();

private:
    const int POS_X = 107;
    const int POS_Y = 267;
    const int POS_Y_BGM = 315;
    bool isPlay = true;
	bool isPlayBGM = true;
    Mix_Chunk* breath = NULL;
    Mix_Chunk* hit = NULL;
    Mix_Chunk* drop = NULL;
    Mix_Music* backgroundMusic;
    SDL_Rect Mute;
    SDL_Rect Active;
};