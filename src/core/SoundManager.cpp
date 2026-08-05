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

void SoundManager::loadElevatorSounds(const std::string& doorPath,
                                      const std::string& movePath,
                                      const std::string& dingPath) {
    if (mElevatorDoorBuffer.loadFromFile(doorPath)) mElevatorDoorSound.setBuffer(mElevatorDoorBuffer);
    else printf("Note: Elevator door sound not found (%s)\n", doorPath.c_str());

    if (mElevatorMoveBuffer.loadFromFile(movePath)) mElevatorMoveSound.setBuffer(mElevatorMoveBuffer);
    else printf("Note: Elevator move sound not found (%s)\n", movePath.c_str());

    if (mElevatorDingBuffer.loadFromFile(dingPath)) mElevatorDingSound.setBuffer(mElevatorDingBuffer);
    else printf("Note: Elevator ding sound not found (%s)\n", dingPath.c_str());
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

void SoundManager::loadLevelDeathSounds() {
    for (int i = 1; i <= 5; ++i) {
        std::string path = "assets/sounds/lv" + std::to_string(i) + "_sd/death_" + std::to_string(i) + ".ogg";
        if (mLevelDeathBuffers[i - 1].loadFromFile(path)) {
            mLevelDeathSounds[i - 1].setBuffer(mLevelDeathBuffers[i - 1]);
        } else {
            printf("Note: Level %d death sound not found at (%s). Fallback to dead.ogg\n", i, path.c_str());
        }
    }
}

void SoundManager::playLevelDeathSound(int level) {
    if (level >= 1 && level <= 5 && mLevelDeathSounds[level - 1].getBuffer()) {
        mLevelDeathSounds[level - 1].play();
    } else {
        playDead();
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
    mElevatorDoorSound.stop();
    mElevatorMoveSound.stop();
    mElevatorDingSound.stop();
    for (int i = 0; i < 5; ++i) {
        mLevelDeathSounds[i].stop();
    }
}

void SoundManager::playVictory() { mVictorySound.play(); }
void SoundManager::playDead() { mDeadSound.play(); }
void SoundManager::playLevelClear() { mLevelClearSound.play(); }
void SoundManager::stopLevelClear() { mLevelClearSound.stop(); }

void SoundManager::playElevatorDoor() {
    if (mElevatorDoorSound.getBuffer()) mElevatorDoorSound.play();
}

void SoundManager::playElevatorMove() {
    if (mElevatorMoveSound.getBuffer() && mElevatorMoveSound.getStatus() != sf::Sound::Playing) {
        mElevatorMoveSound.setLoop(true);
        mElevatorMoveSound.play();
    }
}

void SoundManager::stopElevatorMove() {
    if (mElevatorMoveSound.getBuffer()) mElevatorMoveSound.stop();
}

void SoundManager::playElevatorDing() {
    if (mElevatorDingSound.getBuffer()) mElevatorDingSound.play();
}

void SoundManager::setMusicVolume(float v) {
    mLevelMusic.setVolume(v);
}

void SoundManager::setSFXVolume(float v) {
    mDeadSound.setVolume(v);
    mVictorySound.setVolume(v);
    mLevelClearSound.setVolume(v);
    mElevatorDoorSound.setVolume(v);
    mElevatorMoveSound.setVolume(v);
    mElevatorDingSound.setVolume(v);
    for (int i = 0; i < 5; ++i) {
        mLevelDeathSounds[i].setVolume(v);
    }
}
