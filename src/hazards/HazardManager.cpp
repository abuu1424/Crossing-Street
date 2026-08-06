#include "HazardManager.h"
#include "Utils.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

HazardManager::HazardManager() {
    // Generate 130 smooth circular dust specks
    mSandParticles.resize(130);
    for (auto& p : mSandParticles) {
        p.position = sf::Vector2f(static_cast<float>(rand() % Win_W), static_cast<float>(rand() % Win_H));
        p.speed = 360.f + static_cast<float>(rand() % 380);
        p.radius = 1.8f + static_cast<float>(rand() % 38) * 0.1f; // 1.8px - 5.6px smooth circular radius
        p.colorIdx = rand() % 4; // Desert palette colors
        p.alpha = 130.f + static_cast<float>(rand() % 110);
    }

    // Generate 35 pixel art wind gust streaks
    mWindStreaks.resize(35);
    for (auto& w : mWindStreaks) {
        w.position = sf::Vector2f(static_cast<float>(rand() % Win_W), static_cast<float>(rand() % Win_H));
        w.speed = 480.f + static_cast<float>(rand() % 450);
        w.scaleX = 0.8f + static_cast<float>(rand() % 12) * 0.1f;
        w.scaleY = 0.6f + static_cast<float>(rand() % 6) * 0.1f;
        w.alpha = 120.f + static_cast<float>(rand() % 95);
    }
}

