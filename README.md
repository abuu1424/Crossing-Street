# Crossing Street

A 2D road-crossing game developed in C++17 using SFML 2.6.2 for the Object-Oriented Programming course at HCMUS.

## Features

* 5 historical-themed levels:

    * Prehistoric
    * Ancient
    * Medieval
    * Modern
    * Future
* Animated sprites
* Traffic light system
* Save / Load with 3 slots
* Score and timer system
* Pause menu
* Dead / Victory screens
* Background music and sound effects
* Cross-platform support (Windows & macOS)

## Controls

| Key                         | Action              |
|-----------------------------| ------------------- |
| W / A / S / D or Arrow keys | Move                |
| Arrow Keys                  | Move                |
| P                           | Pause / Resume      |
| R                           | Restart Level       |
| ESC                         | Quit Dialog         |
| F1 / F2 / F3                | Save Slot 1 / 2 / 3 |
| F4 / F5 / F6                | Load Slot 1 / 2 / 3 |

## Project Structure

```text
src/
├── core/
├── vehicle/
├── animals/
├── levels/
├── ui/
└── main.cpp

assets/
saves/
```

## OOP Concepts Used

* Encapsulation
* Inheritance
* Polymorphism
* Abstraction
* Factory Method

## Build

### Windows

Requirements:

* Visual Studio 2022
* SFML 2.6.2
* CMake 3.16+

```bash
build.bat run
```

### macOS

Requirements:

```bash
brew install sfml@2 cmake
```

Build and run:

```bash
chmod +x build.sh
./build.sh run
```

## Team Members

| Student           | Responsibility                          |
| ----------------- | --------------------------------------- |
| Phan Gia Huy      | CGAME, SaveData, Animation, LevelConfig |
| Nguyễn Hoàng Nhật | CPEOPLE, obstacles, backgrounds         |
| Nguyễn Văn Hải    | CANIMAL, sounds, flying objects         |
| Phan Đình Quốc    | Menu, HUD, UI                           |

## Course

Object-Oriented Programming
Faculty of Information Technology
Ho Chi Minh City University of Science (HCMUS)
