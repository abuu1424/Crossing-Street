#include "StoryCutsceneManager.h"
#include "SoundManager.h"
#include <iostream>
#include <cmath>

StoryCutsceneManager::StoryCutsceneManager() {
}

void StoryCutsceneManager::init(const sf::Font& font, SoundManager* sound) {
    mFont = font;
    mSound = sound;

    // Khởi tạo nút Skip Cutscene (Đặt gọn gàng ở góc dưới bên phải)
    mSkipButton.setup("assets/ui/menu/btn_yes.png", "SKIP", mFont, 1180.f, 630.f, "assets/ui/menu/btn_yes_hover.png", 16);

    // Thiết lập giao diện điện ảnh siêu mỏng (Sleek Cinematic Bars - Không che khuất tranh)
    mCinematicBarTop.setSize(sf::Vector2f(1280.f, 42.f));
    mCinematicBarTop.setFillColor(sf::Color(8, 10, 16, 220));
    mCinematicBarTop.setPosition(0.f, 0.f);

    mCinematicBarBottom.setSize(sf::Vector2f(1280.f, 105.f));
    mCinematicBarBottom.setFillColor(sf::Color(8, 10, 16, 170));
    mCinematicBarBottom.setPosition(0.f, 615.f);

    // Khung hộp thoại bán trong suốt gọn gàng (Subtitle Card Container)
    mSubtitleBox.setSize(sf::Vector2f(1120.f, 85.f));
    mSubtitleBox.setFillColor(sf::Color(10, 12, 22, 215));
    mSubtitleBox.setOutlineColor(sf::Color(255, 215, 0, 90));
    mSubtitleBox.setOutlineThickness(1.f);
    mSubtitleBox.setPosition(80.f, 620.f);

    // Tiêu đề Chapter góc trên màn hình
    mChapterTitleText.setFont(mFont);
    mChapterTitleText.setCharacterSize(16);
    mChapterTitleText.setFillColor(sf::Color(240, 220, 130, 220));
    mChapterTitleText.setStyle(sf::Text::Bold);

    // Subtitle & Speaker UI
    mSpeakerText.setFont(mFont);
    mSpeakerText.setCharacterSize(17);
    mSpeakerText.setFillColor(sf::Color(255, 215, 0)); // Màu vàng kim
    mSpeakerText.setStyle(sf::Text::Bold);
    mSpeakerText.setPosition(102.f, 626.f);

    mDialogueText.setFont(mFont);
    mDialogueText.setCharacterSize(17);
    mDialogueText.setFillColor(sf::Color(245, 245, 250));
    mDialogueText.setPosition(102.f, 652.f);

    // End Credit Text
    mEndCreditText.setFont(mFont);
    mEndCreditText.setCharacterSize(22);
    mEndCreditText.setFillColor(sf::Color(240, 240, 240));
    mEndCreditText.setStyle(sf::Text::Italic);
}

void StoryCutsceneManager::startChapter(int chapterNumber) {
    mIsActive = true;
    mIsFinished = false;
    mCurrentEventIndex = 0;
    mEventTimer = 0.f;
    mInSoundVacuum = false;
    mVacuumTimer = 0.f;

    loadChapterScript(chapterNumber);
    if (!mCurrentScript.events.empty()) {
        executeCurrentEvent();
    } else {
        mIsFinished = true;
        mIsActive = false;
    }
}

void StoryCutsceneManager::loadChapterScript(int chapter) {
    switch (chapter) {
        case 1: mCurrentScript = createChapter1Script(); break;
        case 2: mCurrentScript = createChapter2Script(); break;
        case 3: mCurrentScript = createChapter3Script(); break;
        case 4: mCurrentScript = createChapter4Script(); break;
        case 5: mCurrentScript = createChapter5Script(); break;
        case 6: mCurrentScript = createEndingScript(); break;
        default: mCurrentScript = createChapter1Script(); break;
    }

    // Căn giữa tiêu đề Chapter ở thanh Bar trên cùng
    mChapterTitleText.setString(mCurrentScript.chapterTitle);
    sf::FloatRect bounds = mChapterTitleText.getLocalBounds();
    mChapterTitleText.setOrigin(bounds.width / 2.f, 0.f);
    mChapterTitleText.setPosition(640.f, 10.f);
}

