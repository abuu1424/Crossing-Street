# 🎮 CROSSING STREET: TIME ODYSSEY ⏳

> **Course Project: Object-Oriented Programming (OOP)**  
> **Faculty of Information Technology — Ho Chi Minh City University of Science, VNU-HCM (HCMUS)**

<div align="center">

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/Library-SFML_2.6.2-8CC651?style=for-the-badge&logo=sfml)
![CMake](https://img.shields.io/badge/Build-CMake_3.16%2B-064F8C?style=for-the-badge&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-macOS_%7C_Windows-blue?style=for-the-badge)
![Status](https://img.shields.io/badge/Release-v1.0_Final-success?style=for-the-badge)

</div>

---

## 📖 1. Project Overview

**Crossing Street: Time Odyssey** is a feature-rich 2D time-crossing arcade game developed in **C++17** and **SFML 2.6.2 / 2.5**.

Players guide their character across dangerous, multi-lane traffic while traveling across **5 distinct historical eras**, from the prehistoric wilderness to a cybernetic future. Engineered around clean Object-Oriented Architecture and SOLID principles, the game offers a diverse set of gameplay modes including a cinematic Story Campaign, split-keyboard Local 2-Player Versus, a smart adversary powered by a **Spatio-Temporal A\*** AI algorithm, active cooldown-based skills, in-lane collectible power-ups, an item shop economy, dynamic environment hazards, and custom animated death cutscenes.

---

## 👥 2. Executive Development Team

| Team Member | Engineering Role | Core Modules & Technical Contributions |
| :--- | :--- | :--- |
| **Phan Gia Huy** | **Lead Architect & AI Engineer** | • **Core Modules:** `CGAME`, `BotAI` (Spatio-Temporal A*), `SaveData`, `LevelConfig`, `ElevatorCutscene`, `DeathCutscene`<br>• Designed the engine architecture, multi-state game loop (State Machine), 16.6Hz real-time obstacle avoidance bot pathfinding, cinematic elevator transitions, multi-slot save serialization, and memory management. |
| **Nguyễn Văn Hải** | **Audio & Entity Simulation Engineer** | • **Core Modules:** `CANIMAL` (Base & Derived), `SoundManager`, Dynamic Audio Ducking<br>• Implemented the adaptive multi-channel sound engine with real-time dynamic ducking, animal AI movement patterns, flying projectiles (20+ entities), and historical era ambient soundscapes. |
| **Phan Đình Quốc** | **UI/UX Director & Systems Designer** | • **Core Modules:** `Menu` Systems, `HUD` Telemetry, `ShopData`, Item Shop, High Scores, Movie Roll Credits<br>• Created multi-screen stateful menu navigation, real-time dual telemetry HUD, persistent multi-slot save system, shop economy modal dialogs, and the movie-roll cinematic credits system. |
| **Nguyễn Hoàng Nhật** | **World Artist & Gameplay Programmer** | • **Core Modules:** `CPEOPLE`, Obstacles & Traffic (`CVEHICLE`, `CTRAFFICLIGHT`), `HazardManager`, `CollisionEffect`<br>• Developed player physics and active skill mechanics (Sprint, Speed Surge, Magnet Radar, Time Freeze), dynamic cataclysm hazard systems, particle collision VFX, and 5 era-tailored death cutscenes. |

---

## 🕹️ 3. Game Modes

### 🏛️ 1. Story Campaign (5 Historical Eras)
- Journey through **5 distinct chronological eras**, each featuring custom backgrounds, obstacles, speeds, ambient audio, and level timers.
- Smooth mechanical **Elevator Cutscenes** seamlessly transition players between timelines.
- Custom animated **Death Cutscenes** tailored to the unique theme of each era upon player elimination.

### ⚔️ 2. Local 2-Player Versus (1v1 Split Keyboard)
- Shared keyboard split-screen competition (`WASD` for Player 1 vs `Arrow Keys` for Player 2).
- Independent health bars (HP), stamina meters, skill cooldowns, and round win counters (`P1` vs `P2`).
- Real-time power-up contest in traffic lanes with instant rematch capability (`R`).

### 🤖 3. VS Bot AI (Smart Adversary)
- Race against an autonomous AI opponent powered by **Spatio-Temporal A\*** (space-time search with forward trajectory prediction):
  - **Easy:** Relaxed pace, safe crossings, beginner-friendly.
  - **Normal:** Matches player speed, performs lane-gap timing, and actively collects power-ups.
  - **Hard:** 16.6Hz lookahead updates, micro-dodging, zero mistakes, and aggressive path optimization.
- Real-time dual telemetry HUD tracking bot status (`RACING`, `FROZEN`, `ELIMINATED`), health, and progress.

### ♾️ 4. Endless Chrono Mode
- Infinite wave progression across eras with escalating score multipliers.
- Dynamic hazards and obstacle speeds scale exponentially as waves progress.
- High score persistence with highest wave reached saved to disk.

---

## ⚡ 4. Active Skills & Items

### 🔮 Active Character Skills
| Skill | P1 Key | P2 Key | Duration | Cooldown | Effect Details |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Sprint** | `LShift` | `RShift` / `RCtrl` | Hold key | Auto-recharge | +50% movement speed consuming Stamina. Recharges when walking or idle. |
| **Speed Surge** | `E` / `Space` | `I` / `[` | 5.0s | 10.0s | +50% speed boost with lightning aura and trail VFX. |
| **Coin Magnet Radar** | `Q` | `K` / `]` / `O` | 6.0s | 10.0s | Emits a radar pulse that pulls all coins within 700px and slows nearby hazards. |
| **Time Freeze Clock** | `T` | `L` / `\` | 5.0s | 12.0s | Freezes time, paralyzing all vehicles, animals, and hazards across the map. |

### 🎁 In-Lane Floating Collectibles
- 🧲 **Magnet Orb (Blue):** Pulls all coins on the map toward the player for 8s.
- ⏱️ **Time Stop (Clock):** Slows down 85% of traffic, animals, and hazards for 4s.
- 🔥 **Speed Boost (Flame):** +60% movement speed with an energetic flame trail for 6s.
- 🛡️ **Bubble Shield (Orb):** Grants complete immunity against 1 fatal collision + knockback protection.
- ⭐ **2X Multiplier (Medal):** Doubles all gold and score points earned for 10s.

### 🛒 Item Shop (Main Menu & Level Clear)
- **Energy Shield (250 Coins):** Single-use consumable energy shield absorbing 1 fatal collision.
- **Speed Boots (400 Coins):** Permanent +15% base movement speed & unlocks the Speed Surge skill (`E`).
- **Time Extender (350 Coins):** +8s level time limit bonus & unlocks the Time Freeze skill (`T`).
- **Coin Magnet Radar (500 Coins):** -15% passive hazard speed slowdown & unlocks the Radar skill (`Q`).
- **Extra Heart (1500 Coins):** Expands maximum HP capacity and fully restores player health.

---

## 🌪️ 5. Historical Eras & Dynamic Level Hazards

| Level | Era | Ground Obstacles | Flying Objects | Catastrophic Hazard | Death Cutscene (VFX) |
| :---: | :--- | :--- | :--- | :--- | :--- |
| **Level 1** | **Prehistoric Jungle** | Tyrannosaurus Rex, Mammoths | Ancient Pterodactyls | **Dino Stampede Surge**<br>Herds stampede across lanes with screen-shaking seismic rumbles. | Devoured by Swamp Monster *(Swamp Monster Chomp)* |
| **Level 2** | **Ancient Egyptian Empire** | Desert Camels, Sphinx Statues | Horus Eagles, Vultures | **Sandstorm Wind Drift**<br>Violent sandstorms obscure vision and generate wind forces that drift the player. | Swallowed by Quicksand *(Quicksand Sink)* |
| **Level 3** | **Medieval Kingdom** | Warhorses, Wooden Logs, Rickshaws | Arrow Volleys, Raven Flocks | **Flaming Arrow Rain**<br>Flaming arrows rain down from above targeting red warning reticles. | Swept away in autumn leaves *(Leaf Whirlwind)* |
| **Level 4** | **Modern Metropolis** | Supercars, Transit Buses, Motorbikes | Jetliners, Guided Missiles | **Rush Hour & Thunderstorm**<br>Sudden traffic surges coupled with lethal vertical lightning strikes. | Dissolved into dark smoke *(Smoke Dissolve)* |
| **Level 5** | **Cybernetic Future** | Smart Electric Cars, Hyperloop Trains | Alien UFOs, Combat Drones, Flying Cars | **Singularity Black Hole & Lasers**<br>Gravitational singularity pulls entities while lethal laser beams sweep across lanes. | Disintegrated by Plasma *(Plasma Disintegration)* |

---

## ⌨️ 6. Controls Guide

### Gameplay Controls
| Key | Action / Function |
| :--- | :--- |
| **`W` `A` `S` `D`** | Player 1 / Single Player Movement (Up / Left / Down / Right) |
| **`LShift`** | Player 1: Sprint (Consumes Stamina) |
| **`E` / `Space`** | Player 1: Speed Surge Skill |
| **`Q`** | Player 1: Coin Magnet Radar Skill |
| **`T`** | Player 1: Time Freeze Clock Skill |
| **`1` `2` `3` `4`** | Inventory Quick Use (Shield, Boots, Time Extender, Radar) |
| **`↑` `←` `↓` `→`** | Player 2 Movement (2-Player Mode) |
| **`RShift` / `RCtrl` / `Num 0`** | Player 2: Sprint (2-Player Mode) |
| **`I` / `[`** | Player 2: Speed Surge Skill |
| **`K` / `]` / `O`** | Player 2: Coin Magnet Radar Skill |
| **`L` / `\`** | Player 2: Time Freeze Clock Skill |

### System & Utility Hotkeys
| Key | Action / Function |
| :--- | :--- |
| **`P`** | Pause Game / Audio Settings (BGM & SFX Sliders) |
| **`R`** | Quick Restart / Rematch Current Level |
| **`S`** | Open Item Shop (Level Clear / Victory Screen) |
| **`M`** | Open Return to Main Menu Confirmation |
| **`ESC`** | Back / Open Quit Confirmation Dialog |
| **`Enter` / `Space`** | Skip Cutscene |
| **`F1` — `F3`** | Quick Save to Slot 1 — Slot 3 |
| **`F4` — `F6`** | Quick Load from Slot 1 — Slot 3 |
| **`F11` / `Alt + Enter`** | Toggle Fullscreen Mode |
| **`V`** | Toggle Hitbox Debug Overlay |

---

## 🛠️ 7. Developer Console & Cheat Codes

> Press **`~`** (or `` ` ``, `F8`, `F9`, `F12`, `Tab`, `]`, `/`) during gameplay to toggle the interactive Developer Command Console.

| Command | Description & Execution Result |
| :--- | :--- |
| `god` / `invuln` | Toggle God Mode *(Infinite Invulnerability)* |
| `coins <Amount>` | Add coins to the active profile (e.g. `coins 5000`) |
| `hp <Amount>` | Set player maximum HP between 1 and 10 hearts (e.g. `hp 5`) |
| `heal` | Fully restore player HP and Stamina energy |
| `level <1-5>` / `lvl <1-5>` | Teleport immediately to the specified level (e.g. `level 5`) |
| `skill all` / `buy all` | Unlock all shop items and enable all active skills |
| `nrg` / `energy` | Instantly refill Stamina energy to 100% |
| `score <Amount>` | Add points directly to the current game score (e.g. `score 1000`) |
| `help` | Display list of all available console commands |

---

## 🎬 8. Cinematic Movie Roll Credits

Select **CREDITS** from the Main Menu to view the cinematic rolling credits sequence:
- **Smooth Auto-Scroll:** Features university credentials, OOP faculty acknowledgments, development team portraits, module breakdowns, and special thanks.
- **Interactive Controls:**
  - `Space` or on-screen button: Toggle scroll speed between **1X / 3X**.
  - `P`: Pause / Resume scrolling.
  - `R`: Restart credits roll from the beginning.
  - `Mouse Scroll` or `W`/`S` (`↑`/`↓`): Manual position scrolling.
  - `ESC`: Return to Main Menu.

---

## 🏗️ 9. Software Architecture & OOP Design

### Project Directory Structure
```text
Crossing Street/
├── CMakeLists.txt              # Cross-platform CMake build configuration
├── build.sh                    # Automated build & launch script for macOS
├── build.bat                   # Automated build & launch script for Windows
├── assets/                     # Game textures, audio, fonts, and UI assets
│   ├── background/             # 5 Era background images
│   ├── credits/                # Team member avatar portraits
│   ├── font/                   # Pixel Operator font family
│   ├── hazards/                # 12-frame animated hazard spritesheets
│   ├── shop/                   # Shop icons, skill VFX animations
│   ├── sounds/                 # Era-specific BGM tracks and SFX clips
│   ├── sprites/                # Characters, vehicles, animals, and airborne obstacles
│   └── ui/                     # Buttons, dialog boxes, HUD textures
├── saves/                      # Persistent save slots & high score storage
└── src/                        # C++ Source and Header Files
    ├── main.cpp                # Application entry point
    ├── ai/                     # Artificial Intelligence
    │   ├── BotAI.h/.cpp        # Spatio-Temporal A* pathfinding & hazard lookahead
    ├── core/                   # Game Engine Core
    │   ├── CGAME.h/.cpp        # Main game loop, state machine, rendering, dev console
    │   ├── SaveData.h/.cpp     # Save/load file serialization (Slots 1-3)
    │   ├── SoundManager.h/.cpp # Multi-channel audio engine with dynamic ducking
    │   └── TextureManager.h/.cpp # Texture resource cache and memory manager
    ├── cutscenes/              # Cinematic Sequences & Animations
    │   ├── Animation.h/.cpp    # Spritesheet frame animation controller
    │   ├── ElevatorCutscene.h/.cpp # Mechanical time elevator transitions
    │   ├── DeathCutscene.h/.cpp    # 5 era-tailored animated death cutscenes
    │   └── CollisionEffect.h/.cpp  # Particle explosion effects upon collision
    ├── entities/               # Entity Model Hierarchy
    │   ├── base/               # Base classes (CPEOPLE, CANIMAL, CVEHICLE, CTRAFFICLIGHT, PlayerStats)
    │   ├── EntityManager.h/.cpp# Lane lifecycle, entity spawning & collision detection
    │   ├── CoinManager.h/.cpp  # Coin distribution & magnetic suction physics
    │   ├── PowerUpManager.h/.cpp # Lane collectibles & active buff timers
    │   └── obstacles/          # Era-specific derived entity classes (lv1 -> lv5)
    ├── hazards/                # Catastrophic Dynamic Hazards
    │   └── HazardManager.h/.cpp# Stampede, Sandstorm, Arrow Rain, Thunderstorm, Black Hole
    ├── levels/                 # Level Configuration
    │   └── LevelConfig.h/.cpp  # Per-era parameters, speeds, lane setups, and audio
    ├── ui/                     # User Interface
    │   ├── HUD.h/.cpp          # Dual telemetry HUD, health, stamina, timers, scores
    │   ├── Menu.h/.cpp         # Main menu, pause, challenges, shop, settings, credits
    │   ├── MenuButton.h        # Interactive UI buttons with hover scaling
    │   └── UIText.h            # Styled text helpers and alignment utilities
    └── utils/                  # Utility Services
        ├── HighScore.h/.cpp    # High score file I/O persistence
        ├── ShopData.h/.cpp     # Economic state, purchases, and slot persistence
        └── Utils.h             # Global constants, window dimensions & timing
```

### Applied OOP Principles & Design Patterns:
1. **Encapsulation:** Internal properties of characters (`CPEOPLE`), audio engines (`SoundManager`), entities (`CANIMAL`, `CVEHICLE`), and shop state (`ShopData`) are strictly protected via private members with controlled public interfaces.
2. **Inheritance:** Derived obstacles across all 5 eras inherit from abstract base classes `CANIMAL` and `CVEHICLE`.
3. **Polymorphism:** Dynamic virtual dispatch (`virtual void Draw()`, `virtual void Move()`, `virtual void update()`) enables `EntityManager` to uniformly process dozens of heterogeneous entities within shared lane structures.
4. **Abstraction & Decoupling:** `CGAME` is decoupled from low-level subsystem logic through specialized managers (`HazardManager`, `PowerUpManager`, `CoinManager`, `EntityManager`).
5. **Design Patterns:**
   - **Singleton Pattern:** Global resource management via `TextureManager`.
   - **Factory Pattern:** Dynamic instantiation of era-specific obstacles and configurations via `LevelConfig`.
   - **State Machine Pattern:** Smooth transitions across states: Main Menu, In-Game, Paused, Shop, Elevator Cutscene, Dying, Victory, and Credits.

---

## 💻 10. Build & Run Instructions

### 🍎 macOS

**Prerequisites:** `cmake`, `clang`, and `sfml@2` installed via Homebrew.

```bash
# 1. Install prerequisites via Homebrew (if not already installed)
brew install sfml@2 cmake

# 2. Grant execution permissions to build script
chmod +x build.sh

# 3. Build and launch the game
./build.sh run
```

*Useful build options:*
- `./build.sh` : Compile project only.
- `./build.sh clean` : Remove previous build artifacts.
- `./build.sh clean run` : Perform clean rebuild and run.

---

### 🪟 Windows

**Prerequisites:** Visual Studio 2022 (with *Desktop development with C++* workload), CMake 3.16+, and SFML 2.6.2 (located at `C:\SFML-2.6.2` or referenced by the `SFML_DIR` environment variable).

```cmd
:: 1. Run automated CMake configuration, Release build, and DLL deployment
build.bat run
```

*Useful build options:*
- `build.bat` : Build project binary to `build_win/Release/CrossingGame.exe`.
- `build.bat clean run` : Clean previous build, reconfigure, and run.

---

## 🏫 11. Academic Course Information

- **Course:** Object-Oriented Programming (OOP)
- **Department:** Faculty of Information Technology
- **Institution:** Ho Chi Minh City University of Science, VNU-HCM (HCMUS)
- **Academic Year:** 2026

---

<div align="center">
  <sub>Engineered with passion by the <b>Crossing Street Project Team — HCMUS</b>. Enjoy the timeline journey!</sub>
</div>
