#include "HazardManager.h"
#include "SoundManager.h"
#include "Utils.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

// =========================================================
// Constructor – init sandstorm particles & rain drops
// =========================================================
HazardManager::HazardManager() {
    mSandParticles.resize(130);
    for (auto& p : mSandParticles) {
        p.position = { (float)(rand() % Win_W), (float)(rand() % Win_H) };
        p.speed    = 360.f + (float)(rand() % 380);
        p.radius   = 1.8f  + (float)(rand() % 38) * 0.1f;
        p.colorIdx = rand() % 4;
        p.alpha    = 130.f + (float)(rand() % 110);
    }
    mWindStreaks.resize(35);
    for (auto& w : mWindStreaks) {
        w.position = { (float)(rand() % Win_W), (float)(rand() % Win_H) };
        w.speed  = 480.f + (float)(rand() % 450);
        w.scaleX = 0.8f  + (float)(rand() % 12) * 0.1f;
        w.scaleY = 0.6f  + (float)(rand() % 6)  * 0.1f;
        w.alpha  = 120.f + (float)(rand() % 95);
    }
    mRainDrops.resize(120);
    for (auto& r : mRainDrops) {
        r.position = { (float)(rand() % Win_W), (float)(rand() % Win_H) };
        r.speed    = 650.f + (float)(rand() % 350);
        r.length   = 14.f  + (float)(rand() % 16);
    }
    mBlackHoleParticles.resize(90);
    const sf::Color bhColors[] = {
        sf::Color(0, 220, 255),
        sf::Color(30, 140, 255),
        sf::Color(100, 210, 255),
        sf::Color(0, 180, 255)
    };
    for (auto& bp : mBlackHoleParticles) {
        bp.angle  = (float)(rand() % 360) * 3.14159f / 180.f;
        bp.dist   = 20.f + (float)(rand() % 240);
        bp.speed  = 1.5f + (float)(rand() % 25) * 0.1f;
        bp.radius = 1.5f + (float)(rand() % 25) * 0.1f;
        bp.color  = bhColors[rand() % 4];
    }
}

// =========================================================
// init – load textures and build procedural assets
// =========================================================
void HazardManager::init(const sf::Font& font, SoundManager* sound) {
    mFont  = font;
    mSound = sound;

    // Warning banner
    mWarningBox.setFillColor(sf::Color(190, 25, 25, 235));
    mWarningBox.setOutlineColor(sf::Color(255, 215, 0));
    mWarningBox.setOutlineThickness(3.f);
    mWarningText.setFont(mFont);
    mWarningText.setCharacterSize(22);
    mWarningText.setFillColor(sf::Color::White);
    mWarningText.setOutlineColor(sf::Color::Black);
    mWarningText.setOutlineThickness(2.f);

    // Level 4: Thundercloud & Lightning (12-Frame Animations)
    mThundercloudAnimTexture.loadFromFile("assets/hazards/lv4/thundercloud_anim.png");
    mThundercloudAnimTexture.setSmooth(false);
    mLightningAnimTexture.loadFromFile("assets/hazards/lv4/lightning_bolt_anim.png");
    mLightningAnimTexture.setSmooth(false);
    mLightningWarningAnimTexture.loadFromFile("assets/hazards/lv4/lightning_warning_anim.png");
    mLightningWarningAnimTexture.setSmooth(false);

    // Level 1: 12-frame warning sign & lane warning animations
    mDinoWarningSignAnimTexture.loadFromFile("assets/hazards/lv1/dino_warning_sign_anim.png");
    mDinoWarningSignAnimTexture.setSmooth(false);
    mDinoLaneWarningAnimTexture.loadFromFile("assets/hazards/lv1/dino_lane_warning_anim.png");
    mDinoLaneWarningAnimTexture.setSmooth(false);

    // Lv1 dino sprites – 128×128 sheet, 2×2 grid = 64×64/frame
    mDinoTextures.clear();
    mDinoFrameData.clear();
    const std::vector<std::string> dinoPaths = {
        "assets/sprites/lv1_sp/dino/dino1.png",      // idx 0  dir=+1
        "assets/sprites/lv1_sp/dino/dino2_trai.png", // idx 1  dir=-1
        "assets/sprites/lv1_sp/mammoth/MAMUT.png",   // idx 2  dir=+1
        "assets/sprites/lv1_sp/dino/dino3_trai.png", // idx 3  dir=-1
    };
    for (const auto& p : dinoPaths) {
        sf::Texture tex;
        if (tex.loadFromFile(p)) {
            mDinoTextures.push_back(tex);
            mDinoFrameData.push_back({4, 64, 64});
        } else {
            sf::Image img; img.create(64,64,sf::Color::Red);
            sf::Texture ft; ft.loadFromImage(img);
            mDinoTextures.push_back(ft);
            mDinoFrameData.push_back({1,64,64});
        }
    }

    // Lv2 – 12-frame sandstorm animation
    mSandstormAnimTexture.loadFromFile("assets/hazards/lv2/sandstorm_anim.png");
    mSandstormAnimTexture.setSmooth(false);

    // Lv3 – arrow texture + 12-frame target reticle animation
    if (!mArrowTexture.loadFromFile("assets/sprites/lv3_sp/animal_lv3/muiten.png")) {
        sf::Image img; img.create(64, 64, sf::Color::Yellow); mArrowTexture.loadFromImage(img);
    }
    mReticleAnimTexture.loadFromFile("assets/hazards/lv3/target_reticle_anim.png");
    mReticleAnimTexture.setSmooth(false);

    // Lv5 – laser beam sprite & black hole sprite
    mLaserAnimTexture.loadFromFile("assets/hazards/lv5/laser_beam_anim.png");
    mLaserAnimTexture.setSmooth(false);
    mBlackHoleAnimTexture.loadFromFile("assets/hazards/lv5/black_hole_anim.png");
    mBlackHoleAnimTexture.setSmooth(false);
    mBlackHoleRingAnimTexture.loadFromFile("assets/hazards/lv5/black_hole_ring_anim.png");
    mBlackHoleRingAnimTexture.setSmooth(false);
}

