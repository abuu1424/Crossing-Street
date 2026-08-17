# Crossing Street — Time Crossing Arcade Game

> **Object-Oriented Programming Course Project**  
> **Faculty of Information Technology — Ho Chi Minh City University of Science (HCMUS)**

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=flat&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/Library-SFML_2.6.2-8CC651?style=flat&logo=sfml)
![CMake](https://img.shields.io/badge/Build-CMake_3.16%2B-064F8C?style=flat&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-macOS_%7C_Windows-blue)

---

## 🌟 Overview

**Crossing Street** is a 2D road-crossing arcade game built with **C++17** and **SFML 2.6.2**. 

Players control a character navigating through hazardous lanes filled with unique obstacles (vehicles, prehistoric creatures, aircraft, UFOs) while traveling through **5 distinct historical eras**, from the Prehistoric era to the Cyber Future.

---

## 🚀 Key Features

### 1. 🏛️ 5 Historical Era Levels & Extreme Hazards
- **Level 1 — Prehistoric Era:** Dinosaurs, Mammoths, Ancient Birds, and Swamp Monsters.  
  *Hazard:* **Dino Stampede** — Thundering herd charges across lane with warning sirens.
- **Level 2 — Ancient Egypt:** Camels, Sphinxes, Egyptian Eagles, and Sand Bursts.  
  *Hazard:* **Sandstorm** — Blinding desert wind streaks impairing visibility & movement.
- **Level 3 — Medieval Era:** Rickshaws, Horses, Wooden Logs, Volley Arrows, and Leaf Whirlwinds.  
  *Hazard:* **Arrow Rain** — Target warning markers followed by a lethal volley of flaming arrows.
- **Level 4 — Modern City:** Cars, Motorbikes, Buses, Planes, Missiles, and Smoke Explosions.  
  *Hazard:* **Rush Hour & Thunderstorm** — Turbo-charged traffic surge with lightning strikes.
- **Level 5 — Cyber Future:** Electric Cars, Electric Trains, Flying Cars, UFOs, Drones, and Plasma Spheres.  
  *Hazard:* **Black Hole & Laser Lanes** — Gravitational pull and charged laser beam sweeps.

### 2. ⚡ Active Combat Skills & Movement
- **Sprint:** Hold `Shift` (Left/Right Shift) to sprint (+50% speed) using Stamina. Stamina recovers when walking/idle.
- **`E` Key — Speed Surge:** +50% movement speed boost for 5s (Cooldown: 10s). *Requires Speed Boots.*
- **`Q` Key — Coin Magnet Radar:** Magnetic suction vortex attracting all coins across lanes to the player for 6s (Cooldown: 10s). *Requires Coin Magnet Radar.*
- **`T` Key — Time Freeze Clock:** Freezes all traffic and obstacles for 5s (Cooldown: 12s). *Requires Time Extender.*

### 3. 🎁 In-Lane Power-Ups & Item Shop
- **In-Lane Power-Ups (Floating Collectibles):**
  - 🧲 **Magnet (Blue):** Pulls all coins across wide range into the player (8s).
  - ⏱️ **Time Stop (Clock):** Freezes 85% of traffic & hazards (4s).
  - 🔥 **Speed Boost (Flame):** +50% Speed surge with lightning/after-image trails (6s).
  - 🛡️ **Shield (Orb):** +1 Energy Shield (fully synchronized with Shop inventory and HUD counter).
  - ⭐ **2X Coins & Score (Coin):** Doubles score earnings and gold coin rewards (10s).
- **Item Shop (Menu & In-Game Level Clear):**
  - **Energy Shield (250c):** Blocks 1 fatal hit + knockback (Consumable, stacks with in-lane pickups).
  - **Speed Boots (400c):** +15% Passive speed & unlocks `E` Skill.
  - **Time Extender (350c):** +8s Level time limit & unlocks `T` Skill.
  - **Coin Magnet Radar (500c):** Pulls all coins & unlocks `Q` Skill.
  - **Extra Heart (1500c):** +1 Extra Life / Restores player HP.
  - *3 Independent Save/Profile Slots for shop inventory and coins.*

### 4. ♾️ Endless Chrono Mode & Campaign
- **Story Campaign:** 5 historical eras with cinematic era elevator transitions.
- **Endless Chrono Mode:** Infinite wave progression through historical eras with escalating speed, score multipliers, and dynamic hazards.

### 5. 🎬 Cinematic Cutscene System
- **Elevator Era Cutscenes:** Seamless transition phase (`DOOR_CLOSING` ➔ `MOVING` ➔ `MORPHING` ➔ `DOOR_OPENING`) with era info cards and transformation ding.
- **5 Level-Specific Death Cutscenes:**
  - **Level 1:** Prehistoric swamp monster emerges, opens a jaw, and swallows the player.
  - **Level 2:** Golden sand explosion erupts, sinking the player into quicksand.
  - **Level 3:** A leaf whirlwind spins over the player, lifting and whisking them away.
  - **Level 4:** Urban smoke explosion envelops and dissolves the player.
  - **Level 5:** Sci-Fi plasma energy ball disintegrates the player into light particles.
  - *Includes Dynamic Screen Shake, Player Occlusion/Hiding, Screen Fade Overlay, and Level-Specific SFX (`death_1.ogg` to `death_5.ogg`).*

### 6. 🚦 Traffic Signal System
- Red lights temporarily halt obstacle movement across lanes, opening safe windows for passage.

### 7. 💾 Save & Load System
- **3 dedicated save slots** with custom save file names.
- Saves level progress, score, coins, inventory, and player position.
- Quick save (`F1`-`F3`) and Quick load (`F4`-`F6`) hotkeys.

### 8. 🏆 High Score & Audio Control
- High score file persistence (`HighScore.txt`).
- Full SFX and BGM volume controls via Pause Menu and Settings sliders.

---

## 🎮 Controls

| Key | Action |
| :--- | :--- |
| **`W` / `A` / `S` / `D`** | Move Player (Up / Left / Down / Right) |
| **`Arrow Keys (↑ ← ↓ →)`** | Move Player (Up / Left / Down / Right) |
| **`Shift` (Hold)** | Sprint (+50% Speed, consumes Stamina) |
| **`E`** | Activate Speed Surge Skill (Requires Speed Boots) |
| **`Q`** | Activate Radar EMP Pulse Skill (Requires Hazard Radar) |
| **`T`** | Activate Time Freeze Skill (Requires Time Extender) |
| **`P`** | Pause / Resume Game |
| **`R`** | Quick Restart Level |
| **`ESC`** | Open Quit Confirmation / Go Back |
| **`M`** | Open Return to Main Menu Confirmation |
| **`S`** | Open Item Shop (on Level Clear screen) |
| **`Space` / `Enter`** | Skip Elevator / Death Cutscene |
| **`F1` / `F2` / `F3`** | Quick Save to Slot 1 / Slot 2 / Slot 3 |
| **`F4` / `F5` / `F6`** | Quick Load from Slot 1 / Slot 2 / Slot 3 |
| **`F11` / `Alt+Enter`** | Toggle Fullscreen Mode |
| **`V`** | Toggle Hitbox Debug Overlay |

---

## 🗺️ Levels Summary

| Level | Era | Player | Ground Obstacles | Flying Objects | Death Cutscene |
| :---: | :--- | :--- | :--- | :--- | :--- |
| **1** | Prehistoric | Caveman | Dinosaurs, Mammoths | Ancient Birds | Monster Chomp |
| **2** | Ancient Egypt | Egyptian | Camels, Sphinxes | Eagles, Horus Birds | Quicksand Sink |
| **3** | Medieval | Knight | Rickshaws, Horses, Logs | Volley Arrows, Crows | Leaf Whirlwind |
| **4** | Modern City | Civilian | Cars, Motorbikes, Buses | Planes, Missiles | Smoke Dissolve |
| **5** | Cyber Future | Cyborg | Electric Cars, Electric Trains | Flying Cars, UFOs, Drones | Plasma Disintegration |

---

## 🏗️ Architecture & OOP Design

The codebase follows **Clean Modular Architecture** and Object-Oriented Programming principles:

```text
src/
├── main.cpp                # Application entry point
├── core/                   # Main engine loop & system managers
│   ├── CGAME.h/.cpp        # Game loop, event handling, rendering, state logic
│   ├── SaveData.h/.cpp    # Save slot file I/O
│   ├── SoundManager.h/.cpp# Audio system (BGM, SFX, Death Sounds)
│   └── TextureManager.h/.cpp # Texture cache manager
├── entities/               # Game objects and entities
│   ├── base/               # Base classes (CPEOPLE, CANIMAL, CVEHICLE, CTRAFFICLIGHT)
│   ├── EntityManager.h/.cpp# Entity lifecycle and collision management
│   └── obstacles/          # Level-specific obstacle implementations (lv1 - lv5)
├── cutscenes/              # Cinematic animation modules
│   ├── ElevatorCutscene.h/.cpp # Era elevator transition
│   ├── DeathCutscene.h/.cpp    # Level-tailored death cutscenes
│   ├── CollisionEffect.h/.cpp  # Animated collision sprites
│   └── Animation.h/.cpp       # Frame animation processing
├── levels/                 # Level configuration & mapping
│   └── LevelConfig.h/.cpp
├── ui/                     # User interface components
│   ├── HUD.h/.cpp          # Score & timer HUD bar
│   ├── Menu.h/.cpp         # Main menu & pause screens
│   ├── MenuButton.h        # Interactive UI buttons
│   └── UIText.h            # Styled text helpers
└── utils/                  # Utility helpers
    ├── Utils.h             # Global constants & helpers
    └── HighScore.h/.cpp    # High score file I/O
```

### Applied OOP Concepts:
1. **Encapsulation:** Internal state (Player, Audio, Cutscene Phase) is protected with accessor/mutator methods.
2. **Inheritance:** Base classes `CANIMAL`, `CVEHICLE`, and `CTRAFFICLIGHT` provide common interfaces for level-specific obstacles (`CDINOSOUR`, `CCAR`, `CUFO`, etc.).
3. **Polymorphism:** `Draw()`, `Move()`, `update()`, and `loadSprite()` are overridden in derived classes for unique movement and animation behaviors.
4. **Abstraction:** `EntityManager` abstracts complex entity spawning, updating, and collision detection away from `CGAME`.
5. **Factory Method Pattern:** `createObstacle()` and `createAnimal()` instantiate level entities based on level configuration.

---

## 🛠️ Build & Run Instructions

### macOS

**Prerequisites:** `sfml@2`, `cmake`, `clang`

1. Install dependencies via Homebrew:
   ```bash
   brew install sfml@2 cmake
   ```
2. Build and run:
   ```bash
   chmod +x build.sh
   ./build.sh run
   ```
3. Clean rebuild:
   ```bash
   ./build.sh clean run
   ```

---

### Windows

**Prerequisites:** Visual Studio 2022 (C++ Desktop Development), SFML 2.6.2, CMake 3.16+

1. Open Command Prompt in the project root directory.
2. Run the build script:
   ```cmd
   build.bat run
   ```

---

## 👥 Team Members

| Student | Responsibility |
| :--- | :--- |
| **Phan Gia Huy** | CGAME, SaveData, Animation, LevelConfig, DeathCutscene |
| **Nguyễn Hoàng Nhật** | CPEOPLE, obstacles, backgrounds |
| **Nguyễn Văn Hải** | CANIMAL, sounds, flying objects |
| **Phan Đình Quốc** | Menu, HUD, UI |

---

## 📚 Course Details

- **Course:** Object-Oriented Programming
- **Faculty:** Faculty of Information Technology
- **University:** Ho Chi Minh City University of Science (HCMUS)
