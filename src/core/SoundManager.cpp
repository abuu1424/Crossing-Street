#include "SoundManager.h"
#include <cstdio>

void SoundManager::loadEffects(const std::string &victoryPath,
                               const std::string &deadPath,
                               const std::string &levelClearPath) {
  if (mVictoryBuffer.loadFromFile(victoryPath))
    mVictorySound.setBuffer(mVictoryBuffer);
  else
    printf("Failed to load victory sound\n");

  if (mDeadBuffer.loadFromFile(deadPath))
    mDeadSound.setBuffer(mDeadBuffer);
  else
    printf("Failed to load dead sound\n");

  if (mLevelClearBuffer.loadFromFile(levelClearPath))
    mLevelClearSound.setBuffer(mLevelClearBuffer);
  else
    printf("Failed to load level clear sound\n");
}

void SoundManager::loadElevatorSounds(const std::string &doorPath,
                                      const std::string &movePath,
                                      const std::string &dingPath) {
  if (mElevatorDoorBuffer.loadFromFile(doorPath))
    mElevatorDoorSound.setBuffer(mElevatorDoorBuffer);
  else
    printf("Note: Elevator door sound not found (%s)\n", doorPath.c_str());

  if (mElevatorMoveBuffer.loadFromFile(movePath))
    mElevatorMoveSound.setBuffer(mElevatorMoveBuffer);
  else
    printf("Note: Elevator move sound not found (%s)\n", movePath.c_str());

  if (mElevatorDingBuffer.loadFromFile(dingPath))
    mElevatorDingSound.setBuffer(mElevatorDingBuffer);
  else
    printf("Note: Elevator ding sound not found (%s)\n", dingPath.c_str());
}

void SoundManager::playLevelMusic(const std::string &musicPath, float volume) {
  mBaseMusicVolume = volume;
  if (mCurrentMusicPath == musicPath &&
      mLevelMusic.getStatus() == sf::Music::Playing) {
    mLevelMusic.setVolume(mBaseMusicVolume * mCurrentDuckingFactor);
    return;
  }

  mLevelMusic.stop();
  if (mLevelMusic.openFromFile(musicPath)) {
    mCurrentMusicPath = musicPath;
    mLevelMusic.setLoop(true);
    mCurrentDuckingFactor = 1.0f;
    mTargetDuckingFactor = 1.0f;
    mLevelMusic.setVolume(mBaseMusicVolume * mCurrentDuckingFactor);
    mLevelMusic.play();
  }
}

void SoundManager::loadLevelDeathSounds() {
  for (int i = 1; i <= 5; ++i) {
    std::string path = "assets/sounds/lv" + std::to_string(i) + "_sd/death_" +
                       std::to_string(i) + ".ogg";
    if (mLevelDeathBuffers[i - 1].loadFromFile(path)) {
      mLevelDeathSounds[i - 1].setBuffer(mLevelDeathBuffers[i - 1]);
    } else {
      printf("Note: Level %d death sound not found at (%s). Fallback to "
             "dead.ogg\n",
             i, path.c_str());
    }
  }
}

void SoundManager::playLevelDeathSound(int level) {
  stopLevelDeathSounds();
  if (level >= 1 && level <= 5 && mLevelDeathSounds[level - 1].getBuffer()) {
    mLevelDeathSounds[level - 1].play();
  } else {
    playDead();
  }
}

