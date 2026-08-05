#pragma once
#include <string>

class HighScore {
public:
    static int load();
    static void save(int score);
    static bool updateIfHigher(int score);

private:
    static std::string getFilePath();
};