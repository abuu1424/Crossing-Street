#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "PlayerStats.h"
#include "BotAI.h"

class HUD {
private:
    sf::Font    mFont;
    sf::Texture mHudTexture;
    sf::Sprite  mHudSprite;

    sf::Text mLevelText;
    sf::Text mScoreText;
    sf::Text mTimeText;

    sf::Vector2f mLevelCenter;
    sf::Vector2f mScoreCenter;
    sf::Vector2f mTimeCenter;

    // Item Panel (Top-Left)
    sf::RectangleShape mItemPanelBox;
    sf::Texture mItemTextures[4];
    sf::Sprite  mItemSprites[4];
    sf::Text    mItemCountTexts[4];
    sf::Text    mItemHotkeyTexts[4];
    bool        mItemPanelLoaded = false;

    void setupItemPanel();
    void drawItemPanel(sf::RenderWindow& window);

    bool mLoaded;
    bool mHudBarLoaded = false;

    // Cache state to avoid string allocation every frame
    int mLastLevel = -1;
    int mLastScore = -1;
    int mLastRemainingSec = -1;

    // Heart sprites (Top-Right HP)
    sf::Texture mHeartTexture;
    sf::Sprite  mHeartSprite;
    sf::Texture mHeartHalfTexture;
    sf::Sprite  mHeartHalfSprite;
    sf::Texture mHeartEmptyTexture;
    sf::Sprite  mHeartEmptySprite;
    bool        mHeartLoaded = false;
    bool        mHeartHalfLoaded = false;
private:
    void setupText(sf::Text& text, unsigned int size, float xRatio, float yRatio, sf::Vector2f& centerOut);
    std::string formatTime(float seconds) const;

public:
    HUD();

    bool isLoaded() const;

    void update(int level, int score, float timeSeconds);
    void draw(sf::RenderWindow& window);
    void drawStats(sf::RenderWindow& window, const PlayerStats& stats, int currentCoins = 0);
    void drawVsBotStats(sf::RenderWindow& window, const PlayerStats& playerStats, const PlayerStats& botStats, BotDifficulty diff, float playerY, float botY);
    void drawPowerUpBuffs(sf::RenderWindow& window, float magnetRem, float timeStopRem,
                          float speedRem, float scoreX2Rem, bool hasShield);
    void setEndlessMode(bool isEndless, int wave = 1) { mIsEndless = isEndless; mEndlessWave = wave; }
    bool isEndlessMode() const { return mIsEndless; }

    void setTwoPlayerMode(bool is2P, int p1Wins = 0, int p2Wins = 0) {
        mIsTwoPlayer = is2P;
        mP1Wins = p1Wins;
        mP2Wins = p2Wins;
    }
    bool isTwoPlayerMode() const { return mIsTwoPlayer; }
    void drawTwoPlayerStats(sf::RenderWindow& window, const PlayerStats& p1Stats, const PlayerStats& p2Stats, int p1Score = 0, int p2Score = 0);

    void reloadHudBar(const std::string& hudPath);
    sf::FloatRect getPauseIconBounds() const;
private:
    bool mIsEndless = false;
    int  mEndlessWave = 1;
    bool mIsTwoPlayer = false;
    int  mP1Wins = 0;
    int  mP2Wins = 0;
};