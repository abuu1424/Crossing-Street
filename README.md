# Crossing Street — Time Crossing Arcade Game

> **Object-Oriented Programming Course Project**
> **Faculty of Information Technology — Ho Chi Minh City University of Science (HCMUS)**

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=flat&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/Library-SFML_2.6.2-8CC651?style=flat&logo=sfml)
![CMake](https://img.shields.io/badge/Build-CMake_3.16%2B-064F8C?style=flat&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-macOS_%7C_Windows-blue)

---

## 🌟 Overview

**Crossing Street** is a feature-rich 2D road-crossing arcade game built in **C++17** and **SFML 2.6.2**.

Players navigate across dangerous traffic lanes while traveling through **5 distinct historical eras**, from the Prehistoric jungle to the Cyber Future. The game includes single-player story progression, local multiplayer, AI opponent racing, active skills, in-lane power-ups, an item shop, and dynamic level hazards.

---

## 🚀 Game Modes

### 1. 🏛️ Story Campaign

- Travel through **5 historical eras** with cinematic era elevator transitions.
- Complete era objectives within the time limit while dodging obstacles and era-specific hazards.

### 2. ⚔️ Local 2-Player Versus (1v1)

- Shared keyboard split-screen gameplay with dedicated controls (`WASD` vs `Arrow Keys`).
- Independent health bars, stamina meters, power-up contest, round win counters (`P1` vs `P2`), and instant rematch (`restartLevel()`).

### 3. 🤖 VS Bot AI (Smart Adversary)

- Race against an intelligent bot powered by **Spatio-Temporal A\*** pathfinding with 3 difficulty tiers:
  - **Easy:** Relaxed speed, beginner-friendly.
  - **Normal:** Matched player speed, lane-gap timing, active power-up collection.
  - **Hard:** 16.6 Hz real-time lookahead, micro-dodging, zero mistakes, aggressive speed.
- Real-time dual telemetry HUD tracking bot status (`RACING`, `FROZEN`, `ELIMINATED`), health, speed, and progress.

### 4. ♾️ Endless Chrono Mode

- Infinite wave progression across eras with escalating speed, score multipliers, and dynamic hazards.
- High score persistence with wave badges.

---

## ⚡ Active Skills & In-Lane Power-Ups

### Active Character Skills (Cooldown-based)

- **Sprint (`Shift`):** +50% movement speed consuming Stamina. Stamina recovers when walking/idle.
- **Speed Surge (`E` / `Space` | P2: `I` / `[`):** +50% speed boost with lightning aura for 5s (Cooldown: 10s).
- **Coin Magnet Radar (`Q` | P2: `K` / `]` / `O`):** Sucks all coins within 700px to the player and slows hazards by 70% for 6s (Cooldown: 10s).
- **Time Freeze Clock (`T` | P2: `L` / `\`):** Freezes all traffic and obstacles for 5s (Cooldown: 12s).

### In-Lane Floating Power-Ups

- 🧲 **Magnet (Blue):** Pulls all coins across wide range into the player (8s).
- ⏱️ **Time Stop (Clock):** Slows down 85% of traffic & hazards (4s).
- 🔥 **Speed Boost (Flame):** +50% speed surge with flame trail (6s).
- 🛡️ **Bubble Shield (Orb):** Absorbs 1 fatal hit + knockback protection.
- ⭐ **2X Multiplier (Coin):** Doubles gold and score earnings (10s).

### Item Shop (Main Menu & Level Clear)

- **Energy Shield (250c):** Consumable shield absorbing 1 fatal collision.
- **Speed Boots (400c):** +15% permanent movement speed bonus.
- **Time Extender (350c):** +8s level time limit bonus.
- **Coin Radar (500c):** -15% passive hazard speed slowdown.
- **Extra Heart (1500c):** Restores and expands player HP.

---

## 🎮 Controls

| Key                                   | Action                                                |
| :------------------------------------ | :---------------------------------------------------- |
| **`W` `A` `S` `D`**     | Player 1 Movement (Up / Left / Down / Right)          |
| **`LShift`**                  | Player 1 Sprint (Consumes Stamina)                    |
| **`E` / `Space`**           | Player 1 Speed Surge Skill                            |
| **`Q`**                       | Player 1 Coin Magnet Radar Skill                      |
| **`T`**                       | Player 1 Time Freeze Clock Skill                      |
| **`1` `2` `3` `4`**     | Player 1 Inventory Items (Shield, Boots, Time, Radar) |
| **`↑` `←` `↓` `→`** | Player 2 Movement (2-Player Mode)                     |
| **`RShift` / `RCtrl`**      | Player 2 Sprint (2-Player Mode)                       |
| **`I` / `[`**               | Player 2 Speed Surge Skill                            |
| **`K` / `]` / `O`**       | Player 2 Coin Magnet Radar Skill                      |
| **`L` / `\`**               | Player 2 Time Freeze Clock Skill                      |
| **`P`**                       | Pause Game / Open Settings & Keybind Guide            |
| **`R`**                       | Quick Restart / Rematch Current Level                 |
| **`S`**                       | Open Item Shop (Level Clear / Victory Screen)         |
| **`M`**                       | Return to Main Menu Confirmation                      |
| **`ESC`**                     | Back / Open Quit Confirmation                         |
| **`Enter` / `Space`**       | Skip Cutscene                                         |
| **`F1` - `F3`**             | Quick Save to Slot 1 - Slot 3                         |
| **`F4` - `F6`**             | Quick Load from Slot 1 - Slot 3                       |
| **`F11` / `Alt+Enter`**     | Toggle Fullscreen Mode                                |
| **`V`**                       | Toggle Hitbox Debug Overlay                           |

---

## 🗺️ Levels Summary

|    Level    | Era           | Ground Obstacles        | Flying Objects            | Extreme Hazard                  | Death Cutscene        |
| :---------: | :------------ | :---------------------- | :------------------------ | :------------------------------ | :-------------------- |
| **1** | Prehistoric   | Dinosaurs, Mammoths     | Ancient Birds             | **Dino Stampede**         | Swamp Monster Chomp   |
| **2** | Ancient Egypt | Camels, Sphinxes        | Eagles, Horus Birds       | **Sandstorm Wind Drift**  | Quicksand Sink        |
| **3** | Medieval      | Horses, Logs, Rickshaws | Crows, Volley Arrows      | **Flaming Arrow Rain**    | Leaf Whirlwind        |
| **4** | Modern City   | Cars, Motorbikes, Buses | Planes, Missiles          | **Rush Hour & Lightning** | Smoke Dissolve        |
| **5** | Cyber Future  | Electric Cars & Trains  | UFOs, Drones, Flying Cars | **Black Hole & Lasers**   | Plasma Disintegration |

---

## 🏗️ Architecture & OOP Design

The project is structured with modular OOP principles and clean separation of concerns:

```text
src/
├── main.cpp                # Application entry point
├── ai/                     # Artificial Intelligence
│   └── BotAI.h/.cpp        # Spatio-Temporal A* pathfinding & obstacle prediction
├── core/                   # Game engine core
│   ├── CGAME.h/.cpp        # Main game loop, states, rendering, event handling
│   ├── SaveData.h/.cpp     # Save/load file serialization
│   ├── SoundManager.h/.cpp # Audio engine (BGM ducking, SFX, cutscene audio)
│   └── TextureManager.h/.cpp # Texture resource caching
├── cutscenes/              # Cinematic systems
│   ├── ElevatorCutscene.h/.cpp # Seamless era elevator transitions
│   ├── DeathCutscene.h/.cpp    # 5 level-tailored animated death cutscenes
│   ├── CollisionEffect.h/.cpp  # Particle & explosion collision sprites
│   └── Animation.h/.cpp        # Sprite frame animation controller
├── entities/               # Game objects & entities
│   ├── base/               # CPEOPLE, CANIMAL, CVEHICLE, CTRAFFICLIGHT, PlayerStats
│   ├── EntityManager.h/.cpp# Spawning, lane recycling & collision checks
│   ├── CoinManager.h/.cpp  # Coin distribution, collection & magnetic suction
│   ├── PowerUpManager.h/.cpp # Floating collectibles & active buff timers
│   └── obstacles/          # Level-specific entity classes (lv1 to lv5)
├── hazards/                # Dynamic environment hazards
│   └── HazardManager.h/.cpp# Stampede, Sandstorm, Arrow Rain, Thunder, Black Hole
├── levels/                 # Level configurations
│   └── LevelConfig.h/.cpp  # Per-level speeds, lanes, assets, and music setup
├── ui/                     # User interface
│   ├── HUD.h/.cpp          # Score, timer, stamina, health, and bot telemetry HUDs
│   ├── Menu.h/.cpp         # Main menu, pause, challenges, settings & shop
│   ├── MenuButton.h        # Interactive UI buttons with hover animations
│   └── UIText.h            # Styled text helpers
└── utils/                  # Utilities
    ├── Utils.h             # Constants & geometry helpers
    ├── HighScore.h/.cpp    # High score file I/O
    └── ShopData.h/.cpp     # Shop state & slot persistence caching
```

### Applied OOP Concepts:

1. **Encapsulation:** Entities, audio, and hazards encapsulate internal states with accessor and mutator interfaces.
2. **Inheritance:** Derived obstacles inherit from abstract base classes `CANIMAL` and `CVEHICLE`.
3. **Polymorphism:** Virtual methods `Draw()`, `Move()`, `update()`, and `loadSprite()` enable distinct entity behaviors.
4. **Abstraction:** `EntityManager`, `HazardManager`, `PowerUpManager`, and `CoinManager` decouple complex subsystem logic from `CGAME`.
5. **Factory Method:** Dynamic creation of era-specific obstacles and animals based on `LevelConfig`.

---

## 🛠️ Build & Run Instructions

### macOS

**Prerequisites:** `sfml@2`, `cmake`, `clang`

```bash
brew install sfml@2 cmake
chmod +x build.sh
./build.sh run
```

### Windows

**Prerequisites:** Visual Studio 2022 (C++ Desktop Development), SFML 2.6.2, CMake 3.16+

```cmd
build.bat run
```

---

## 👥 Team Members

| Student                        | Responsibility                                                |
| :----------------------------- | :------------------------------------------------------------ |
| **Phan Gia Huy**         | CGAME, SaveData, Animation, LevelConfig, DeathCutscene, BotAI |
| **Nguyễn Hoàng Nhật** | CPEOPLE, obstacles, backgrounds                               |
| **Nguyễn Văn Hải**    | CANIMAL, sounds, flying objects                               |
| **Phan Đình Quốc**    | Menu, HUD, UI, Shop                                           |

---

## 📚 Course Details

- **Course:** Object-Oriented Programming
- **Faculty:** Faculty of Information Technology
- **University:** Ho Chi Minh City University of Science (HCMUS)