void HazardManager::init(const sf::Font& font) {
    mFont = font;

    // Warning Banner - Positioned cleanly below HUD at y = 120.f
    mWarningBox.setSize(sf::Vector2f(540.f, 55.f));
    mWarningBox.setFillColor(sf::Color(190, 25, 25, 235));
    mWarningBox.setOutlineColor(sf::Color(255, 215, 0));
    mWarningBox.setOutlineThickness(3.5f);
    mWarningBox.setOrigin(270.f, 27.5f);
    mWarningBox.setPosition(Win_W / 2.f, 120.f);

    mWarningText.setFont(mFont);
    mWarningText.setCharacterSize(22);
    mWarningText.setFillColor(sf::Color::White);
    mWarningText.setOutlineColor(sf::Color::Black);
    mWarningText.setOutlineThickness(2.f);

    // Dense Sandstorm Blinding Overlay
    mSandOverlay.setSize(sf::Vector2f(static_cast<float>(Win_W), static_cast<float>(Win_H)));
    mSandOverlay.setFillColor(sf::Color(180, 120, 25, 175));

    // Rush Hour Banner
    mRushHourBanner.setSize(sf::Vector2f(static_cast<float>(Win_W), 40.f));
    mRushHourBanner.setFillColor(sf::Color(200, 30, 30, 200));
    mRushHourBanner.setPosition(0.f, 0.f);

    mRushHourText.setFont(mFont);
    mRushHourText.setString("<<< WARNING: RUSH HOUR TRAFFIC - 2X VEHICLE SPEED! >>>");
    mRushHourText.setCharacterSize(20);
    mRushHourText.setFillColor(sf::Color(255, 230, 0));
    sf::FloatRect rb = mRushHourText.getLocalBounds();
    mRushHourText.setOrigin(rb.left + rb.width / 2.f, rb.top + rb.height / 2.f);
    mRushHourText.setPosition(Win_W / 2.f, 20.f);

    // Generate Procedural 2D Pixel Art Sandstorm Wind Gust Asset (128x32)
    sf::RenderTexture windRt;
    if (windRt.create(128, 32)) {
        windRt.clear(sf::Color::Transparent);

        // Main Pixelated Tapered Wind Line 1
        sf::RectangleShape line1(sf::Vector2f(96.f, 4.f));
        line1.setPosition(8.f, 6.f);
        line1.setFillColor(sf::Color(255, 215, 110, 220));
        windRt.draw(line1);

        sf::RectangleShape line1_tip(sf::Vector2f(16.f, 2.f));
        line1_tip.setPosition(104.f, 7.f);
        line1_tip.setFillColor(sf::Color(255, 215, 110, 160));
        windRt.draw(line1_tip);

        // Secondary Wispy Wind Line 2
        sf::RectangleShape line2(sf::Vector2f(64.f, 3.f));
        line2.setPosition(24.f, 18.f);
        line2.setFillColor(sf::Color(240, 175, 75, 190));
        windRt.draw(line2);

        sf::RectangleShape line2_tip(sf::Vector2f(12.f, 2.f));
        line2_tip.setPosition(88.f, 18.5f);
        line2_tip.setFillColor(sf::Color(240, 175, 75, 140));
        windRt.draw(line2_tip);

        // Small Pixel Particles inside wind gust
        sf::RectangleShape p1(sf::Vector2f(4.f, 2.f));
        p1.setPosition(116.f, 7.f);
        p1.setFillColor(sf::Color(255, 235, 150, 200));
        windRt.draw(p1);

        windRt.display();
        mWindTexture = windRt.getTexture();
        mWindTexture.setSmooth(false); // Crisp Pixel Art Filter!

        sf::Image windImg = mWindTexture.copyToImage();
        windImg.saveToFile("assets/hazards/lv2/sand_wind.png");
    }

    // Load Dinosaur & Mammoth textures for Map 1 Stampede
    mDinoTextures.clear();
    const std::vector<std::string> paths = {
        "assets/sprites/lv1_sp/dino/dino1.png",
        "assets/sprites/lv1_sp/dino/dino2_trai.png",
        "assets/sprites/lv1_sp/dino/dino3_trai.png",
        "assets/sprites/lv1_sp/mammoth/MAMUT.png"
    };

    for (const auto& path : paths) {
        sf::Texture tex;
        if (tex.loadFromFile(path)) {
            mDinoTextures.push_back(tex);
        }
    }

    if (mDinoTextures.empty()) {
        sf::Image img;
        img.create(64, 64, sf::Color::Red);
        sf::Texture fallback;
        fallback.loadFromImage(img);
        mDinoTextures.push_back(fallback);
    }

    // Load Arrow Texture for Level 3
    if (!mArrowTexture.loadFromFile("assets/sprites/lv3_sp/animal_lv3/muiten.png")) {
        sf::Image img;
        img.create(64, 64, sf::Color::Yellow);
        mArrowTexture.loadFromImage(img);
    }

    // Generate Procedural 100% Transparent Medieval Target Reticle Texture
    sf::RenderTexture rt;
    if (rt.create(128, 128)) {
        rt.clear(sf::Color::Transparent);

        // 1. Outer Gold Ring
        sf::CircleShape outerRing(54.f);
        outerRing.setOrigin(54.f, 54.f);
        outerRing.setPosition(64.f, 64.f);
        outerRing.setFillColor(sf::Color::Transparent);
        outerRing.setOutlineColor(sf::Color(255, 205, 30, 240));
        outerRing.setOutlineThickness(4.f);
        rt.draw(outerRing);

        // 2. Dark Iron Accent Ring
        sf::CircleShape ironRing(42.f);
        ironRing.setOrigin(42.f, 42.f);
        ironRing.setPosition(64.f, 64.f);
        ironRing.setFillColor(sf::Color(30, 20, 15, 160));
        ironRing.setOutlineColor(sf::Color(220, 140, 30, 220));
        ironRing.setOutlineThickness(3.f);
        rt.draw(ironRing);

        // 3. Inner Crimson Bullseye Zone
        sf::CircleShape crimsonZone(26.f);
        crimsonZone.setOrigin(26.f, 26.f);
        crimsonZone.setPosition(64.f, 64.f);
        crimsonZone.setFillColor(sf::Color(220, 35, 35, 210));
        crimsonZone.setOutlineColor(sf::Color(255, 230, 80, 230));
        crimsonZone.setOutlineThickness(2.f);
        rt.draw(crimsonZone);

        // 4. Gold Crosshair Lines (+)
        sf::RectangleShape lineH(sf::Vector2f(88.f, 4.f));
        lineH.setOrigin(44.f, 2.f);
        lineH.setPosition(64.f, 64.f);
        lineH.setFillColor(sf::Color(255, 220, 40, 230));

        sf::RectangleShape lineV(sf::Vector2f(4.f, 88.f));
        lineV.setOrigin(2.f, 44.f);
        lineV.setPosition(64.f, 64.f);
        lineV.setFillColor(sf::Color(255, 220, 40, 230));

        rt.draw(lineH);
        rt.draw(lineV);

        // 5. Bright Center Spot
        sf::CircleShape centerSpot(7.f);
        centerSpot.setOrigin(7.f, 7.f);
        centerSpot.setPosition(64.f, 64.f);
        centerSpot.setFillColor(sf::Color(255, 255, 255));
        rt.draw(centerSpot);

        rt.display();
        mReticleTexture = rt.getTexture();
        mReticleTexture.setSmooth(true);

        sf::Image cleanImg = mReticleTexture.copyToImage();
        cleanImg.saveToFile("assets/hazards/lv3/target_reticle.png");
    }

    // Generate High-Quality Cyberpunk Plasma Laser Beam Asset for Level 5
    sf::RenderTexture laserRt;
    if (laserRt.create(512, 32)) {
        laserRt.clear(sf::Color::Transparent);

        sf::RectangleShape outerGlow(sf::Vector2f(512.f, 32.f));
        outerGlow.setFillColor(sf::Color(0, 230, 255, 110));
        laserRt.draw(outerGlow);

        sf::RectangleShape innerBeam(sf::Vector2f(512.f, 16.f));
        innerBeam.setPosition(0.f, 8.f);
        innerBeam.setFillColor(sf::Color(255, 40, 180, 200));
        laserRt.draw(innerBeam);

        sf::RectangleShape coreLine(sf::Vector2f(512.f, 6.f));
        coreLine.setPosition(0.f, 13.f);
        coreLine.setFillColor(sf::Color(255, 255, 255, 255));
        laserRt.draw(coreLine);

        laserRt.display();
        mLaserTexture = laserRt.getTexture();
        mLaserTexture.setSmooth(true);

        sf::Image laserImg = mLaserTexture.copyToImage();
        laserImg.saveToFile("assets/hazards/lv5/laser_beam.png");
    }
}

