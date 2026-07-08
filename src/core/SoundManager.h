#pragma once
#include <SFML/Audio.hpp>
#include <string>

// SoundManager: gom 3 sound effect (victory/dead/levelclear) + nhạc nền level
// vào 1 chỗ. Trước đây các field này nằm rải rác trong CGAME và việc chỉnh
// volume phải sửa 4 nơi khác nhau (updatePauseSliders) — giờ chỉ cần gọi
// setSFXVolume/setMusicVolume.
class SoundManager {
public:
    void loadEffects(const std::string& victoryPath,
                      const std::string& deadPath,
                      const std::string& levelClearPath);

    // Đổi nhạc nền theo level (dừng bài cũ, mở + loop bài mới).
    void playLevelMusic(const std::string& musicPath, float volume = 40.f);
    void stopMusic();
    void stopAllEffects(); // dừng cả 3 sound effect (dùng khi reset())

    void playVictory();
    void playDead();
    void playLevelClear();
    void stopLevelClear();

    void setMusicVolume(float v);
    void setSFXVolume(float v); // áp dụng cho cả 3 sound effect

private:
    sf::SoundBuffer mVictoryBuffer, mDeadBuffer, mLevelClearBuffer;
    sf::Sound       mVictorySound, mDeadSound, mLevelClearSound;
    sf::Music       mLevelMusic;
};