void StoryCutsceneManager::executeCurrentEvent() {
    if (mCurrentEventIndex >= mCurrentScript.events.size()) {
        mIsFinished = true;
        mIsActive = false;
        return;
    }

    const CutsceneEvent& evt = mCurrentScript.events[mCurrentEventIndex];
    mEventTimer = 0.f;

    switch (evt.type) {
        case CutsceneEventType::PANEL_SHOW: {
            if (mCurrentPanelTexture.loadFromFile(evt.imagePath)) {
                mCurrentPanelSprite.setTexture(mCurrentPanelTexture, true);
                sf::Vector2u texSize = mCurrentPanelTexture.getSize();
                mCurrentPanelSprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
                mCurrentPanelSprite.setPosition(640.f, 360.f);

                // Scale vừa màn hình 1280x720
                float scaleX = 1280.f / texSize.x;
                float scaleY = 720.f / texSize.y;
                float baseScale = std::max(scaleX, scaleY);
                mCurrentPanelSprite.setScale(baseScale, baseScale);
            }
            mZoomStart = evt.zoomStart;
            mZoomEnd = evt.zoomEnd;
            mCurrentZoom = mZoomStart;
            mPanOffset = evt.panOffset;
            break;
        }

        case CutsceneEventType::DIALOGUE: {
            mFullDialogueText = evt.text;
            mDisplayedDialogueText = "";
            mTypingIndex = 0;
            mTypingTimer = 0.f;
            mIsTyping = true;
            mDialoguePauseTimer = 0.f;

            mSpeakerText.setString(evt.speaker);

            if (evt.style == DialogueStyle::WHISPER) {
                mSpeakerText.setFillColor(sf::Color(180, 180, 200));
                mDialogueText.setFillColor(sf::Color(200, 200, 220));
                mDialogueText.setStyle(sf::Text::Italic);
            } else if (evt.style == DialogueStyle::SYSTEM_AI) {
                mSpeakerText.setFillColor(sf::Color(0, 240, 255)); // Neon Cyan
                mDialogueText.setFillColor(sf::Color(180, 255, 255));
                mDialogueText.setStyle(sf::Text::Regular);
            } else {
                mSpeakerText.setFillColor(sf::Color(255, 215, 0)); // Gold
                mDialogueText.setFillColor(sf::Color(245, 245, 250));
                mDialogueText.setStyle(sf::Text::Regular);
            }
            break;
        }

        case CutsceneEventType::SOUND_VACUUM: {
            mInSoundVacuum = true;
            mVacuumTimer = evt.soundVacuumDuration;
            if (mSound) {
                mSound->setMusicDuckingFactor(0.0f);
                mSound->stopAllEffects();
            }
            break;
        }

        case CutsceneEventType::PLAY_SFX: {
            // Sound play logic
            break;
        }

        case CutsceneEventType::LIGHTING_TINT: {
            mStartTint = mCurrentTint;
            mTargetTint = evt.tintColor;
            mTintTimer = 0.f;
            mTintDuration = evt.tintDuration;
            break;
        }

        case CutsceneEventType::CAMERA_SHAKE: {
            mShakeTimer = evt.duration;
            break;
        }

        case CutsceneEventType::DELAY: {
            break;
        }
    }
}

void StoryCutsceneManager::advanceEvent() {
    mCurrentEventIndex++;
    if (mCurrentEventIndex < mCurrentScript.events.size()) {
        executeCurrentEvent();
    } else {
        mIsFinished = true;
        mIsActive = false;
    }
}

