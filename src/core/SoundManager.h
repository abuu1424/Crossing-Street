#pragma once
#include <SFML/Audio.hpp>
#include <string>

class SoundManager {
public:
    void loadEffects(const std::string& victoryPath,
                      const std::string& deadPath,
                      const std::string& levelClearPath);

    void loadElevatorSounds(const std::string& doorPath,
                            const std::string& movePath,
                            const std::string& dingPath);

    void playLevelMusic(const std::string& musicPath, float volume = 40.f);
    void stopMusic();
    void stopAllEffects();

    void playVictory();
    void playDead();
    void playLevelClear();
    void stopLevelClear();

    // Sound Thang Máy Cutscene
    void playElevatorDoor();
    void playElevatorMove();
    void stopElevatorMove();
    void playElevatorDing();

    void setMusicVolume(float v);
    void setSFXVolume(float v);

private:
    sf::SoundBuffer mVictoryBuffer, mDeadBuffer, mLevelClearBuffer;
    sf::Sound mVictorySound, mDeadSound, mLevelClearSound;

    sf::SoundBuffer mElevatorDoorBuffer, mElevatorMoveBuffer, mElevatorDingBuffer;
    sf::Sound mElevatorDoorSound, mElevatorMoveSound, mElevatorDingSound;

    sf::Music mLevelMusic;
    std::string mCurrentMusicPath;
};