void HazardManager::startLevel(int levelNumber) {
    mCurrentLevel = levelNumber;
    reset();

    switch (levelNumber) {
    case 1: mCurrentHazard = HazardType::DINO_STAMPEDE; break;
    case 2: mCurrentHazard = HazardType::SANDSTORM; break;
    case 3: mCurrentHazard = HazardType::ARROW_RAIN; break;
    case 4: mCurrentHazard = HazardType::RUSH_HOUR; break;
    case 5: mCurrentHazard = HazardType::LASER_SCAN; break;
    default: mCurrentHazard = HazardType::NONE; break;
    }
}

void HazardManager::reset() {
    mCooldownTimer = 6.0f + static_cast<float>(rand() % 4);
    mWarningTimer = 0.0f;
    mActiveTimer = 0.0f;
    mIsWarning = false;
    mIsActive = false;
    mShakeOffset = { 0.f, 0.f };
    mWindDrift = { 0.f, 0.f };
    mSpeedMultiplier = 1.0f;
    mStampedeProgress = 0.f;
    mStampedeDinos.clear();
    mArrowTargets.clear();
    mLaserLanes.clear();
}

void HazardManager::triggerHazard() {
    mIsActive = true;
    mIsWarning = false;
    mActiveTimer = 5.0f + static_cast<float>(rand() % 3);

    if (mCurrentHazard == HazardType::DINO_STAMPEDE) {
        mStampedeProgress = -200.f;
        mStampedeDinos.clear();

        const float lanes[2] = { 180.f, 315.f };
        for (int l = 0; l < 2; ++l) {
            float laneY = lanes[l];
            for (int i = 0; i < 4; ++i) {
                StampedeDino dino;
                dino.textureIdx = rand() % mDinoTextures.size();
                dino.xOffset = i * 220.f + static_cast<float>(rand() % 40);
                dino.laneY = laneY;
                dino.scale = 2.8f + static_cast<float>(rand() % 4) * 0.1f;
                mStampedeDinos.push_back(dino);
            }
        }
    } else if (mCurrentHazard == HazardType::ARROW_RAIN) {
        mArrowTargets.clear();
        const std::vector<float> validLanes = { 150.f, 215.f, 270.f, 380.f, 452.f, 505.f };
        const float xZones[4] = { 220.f, 480.f, 750.f, 1020.f };

        for (int i = 0; i < 4; ++i) {
            ArrowTarget target;
            float targetLaneY = validLanes[(i * 2 + rand() % 2) % validLanes.size()];
            float targetX = xZones[i] + static_cast<float>((rand() % 100) - 50);

            target.position = sf::Vector2f(targetX, targetLaneY + 20.f);
            target.warningTimer = 1.8f;
            target.maxWarningTime = 1.8f;
            target.active = true;
            target.struck = false;
            target.arrowY = target.position.y - 350.f;
            mArrowTargets.push_back(target);
        }
    } else if (mCurrentHazard == HazardType::LASER_SCAN) {
        mLaserLanes.clear();
        const std::vector<float> playableLanes = { 140.f, 180.f, 250.f, 300.f, 400.f, 505.f };

        int count = 2 + (rand() % 2);
        std::vector<float> shuffledLanes = playableLanes;
        for (size_t i = 0; i < shuffledLanes.size(); ++i) {
            size_t j = rand() % shuffledLanes.size();
            std::swap(shuffledLanes[i], shuffledLanes[j]);
        }

        for (int i = 0; i < count; ++i) {
            LaserLane laser;
            laser.laneY = shuffledLanes[i];
            laser.warningTimer = 1.2f + static_cast<float>(rand() % 6) * 0.1f;
            laser.maxWarningTime = laser.warningTimer;
            laser.activeTimer = 3.5f + static_cast<float>(rand() % 20) * 0.1f;
            laser.active = true;
            mLaserLanes.push_back(laser);
        }
    }
}

