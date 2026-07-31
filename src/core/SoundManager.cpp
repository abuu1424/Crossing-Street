#include "SoundManager.h"
#include <cstdio>

void SoundManager::loadEffects(const std::string& victoryPath,
                                const std::string& deadPath,
                                const std::string& levelClearPath) {
    if (mVictoryBuffer.loadFromFile(victoryPath)) mVictorySound.setBuffer(mVictoryBuffer);
    else printf("Failed to load victory sound\n");

    if (mDeadBuffer.loadFromFile(deadPath)) mDeadSound.setBuffer(mDeadBuffer);
    else printf("Failed to load dead sound\n");

    if (mLevelClearBuffer.loadFromFile(levelClearPath)) mLevelClearSound.setBuffer(mLevelClearBuffer);
    else printf("Failed to load level clear sound\n");
}

void SoundManager::playLevelMusic(const std::string& musicPath, float volume) {
    if (mCurrentMusicPath == musicPath && mLevelMusic.getStatus() == sf::Music::Playing) {
        mLevelMusic.setVolume(volume);
        return;
    }

    mLevelMusic.stop();
    if (mLevelMusic.openFromFile(musicPath)) {
        mCurrentMusicPath = musicPath;
        mLevelMusic.setLoop(true);
        mLevelMusic.setVolume(volume);
        mLevelMusic.play();
    }
}

void SoundManager::stopMusic() {
    mLevelMusic.stop();
    mCurrentMusicPath.clear();
}

void SoundManager::stopAllEffects() {
    mDeadSound.stop();
    mVictorySound.stop();
    mLevelClearSound.stop();
}

void SoundManager::playVictory() { mVictorySound.play(); }
void SoundManager::playDead() { mDeadSound.play(); }
void SoundManager::playLevelClear() { mLevelClearSound.play(); }
void SoundManager::stopLevelClear() { mLevelClearSound.stop(); }

void SoundManager::setMusicVolume(float v) {
    mLevelMusic.setVolume(v);
}

void SoundManager::setSFXVolume(float v) {
    mDeadSound.setVolume(v);
    mVictorySound.setVolume(v);
    mLevelClearSound.setVolume(v);
}
