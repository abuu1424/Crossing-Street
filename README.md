c

# Crossing Street — Time Crossing Arcade Game

> **Object-Oriented Programming Course Project**
> **Faculty of Information Technology — Ho Chi Minh City University of Science (HCMUS)**

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=flat&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/Library-SFML_2.6.2-8CC651?style=flat&logo=sfml)
![CMake](https://img.shields.io/badge/Build-CMake_3.16%2B-064F8C?style=flat&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-macOS_%7C_Windows-blue)

---

## Overview

**Crossing Street** is a 2D road-crossing arcade game developed in **C++17** using **SFML 2.6.2**.

Players control a character navigating through hazardous lanes filled with unique obstacles (vehicles, prehistoric creatures, aircraft, UFOs) while traveling through **5 distinct historical eras**, from the Prehistoric era to the Cyber Future.

---

## Features

- **5 Historical Era Levels:**
  - **Level 1 — Prehistoric Era:** Dinosaurs, Mammoths, and Ancient Birds.
  - **Level 2 — Ancient Egypt:** Camels, Sphinxes, and Egyptian Eagles.
  - **Level 3 — Medieval Era:** Rickshaws, Horses, Wooden Logs, and Arrows.
  - **Level 4 — Modern City:** Cars, Motorbikes, Buses, Planes, and Missiles.
  - **Level 5 — Cyber Future:** Electric Cars, Electric Trains, Flying Cars, UFOs, and Drones.
- **Traffic Signal System:** Red lights temporarily stop obstacle movement, allowing safe passage.
- **Save & Load System:**
  - **3 dedicated save slots** with custom save file names.
  - Saves current level, score, and player position.
  - Quick save and quick load hotkeys (`F1` to `F6`).
- **Scoring & Countdown Timer:** Calculates score based on level difficulty and time bonus.
- **High Score Tracking:** Automatically saves and updates highest score records.
- **Audio Management:** Background music and sound effects with volume sliders in the Pause menu.
- **Cross-Platform Support:** Runs natively on macOS and Windows.

---

## Controls

| Key                                     | Action                                      |
| :-------------------------------------- | :------------------------------------------ |
| **`W` / `A` / `S` / `D`** | Move Player (Up / Left / Down / Right)      |
| **`Arrow Keys (↑ ← ↓ →)`**  | Move Player (Up / Left / Down / Right)      |
| **`P`**                         | Pause / Resume Game                         |
| **`R`**                         | Restart Current Level (In-game / Game Over) |
| **`ESC`**                       | Open Quit Confirmation / Go Back            |
| **`M`**                         | Open Return to Menu Confirmation            |
| **`F1` / `F2` / `F3`**      | Quick Save to Slot 1 / Slot 2 / Slot 3      |
| **`F4` / `F5` / `F6`**      | Quick Load from Slot 1 / Slot 2 / Slot 3    |

---

## Levels Summary

|    Level    | Era           | Player   | Ground Obstacles               | Flying Objects            | Traffic Signal    |
| :---------: | :------------ | :------- | :----------------------------- | :------------------------ | :---------------- |
| **1** | Prehistoric   | Caveman  | Dinosaurs, Mammoths            | Ancient Birds             | Torches           |
| **2** | Ancient Egypt | Egyptian | Camels, Sphinxes               | Eagles, Horus Birds       | Egyptian Braziers |
| **3** | Medieval      | Knight   | Rickshaws, Horses, Logs        | Volley Arrows, Crows      | Moon Phases       |
| **4** | Modern City   | Civilian | Cars, Motorbikes, Buses        | Planes, Missiles          | Traffic Lights    |
| **5** | Cyber Future  | Cyborg   | Electric Cars, Electric Trains | Flying Cars, UFOs, Drones | STOP / MOVE Sign  |

---

## Architecture & OOP Design

The codebase follows **Object-Oriented Programming** principles and clean architecture:

```text
src/
├── core/               # Main game loop, entities, audio, save/load
│   ├── CGAME.h/.cpp           # Game loop, event handling, rendering, state logic
│   ├── CPEOPLE.h/.cpp         # Main player character class
│   ├── EntityManager.h/.cpp   # Obstacle and animal entity management
│   ├── SoundManager.h/.cpp   # Music and SFX management
│   ├── SaveData.h/.cpp       # Save slot file I/O
│   ├── HighScore.h/.cpp      # High score file I/O
│   └── Animation.h/.cpp      # Sprite animation processing
├── vehicle/            # Base class CVEHICLE for ground obstacles
├── animals/            # Base class CANIMAL for flying objects
├── obstacles/          # Level-specific obstacle implementations
├── levels/             # Level configuration (positions, speeds, sprites)
├── ui/                 # HUD, Main Menu, Buttons, UI Text
└── main.cpp            # Application entry point
```

### Applied OOP Concepts:

1. **Encapsulation:** Object states (Player, Obstacles, Audio) are encapsulated with `private`/`protected` access modifiers and safe getter/setter methods.
2. **Inheritance:** `CVEHICLE` and `CANIMAL` act as base classes for specific obstacle implementations (`CCAR`, `CETRAIN`, `CDINOSOUR`, `CUFO`, etc.).
3. **Polymorphism:** `loadSprite()`, `Move()`, `update()`, and `Draw()` are overridden in derived classes for custom sprite orientation and animations.
4. **Abstraction:** `EntityManager` abstracts complex entity spawning and updating logic away from the main game loop.
5. **Factory Method Pattern:** `createObstacle()` and `createAnimal()` dynamically instantiate entities based on level configuration enums.

---

## Build & Run Instructions

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

## Team Members

| Student              | Responsibility                          |
| -------------------- | --------------------------------------- |
| Phan Gia Huy         | CGAME, SaveData, Animation, LevelConfig |
| Nguyễn Hoàng Nhật | CPEOPLE, obstacles, backgrounds         |
| Nguyễn Văn Hải    | CANIMAL, sounds, flying objects         |
| Phan Đình Quốc    | Menu, HUD, UI                           |

---

## Course Details

- **Course:** Object-Oriented Programming
- **Faculty:** Faculty of Information Technology
- **University:** Ho Chi Minh City University of Science (HCMUS)