void StoryCutsceneManager::update(float dt, sf::Vector2f mousePos) {
    if (!mIsActive || mIsFinished) return;

    mEventTimer += dt;
    const CutsceneEvent& evt = mCurrentScript.events[mCurrentEventIndex];

    // Cập nhật Sound Vacuum timer
    if (mInSoundVacuum) {
        mVacuumTimer -= dt;
        if (mVacuumTimer <= 0.f) {
            mInSoundVacuum = false;
            if (mSound) {
                mSound->setMusicDuckingFactor(1.0f);
            }
        }
    }

    // Cập nhật từng loại sự kiện
    switch (evt.type) {
        case CutsceneEventType::PANEL_SHOW: {
            updateKenBurns(dt);
            if (mEventTimer >= evt.duration) {
                advanceEvent();
            }
            break;
        }

        case CutsceneEventType::DIALOGUE: {
            updateTyping(dt);
            if (!mIsTyping) {
                mDialoguePauseTimer += dt;
                if (mDialoguePauseTimer >= evt.pauseAfter) {
                    advanceEvent();
                }
            }
            break;
        }

        case CutsceneEventType::LIGHTING_TINT: {
            updateLightingTint(dt);
            if (mEventTimer >= evt.tintDuration) {
                advanceEvent();
            }
            break;
        }

        case CutsceneEventType::CAMERA_SHAKE: {
            updateCameraShake(dt);
            if (mEventTimer >= evt.duration) {
                mShakeOffset = sf::Vector2f(0.f, 0.f);
                advanceEvent();
            }
            break;
        }

        case CutsceneEventType::SOUND_VACUUM:
        case CutsceneEventType::PLAY_SFX:
        case CutsceneEventType::DELAY: {
            if (mEventTimer >= evt.duration) {
                advanceEvent();
            }
            break;
        }
    }

    mSkipButton.update(mousePos, dt);
}

void StoryCutsceneManager::updateTyping(float dt) {
    if (!mIsTyping) return;

    mTypingTimer += dt;
    if (mTypingTimer >= mTypingSpeed) {
        mTypingTimer = 0.f;
        if (mTypingIndex < mFullDialogueText.size()) {
            mDisplayedDialogueText += mFullDialogueText[mTypingIndex];
            mTypingIndex++;

            // Tự động xuống dòng khi câu thoại quá dài (Max 75 ký tự mỗi dòng)
            std::string wrapped = mDisplayedDialogueText;
            size_t lastSpace = 0;
            size_t lineStart = 0;
            for (size_t i = 0; i < wrapped.size(); ++i) {
                if (wrapped[i] == ' ') lastSpace = i;
                if (wrapped[i] == '\n') lineStart = i + 1;
                if (i - lineStart > 75 && lastSpace > lineStart) {
                    wrapped[lastSpace] = '\n';
                    lineStart = lastSpace + 1;
                }
            }
            mDialogueText.setString(wrapped);
        } else {
            mIsTyping = false;
        }
    }
}

void StoryCutsceneManager::updateKenBurns(float dt) {
    const CutsceneEvent& evt = mCurrentScript.events[mCurrentEventIndex];
    if (evt.duration <= 0.f) return;

    float progress = std::min(mEventTimer / evt.duration, 1.0f);
    mCurrentZoom = mZoomStart + (mZoomEnd - mZoomStart) * progress;

    sf::Vector2u texSize = mCurrentPanelTexture.getSize();
    float scaleX = (1280.f / texSize.x) * mCurrentZoom;
    float scaleY = (720.f / texSize.y) * mCurrentZoom;
    float baseScale = std::max(scaleX, scaleY);
    mCurrentPanelSprite.setScale(baseScale, baseScale);

    // Slide/Pan nhẹ
    sf::Vector2f pan = evt.panOffset * progress;
    mCurrentPanelSprite.setPosition(640.f + pan.x + mShakeOffset.x, 360.f + pan.y + mShakeOffset.y);
}

void StoryCutsceneManager::updateLightingTint(float dt) {
    if (mTintDuration <= 0.f) return;

    mTintTimer += dt;
    float progress = std::min(mTintTimer / mTintDuration, 1.0f);

    sf::Uint8 r = static_cast<sf::Uint8>(mStartTint.r + (mTargetTint.r - mStartTint.r) * progress);
    sf::Uint8 g = static_cast<sf::Uint8>(mStartTint.g + (mTargetTint.g - mStartTint.g) * progress);
    sf::Uint8 b = static_cast<sf::Uint8>(mStartTint.b + (mTargetTint.b - mStartTint.b) * progress);
    sf::Uint8 a = static_cast<sf::Uint8>(mStartTint.a + (mTargetTint.a - mStartTint.a) * progress);

    mCurrentTint = sf::Color(r, g, b, a);
    mLightingOverlay.setSize(sf::Vector2f(1280.f, 720.f));
    mLightingOverlay.setFillColor(mCurrentTint);
}

