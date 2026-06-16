#pragma once
#include <string>
#include <vector>

struct SaveSlot {
    int         slot;       // slot số mấy
    int         level;      // level đang chơi
    int         score;      // điểm hiện tại
    std::string dateTime;   // ngày giờ lưu
    bool        isEmpty = true;
};

class SaveData {
    static const int MAX_SLOTS = 3;
    static const std::string SAVE_DIR;

public:
    // Lưu game vào slot
    static bool save(int slot, int level, int score);

    // Tải game từ slot
    static bool load(int slot, int& level, int& score);

    // Xóa slot
    static bool deleteSlot(int slot);

    // Đọc thông tin tất cả slot
    static std::vector<SaveSlot> getAllSlots();

    // Kiểm tra slot có dữ liệu không
    static bool hasData(int slot);

private:
    static std::string getFilePath(int slot);
    static std::string getCurrentDateTime();
};