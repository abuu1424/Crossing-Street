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
    void stopVictory();
    void playDead();
    void playLevelClear();
    void stopLevelClear();

    // Sound Thang Máy Cutscene
    void playElevatorDoor();
    void playElevatorMove();
    void stopElevatorMove();
    void playElevatorDing();

    // Sound Nhặt Coin
    void loadCoinSound(const std::string& path = "assets/sounds/coin/coin_collection.wav");
    void playCoinSound();

    // Sound Death Cutscenes Cho 5 Level
    void loadLevelDeathSounds();
    void playLevelDeathSound(int level);
    void stopLevelDeathSounds();

    // Sound Hazard Cho 5 Level
    void loadHazardSounds();
    void playHazardSound(int level);
    void stopHazardSounds();
    void playDinoRoar();
    void playStampede();
    void playSandstorm();
    void playArrowVolley();
    void playRushHour();
    void playLaserBeam();
    void stopLaserBeam();
    void playBlackHole();
    void stopBlackHole();
    void playLightning();

    void setMusicVolume(float v);
    void setSFXVolume(float v);

    // Music Ducking & Transition Management
    void update(float dt);
    void setMusicDuckingFactor(float factor);
    void resetDucking();
    float getMusicDuckingFactor() const { return mTargetDuckingFactor; }

private:
    sf::SoundBuffer mVictoryBuffer, mDeadBuffer, mLevelClearBuffer;
    sf::Sound mVictorySound, mDeadSound, mLevelClearSound;

    sf::SoundBuffer mElevatorDoorBuffer, mElevatorMoveBuffer, mElevatorDingBuffer;
    sf::Sound mElevatorDoorSound, mElevatorMoveSound, mElevatorDingSound;

    sf::SoundBuffer mCoinBuffer;
    sf::Sound mCoinSound;

    sf::SoundBuffer mLevelDeathBuffers[5];
    sf::Sound mLevelDeathSounds[5];

    sf::SoundBuffer mHazardBuffers[5];
    sf::Sound mHazardSounds[5];

    sf::SoundBuffer mDinoRoarBuffer;
    sf::Sound mDinoRoarSound;

    sf::SoundBuffer mLightningBuffer;
    sf::Sound mLightningSound;

    sf::SoundBuffer mBlackHoleBuffer;
    sf::Sound mBlackHoleSound;

    sf::Music mLevelMusic;
    std::string mCurrentMusicPath;
    float mBaseMusicVolume = 40.f;
    float mCurrentDuckingFactor = 1.0f;
    float mTargetDuckingFactor = 1.0f;
    float mCoinSoundCooldown = 0.f;
};