void HazardManager::endHazard() {
    mIsActive = false;
    mIsWarning = false;
    mCooldownTimer = 6.0f + static_cast<float>(rand() % 5);
    mShakeOffset = { 0.f, 0.f };
    mWindDrift = { 0.f, 0.f };
    mSpeedMultiplier = 1.0f;
    mStampedeDinos.clear();
    mArrowTargets.clear();
    mLaserLanes.clear();
}

void HazardManager::updateParticles(float dt) {
    // Update circular dust specks
    for (auto& p : mSandParticles) {
        p.position.x += p.speed * dt;
        p.position.y += static_cast<float>(rand() % 3 - 1) * 7.f * dt;

        if (p.position.x > Win_W + 20.f) {
            p.position.x = -20.f;
            p.position.y = static_cast<float>(rand() % Win_H);
        }
    }

    // Update pixel art wind streaks
    for (auto& w : mWindStreaks) {
        w.position.x += w.speed * dt;
        if (w.position.x > Win_W + 150.f) {
            w.position.x = -150.f;
            w.position.y = static_cast<float>(rand() % Win_H);
        }
    }
}

void HazardManager::update(float dt, const sf::Vector2f& playerPos, std::vector<std::pair<sf::FloatRect, float>>& extraObstacleHitboxes) {
    mPlayerPos = playerPos;

    if (mCurrentHazard == HazardType::NONE) return;

    // State machine: Cooldown -> Warning -> Active -> Cooldown
    if (!mIsWarning && !mIsActive) {
        mCooldownTimer -= dt;
        if (mCooldownTimer <= 2.5f) {
            mIsWarning = true;
            mWarningTimer = 2.5f;

            std::string warnStr = "HAZARD WARNING!";
            if (mCurrentHazard == HazardType::DINO_STAMPEDE) warnStr = "WARNING: 2-LANE DINO STAMPEDE APPROACHING!";
            else if (mCurrentHazard == HazardType::SANDSTORM) warnStr = "WARNING: BLINDING SANDSTORM & HIGH WINDS!";
            else if (mCurrentHazard == HazardType::ARROW_RAIN) warnStr = "WARNING: ARCHERS AIMING - ARROW VOLLEY!";
            else if (mCurrentHazard == HazardType::RUSH_HOUR) warnStr = "WARNING: RUSH HOUR - TRAFFIC SPEEDUP!";
            else if (mCurrentHazard == HazardType::LASER_SCAN) warnStr = "WARNING: RANDOM LANE LASER BEAMS CHARGING!";

            mWarningText.setString(warnStr);
            sf::FloatRect bounds = mWarningText.getLocalBounds();
            mWarningText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            mWarningText.setPosition(Win_W / 2.f, 120.f);
        }
    } else if (mIsWarning) {
        mWarningTimer -= dt;
        if (mWarningTimer <= 0.f) {
            triggerHazard();
        }
    } else if (mIsActive) {
        mActiveTimer -= dt;
        if (mActiveTimer <= 0.f) {
            endHazard();
            return;
        }

        // Active Hazard Logic
        switch (mCurrentHazard) {
        case HazardType::DINO_STAMPEDE: {
            float shakeMag = 7.f;
            mShakeOffset.x = (static_cast<float>(rand() % 100) / 100.f - 0.5f) * shakeMag * 2.f;
            mShakeOffset.y = (static_cast<float>(rand() % 100) / 100.f - 0.5f) * shakeMag * 2.f;

            mStampedeProgress += 850.f * dt;
            for (const auto& dino : mStampedeDinos) {
                float posX = mStampedeProgress - dino.xOffset;
                float dinoWidth = 140.f * (dino.scale / 3.0f);
                float dinoHeight = 90.f * (dino.scale / 3.0f);

                sf::FloatRect dinoHitbox(posX, dino.laneY, dinoWidth, dinoHeight);
                extraObstacleHitboxes.push_back({ dinoHitbox, dino.laneY });
            }
            break;
        }
        case HazardType::SANDSTORM: {
            updateParticles(dt);
            mWindDrift = sf::Vector2f(42.f * dt, 0.f);
            break;
        }
        case HazardType::ARROW_RAIN: {
            for (auto& target : mArrowTargets) {
                if (!target.active) continue;

                if (target.warningTimer > 0.f) {
                    target.warningTimer -= dt;
                } else {
                    if (target.arrowY < target.position.y) {
                        target.arrowY += 1200.f * dt;
                        if (target.arrowY >= target.position.y) {
                            target.arrowY = target.position.y;
                            target.struck = true;
                        }
                    }

                    sf::FloatRect arrowHitbox(target.position.x - 18.f, target.position.y - 18.f, 36.f, 36.f);
                    extraObstacleHitboxes.push_back({ arrowHitbox, target.position.y });
                }
            }
            break;
        }
        case HazardType::RUSH_HOUR: {
            mSpeedMultiplier = 2.2f;
            break;
        }
        case HazardType::LASER_SCAN: {
            for (auto& laser : mLaserLanes) {
                if (!laser.active) continue;

                if (laser.warningTimer > 0.f) {
                    laser.warningTimer -= dt;
                } else {
                    laser.activeTimer -= dt;
                    if (laser.activeTimer <= 0.f) {
                        laser.active = false;
                    } else {
                        sf::FloatRect laserBox(0.f, laser.laneY + 20.f, static_cast<float>(Win_W), 30.f);
                        extraObstacleHitboxes.push_back({ laserBox, laser.laneY });
                    }
                }
            }
            break;
        }
        default: break;
        }
    }
}

