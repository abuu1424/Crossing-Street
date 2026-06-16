#pragma once
#include <string>
#include <vector>

struct SaveSlot {
    int slot = 0;
    int level = 1;
    int score = 0;
    float playerX = 0.f;
    float playerY = 0.f;
    std::string dateTime = "";
    std::string saveName = "";
    bool isEmpty = true;
};

class SaveData {
private:
    static const int MAX_SLOTS = 3;
    static const std::string SAVE_DIR;

    static bool isValidSlot(int slot);
    static std::string getFilePath(int slot);
    static std::string getCurrentDateTime();

public:
    static bool save(
        int slot,
        int level,
        int score,
        float playerX,
        float playerY,
        const std::string& saveName
    );

    static bool load(int slot, int& level, int& score);
    static bool load(int slot, int& level, int& score, float& playerX, float& playerY);

    static bool deleteSlot(int slot);
    static bool hasData(int slot);
    static std::vector<SaveSlot> getAllSlots();
};