#include "HighScore.h"
#include <fstream>
#include <filesystem>

std::string HighScore::getFilePath() {
    return "saves/highscore.txt";
}

int HighScore::load() {
    std::ifstream file(getFilePath());

    if (!file.is_open()) {
        return 0;
    }

    int highScore = 0;
    file >> highScore;
    return highScore;
}

void HighScore::save(int score) {
    std::filesystem::create_directories("saves");

    std::ofstream file(getFilePath());

    if (!file.is_open()) {
        return;
    }

    file << score;
}

bool HighScore::updateIfHigher(int score) {
    int currentHighScore = load();

    if (score > currentHighScore) {
        save(score);
        return true;
    }

    return false;
}