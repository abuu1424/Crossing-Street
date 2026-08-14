#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include "StoryCutsceneData.h"
#include "MenuButton.h"

class SoundManager;

class StoryCutsceneManager {
public:
    StoryCutsceneManager();

    void init(const sf::Font& font, SoundManager* sound = nullptr);
    void setSoundManager(SoundManager* sound) { mSound = sound; }

    void startChapter(int chapterNumber); // 1 to 5, 6 for Ending
    void update(float dt, sf::Vector2f mousePos = sf::Vector2f(0.f, 0.f));
    void render(sf::RenderWindow& window);
    void skip();

    bool isFinished() const { return mIsFinished; }
    bool isActive() const { return mIsActive; }
    bool isSkipButtonClicked(sf::Vector2f mousePos) const;
    int getCurrentChapter() const { return mCurrentScript.chapterNumber; }

private:
    bool mIsActive = false;
    bool mIsFinished = false;

    ChapterScript mCurrentScript;
    size_t mCurrentEventIndex = 0;
    float mEventTimer = 0.f;

    // Typewriter Text Effect & Dialogue State
    std::string mFullDialogueText;
    std::string mDisplayedDialogueText;
    size_t mTypingIndex = 0;
    float mTypingTimer = 0.f;
    float mTypingSpeed = 0.04f; // Giây mỗi ký tự
    bool mIsTyping = false;
    float mDialoguePauseTimer = 0.f;

    // Visual Panel & Ken Burns Zoom/Pan State
    sf::Texture mCurrentPanelTexture;
    sf::Sprite mCurrentPanelSprite;
    float mZoomStart = 1.0f;
    float mZoomEnd = 1.05f;
    float mCurrentZoom = 1.0f;
    sf::Vector2f mPanOffset;

    // Color Tint & Lighting Overlay State
    sf::RectangleShape mLightingOverlay;
    sf::Color mStartTint = sf::Color(0, 0, 0, 0);
    sf::Color mTargetTint = sf::Color(0, 0, 0, 0);
    sf::Color mCurrentTint = sf::Color(0, 0, 0, 0);
    float mTintTimer = 0.f;
    float mTintDuration = 1.0f;

    // Camera Shake
    sf::Vector2f mShakeOffset;
    float mShakeTimer = 0.f;

    // Sound Vacuum State
    bool mInSoundVacuum = false;
    float mVacuumTimer = 0.f;

    // UI Elements
    sf::Font mFont;
    SoundManager* mSound = nullptr;
    MenuButton mSkipButton;

    // Subtitle & Dialogue Panels
    sf::RectangleShape mSubtitleBox;
    sf::RectangleShape mCinematicBarTop;
    sf::RectangleShape mCinematicBarBottom;

    sf::Text mSpeakerText;
    sf::Text mDialogueText;
    sf::Text mEndCreditText; // Chữ trắng trên nền đen cho Cảnh Kết
    sf::Text mChapterTitleText; // Tiêu đề chương góc trên
    sf::RectangleShape mSpeakerBadge; // Badge làm nổi bật tên nhân vật

    void loadChapterScript(int chapter);
    void executeCurrentEvent();
    void advanceEvent();
    void updateTyping(float dt);
    void updateLightingTint(float dt);
    void updateKenBurns(float dt);
    void updateCameraShake(float dt);

    ChapterScript createChapter1Script();
    ChapterScript createChapter2Script();
    ChapterScript createChapter3Script();
    ChapterScript createChapter4Script();
    ChapterScript createChapter5Script();
    ChapterScript createEndingScript();
};
