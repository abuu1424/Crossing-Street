#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "LevelConfig.h"
#include "Utils.h"
#include "MenuButton.h"

enum class ElevatorPhase {
    IDLE,
    DOOR_CLOSING,      // Phase 1: Cửa trượt khép lại (khớp 2.47s của elevator_door.ogg)
    MOVING,            // Phase 2: Thang cuộn di chuyển & rung lắc (khớp 6.38s của elevator_move.ogg)
    MORPHING,          // Phase 3: Đèn chớp sáng & DING lột xác (khớp 1.08s của elevator_ding.ogg)
    DOOR_OPENING,      // Phase 4: Cửa trượt mở ra (khớp 2.47s của elevator_door.ogg)
    FINISHED           // Hoàn thành
};

class SoundManager;

class ElevatorCutscene {
public:
    ElevatorCutscene();

    void init(const sf::Font& font, SoundManager* sound = nullptr);
    void setSoundManager(SoundManager* sound) { mSound = sound; }
    void start(int fromLevel, int toLevel);
    void update(float dt, sf::Vector2f mousePos = sf::Vector2f(0.f, 0.f));
    void render(sf::RenderWindow& window);
    void skip();

    bool isFinished() const { return mPhase == ElevatorPhase::FINISHED; }
    bool isActive() const { return mPhase != ElevatorPhase::IDLE && mPhase != ElevatorPhase::FINISHED; }
    bool isSkipButtonClicked(sf::Vector2f mousePos) const;
    int getTargetLevel() const { return mToLevel; }

private:
    ElevatorPhase mPhase = ElevatorPhase::IDLE;
    int mFromLevel = 1;
    int mToLevel = 2;

    float mPhaseTimer = 0.f;
    float mDoorProgress = 0.f; // 0.f (mở hẳn) -> 1.f (đóng kín)
    float mScrollOffset = 0.f; // Vị trí cuộn phông nền bảo tàng
    float mFlashAlpha = 0.f;   // Độ sáng hiệu ứng chớp đèn khi biến đổi
    sf::Vector2f mShakeOffset; // Hiệu ứng rung lắc thang máy khi di chuyển (Screen Shake Offset)

    sf::Font mFont;
    SoundManager* mSound = nullptr;

    // Sprite nhân vật (cắt chuẩn 1 frame 64x64)
    sf::Texture mOldPlayerTexture;
    sf::Sprite mOldPlayerSprite;
    sf::Texture mNewPlayerTexture;
    sf::Sprite mNewPlayerSprite;
    bool mIsMorphed = false;

    // Image assets từ thư mục assets/ui/elevator/
    sf::Texture mFramePixelTexture;
    sf::Texture mDoorPixelTexture;
    sf::Texture mEraCardTextures[5];
    sf::Sprite mEraCardSprites[5];

    // Popup Skip Button UI
    MenuButton mSkipButton;

    // Thông tin các tầng thời kỳ bảo tàng (Toàn bộ bằng Tiếng Anh)
    struct EraFloorInfo {
        int level;
        std::string name;
        std::string period;
        std::string description;
        sf::Color color;
    };
    std::vector<EraFloorInfo> mFloors;

    // Graphic Elements & UI Panels
    sf::RectangleShape mBgOverlay;
    sf::RectangleShape mElevatorGlassWindow;
    sf::RectangleShape mLedPanel;
    sf::RectangleShape mFlashOverlay;

    sf::Text mLedText;
    sf::Text mFloorTitleText;
    sf::Text mSubTitleText;
    sf::Text mEraDescText;

    void loadElevatorAssets();
    void setupLayout();
    void updateLedDisplay();
    void drawPixelElevatorFrame(sf::RenderWindow& window, float glassX, float glassY, float glassW, float glassH);
    void drawPixelDoors(sf::RenderWindow& window, float glassX, float glassY, float glassW, float glassH);
};