void SoundManager::stopLevelDeathSounds() {
  for (int i = 0; i < 5; ++i) {
    mLevelDeathSounds[i].stop();
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
  stopHazardSounds();
  stopLevelDeathSounds();
}

void SoundManager::playVictory() { 
  stopLevelDeathSounds();
  mVictorySound.play(); 
}

void SoundManager::playDead() { 
  stopLevelDeathSounds();
  mDeadSound.play(); 
}
void SoundManager::playLevelClear() { mLevelClearSound.play(); }
void SoundManager::stopLevelClear() { mLevelClearSound.stop(); }

void SoundManager::playElevatorDoor() {
  if (mElevatorDoorSound.getBuffer())
    mElevatorDoorSound.play();
}

void SoundManager::playElevatorMove() {
  if (mElevatorMoveSound.getBuffer() &&
      mElevatorMoveSound.getStatus() != sf::Sound::Playing) {
    mElevatorMoveSound.setLoop(true);
    mElevatorMoveSound.play();
  }
}

void SoundManager::stopElevatorMove() {
  if (mElevatorMoveSound.getBuffer())
    mElevatorMoveSound.stop();
}

void SoundManager::playElevatorDing() {
  if (mElevatorDingSound.getBuffer())
    mElevatorDingSound.play();
}

void SoundManager::loadCoinSound(const std::string &path) {
  if (mCoinBuffer.loadFromFile(path)) {
    mCoinSound.setBuffer(mCoinBuffer);
  } else if (mCoinBuffer.loadFromFile("assets/sounds/coin/coin_collection.ogg")) {
    mCoinSound.setBuffer(mCoinBuffer);
  } else {
    printf("Note: Coin sound not found (%s)\n", path.c_str());
  }
}

void SoundManager::playCoinSound() {
  if (mCoinSoundCooldown <= 0.f && mCoinSound.getBuffer()) {
    mCoinSound.play();
    mCoinSoundCooldown = 0.055f; // 55ms throttle prevents lag during rapid coin suction
  }
}

void SoundManager::setMusicVolume(float v) {
  mBaseMusicVolume = v;
  mLevelMusic.setVolume(mBaseMusicVolume * mCurrentDuckingFactor);
}

void SoundManager::setMusicDuckingFactor(float factor) {
  if (factor < 0.f)
    factor = 0.f;
  if (factor > 1.f)
    factor = 1.f;
  mTargetDuckingFactor = factor;
}

void SoundManager::update(float dt) {
  if (mCoinSoundCooldown > 0.f) {
    mCoinSoundCooldown -= dt;
    if (mCoinSoundCooldown < 0.f)
      mCoinSoundCooldown = 0.f;
  }

  if (std::abs(mCurrentDuckingFactor - mTargetDuckingFactor) > 0.001f) {
    float speed = 2.5f; // Fast and smooth fade transition (~0.4s)
    if (mCurrentDuckingFactor < mTargetDuckingFactor) {
      mCurrentDuckingFactor += dt * speed;
      if (mCurrentDuckingFactor > mTargetDuckingFactor)
        mCurrentDuckingFactor = mTargetDuckingFactor;
    } else {
      mCurrentDuckingFactor -= dt * speed;
      if (mCurrentDuckingFactor < mTargetDuckingFactor)
        mCurrentDuckingFactor = mTargetDuckingFactor;
    }
    mLevelMusic.setVolume(mBaseMusicVolume * mCurrentDuckingFactor);
  }
}

void SoundManager::loadHazardSounds() {
  const std::string hazardPaths[5] = {"assets/sounds/hazards/stampede.wav",
                                      "assets/sounds/hazards/sandstorm.wav",
                                      "assets/sounds/hazards/arrow_volley.wav",
                                      "assets/sounds/hazards/rush_hour.wav",
                                      "assets/sounds/hazards/laser_beam.wav"};

  for (int i = 0; i < 5; ++i) {
    if (mHazardBuffers[i].loadFromFile(hazardPaths[i])) {
      mHazardSounds[i].setBuffer(mHazardBuffers[i]);
      float vol = (i == 3) ? 55.f : 70.f;
      mHazardSounds[i].setVolume(vol);
    }
  }

  if (mDinoRoarBuffer.loadFromFile("assets/sounds/hazards/dino_roar.wav")) {
    mDinoRoarSound.setBuffer(mDinoRoarBuffer);
    mDinoRoarSound.setVolume(100.f);
  }

  if (mLightningBuffer.loadFromFile("assets/sounds/hazards/lightning.wav")) {
    mLightningSound.setBuffer(mLightningBuffer);
    mLightningSound.setVolume(100.f);
  } else if (mHazardBuffers[3].getSampleCount() > 0) {
    mLightningSound.setBuffer(mHazardBuffers[3]);
    mLightningSound.setVolume(100.f);
  }

  if (mBlackHoleBuffer.loadFromFile("assets/sounds/hazards/black_hole.wav")) {
    mBlackHoleSound.setBuffer(mBlackHoleBuffer);
    mBlackHoleSound.setVolume(85.f);
  } else if (mHazardBuffers[4].getSampleCount() > 0) {
    mBlackHoleSound.setBuffer(mHazardBuffers[4]);
    mBlackHoleSound.setVolume(75.f);
  }
}

void SoundManager::playHazardSound(int level) {
  switch (level) {
  case 1:
    playStampede();
    break;
  case 2:
    playSandstorm();
    break;
  case 3:
    playArrowVolley();
    break;
  case 4:
    playRushHour();
    break;
  case 5:
    playLaserBeam();
    playBlackHole();
    break;
  default:
    break;
  }
}

void SoundManager::stopHazardSounds() {
  mDinoRoarSound.stop();
  mLightningSound.stop();
  mBlackHoleSound.stop();
  for (int i = 0; i < 5; ++i) {
    mHazardSounds[i].stop();
  }
}

void SoundManager::playDinoRoar() {
  if (mDinoRoarSound.getBuffer()) {
    mDinoRoarSound.play();
  }
}

void SoundManager::playLightning() {
  if (mLightningSound.getBuffer()) {
    mLightningSound.play();
  }
}

void SoundManager::playBlackHole() {
  if (mBlackHoleSound.getBuffer()) {
    mBlackHoleSound.setLoop(true);
    if (mBlackHoleSound.getStatus() != sf::Sound::Playing) {
      mBlackHoleSound.play();
    }
  }
}

void SoundManager::stopBlackHole() { mBlackHoleSound.stop(); }

void SoundManager::playStampede() {
  if (mHazardSounds[0].getBuffer()) {
    mHazardSounds[0].setLoop(false);
    mHazardSounds[0].stop();
    mHazardSounds[0].play();
  }
}

void SoundManager::playSandstorm() {
  if (mHazardSounds[1].getBuffer()) {
    mHazardSounds[1].setLoop(true);
    if (mHazardSounds[1].getStatus() != sf::Sound::Playing) {
      mHazardSounds[1].play();
    }
  }
}

void SoundManager::playArrowVolley() {
  if (mHazardSounds[2].getBuffer()) {
    mHazardSounds[2].setLoop(false);
    mHazardSounds[2].stop();
    mHazardSounds[2].play();
  }
}

void SoundManager::playRushHour() {
  if (mHazardSounds[3].getBuffer()) {
    mHazardSounds[3].setLoop(true);
    if (mHazardSounds[3].getStatus() != sf::Sound::Playing) {
      mHazardSounds[3].play();
    }
  }
}

void SoundManager::playLaserBeam() {
  if (mHazardSounds[4].getBuffer()) {
    mHazardSounds[4].setLoop(false);
    mHazardSounds[4].stop();
    mHazardSounds[4].play();
  }
}

void SoundManager::stopLaserBeam() {
  if (mHazardSounds[4].getBuffer()) {
    mHazardSounds[4].stop();
  }
}

void SoundManager::setSFXVolume(float v) {
  mDeadSound.setVolume(v);
  mVictorySound.setVolume(v);
  mLevelClearSound.setVolume(v);
  mElevatorDoorSound.setVolume(v);
  mElevatorMoveSound.setVolume(v);
  mElevatorDingSound.setVolume(v);
  mCoinSound.setVolume(v);
  mDinoRoarSound.setVolume(v);
  mLightningSound.setVolume(v);
  mBlackHoleSound.setVolume(v);
  for (int i = 0; i < 5; ++i) {
    mLevelDeathSounds[i].setVolume(v);
    mHazardSounds[i].setVolume(v);
  }
}
