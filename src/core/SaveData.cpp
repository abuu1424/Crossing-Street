#include "SaveData.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>

const std::string SaveData::SAVE_DIR = "saves/";

std::string SaveData::getFilePath(int slot) {
    return SAVE_DIR + "save_slot" + std::to_string(slot) + ".txt";
}

std::string SaveData::getCurrentDateTime() {
    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", localtime(&now));
    return std::string(buf);
}

bool SaveData::save(int slot, int level, int score) {
    std::ofstream file(getFilePath(slot));
    if (!file.is_open()) {
        printf("FAILED to save slot %d\n", slot);
        return false;
    }

    file << "slot="     << slot              << "\n";
    file << "level="    << level             << "\n";
    file << "score="    << score             << "\n";
    file << "datetime=" << getCurrentDateTime() << "\n";

    file.close();
    printf("Saved slot %d: level=%d score=%d\n", slot, level, score);
    return true;
}

bool SaveData::load(int slot, int& level, int& score) {
    std::ifstream file(getFilePath(slot));
    if (!file.is_open()) {
        printf("No save data in slot %d\n", slot);
        return false;
    }

    std::string line, key, value;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::getline(ss, key,   '=');
        std::getline(ss, value, '=');

        if      (key == "level") level = std::stoi(value);
        else if (key == "score") score = std::stoi(value);
    }

    file.close();
    printf("Loaded slot %d: level=%d score=%d\n", slot, level, score);
    return true;
}

bool SaveData::deleteSlot(int slot) {
    return std::remove(getFilePath(slot).c_str()) == 0;
}

bool SaveData::hasData(int slot) {
    std::ifstream file(getFilePath(slot));
    return file.is_open();
}

std::vector<SaveSlot> SaveData::getAllSlots() {
    std::vector<SaveSlot> slots(MAX_SLOTS);

    for (int i = 0; i < MAX_SLOTS; i++) {
        slots[i].slot = i + 1;

        std::ifstream file(getFilePath(i + 1));
        if (!file.is_open()) {
            slots[i].isEmpty = true;
            continue;
        }

        slots[i].isEmpty = false;
        std::string line, key, value;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::getline(ss, key,   '=');
            std::getline(ss, value, '=');

            if      (key == "level")    slots[i].level    = std::stoi(value);
            else if (key == "score")    slots[i].score    = std::stoi(value);
            else if (key == "datetime") slots[i].dateTime = value;
        }
    }
    return slots;
}