void StoryCutsceneManager::updateCameraShake(float dt) {
    mShakeTimer -= dt;
    if (mShakeTimer > 0.f) {
        float rx = ((rand() % 100) / 100.f - 0.5f) * 6.f;
        float ry = ((rand() % 100) / 100.f - 0.5f) * 6.f;
        mShakeOffset = sf::Vector2f(rx, ry);
    } else {
        mShakeOffset = sf::Vector2f(0.f, 0.f);
    }
}

void StoryCutsceneManager::skip() {
    mIsFinished = true;
    mIsActive = false;
    if (mSound) {
        mSound->setMusicDuckingFactor(1.0f);
    }
}

bool StoryCutsceneManager::isSkipButtonClicked(sf::Vector2f mousePos) const {
    return mSkipButton.contains(mousePos);
}

void StoryCutsceneManager::render(sf::RenderWindow& window) {
    if (!mIsActive || mIsFinished) return;

    // 1. Render Panel ảnh minh họa Cutscene
    window.draw(mCurrentPanelSprite);

    // 2. Render Overlay đổi màu ánh sáng (Tint)
    if (mCurrentTint.a > 0) {
        window.draw(mLightingOverlay);
    }

    // 3. Render Thanh viền điện ảnh siêu mỏng (Top & Bottom Bars)
    window.draw(mCinematicBarTop);
    window.draw(mChapterTitleText);
    window.draw(mCinematicBarBottom);

    // 4. Render Khung Subtitle & Chữ Thoại UI (Khi có lời thoại)
    const CutsceneEvent& evt = mCurrentScript.events[mCurrentEventIndex];
    if (evt.type == CutsceneEventType::DIALOGUE && !mFullDialogueText.empty()) {
        window.draw(mSubtitleBox);
        if (!mSpeakerText.getString().isEmpty()) {
            window.draw(mSpeakerText);
        }
        window.draw(mDialogueText);
    }

    // 5. Render Cảnh Kết (Nền đen chữ trắng)
    if (mCurrentScript.chapterNumber == 6 && evt.type == CutsceneEventType::DIALOGUE) {
        sf::RectangleShape blackScreen(sf::Vector2f(1280.f, 720.f));
        blackScreen.setFillColor(sf::Color(0, 0, 0, 240));
        window.draw(blackScreen);

        mEndCreditText.setString(mDisplayedDialogueText);
        sf::FloatRect bounds = mEndCreditText.getLocalBounds();
        mEndCreditText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        mEndCreditText.setPosition(640.f, 360.f);
        window.draw(mEndCreditText);
    }

    // 6. Render Skip Button UI
    mSkipButton.draw(window);
}