// =========================================================
// startLevel / reset
// =========================================================
void HazardManager::startLevel(int level) {
    mCurrentLevel = level;
    reset();
    switch (level) {
    case 1: mCurrentHazard = HazardType::DINO_STAMPEDE; break;
    case 2: mCurrentHazard = HazardType::SANDSTORM;     break;
    case 3: mCurrentHazard = HazardType::ARROW_RAIN;    break;
    case 4: mCurrentHazard = HazardType::RUSH_HOUR;     break;
    case 5: mCurrentHazard = HazardType::BLACK_HOLE;    break;
    default: mCurrentHazard = HazardType::NONE;         break;
    }
}

void HazardManager::reset() {
    if (mSound) mSound->stopHazardSounds();
    mCooldownTimer    = 6.f + (float)(rand() % 4);
    mWarningTimer     = 0.f;
    mActiveTimer      = 0.f;
    mIsWarning        = false;
    mIsActive         = false;
    mShakeOffset      = {0.f, 0.f};
    mWindDrift        = {0.f, 0.f};
    mSpeedMultiplier  = 1.f;
    mHerdLanes.clear();
    mArrowTargets.clear();
    mLaserLanes.clear();
    mLightningStrikes.clear();
    mBlackHoles.clear();
    mCloudPosX          = 0.f;
    mLightningTimer     = 0.f;
}

