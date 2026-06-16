#include "SaveData.h"

#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <filesystem>

const std::string SaveData::SAVE_DIR = "saves/";

bool SaveData::isValidSlot(int slot) {
    return slot >= 1 && slot <= MAX_SLOTS;
}

std::string SaveData::getFilePath(int slot) {
    return SAVE_DIR + "save_slot" + std::to_string(slot) + ".txt";
}

std::string SaveData::getCurrentDateTime() {
    time_t now = time(nullptr);
    char buf[32];

    tm* local = localtime(&now);
    if (local == nullptr) return "unknown";

    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", local);
    return std::string(buf);
}

bool SaveData::save(int slot, int level, int score, float playerX, float playerY, const std::string& saveName) {
    if (!isValidSlot(slot)) {
        printf("Invalid save slot %d\n", slot);
        return false;
    }

    std::filesystem::create_directories(SAVE_DIR);

    std::ofstream file(getFilePath(slot));
    if (!file.is_open()) {
        printf("FAILED to save slot %d\n", slot);
        return false;
    }

    file << "version=1\n";
    file << "slot=" << slot << "\n";
    file << "saveName=" << saveName << "\n";
    file << "level=" << level << "\n";
    file << "score=" << score << "\n";
    file << "playerX=" << playerX << "\n";
    file << "playerY=" << playerY << "\n";
    file << "datetime=" << getCurrentDateTime() << "\n";

    printf("Saved slot %d: level=%d score=%d player=(%.1f, %.1f)\n",
           slot, level, score, playerX, playerY);

    return true;
}

bool SaveData::load(int slot, int& level, int& score) {
    float playerX = 0.f;
    float playerY = 0.f;
    return load(slot, level, score, playerX, playerY);
}

bool SaveData::load(int slot, int& level, int& score, float& playerX, float& playerY) {
    if (!isValidSlot(slot)) {
        printf("Invalid load slot %d\n", slot);
        return false;
    }

    std::ifstream file(getFilePath(slot));
    if (!file.is_open()) {
        printf("No save data in slot %d\n", slot);
        return false;
    }

    bool hasLevel = false;
    bool hasScore = false;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string key, value;

        std::getline(ss, key, '=');
        std::getline(ss, value);

        if (value.empty()) continue;

        try {
            if (key == "level") {
                level = std::stoi(value);
                hasLevel = true;
            }
            else if (key == "score") {
                score = std::stoi(value);
                hasScore = true;
            }
            else if (key == "playerX") {
                playerX = std::stof(value);
            }
            else if (key == "playerY") {
                playerY = std::stof(value);
            }
        }
        catch (...) {
            printf("Save slot %d has invalid data\n", slot);
            return false;
        }
    }

    if (!hasLevel || !hasScore) {
        printf("Save slot %d is missing level or score\n", slot);
        return false;
    }

    printf("Loaded slot %d: level=%d score=%d player=(%.1f, %.1f)\n",
           slot, level, score, playerX, playerY);

    return true;
}

bool SaveData::deleteSlot(int slot) {
    if (!isValidSlot(slot)) return false;
    return std::remove(getFilePath(slot).c_str()) == 0;
}

bool SaveData::hasData(int slot) {
    if (!isValidSlot(slot)) return false;

    std::ifstream file(getFilePath(slot));
    return file.is_open();
}

std::vector<SaveSlot> SaveData::getAllSlots() {
    std::vector<SaveSlot> slots(MAX_SLOTS);

    for (int i = 0; i < MAX_SLOTS; i++) {
        int slotNumber = i + 1;
        slots[i].slot = slotNumber;

        std::ifstream file(getFilePath(slotNumber));
        if (!file.is_open()) {
            slots[i].isEmpty = true;
            continue;
        }

        slots[i].isEmpty = false;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string key, value;

            std::getline(ss, key, '=');
            std::getline(ss, value);

            if (value.empty()) continue;

            try {
                if (key == "level") slots[i].level = std::stoi(value);
                else if (key == "score") slots[i].score = std::stoi(value);
                else if (key == "playerX") slots[i].playerX = std::stof(value);
                else if (key == "playerY") slots[i].playerY = std::stof(value);
                else if (key == "datetime") slots[i].dateTime = value;
                else if (key == "saveName") slots[i].saveName = value;
            }
            catch (...) {
                slots[i].isEmpty = true;
            }
        }
    }

    return slots;
}