// ==================== CHAPTER 1 SCRIPT ====================
ChapterScript StoryCutsceneManager::createChapter1Script() {
    ChapterScript script;
    script.chapterNumber = 1;
    script.chapterTitle = "CHAPTER 1 — KROU: THE REALM OF SOLITUDE";

    // Event 0A: Prologue Panel — The Shattered Time Prism
    CutsceneEvent e0a;
    e0a.type = CutsceneEventType::PANEL_SHOW;
    e0a.imagePath = "assets/cutscenes/intro/intro_shattered_prism.jpg";
    e0a.zoomStart = 1.0f;
    e0a.zoomEnd = 1.04f;
    e0a.duration = 4.0f;
    script.events.push_back(e0a);

    // Event 0B: Prologue Dialogue — Origin of the 5 Memory Stones
    CutsceneEvent e0b;
    e0b.type = CutsceneEventType::DIALOGUE;
    e0b.speaker = "Legend";
    e0b.text = "\"When human sorrow reached its peak, the Time Prism shattered into 5 Memory Stones, scattering across five eras...\"";
    e0b.style = DialogueStyle::WHISPER;
    e0b.pauseAfter = 2.5f;
    script.events.push_back(e0b);

    // Event 0C: Prologue Panel — The Realm Elevator Mission
    CutsceneEvent e0c;
    e0c.type = CutsceneEventType::PANEL_SHOW;
    e0c.imagePath = "assets/cutscenes/intro/intro_elevator_mission.jpg";
    e0c.zoomStart = 1.0f;
    e0c.zoomEnd = 1.03f;
    e0c.duration = 3.5f;
    script.events.push_back(e0c);

    // Event 0D: Prologue Dialogue — Mission Call
    CutsceneEvent e0d;
    e0d.type = CutsceneEventType::DIALOGUE;
    e0d.speaker = "Legend";
    e0d.text = "\"To heal the rift, someone must cross the road of time, collect all 5 Stones, and bring lost souls together.\"";
    e0d.style = DialogueStyle::WHISPER;
    e0d.pauseAfter = 2.5f;
    script.events.push_back(e0d);

    // Event 1: Static panel of Krou's back (Hold 3.5s lonely silence)
    CutsceneEvent e1;
    e1.type = CutsceneEventType::PANEL_SHOW;
    e1.imagePath = "assets/cutscenes/ch1/ch1_krou_intro_back.jpg";
    e1.zoomStart = 1.0f;
    e1.zoomEnd = 1.03f;
    e1.duration = 3.5f;
    script.events.push_back(e1);

    // Event 2: Krou whisper monologue
    CutsceneEvent e2;
    e2.type = CutsceneEventType::DIALOGUE;
    e2.speaker = "Krou";
    e2.text = "\"Don't leave me behind again...\"";
    e2.style = DialogueStyle::WHISPER;
    e2.pauseAfter = 2.0f;
    script.events.push_back(e2);

    // Event 3: Panel of Krou entering the Elevator
    CutsceneEvent e3;
    e3.type = CutsceneEventType::PANEL_SHOW;
    e3.imagePath = "assets/cutscenes/ch1/ch1_elevator_enter.jpg";
    e3.zoomStart = 1.0f;
    e3.zoomEnd = 1.05f;
    e3.duration = 3.0f;
    script.events.push_back(e3);

    // Event 4: Warm orange light ray through closing doors
    CutsceneEvent e4;
    e4.type = CutsceneEventType::LIGHTING_TINT;
    e4.tintColor = sf::Color(255, 160, 60, 50); // Soft warm orange
    e4.tintDuration = 2.0f;
    script.events.push_back(e4);

    // Event 5: Elevator dialogue
    CutsceneEvent e5;
    e5.type = CutsceneEventType::DIALOGUE;
    e5.speaker = "Krou";
    e5.text = "\"If... if I come with you, will you leave me behind halfway?\"";
    e5.style = DialogueStyle::NORMAL;
    e5.pauseAfter = 2.5f;
    script.events.push_back(e5);

    return script;
}

// ==================== CHAPTER 2 SCRIPT ====================
ChapterScript StoryCutsceneManager::createChapter2Script() {
    ChapterScript script;
    script.chapterNumber = 2;
    script.chapterTitle = "CHAPTER 2 — NEFERA: THE REALM OF DOUBT";

    CutsceneEvent e1;
    e1.type = CutsceneEventType::PANEL_SHOW;
    e1.imagePath = "assets/cutscenes/ch2/ch2_nefera_defensive.jpg";
    e1.zoomStart = 1.0f;
    e1.zoomEnd = 1.04f;
    e1.duration = 3.0f;
    script.events.push_back(e1);

    CutsceneEvent e2;
    e2.type = CutsceneEventType::DIALOGUE;
    e2.speaker = "Nefera";
    e2.text = "\"What do you want?... Everyone wants something.\"";
    e2.style = DialogueStyle::NORMAL;
    e2.pauseAfter = 1.5f;
    script.events.push_back(e2);

    CutsceneEvent e3;
    e3.type = CutsceneEventType::DIALOGUE;
    e3.speaker = "Krou";
    e3.text = "\"I don't know what you have to take. I just didn't want you to fall alone.\"";
    e3.style = DialogueStyle::NORMAL;
    e3.pauseAfter = 2.0f;
    script.events.push_back(e3);

    // Handing over Solar Crystal
    CutsceneEvent e4;
    e4.type = CutsceneEventType::PANEL_SHOW;
    e4.imagePath = "assets/cutscenes/ch2/ch2_solar_crystal.jpg";
    e4.zoomStart = 1.0f;
    e4.zoomEnd = 1.05f;
    e4.duration = 3.0f;
    script.events.push_back(e4);

    // Light tint lerp to soft sunset tone
    CutsceneEvent e5;
    e5.type = CutsceneEventType::LIGHTING_TINT;
    e5.tintColor = sf::Color(230, 130, 40, 60);
    e5.tintDuration = 2.0f;
    script.events.push_back(e5);

    CutsceneEvent e6;
    e6.type = CutsceneEventType::DIALOGUE;
    e6.speaker = "Nefera";
    e6.text = "\"This is the only thing I have left from the one who trusted me unconditionally... I think you deserve to know why I find it so hard to trust people.\"";
    e6.style = DialogueStyle::NORMAL;
    e6.pauseAfter = 2.5f;
    script.events.push_back(e6);

    return script;
}

