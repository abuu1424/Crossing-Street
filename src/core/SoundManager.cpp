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
    mDinoRoarSound.stop();
    for (int i = 0; i < 5; ++i) {
        mLevelDeathSounds[i].stop();
        mHazardSounds[i].stop();
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

void SoundManager::loadHazardSounds() {
    const std::string hazardPaths[5] = {
        "assets/sounds/hazards/stampede.wav",
        "assets/sounds/hazards/sandstorm.wav",
        "assets/sounds/hazards/arrow_volley.wav",
        "assets/sounds/hazards/rush_hour.wav",
        "assets/sounds/hazards/laser_beam.wav"
    };

    for (int i = 0; i < 5; ++i) {
        if (mHazardBuffers[i].loadFromFile(hazardPaths[i])) {
            mHazardSounds[i].setBuffer(mHazardBuffers[i]);
            mHazardSounds[i].setVolume(70.f);
        }
    }

    if (mDinoRoarBuffer.loadFromFile("assets/sounds/hazards/dino_roar.wav")) {
        mDinoRoarSound.setBuffer(mDinoRoarBuffer);
        mDinoRoarSound.setVolume(85.f);
    }
}

void SoundManager::playHazardSound(int level) {
    switch (level) {
    case 1: playStampede(); break;
    case 2: playSandstorm(); break;
    case 3: playArrowVolley(); break;
    case 4: playRushHour(); break;
    case 5: playLaserBeam(); break;
    default: break;
    }
}

void SoundManager::stopHazardSounds() {
    mDinoRoarSound.stop();
    for (int i = 0; i < 5; ++i) {
        mHazardSounds[i].stop();
    }
}

void SoundManager::playDinoRoar() {
    if (mDinoRoarSound.getBuffer()) {
        mDinoRoarSound.play();
    }
}

void SoundManager::playStampede() {
    if (mHazardSounds[0].getBuffer()) {
        mHazardSounds[0].play();
    }
}

void SoundManager::playSandstorm() {
    if (mHazardSounds[1].getBuffer()) {
        mHazardSounds[1].play();
    }
}

void SoundManager::playArrowVolley() {
    if (mHazardSounds[2].getBuffer()) {
        mHazardSounds[2].play();
    }
}

void SoundManager::playRushHour() {
    if (mHazardSounds[3].getBuffer()) {
        mHazardSounds[3].play();
    }
}

void SoundManager::playLaserBeam() {
    if (mHazardSounds[4].getBuffer()) {
        mHazardSounds[4].play();
    }
}

void SoundManager::setSFXVolume(float v) {
    mDeadSound.setVolume(v);
    mVictorySound.setVolume(v);
    mLevelClearSound.setVolume(v);
    mElevatorDoorSound.setVolume(v);
    mElevatorMoveSound.setVolume(v);
    mElevatorDingSound.setVolume(v);
    mDinoRoarSound.setVolume(v);
    for (int i = 0; i < 5; ++i) {
        mLevelDeathSounds[i].setVolume(v);
        mHazardSounds[i].setVolume(v);
    }
}