// =========================================================
// triggerHazard – called when warning expires
// =========================================================
void HazardManager::triggerHazard() {
    mIsActive  = true;
    mIsWarning = false;
    mActiveTimer = 7.f + (float)(rand() % 3);

    // --- Level 1: Herd Stampede ---
    if (mCurrentHazard == HazardType::DINO_STAMPEDE) {
        mHerdLanes.clear();

        struct LI { float laneY; int dir, tex; };
        const LI all[] = {{180.f,1,0},{315.f,-1,1},{270.f,1,2},{405.f,-1,3}};

        int idx[4] = {0,1,2,3};
        for (int i=3;i>0;--i) std::swap(idx[i], idx[rand()%(i+1)]);

        for (int pick=0; pick<2; ++pick) {
            const LI& li = all[idx[pick]];
            HerdLane herd;
            herd.laneY       = li.laneY;
            herd.direction   = li.dir;
            herd.warningTimer= 1.5f + (float)(rand()%5)*0.1f;
            herd.maxWarningTime = herd.warningTimer;
            herd.active      = true;

            int count   = 10 + rand()%4;
            float gap   = 200.f;
            for (int a=0; a<count; ++a) {
                HerdAnimal an;
                an.laneY     = li.laneY;
                an.direction = li.dir;
                an.speed     = 400.f + (float)(rand()%80);
                an.textureIdx= li.tex;
                an.scale     = (li.tex==2) ? 2.5f : 3.0f;
                if (li.tex < (int)mDinoFrameData.size()) {
                    an.frameCount = mDinoFrameData[li.tex].frameCount;
                    an.frameW     = mDinoFrameData[li.tex].frameW;
                    an.frameH     = mDinoFrameData[li.tex].frameH;
                } else { an.frameCount=1; an.frameW=64; an.frameH=64; }
                an.posX = (li.dir==1) ? -80.f - a*gap : (float)Win_W + 80.f + a*gap;
                herd.animals.push_back(an);
            }
            mHerdLanes.push_back(herd);
        }

    // --- Level 2: Sandstorm ---
    } else if (mCurrentHazard == HazardType::SANDSTORM) {
        mSandParticles.clear();
        for (int i = 0; i < 110; ++i) {
            SandParticle p;
            p.position = { (float)(rand() % Win_W), (float)(rand() % Win_H) };
            p.speed    = 180.f + (float)(rand() % 140);
            p.radius   = 1.0f + (float)(rand() % 8) * 0.12f; // Fine sand specks (1.0 - 1.9px)
            p.alpha    = 110.f + (float)(rand() % 110);
            p.colorIdx = rand() % 4;
            mSandParticles.push_back(p);
        }
        if (mSound) mSound->playSandstorm();

    // --- Level 3: Arrow Rain ---
    } else if (mCurrentHazard == HazardType::ARROW_RAIN) {
        mArrowTargets.clear();
        mArrowSpawnTimer = 0.6f;

        const float lanes[] = {150.f, 215.f, 270.f, 380.f, 452.f, 505.f};
        int numInitial = 5 + (rand() % 2); // 5 to 6 initial targets (~30% reduction)
        for (int i = 0; i < numInitial; ++i) {
            ArrowTarget t;
            float tx = 140.f + (float)(rand() % (Win_W - 280));
            float ty = lanes[i % 6];
            t.position = { tx, ty };
            t.warningTimer = t.maxWarningTime = 1.3f + (float)(rand() % 4) * 0.15f;
            t.arrowY       = t.position.y - 350.f;
            t.active       = true;
            t.struck       = false;
            t.soundPlayed  = false;
            t.animTimer    = (float)(rand() % 12) * 0.08f;
            t.currentFrame = rand() % 12;
            mArrowTargets.push_back(t);
        }

    // --- Level 4: Thundercloud & Heavy Rain ---
    } else if (mCurrentHazard == HazardType::RUSH_HOUR) {
        mLightningStrikes.clear();
        mCloudPosX      = 0.f;
        mLightningTimer = 0.4f; // start first lightning warnings quickly
        if (mSound) mSound->playRushHour();

    // --- Level 5: 3-4 Small 12-Frame Black Holes & Laser Beams ---
    } else if (mCurrentHazard == HazardType::BLACK_HOLE) {
        mBlackHoles.clear();
        mLaserLanes.clear();

        const float playableLanes[] = { 180.f, 260.f, 340.f, 420.f, 505.f };
        int count = 3 + (rand() % 2); // 3 or 4 Black Holes

        for (int i = 0; i < count; ++i) {
            BlackHoleInstance bh;
            float bx = 160.f + (float)(rand() % 960);
            float by = playableLanes[i % 5];
            bh.position = { bx, by };
            bh.animTimer = (float)(rand() % 12) * 0.08f;
            bh.currentFrame = rand() % 12;
            mBlackHoles.push_back(bh);
        }

        // Spawn 2 Laser Beams on playable lanes
        int pick1 = rand() % 5;
        int pick2 = (pick1 + 2) % 5;
        for (int lIdx : { pick1, pick2 }) {
            LaserLane l;
            l.laneY = playableLanes[lIdx];
            l.warningTimer = l.maxWarningTime = 1.0f;
            l.activeTimer = 4.5f;
            l.active = true;
            l.soundPlayed = false;
            mLaserLanes.push_back(l);
        }
        if (mSound) mSound->playLaserBeam();
    }
}

// =========================================================
// endHazard
// =========================================================
void HazardManager::endHazard() {
    if (mSound) mSound->stopHazardSounds();
    mIsActive = mIsWarning = false;
    mCooldownTimer   = 6.f + (float)(rand()%5);
    mShakeOffset     = {0.f,0.f};
    mWindDrift       = {0.f,0.f};
    mSpeedMultiplier = 1.f;
    mHerdLanes.clear();
    mArrowTargets.clear();
    mLaserLanes.clear();
    mLightningStrikes.clear();
    mLightningTimer = 0.f;
}

// =========================================================
// updateParticles (sandstorm)
// =========================================================
void HazardManager::updateParticles(float dt) {
    for (auto& p : mSandParticles) {
        p.position.x += p.speed * dt;
        p.position.y += (float)(rand()%3-1) * 7.f * dt;
        if (p.position.x > Win_W+20) { p.position.x=-20; p.position.y=(float)(rand()%Win_H); }
    }
    for (auto& w : mWindStreaks) {
        w.position.x += w.speed * dt;
        if (w.position.x > Win_W+150) { w.position.x=-150; w.position.y=(float)(rand()%Win_H); }
    }
}