// ==================== CHAPTER 3 SCRIPT ====================
ChapterScript StoryCutsceneManager::createChapter3Script() {
    ChapterScript script;
    script.chapterNumber = 3;
    script.chapterTitle = "CHAPTER 3 — LIU: THE REALM OF SELF-PUNISHMENT";

    CutsceneEvent e1;
    e1.type = CutsceneEventType::PANEL_SHOW;
    e1.imagePath = "assets/cutscenes/ch3/ch3_liu_arrow_rain.jpg";
    e1.zoomStart = 1.0f;
    e1.zoomEnd = 1.04f;
    e1.duration = 3.0f;
    script.events.push_back(e1);

    CutsceneEvent e2;
    e2.type = CutsceneEventType::DIALOGUE;
    e2.speaker = "Liu";
    e2.text = "\"You don't understand... I am the reason they died!\"";
    e2.style = DialogueStyle::NORMAL;
    e2.pauseAfter = 1.5f;
    script.events.push_back(e2);

    // Sound Vacuum (Total sound vacuum when Liu kneels)
    CutsceneEvent e3;
    e3.type = CutsceneEventType::SOUND_VACUUM;
    e3.soundVacuumDuration = 2.5f;
    e3.duration = 2.5f;
    script.events.push_back(e3);

    CutsceneEvent e4;
    e4.type = CutsceneEventType::DIALOGUE;
    e4.speaker = "Nefera";
    e4.text = "\"Then don't turn their death into a reason for you to die too. They deserved better than that.\"";
    e4.style = DialogueStyle::NORMAL;
    e4.pauseAfter = 2.0f;
    script.events.push_back(e4);

    // Trio sitting under moonlight panel
    CutsceneEvent e5;
    e5.type = CutsceneEventType::PANEL_SHOW;
    e5.imagePath = "assets/cutscenes/ch3/ch3_trio_moonlight.jpg";
    e5.zoomStart = 1.0f;
    e5.zoomEnd = 1.03f;
    e5.duration = 3.5f;
    script.events.push_back(e5);

    CutsceneEvent e6;
    e6.type = CutsceneEventType::DIALOGUE;
    e6.speaker = "Liu";
    e6.text = "\"It's been a long time since I sat beside someone without thinking about protecting them with my own blood.\"";
    e6.style = DialogueStyle::NORMAL;
    e6.pauseAfter = 3.0f;
    script.events.push_back(e6);

    return script;
}

