#pragma once
#include <SFML/Audio.hpp>
#include <string>

class SoundManager {
public:
    void loadEffects(const std::string& victoryPath,
                      const std::string& deadPath,
                      const std::string& levelClearPath);

    void playLevelMusic(const std::string& musicPath, float volume = 40.f);
    void stopMusic();
    void stopAllEffects();

    void playVictory();
    void playDead();
    void playLevelClear();
    void stopLevelClear();

    void setMusicVolume(float v);
    void setSFXVolume(float v);

private:
    sf::SoundBuffer mVictoryBuffer, mDeadBuffer, mLevelClearBuffer;
    sf::Sound mVictorySound, mDeadSound, mLevelClearSound;
    sf::Music mLevelMusic;
};
