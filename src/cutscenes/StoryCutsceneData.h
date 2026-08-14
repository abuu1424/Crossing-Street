#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Phong cách hiển thị lời thoại
enum class DialogueStyle {
    NORMAL,     // Thoại thường (cạnh tên nhân vật)
    WHISPER,    // Thoại thì thầm / độc thoại nội tâm (mờ, chữ nghiêng, giữa màn hình)
    SYSTEM_AI   // Giọng đọc máy vô cảm của Hệ Thống (chữ xanh neon, viền khung)
};

// Loại sự kiện trong Timeline Cutscene
enum class CutsceneEventType {
    PANEL_SHOW,      // Đổi/Hiện panel ảnh minh họa (có hỗ trợ Ken Burns Zoom & Pan)
    DIALOGUE,        // Hiển thị lời thoại (hỗ trợ gõ từng chữ, thẻ pause)
    SOUND_VACUUM,    // Chân không âm thanh: Ngắt 100% tiếng động & BGM trong N giây
    PLAY_SFX,        // Phát hiệu ứng âm thanh (gió, bước chân, mưa tên, tim đập, piano...)
    LIGHTING_TINT,   // Biến đổi màu ánh sáng overlay mượt từ Color A -> Color B
    CAMERA_SHAKE,    // Hiệu ứng rung camera nhẹ
    DELAY            // Khoảng lặng/tạm dừng N giây giữa các sự kiện
};

// Sự kiện đơn lẻ trong Timeline
struct CutsceneEvent {
    CutsceneEventType type = CutsceneEventType::DELAY;

    // PANEL_SHOW parameters
    std::string imagePath;
    float zoomStart = 1.0f;
    float zoomEnd = 1.05f;
    sf::Vector2f panOffset = sf::Vector2f(0.f, 0.f);

    // DIALOGUE parameters
    std::string speaker;
    std::string text;
    DialogueStyle style = DialogueStyle::NORMAL;
    float pauseAfter = 1.0f; // Khoảng dừng sau câu thoại

    // SOUND parameters
    std::string soundPath;
    float soundVacuumDuration = 2.0f;

    // LIGHTING_TINT parameters
    sf::Color tintColor = sf::Color(0, 0, 0, 0);
    float tintDuration = 1.5f;

    // GENERAL parameters
    float duration = 1.0f;
};

// Kịch bản trọn vẹn cho 1 Chương
struct ChapterScript {
    int chapterNumber = 1;
    std::string chapterTitle;
    std::vector<CutsceneEvent> events;
};