// =========================================================
// update
// =========================================================
void HazardManager::update(float dt, const sf::Vector2f& playerPos,
                           std::vector<std::pair<sf::FloatRect,float>>& extraHitboxes) {
    mPlayerPos = playerPos;
    if (mCurrentHazard == HazardType::NONE) return;

    // State machine: Cooldown → Warning → Active → Cooldown
    if (!mIsWarning && !mIsActive) {
        mCooldownTimer -= dt;
        if (mCooldownTimer <= 2.5f) {
            mIsWarning    = true;
            mWarningTimer = 2.5f;

            std::string warnStr;
            switch (mCurrentHazard) {
            case HazardType::DINO_STAMPEDE: warnStr="! STAMPEDE - HERD CHARGING !"; if(mSound)mSound->playDinoRoar(); break;
            case HazardType::SANDSTORM:     warnStr="! SANDSTORM INCOMING !";        break;
            case HazardType::ARROW_RAIN:    warnStr="! ARROW VOLLEY - TAKE COVER !"; break;
            case HazardType::RUSH_HOUR:     warnStr="! THUNDERSTORM & HEAVY RAIN !"; if(mSound)mSound->playLightning(); break;
            case HazardType::BLACK_HOLE:    warnStr="! WARNING: BLACK HOLE GRAVITY SURGE !"; break;
            default:                        warnStr="! HAZARD WARNING !";            break;
            }
            mWarningText.setString(warnStr);
            sf::FloatRect b = mWarningText.getLocalBounds();
            mWarningText.setOrigin(b.left+b.width/2.f, b.top+b.height/2.f);
            mWarningText.setPosition(Win_W/2.f, 120.f);
            float bw=b.width+48.f, bh=b.height+28.f;
            mWarningBox.setSize({bw,bh});
            mWarningBox.setOrigin(bw/2.f,bh/2.f);
            mWarningBox.setPosition(Win_W/2.f,120.f);
        }
    } else if (mIsWarning) {
        mWarningTimer -= dt;

        // Smoothly update rain drops & cloud drift during warning phase for Lv4 Storm!
        if (mCurrentHazard == HazardType::RUSH_HOUR) {
            for (auto& r : mRainDrops) {
                r.position.y += r.speed * dt;
                r.position.x -= 70.f * dt;
                if (r.position.y > Win_H) {
                    r.position.y = -20.f;
                    r.position.x = (float)(rand() % (Win_W + 200));
                }
            }
            mCloudPosX += 30.f * dt;
            if (mCloudPosX > 120.f) mCloudPosX -= 120.f;
        }

        if (mWarningTimer <= 0.f) triggerHazard();

    } else if (mIsActive) {
        mActiveTimer -= dt;
        if (mActiveTimer <= 0.f) { endHazard(); return; }

        switch (mCurrentHazard) {

        // --- Lv1: Herd ---
        case HazardType::DINO_STAMPEDE: {
            float sh = 4.f;
            mShakeOffset = { ((float)(rand()%100)/100.f-0.5f)*sh*2, ((float)(rand()%100)/100.f-0.5f)*sh*2 };
            for (auto& herd : mHerdLanes) {
                if (!herd.active) continue;
                herd.animTimer += dt;
                herd.currentFrame = (int)(herd.animTimer * 12.0f) % 12;

                if (herd.warningTimer > 0.f) { herd.warningTimer -= dt; continue; }

                if (!herd.soundPlayed) {
                    herd.soundPlayed = true;
                    if (mSound) mSound->playStampede();
                }

                bool anyOn = false;
                for (auto& an : herd.animals) {
                    if (an.offScreen) continue;
                    an.posX += an.direction * an.speed * dt;
                    an.animTimer += dt;
                    if (an.animTimer >= 0.12f) { an.animTimer -= 0.12f; an.currentFrame=(an.currentFrame+1)%an.frameCount; }
                    if (an.direction==1 && an.posX > Win_W+100) { an.offScreen=true; continue; }
                    if (an.direction==-1 && an.posX < -100)     { an.offScreen=true; continue; }
                    anyOn = true;
                    float hw=an.frameW*an.scale*0.4f, hh=an.frameH*an.scale*0.4f;
                    extraHitboxes.push_back({{an.posX-hw,an.laneY-hh,hw*2,hh*2}, an.laneY});
                }
                if (!anyOn) herd.active=false;
            }
            break;
        }

        // --- Lv2: Sandstorm ---
        case HazardType::SANDSTORM:
            mSandAnimTimer += dt;
            updateParticles(dt);
            mWindDrift = {42.f*dt, 0.f};
            break;

        // --- Lv3: Arrow Rain ---
        case HazardType::ARROW_RAIN: {
            mArrowSpawnTimer -= dt;
            if (mArrowSpawnTimer <= 0.f) {
                mArrowSpawnTimer = 0.65f + (float)(rand() % 3) * 0.12f;
                const float lanes[] = {150.f, 215.f, 270.f, 380.f, 452.f, 505.f};
                int numNew = 1 + (rand() % 2); // 1 or 2 new arrow targets per wave
                for (int k = 0; k < numNew; ++k) {
                    ArrowTarget t;
                    t.position = { 120.f + (float)(rand() % (Win_W - 240)), lanes[rand() % 6] };
                    t.warningTimer = t.maxWarningTime = 1.1f + (float)(rand() % 3) * 0.15f;
                    t.arrowY       = t.position.y - 350.f;
                    t.active       = true;
                    t.struck       = false;
                    t.soundPlayed  = false;
                    t.animTimer    = 0.f;
                    t.currentFrame = 0;
                    mArrowTargets.push_back(t);
                }
            }

            for (auto& t : mArrowTargets) {
                if (!t.active) continue;
                t.animTimer += dt;
                t.currentFrame = (int)(t.animTimer * 12.0f) % 12;

                if (t.warningTimer > 0.f) {
                    t.warningTimer -= dt;
                    continue; // SAFE while target reticle warning is displaying!
                }
                if (!t.soundPlayed) {
                    t.soundPlayed = true;
                    if (mSound) mSound->playArrowVolley();
                }
                if (t.arrowY < t.position.y) {
                    t.arrowY += 1200.f * dt;
                    if (t.arrowY >= t.position.y) {
                        t.arrowY = t.position.y;
                        t.struck = true;
                    }
                }
                // ONLY lethal when arrow is physically striking down near the target position!
                if (t.arrowY >= t.position.y - 60.f && !t.struck) {
                    extraHitboxes.push_back({{t.position.x - 18.f, t.position.y - 18.f, 36.f, 36.f}, t.position.y});
                }
            }
            break;
        }

        // --- Lv4: Thundercloud & Heavy Rain ---
        case HazardType::RUSH_HOUR: {
            mCloudAnimTimer += dt;

            // Update rain drops
            for (auto& r : mRainDrops) {
                r.position.y += r.speed * dt;
                r.position.x -= 70.f * dt;
                if (r.position.y > Win_H) {
                    r.position.y = -20.f;
                    r.position.x = (float)(rand() % (Win_W + 200));
                }
            }

            // Cloud cluster drift
            mCloudPosX += 30.f * dt;
            if (mCloudPosX > 120.f) mCloudPosX -= 120.f;

            // Spawn new vertical lightning strikes
            mLightningTimer -= dt;
            if (mLightningTimer <= 0.f) {
                mLightningTimer = 0.6f + (float)(rand() % 5) * 0.15f;
                const float laneOptions[] = {150.f, 215.f, 270.f, 340.f, 405.f, 460.f};
                int numSpawns = 1 + (rand() % 2);
                for (int s = 0; s < numSpawns; ++s) {
                    LightningStrike ls;
                    ls.laneY       = laneOptions[rand() % 6];
                    ls.strikeX     = 100.f + (float)(rand() % (Win_W - 200));
                    ls.warningTimer= 1.2f + (float)(rand() % 4) * 0.15f; // Clear warning period!
                    ls.activeTimer = 0.45f;
                    ls.active      = true;
                    ls.soundPlayed = false;
                    ls.animTimer   = 0.f;
                    ls.currentFrame= 0;
                    mLightningStrikes.push_back(ls);
                }
            }

            // Update lightning strikes
            for (auto& ls : mLightningStrikes) {
                if (!ls.active) continue;
                ls.animTimer += dt;
                ls.currentFrame = (int)(ls.animTimer * 12.0f) % 12;

                if (ls.warningTimer > 0.f) {
                    ls.warningTimer -= dt;
                    continue; // SAFE while ground warning target is displaying!
                } else {
                    if (!ls.soundPlayed) { 
                        ls.soundPlayed = true; 
                        if (mSound) mSound->playLightning(); 
                    }
                    ls.activeTimer -= dt;
                    if (ls.activeTimer <= 0.f) { 
                        ls.active = false; 
                        continue; 
                    }
                    // ONLY lethal while vertical lightning bolt is actively striking down!
                    sf::FloatRect box(ls.strikeX - 34.f, ls.laneY - 20.f, 68.f, 40.f);
                    extraHitboxes.push_back({box, ls.laneY});
                }
            }
            // Cleanup finished strikes
            mLightningStrikes.erase(
                std::remove_if(mLightningStrikes.begin(), mLightningStrikes.end(),
                               [](const LightningStrike& s){ return !s.active && s.warningTimer <= 0.f; }),
                mLightningStrikes.end());
            break;
        }

        // --- Lv5: 3-4 Small 12-Frame Cyber Black Holes ---
        case HazardType::BLACK_HOLE: {
            mWindDrift = { 0.f, 0.f };

            for (auto& bh : mBlackHoles) {
                // Update 12-frame animation timer
                bh.animTimer += dt;
                bh.currentFrame = (int)(bh.animTimer * 12.0f) % 12;

                // Small lethal core hitbox (Radius ~ 15px)
                sf::FloatRect coreBox(bh.position.x - 15.f, bh.position.y - 15.f, 30.f, 30.f);
                extraHitboxes.push_back({ coreBox, bh.position.y });

                // Calculate gentle gravity pull towards closest Black Hole if player in active area
                if (mPlayerPos.y <= 530.f) {
                    float dx = bh.position.x - mPlayerPos.x;
                    float dy = bh.position.y - mPlayerPos.y;
                    float dist = std::sqrt(dx * dx + dy * dy);
                    if (dist > 1.f && dist < 170.f) {
                        float pullSpeed = std::clamp(120.f - dist * 0.4f, 25.f, 90.f);
                        mWindDrift.x += (dx / dist) * pullSpeed * dt;
                        mWindDrift.y += (dy / dist) * pullSpeed * dt;
                    }
                }
            }

            // Update horizontal Laser Beams
            for (auto& l : mLaserLanes) {
                if (!l.active) continue;
                if (l.warningTimer > 0.f) {
                    l.warningTimer -= dt;
                    continue;
                }
                l.activeTimer -= dt;
                if (l.activeTimer <= 0.f) {
                    l.active = false;
                    continue;
                }

                // Update 12-frame laser animation timer
                l.animTimer += dt;
                l.currentFrame = (int)(l.animTimer * 12.0f) % 12;

                // Prominent laser hitbox (24px height)
                extraHitboxes.push_back({{0.f, l.laneY - 12.f, (float)Win_W, 24.f}, l.laneY});
            }
            break;
        }

        default: break;
        }
    }
}