// ==================== CHAPTER 4 SCRIPT ====================
ChapterScript StoryCutsceneManager::createChapter4Script() {
    ChapterScript script;
    script.chapterNumber = 4;
    script.chapterTitle = "CHAPTER 4 — LINH: THE REALM OF HASTE";

    CutsceneEvent e1;
    e1.type = CutsceneEventType::PANEL_SHOW;
    e1.imagePath = "assets/cutscenes/ch4/ch4_krou_grabs_sleeve.jpg";
    e1.zoomStart = 1.0f;
    e1.zoomEnd = 1.05f;
    e1.duration = 3.0f;
    script.events.push_back(e1);

    CutsceneEvent e2;
    e2.type = CutsceneEventType::CAMERA_SHAKE;
    e2.duration = 0.5f;
    script.events.push_back(e2);

    CutsceneEvent e3;
    e3.type = CutsceneEventType::DIALOGUE;
    e3.speaker = "Krou";
    e3.text = "\"You don't need to rush anywhere. No one here is keeping time.\"";
    e3.style = DialogueStyle::NORMAL;
    e3.pauseAfter = 2.0f;
    script.events.push_back(e3);

    CutsceneEvent e4;
    e4.type = CutsceneEventType::PANEL_SHOW;
    e4.imagePath = "assets/cutscenes/ch4/ch4_linh_street_food.jpg";
    e4.zoomStart = 1.0f;
    e4.zoomEnd = 1.04f;
    e4.duration = 3.5f;
    script.events.push_back(e4);

    CutsceneEvent e5;
    e5.type = CutsceneEventType::DIALOGUE;
    e5.speaker = "Linh";
    e5.text = "\"Back home, people always say to eat fast or you'll be late... Today... can we eat slowly?\"";
    e5.style = DialogueStyle::NORMAL;
    e5.pauseAfter = 2.5f;
    script.events.push_back(e5);

    return script;
}

// ==================== CHAPTER 5 SCRIPT ====================
ChapterScript StoryCutsceneManager::createChapter5Script() {
    ChapterScript script;
    script.chapterNumber = 5;
    script.chapterTitle = "CHAPTER 5 — ZORA: THE REALM OF SELF-REJECTION";

    CutsceneEvent e1;
    e1.type = CutsceneEventType::PANEL_SHOW;
    e1.imagePath = "assets/cutscenes/ch5/ch5_zora_cyberpunk.jpg";
    e1.zoomStart = 1.0f;
    e1.zoomEnd = 1.04f;
    e1.duration = 3.0f;
    script.events.push_back(e1);

    CutsceneEvent e2;
    e2.type = CutsceneEventType::DIALOGUE;
    e2.speaker = "AI System";
    e2.text = "\"Emotional variance detected. Proceeding with calibration...\"";
    e2.style = DialogueStyle::SYSTEM_AI;
    e2.pauseAfter = 1.5f;
    script.events.push_back(e2);

    CutsceneEvent e3;
    e3.type = CutsceneEventType::DIALOGUE;
    e3.speaker = "Zora";
    e3.text = "\"If crying is an error... then I don't want to be fixed anymore!\"";
    e3.style = DialogueStyle::NORMAL;
    e3.pauseAfter = 2.0f;
    script.events.push_back(e3);

    CutsceneEvent e4;
    e4.type = CutsceneEventType::PANEL_SHOW;
    e4.imagePath = "assets/cutscenes/ch5/ch5_zora_accepted.jpg";
    e4.zoomStart = 1.0f;
    e4.zoomEnd = 1.03f;
    e4.duration = 3.5f;
    script.events.push_back(e4);

    CutsceneEvent e5;
    e5.type = CutsceneEventType::DIALOGUE;
    e5.speaker = "Zora";
    e5.text = "\"Thank you... for showing me that having a heart isn't a glitch.\"";
    e5.style = DialogueStyle::NORMAL;
    e5.pauseAfter = 2.5f;
    script.events.push_back(e5);

    return script;
}

// ==================== ENDING SCRIPT ====================
ChapterScript StoryCutsceneManager::createEndingScript() {
    ChapterScript script;
    script.chapterNumber = 6;
    script.chapterTitle = "ENDING — A NORMAL EVENING";

    CutsceneEvent e1;
    e1.type = CutsceneEventType::PANEL_SHOW;
    e1.imagePath = "assets/cutscenes/ending/ending_normal_evening.jpg";
    e1.zoomStart = 1.0f;
    e1.zoomEnd = 1.03f;
    e1.duration = 4.0f;
    script.events.push_back(e1);

    CutsceneEvent e2;
    e2.type = CutsceneEventType::DIALOGUE;
    e2.speaker = "";
    e2.text = "\"None of us cross the path of our lives alone.\nWe just never noticed who was crossing it alongside us.\"";
    e2.style = DialogueStyle::WHISPER;
    e2.pauseAfter = 4.0f;
    script.events.push_back(e2);

    return script;
}
