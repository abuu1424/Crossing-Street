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

    // Level 4: Thundercloud & Lightning
    mThundercloudTexture.loadFromFile("assets/hazards/lv4/thundercloud.png");
    mThundercloudTexture.setSmooth(true);
    mLightningTexture.loadFromFile("assets/hazards/lv4/lightning_bolt.png");
    mLightningTexture.setSmooth(true);

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

    // Lv2 – wind-gust sprite
    if (!mWindTexture.loadFromFile("assets/hazards/lv2/sand_wind.png")) {
        sf::RenderTexture windRt;
        if (windRt.create(128, 32)) {
            windRt.clear(sf::Color::Transparent);
            auto ln = [&](float x,float y,float w,float h, sf::Color c){
                sf::RectangleShape r({w,h}); r.setPosition(x,y); r.setFillColor(c); windRt.draw(r);
            };
            ln(8,6,96,4,{255,215,110,220}); ln(104,7,16,2,{255,215,110,160});
            ln(24,18,64,3,{240,175,75,190}); ln(88,18.5f,12,2,{240,175,75,140});
            ln(116,7,4,2,{255,235,150,200});
            windRt.display();
            mWindTexture = windRt.getTexture();
            mWindTexture.setSmooth(false);
        }
    }

    // Lv3 – arrow texture + target reticle
    if (!mArrowTexture.loadFromFile("assets/sprites/lv3_sp/animal_lv3/muiten.png")) {
        sf::Image img; img.create(64,64,sf::Color::Yellow); mArrowTexture.loadFromImage(img);
    }
    if (!mReticleTexture.loadFromFile("assets/hazards/lv3/target_reticle.png")) {
        sf::RenderTexture rt;
        if (rt.create(128,128)) {
            rt.clear(sf::Color::Transparent);
            auto ring = [&](float r, sf::Color fill, sf::Color out, float thick){
                sf::CircleShape c(r); c.setOrigin(r,r); c.setPosition(64,64);
                c.setFillColor(fill); c.setOutlineColor(out); c.setOutlineThickness(thick);
                rt.draw(c);
            };
            ring(54,sf::Color::Transparent,{255,205,30,240},4);
            ring(42,{30,20,15,160},{220,140,30,220},3);
            ring(26,{220,35,35,210},{255,230,80,230},2);
            ring(7,{255,255,255,255},sf::Color::Transparent,0);
            auto bar = [&](float w,float h, sf::Color c){
                sf::RectangleShape r({w,h}); r.setOrigin(w/2,h/2); r.setPosition(64,64);
                r.setFillColor(c); rt.draw(r);
            };
            bar(88,4,{255,220,40,230}); bar(4,88,{255,220,40,230});
            rt.display();
            mReticleTexture = rt.getTexture(); mReticleTexture.setSmooth(true);
        }
    }

    // Lv5 – laser beam sprite
    if (!mLaserTexture.loadFromFile("assets/hazards/lv5/laser_beam.png")) {
        sf::RenderTexture laserRt;
        if (laserRt.create(512,32)) {
            laserRt.clear(sf::Color::Transparent);
            auto ln = [&](float y,float h, sf::Color c){
                sf::RectangleShape r({512,h}); r.setPosition(0,y); r.setFillColor(c); laserRt.draw(r);
            };
            ln(0,32,{0,230,255,110}); ln(8,16,{255,40,180,200}); ln(13,6,{255,255,255,255});
            laserRt.display();
            mLaserTexture = laserRt.getTexture(); mLaserTexture.setSmooth(true);
        }
    }
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
    case 5: mCurrentHazard = HazardType::LASER_SCAN;    break;
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
    mCloudPosX      = 0.f;
    mLightningTimer = 0.f;
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
        if (mSound) mSound->playStampede();

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
        if (mSound) mSound->playSandstorm();

    // --- Level 3: Arrow Rain ---
    } else if (mCurrentHazard == HazardType::ARROW_RAIN) {
        mArrowTargets.clear();
        const float lanes[] = {150.f,215.f,270.f,380.f,452.f,505.f};
        const float xZones[]= {220.f,480.f,750.f,1020.f};
        for (int i=0;i<4;++i) {
            ArrowTarget t;
            t.position = { xZones[i]+(float)((rand()%100)-50),
                           lanes[(i*2+rand()%2)%6]+20.f };
            t.warningTimer = t.maxWarningTime = 1.8f;
            t.arrowY       = t.position.y - 350.f;
            t.active=true; t.struck=false; t.soundPlayed=false;
            mArrowTargets.push_back(t);
        }

    // --- Level 4: Thundercloud & Heavy Rain ---
    } else if (mCurrentHazard == HazardType::RUSH_HOUR) {
        mLightningStrikes.clear();
        mCloudPosX      = 0.f;
        mLightningTimer = 0.4f; // start first lightning warnings quickly
        if (mSound) mSound->playRushHour();

    // --- Level 5: Laser ---
    } else if (mCurrentHazard == HazardType::LASER_SCAN) {
        mLaserLanes.clear();
        const float playable[] = {140.f,180.f,250.f,300.f,400.f,505.f};
        std::vector<float> sh(playable, playable+6);
        for (size_t i=0;i<sh.size();++i) std::swap(sh[i],sh[rand()%sh.size()]);
        int count = 2 + rand()%2;
        for (int i=0;i<count;++i) {
            LaserLane l;
            l.laneY       = sh[i];
            l.warningTimer= l.maxWarningTime = 1.2f+(float)(rand()%6)*0.1f;
            l.activeTimer = 3.5f+(float)(rand()%20)*0.1f;
            l.active=true; l.soundPlayed=false;
            mLaserLanes.push_back(l);
        }
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
            case HazardType::DINO_STAMPEDE: warnStr="! STAMPEDE - HERD CHARGING !"; if(mSound)mSound->playStampede(); break;
            case HazardType::SANDSTORM:     warnStr="! SANDSTORM INCOMING !";        break;
            case HazardType::ARROW_RAIN:    warnStr="! ARROW VOLLEY - TAKE COVER !"; break;
            case HazardType::RUSH_HOUR:     warnStr="! THUNDERSTORM & HEAVY RAIN !"; break;
            case HazardType::LASER_SCAN:    warnStr="! LASER BEAMS CHARGING !";      break;
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
                if (herd.warningTimer > 0.f) { herd.warningTimer -= dt; continue; }
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
            updateParticles(dt);
            mWindDrift = {42.f*dt, 0.f};
            break;

        // --- Lv3: Arrow Rain ---
        case HazardType::ARROW_RAIN:
            for (auto& t : mArrowTargets) {
                if (!t.active) continue;
                if (t.warningTimer > 0.f) { t.warningTimer -= dt; continue; }
                if (!t.soundPlayed) { t.soundPlayed=true; if(mSound) mSound->playArrowVolley(); }
                if (t.arrowY < t.position.y) { t.arrowY += 1200.f*dt; if(t.arrowY>=t.position.y){t.arrowY=t.position.y;t.struck=true;} }
                extraHitboxes.push_back({{t.position.x-18,t.position.y-18,36,36}, t.position.y});
            }
            break;

        // --- Lv4: Thundercloud & Heavy Rain ---
        case HazardType::RUSH_HOUR: {
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

            // Spawn new vertical lightning strikes (spawn 1 to 2 warning spots at once)
            mLightningTimer -= dt;
            if (mLightningTimer <= 0.f) {
                mLightningTimer = 0.5f + (float)(rand() % 6) * 0.12f;
                const float laneOptions[] = {150.f, 215.f, 270.f, 340.f, 405.f, 460.f};
                int numSpawns = 1 + (rand() % 2); // 1 or 2 warning spots at once
                for (int s = 0; s < numSpawns; ++s) {
                    LightningStrike ls;
                    ls.laneY       = laneOptions[rand() % 6];
                    ls.strikeX     = 100.f + (float)(rand() % (Win_W - 200));
                    ls.warningTimer= 0.8f + (float)(rand() % 4) * 0.1f; // ~0.8s - 1.1s warning time
                    ls.activeTimer = 0.45f;
                    ls.active      = true;
                    ls.soundPlayed = false;
                    mLightningStrikes.push_back(ls);
                }
            }

            // Update lightning strikes
            for (auto& ls : mLightningStrikes) {
                if (!ls.active) continue;
                if (ls.warningTimer > 0.f) {
                    ls.warningTimer -= dt;
                } else {
                    if (!ls.soundPlayed) { 
                        ls.soundPlayed = true; 
                        if (mSound) mSound->playRushHour(); 
                    }
                    ls.activeTimer -= dt;
                    if (ls.activeTimer <= 0.f) { 
                        ls.active = false; 
                        continue; 
                    }
                    // Hitbox around vertical lightning impact area
                    sf::FloatRect box(ls.strikeX - 38.f, ls.laneY - 22.f, 76.f, 44.f);
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

        // --- Lv5: Laser ---
        case HazardType::LASER_SCAN:
            for (auto& l : mLaserLanes) {
                if (!l.active) continue;
                if (l.warningTimer > 0.f) { l.warningTimer -= dt; continue; }
                if (!l.soundPlayed) { l.soundPlayed=true; if(mSound) mSound->playLaserBeam(); }
                l.activeTimer -= dt;
                if (l.activeTimer <= 0.f) { l.active=false; continue; }
                extraHitboxes.push_back({{0.f, l.laneY+20.f, (float)Win_W, 30.f}, l.laneY});
            }
            break;

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
                float flash = (std::sin(herd.warningTimer*20.f)+1.f)*0.5f;
                sf::RectangleShape tint({(float)Win_W,44.f});
                tint.setPosition(0, herd.laneY-22);
                tint.setFillColor({255,140,0,(sf::Uint8)(40+flash*80)});
                window.draw(tint);
                // Staircase dots
                float dotR=6.f, spacing=(float)Win_W/20;
                sf::Uint8 dotA=(sf::Uint8)(160+flash*90);
                for (int d=0;d<20;++d) {
                    sf::CircleShape dot(dotR); dot.setOrigin(dotR,dotR);
                    dot.setFillColor({255,220,40,dotA});
                    dot.setPosition(spacing*(d+0.5f), herd.laneY+(d%2==0?-8.f:8.f));
                    window.draw(dot);
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

    // Lv2: Sandstorm
    if (mCurrentHazard == HazardType::SANDSTORM && (mIsActive||mIsWarning)) {
        float a = mIsActive ? (140.f+std::sin(mActiveTimer*4.f)*25.f) : 90.f;
        sf::RectangleShape fog({(float)Win_W,(float)Win_H});
        fog.setFillColor({210,145,35,(sf::Uint8)a}); window.draw(fog);
        const sf::Color pal[4]={{255,235,150},{245,194,82},{224,154,50},{199,117,36}};
        for (const auto& p : mSandParticles) {
            sf::CircleShape dot(p.radius); dot.setOrigin(p.radius,p.radius);
            dot.setPosition(p.position);
            sf::Color c=pal[p.colorIdx%4]; c.a=(sf::Uint8)p.alpha;
            dot.setFillColor(c); window.draw(dot);
        }
        if (mWindTexture.getSize().x > 0) {
            sf::Sprite ws; ws.setTexture(mWindTexture);
            for (const auto& w : mWindStreaks) {
                ws.setPosition(w.position); ws.setScale(w.scaleX,w.scaleY);
                ws.setColor({255,220,130,(sf::Uint8)w.alpha}); window.draw(ws);
            }
        }
    }

    // Lv3: Arrow Rain
    if (mCurrentHazard == HazardType::ARROW_RAIN && mIsActive) {
        for (const auto& t : mArrowTargets) {
            if (!t.active) continue;
            if (mReticleTexture.getSize().x > 0) {
                sf::Sprite rs; rs.setTexture(mReticleTexture);
                sf::Vector2u sz=mReticleTexture.getSize();
                rs.setOrigin(sz.x/2.f,sz.y/2.f); rs.setPosition(t.position);
                float sc=65.f/sz.x*(0.9f+0.15f*std::sin(t.warningTimer*8.f));
                rs.setScale(sc,sc); window.draw(rs);
            }
            if (t.warningTimer>0) {
                float cr=6.f+(t.warningTimer/t.maxWarningTime)*24.f;
                sf::CircleShape ring(cr); ring.setOrigin(cr,cr); ring.setPosition(t.position);
                ring.setFillColor(sf::Color::Transparent);
                ring.setOutlineColor({255,230,50,230}); ring.setOutlineThickness(2);
                window.draw(ring);
            }
            if (t.warningTimer<=0) {
                sf::Sprite as; as.setTexture(mArrowTexture);
                as.setTextureRect({0,0,64,64}); as.setOrigin(52,32);
                as.setRotation(90); as.setScale(1.2f,1.2f);
                as.setPosition(t.position.x,t.arrowY); window.draw(as);
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

        // 3. Dense Cloud Ceiling across top of screen (alpha scaled smoothly by stormFade)
        if (mThundercloudTexture.getSize().x > 0) {
            sf::Sprite cloud(mThundercloudTexture);
            sf::Vector2u sz = mThundercloudTexture.getSize();
            float cloudW = 160.f;
            float sc = cloudW / sz.x;
            cloud.setScale(sc, sc);

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

        // 4. Lightning Warning Zones & Strikes
        for (const auto& ls : mLightningStrikes) {
            if (!ls.active) continue;
            if (ls.warningTimer > 0.f) {
                // NO VERTICAL BEAM! Only ground warning spots on the lane
                float flash = (std::sin(ls.warningTimer * 24.f) + 1.f) * 0.5f;
                sf::Uint8 alpha = static_cast<sf::Uint8>(120 + flash * 135);

                // Outer pulsing danger ring on lane
                sf::CircleShape outerWarn(38.f);
                outerWarn.setOrigin(38.f, 38.f);
                outerWarn.setPosition(ls.strikeX, ls.laneY);
                outerWarn.setFillColor(sf::Color(240, 50, 30, static_cast<sf::Uint8>(70 + flash * 70)));
                outerWarn.setOutlineColor(sf::Color(255, 220, 40, alpha));
                outerWarn.setOutlineThickness(3.5f);
                window.draw(outerWarn);

                // Inner bright flashing danger core
                sf::CircleShape innerWarn(18.f);
                innerWarn.setOrigin(18.f, 18.f);
                innerWarn.setPosition(ls.strikeX, ls.laneY);
                innerWarn.setFillColor(sf::Color(255, 235, 80, static_cast<sf::Uint8>(140 + flash * 115)));
                window.draw(innerWarn);
            } else {
                // Active phase: Vertical Lightning Bolt sprite strikes down from clouds to lane
                if (mLightningTexture.getSize().x > 0) {
                    sf::Sprite bolt(mLightningTexture);
                    sf::Vector2u sz = mLightningTexture.getSize();

                    float boltH = ls.laneY;
                    float scaleY = boltH / sz.y;
                    float scaleX = 0.85f + (float)(rand() % 4) * 0.1f;

                    bolt.setScale(scaleX, scaleY);
                    bolt.setOrigin(sz.x / 2.f, 0.f);
                    bolt.setPosition(ls.strikeX, 0.f);

                    float pulseA = 210.f + 45.f * std::sin(ls.activeTimer * 30.f);
                    bolt.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(pulseA)));
                    window.draw(bolt);
                } else {
                    sf::RectangleShape bolt({12.f, ls.laneY});
                    bolt.setOrigin(6.f, 0.f);
                    bolt.setPosition(ls.strikeX, 0.f);
                    bolt.setFillColor(sf::Color(220, 240, 255, 240));
                    window.draw(bolt);
                }

                // Ground impact glow flash
                sf::CircleShape impactGlow(45.f);
                impactGlow.setOrigin(45.f, 45.f);
                impactGlow.setPosition(ls.strikeX, ls.laneY);
                impactGlow.setFillColor(sf::Color(180, 220, 255, 170));
                window.draw(impactGlow);
            }
        }
    }

    // Lv5: Laser
    if (mCurrentHazard == HazardType::LASER_SCAN && mIsActive) {
        for (const auto& l : mLaserLanes) {
            if (!l.active) continue;
            if (l.warningTimer > 0.f) {
                float flash=(std::sin(l.warningTimer*20.f)+1.f)*0.5f;
                sf::RectangleShape warn({(float)Win_W,4.f});
                warn.setPosition(0,l.laneY+30); warn.setFillColor({255,30,30,(sf::Uint8)(120+flash*135)});
                window.draw(warn);
            } else if (mLaserTexture.getSize().x > 0) {
                sf::Sprite ls2; ls2.setTexture(mLaserTexture);
                sf::Vector2u sz=mLaserTexture.getSize();
                ls2.setScale((float)Win_W/sz.x, 32.f/sz.y);
                ls2.setPosition(0,l.laneY+16);
                float pa=200.f+55.f*std::sin(l.activeTimer*12.f);
                ls2.setColor({255,255,255,(sf::Uint8)pa}); window.draw(ls2);
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