// =========================================================
// draw
// =========================================================
void HazardManager::draw(sf::RenderWindow& window) const {
    if (mCurrentHazard == HazardType::NONE) return;

    // Lv1: Herd Stampede
    if (mCurrentHazard == HazardType::DINO_STAMPEDE && (mIsActive||mIsWarning)) {
        for (const auto& herd : mHerdLanes) {
            if (!herd.active) continue;
            if (herd.warningTimer > 0.f) {
                int frameIdx = herd.currentFrame % 12;
                int col = frameIdx % 4;
                int row = frameIdx / 4;

                // 1. 12-Frame Animated Stampede Lane Warning Wash (Perfectly Centered)
                if (mDinoLaneWarningAnimTexture.getSize().x > 0) {
                    sf::Sprite laneWarnSpr(mDinoLaneWarningAnimTexture);
                    laneWarnSpr.setTextureRect(sf::IntRect(col * 512, row * 48, 512, 48));
                    laneWarnSpr.setOrigin(0.f, 24.f); // Centered vertically on laneY
                    laneWarnSpr.setScale((float)Win_W / 512.f, 44.f / 48.f);
                    laneWarnSpr.setPosition(0.f, herd.laneY);
                    window.draw(laneWarnSpr);
                }

                // 2. 12-Frame Animated Exclamation Mark Warning Sign at Road Edges
                if (mDinoWarningSignAnimTexture.getSize().x > 0) {
                    sf::Sprite signSpr(mDinoWarningSignAnimTexture);
                    signSpr.setTextureRect(sf::IntRect(col * 64, row * 64, 64, 64));
                    signSpr.setOrigin(32.f, 32.f);
                    signSpr.setScale(0.9f, 0.9f);

                    // Left edge sign
                    signSpr.setPosition(40.f, herd.laneY);
                    window.draw(signSpr);

                    // Right edge sign
                    signSpr.setPosition((float)Win_W - 40.f, herd.laneY);
                    window.draw(signSpr);
                }
            } else {
                sf::RectangleShape dust({(float)Win_W,14.f});
                dust.setPosition(0,herd.laneY+28); dust.setFillColor({190,160,90,50});
                window.draw(dust);
                for (const auto& an : herd.animals) {
                    if (an.offScreen||an.posX<-100||an.posX>Win_W+100) continue;
                    int idx=an.textureIdx;
                    if (idx>=0&&idx<(int)mDinoTextures.size()) {
                        sf::Sprite spr(mDinoTextures[idx]);
                        int col=an.currentFrame%an.animCols, row=an.currentFrame/an.animCols;
                        spr.setTextureRect({col*an.frameW, row*an.frameH, an.frameW, an.frameH});
                        spr.setOrigin(an.frameW/2.f, an.frameH/2.f);
                        spr.setScale(an.scale, an.scale);
                        spr.setPosition(an.posX+mShakeOffset.x, an.laneY+mShakeOffset.y);
                        window.draw(spr);
                    }
                }
            }
        }
    }

    // Lv2: Sandstorm (12-Frame Animation & Fine Pixel Sand)
    if (mCurrentHazard == HazardType::SANDSTORM && (mIsActive || mIsWarning)) {
        float a = mIsActive ? (120.f + std::sin(mActiveTimer * 4.f) * 20.f) : 80.f;

        // 1. Desert atmosphere fog tint
        sf::RectangleShape fog({(float)Win_W, (float)Win_H});
        fog.setFillColor({210, 145, 35, (sf::Uint8)a});
        window.draw(fog);

        // 2. 12-Frame Sandstorm Wind & Dust Swirl Sprite Sheet
        if (mSandstormAnimTexture.getSize().x > 0) {
            int frameIdx = (int)(mSandAnimTimer * 12.0f) % 12;
            int col = frameIdx % 4;
            int row = frameIdx / 4;

            sf::Sprite sandSpr(mSandstormAnimTexture);
            sandSpr.setTextureRect(sf::IntRect(col * 256, row * 128, 256, 128));
            sandSpr.setScale((float)Win_W / 256.f, (float)Win_H / 128.f);
            sandSpr.setColor(sf::Color(255, 255, 255, (sf::Uint8)(150 + std::sin(mSandAnimTimer * 6.f) * 35.f)));
            window.draw(sandSpr);
        }

        // 3. Fine pixel sand specks (Small & realistic)
        const sf::Color pal[4] = {{255, 235, 150}, {245, 194, 82}, {224, 154, 50}, {199, 117, 36}};
        for (const auto& p : mSandParticles) {
            sf::RectangleShape dot({p.radius * 1.5f, p.radius * 1.5f});
            dot.setPosition(p.position);
            sf::Color c = pal[p.colorIdx % 4];
            c.a = (sf::Uint8)p.alpha;
            dot.setFillColor(c);
            window.draw(dot);
        }
    }

    // Lv3: Arrow Rain (12-Frame Target Reticle + Arrow)
    if (mCurrentHazard == HazardType::ARROW_RAIN && mIsActive) {
        for (const auto& t : mArrowTargets) {
            if (!t.active) continue;

            int frameIdx = t.currentFrame % 12;
            int col = frameIdx % 4;
            int row = frameIdx / 4;

            // 1. 12-Frame Animated Target Reticle
            if (mReticleAnimTexture.getSize().x > 0) {
                sf::Sprite rs(mReticleAnimTexture);
                rs.setTextureRect(sf::IntRect(col * 64, row * 64, 64, 64));
                rs.setOrigin(32.f, 32.f);
                rs.setPosition(t.position);
                rs.setScale(1.1f, 1.1f);
                window.draw(rs);
            }

            // 2. Falling Arrow Sprite
            if (t.warningTimer <= 0) {
                if (mArrowTexture.getSize().x > 0) {
                    sf::Sprite as(mArrowTexture);
                    as.setTextureRect({0, 0, 64, 64});
                    as.setOrigin(52.f, 32.f);
                    as.setRotation(90.f);
                    as.setScale(1.2f, 1.2f);
                    as.setPosition(t.position.x, t.arrowY);
                    window.draw(as);
                }
            }
        }
    }

    // Lv4: Thundercloud & Heavy Rain & Vertical Lightning Strikes
    if (mCurrentHazard == HazardType::RUSH_HOUR && (mIsActive || mIsWarning)) {
        // Calculate smooth transition factor (0.0 to 1.0)
        float stormFade = 1.0f;
        if (mIsWarning) {
            // Smoothly fade in sky, rain, and clouds as warning timer counts down from 2.5s to 0s
            stormFade = std::clamp((2.5f - mWarningTimer) / 2.5f, 0.15f, 1.0f);
        }

        // 1. Smoothly faded dark rainstorm sky tint
        sf::RectangleShape stormTint({(float)Win_W, (float)Win_H});
        stormTint.setFillColor(sf::Color(15, 20, 38, static_cast<sf::Uint8>(130.f * stormFade)));
        window.draw(stormTint);

        // 2. Heavy Rain Drops (alpha scaled smoothly by stormFade)
        sf::VertexArray rainLines(sf::Lines);
        sf::Uint8 rainAlpha = static_cast<sf::Uint8>(170.f * stormFade);
        for (const auto& r : mRainDrops) {
            sf::Color rainCol(160, 200, 255, rainAlpha);
            rainLines.append(sf::Vertex({r.position.x, r.position.y}, rainCol));
            rainLines.append(sf::Vertex({r.position.x - 5.f, r.position.y + r.length}, rainCol));
        }
        window.draw(rainLines);

        // 3. Dense Cloud Ceiling across top of screen (12-Frame Animation)
        if (mThundercloudAnimTexture.getSize().x > 0) {
            int cloudFrame = (int)(mCloudAnimTimer * 12.0f) % 12;
            int cCol = cloudFrame % 4;
            int cRow = cloudFrame / 4;

            sf::Sprite cloud(mThundercloudAnimTexture);
            cloud.setTextureRect(sf::IntRect(cCol * 128, cRow * 64, 128, 64));
            cloud.setScale(1.25f, 1.25f);

            int numClouds = (Win_W / 120) + 3;
            sf::Uint8 cloudAlpha = static_cast<sf::Uint8>(240.f * stormFade);
            for (int i = -1; i < numClouds; ++i) {
                float cx = i * 120.f + fmod(mCloudPosX, 120.f);
                float cy = 10.f + (i % 2 == 0 ? 0.f : 12.f);
                cloud.setPosition(cx, cy);
                cloud.setColor(sf::Color(210, 220, 240, cloudAlpha));
                window.draw(cloud);
            }
        }

        // 4. Lightning Warning Zones & Strikes (12-Frame Animations)
        for (const auto& ls : mLightningStrikes) {
            if (!ls.active) continue;
            if (ls.warningTimer > 0.f) {
                // Ground warning indicator (12-Frame Animation)
                if (mLightningWarningAnimTexture.getSize().x > 0) {
                    int wFrameIdx = ls.currentFrame % 12;
                    int wCol = wFrameIdx % 4;
                    int wRow = wFrameIdx / 4;

                    sf::Sprite warnSpr(mLightningWarningAnimTexture);
                    warnSpr.setTextureRect(sf::IntRect(wCol * 64, wRow * 64, 64, 64));
                    warnSpr.setOrigin(32.f, 32.f);
                    warnSpr.setPosition(ls.strikeX, ls.laneY);
                    warnSpr.setScale(1.1f, 1.1f);
                    window.draw(warnSpr);
                }
            } else {
                // Active phase: Vertical Lightning Bolt (12-Frame Animation)
                if (mLightningAnimTexture.getSize().x > 0) {
                    int bFrameIdx = ls.currentFrame % 12;
                    int bCol = bFrameIdx % 4;
                    int bRow = bFrameIdx / 4;

                    sf::Sprite bolt(mLightningAnimTexture);
                    bolt.setTextureRect(sf::IntRect(bCol * 64, bRow * 256, 64, 256));

                    float scaleY = ls.laneY / 256.f;
                    bolt.setScale(1.0f, scaleY);
                    bolt.setOrigin(32.f, 0.f);
                    bolt.setPosition(ls.strikeX, 0.f);

                    float pulseA = 210.f + 45.f * std::sin(ls.activeTimer * 30.f);
                    bolt.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(pulseA)));
                    window.draw(bolt);
                }
            }
        }
    }

    // Lv5: 3-4 Small 12-Frame Cyber Black Holes & Laser Beams
    if (mCurrentHazard == HazardType::BLACK_HOLE && (mIsActive || mIsWarning)) {
        if (mIsActive) {
            // 1. Draw 12-Frame Animated Laser Beams across lanes (Prominent 36px height)
            for (const auto& l : mLaserLanes) {
                if (!l.active) continue;
                if (l.warningTimer > 0.f) {
                    float flash = (std::sin(l.warningTimer * 20.f) + 1.f) * 0.5f;
                    sf::RectangleShape warnLine({(float)Win_W, 4.f});
                    warnLine.setPosition(0, l.laneY - 2.f);
                    warnLine.setFillColor(sf::Color(0, 220, 255, (sf::Uint8)(110 + flash * 145)));
                    window.draw(warnLine);
                } else if (mLaserAnimTexture.getSize().x > 0) {
                    int lFrameIdx = l.currentFrame % 12;
                    int lCol = lFrameIdx % 4;
                    int lRow = lFrameIdx / 4;

                    sf::Sprite laserSprite(mLaserAnimTexture);
                    laserSprite.setTextureRect(sf::IntRect(lCol * 512, lRow * 48, 512, 48));
                    laserSprite.setScale((float)Win_W / 512.f, 36.f / 48.f); // Prominent 36px thickness
                    laserSprite.setPosition(0.f, l.laneY - 18.f);
                    window.draw(laserSprite);
                }
            }

            // 2. Draw 12-Frame Animated Black Holes & Outer Energy Rings
            for (const auto& bh : mBlackHoles) {
                int frameIdx = bh.currentFrame % 12;
                int col = frameIdx % 4;
                int row = frameIdx / 4;

                // Outer Energy Ring (12-Frame animation)
                if (mBlackHoleRingAnimTexture.getSize().x > 0) {
                    sf::Sprite ringSpr(mBlackHoleRingAnimTexture);
                    ringSpr.setTextureRect(sf::IntRect(col * 80, row * 80, 80, 80));
                    ringSpr.setOrigin(40.f, 40.f);
                    ringSpr.setPosition(bh.position);
                    ringSpr.setScale(0.85f, 0.85f); // Compact size
                    window.draw(ringSpr);
                }

                // Black Hole Core (12-Frame animation)
                if (mBlackHoleAnimTexture.getSize().x > 0) {
                    sf::Sprite bhSpr(mBlackHoleAnimTexture);
                    bhSpr.setTextureRect(sf::IntRect(col * 64, row * 64, 64, 64));
                    bhSpr.setOrigin(32.f, 32.f);
                    bhSpr.setPosition(bh.position);
                    bhSpr.setScale(0.75f, 0.75f); // Compact size
                    window.draw(bhSpr);
                }
            }
        }
    }
}

// =========================================================
// drawUI
// =========================================================
void HazardManager::drawUI(sf::RenderWindow& window) const {
    if (mCurrentHazard == HazardType::NONE) return;
    if (mIsWarning) {
        window.draw(mWarningBox);
        window.draw(mWarningText);
    }
}
