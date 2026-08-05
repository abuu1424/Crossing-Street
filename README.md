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

### 1. 🏛️ 5 Historical Era Levels
- **Level 1 — Prehistoric Era:** Dinosaurs, Mammoths, Ancient Birds, and Swamp Monsters.
- **Level 2 — Ancient Egypt:** Camels, Sphinxes, Egyptian Eagles, and Sand Bursts.
- **Level 3 — Medieval Era:** Rickshaws, Horses, Wooden Logs, Volley Arrows, and Leaf Whirlwinds.
- **Level 4 — Modern City:** Cars, Motorbikes, Buses, Planes, Missiles, and Smoke Explosions.
- **Level 5 — Cyber Future:** Electric Cars, Electric Trains, Flying Cars, UFOs, Drones, and Plasma Energy Spheres.

### 2. 🎬 Cinematic Cutscene System
- **Elevator Era Cutscenes:** Seamless transition phase (`DOOR_CLOSING` ➔ `MOVING` ➔ `MORPHING` ➔ `DOOR_OPENING`) with era info cards and transformation ding.
- **5 Level-Specific Death Cutscenes:**
  - **Level 1:** Prehistoric swamp monster emerges, opens a jaw, and swallows the player.
  - **Level 2:** Golden sand explosion erupts, sinking the player into quicksand.
  - **Level 3:** A leaf whirlwind spins over the player, lifting and whisking them away.
  - **Level 4:** Urban smoke explosion envelops and dissolves the player.
  - **Level 5:** Sci-Fi plasma energy ball disintegrates the player into light particles.
  - *Includes Dynamic Screen Shake, Player Occlusion/Hiding, Screen Fade Overlay, and Level-Specific SFX (`death_1.ogg` to `death_5.ogg`).*

### 3. 🚦 Traffic Signal System
- Red lights temporarily halt obstacle movement across lanes, opening safe windows for passage.

### 4. 💾 Save & Load System
- **3 dedicated save slots** with custom save file names.
- Saves level progress, score, and player position.
- Quick save (`F1`-`F3`) and Quick load (`F4`-`F6`) hotkeys.

### 5. 🏆 High Score & Audio Control
- High score file persistence (`HighScore.txt`).
- Full SFX and BGM volume controls via Pause Menu sliders.

---

## 🎮 Controls

| Key | Action |
| :--- | :--- |
| **`W` / `A` / `S` / `D`** | Move Player (Up / Left / Down / Right) |
| **`Arrow Keys (↑ ← ↓ →)`** | Move Player (Up / Left / Down / Right) |
| **`P`** | Pause / Resume Game |
| **`R`** | Restart Current Level (In-game / Game Over) |
| **`ESC`** | Open Quit Confirmation / Go Back |
| **`M`** | Open Return to Menu Confirmation |
| **`F1` / `F2` / `F3`** | Quick Save to Slot 1 / Slot 2 / Slot 3 |
| **`F4` / `F5` / `F6`** | Quick Load from Slot 1 / Slot 2 / Slot 3 |

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