void HazardManager::draw(sf::RenderWindow& window) const {
    if (mCurrentHazard == HazardType::NONE) return;

    // Draw Map 2 Desert Sandstorm with Smooth Circular Dust & Pixel Art Wind Gust Sprites
    if (mCurrentHazard == HazardType::SANDSTORM && (mIsActive || mIsWarning)) {
        float alphaPulse = mIsActive ? (140.f + std::sin(mActiveTimer * 4.f) * 25.f) : 90.f;
        sf::RectangleShape sandFog(sf::Vector2f(static_cast<float>(Win_W), static_cast<float>(Win_H)));
        sandFog.setFillColor(sf::Color(210, 145, 35, static_cast<sf::Uint8>(alphaPulse)));
        window.draw(sandFog);

        // Desert Palette Colors
        const sf::Color desertPalette[4] = {
            sf::Color(255, 235, 150), // 0: Sunlit Sand Speck
            sf::Color(245, 194, 82),  // 1: Desert Gold Speck
            sf::Color(224, 154, 50),  // 2: Ocher Sand Grain
            sf::Color(199, 117, 36)   // 3: Terracotta Dust
        };

        // 1. Render 130 Smooth Circular Dust Specks
        for (const auto& p : mSandParticles) {
            sf::CircleShape dustDot(p.radius);
            dustDot.setOrigin(p.radius, p.radius);
            dustDot.setPosition(p.position);

            sf::Color col = desertPalette[p.colorIdx % 4];
            col.a = static_cast<sf::Uint8>(p.alpha);
            dustDot.setFillColor(col);
            window.draw(dustDot);
        }

        // 2. Render 35 Pixel Art Wind Gust Sprites (Texture Asset: assets/hazards/lv2/sand_wind.png)
        if (mWindTexture.getSize().x > 0) {
            sf::Sprite windSpr;
            windSpr.setTexture(mWindTexture);

            for (const auto& w : mWindStreaks) {
                windSpr.setPosition(w.position);
                windSpr.setScale(w.scaleX, w.scaleY);
                windSpr.setColor(sf::Color(255, 220, 130, static_cast<sf::Uint8>(w.alpha)));
                window.draw(windSpr);
            }
        }
    }

    // Draw Sci-Fi Per-Lane Laser Beams & Warning Lines
    if (mCurrentHazard == HazardType::LASER_SCAN && mIsActive) {
        for (const auto& laser : mLaserLanes) {
            if (!laser.active) continue;

            if (laser.warningTimer > 0.f) {
                float flash = (std::sin(laser.warningTimer * 20.0f) + 1.f) * 0.5f;
                sf::RectangleShape warnLine(sf::Vector2f(static_cast<float>(Win_W), 4.f));
                warnLine.setPosition(0.f, laser.laneY + 30.f);
                warnLine.setFillColor(sf::Color(255, 30, 30, static_cast<sf::Uint8>(120 + flash * 135)));
                window.draw(warnLine);
            } else {
                if (mLaserTexture.getSize().x > 0) {
                    sf::Sprite laserSpr;
                    laserSpr.setTexture(mLaserTexture);
                    sf::Vector2u sz = mLaserTexture.getSize();
                    laserSpr.setScale(static_cast<float>(Win_W) / sz.x, 32.f / sz.y);
                    laserSpr.setPosition(0.f, laser.laneY + 16.f);

                    float pulseAlpha = 200.f + 55.f * std::sin(laser.activeTimer * 12.0f);
                    laserSpr.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(pulseAlpha)));
                    window.draw(laserSpr);
                }
            }
        }
    }

    // Draw 2-Lane Full-Sized Stampede Dinosaurs & Mammoths
    if (mCurrentHazard == HazardType::DINO_STAMPEDE && mIsActive) {
        for (const auto& dino : mStampedeDinos) {
            if (dino.textureIdx >= 0 && dino.textureIdx < static_cast<int>(mDinoTextures.size())) {
                sf::Sprite spr;
                spr.setTexture(mDinoTextures[dino.textureIdx]);

                sf::Vector2u texSize = mDinoTextures[dino.textureIdx].getSize();
                if (texSize.x >= 128 && texSize.y >= 64) {
                    spr.setTextureRect(sf::IntRect(0, 0, 64, 64));
                }

                spr.setScale(dino.scale, dino.scale);
                spr.setPosition(mStampedeProgress - dino.xOffset, dino.laneY);
                window.draw(spr);
            }
        }
    }

    // Draw Medieval Archer Target Reticles & Single Arrow Drop per Target
    if (mCurrentHazard == HazardType::ARROW_RAIN && mIsActive) {
        for (const auto& target : mArrowTargets) {
            if (!target.active) continue;

            if (mReticleTexture.getSize().x > 0) {
                sf::Sprite reticleSpr;
                reticleSpr.setTexture(mReticleTexture);
                sf::Vector2u sz = mReticleTexture.getSize();
                reticleSpr.setOrigin(sz.x / 2.f, sz.y / 2.f);
                reticleSpr.setPosition(target.position);

                float targetWidth = 65.f;
                float baseScale = targetWidth / static_cast<float>(sz.x);
                float scalePulse = baseScale * (0.9f + 0.15f * std::sin(target.warningTimer * 8.0f));
                reticleSpr.setScale(scalePulse, scalePulse);
                window.draw(reticleSpr);
            }

            float progressRatio = target.warningTimer / target.maxWarningTime;
            if (target.warningTimer > 0.f) {
                float currentRadius = 6.f + progressRatio * 24.f;
                sf::CircleShape countdownRing(currentRadius);
                countdownRing.setOrigin(currentRadius, currentRadius);
                countdownRing.setPosition(target.position);
                countdownRing.setFillColor(sf::Color::Transparent);
                countdownRing.setOutlineColor(sf::Color(255, 230, 50, 230));
                countdownRing.setOutlineThickness(2.f);
                window.draw(countdownRing);
            }

            if (target.warningTimer <= 0.f) {
                sf::Sprite arrowSpr;
                arrowSpr.setTexture(mArrowTexture);
                arrowSpr.setTextureRect(sf::IntRect(0, 0, 64, 64));
                arrowSpr.setOrigin(52.f, 32.f);
                arrowSpr.setRotation(90.f);
                arrowSpr.setScale(1.2f, 1.2f);
                arrowSpr.setPosition(target.position.x, target.arrowY);
                window.draw(arrowSpr);
            }
        }
    }
}

void HazardManager::drawUI(sf::RenderWindow& window) const {
    if (mCurrentHazard == HazardType::NONE) return;

    if (mIsWarning) {
        window.draw(mWarningBox);
        window.draw(mWarningText);
    }

    if (mCurrentHazard == HazardType::RUSH_HOUR && mIsActive) {
        window.draw(mRushHourBanner);
        window.draw(mRushHourText);
    }
}
