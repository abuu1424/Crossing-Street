#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Các hàm helper nhỏ dùng chung để tạo sf::Text đã canh giữa (origin ở tâm).
// Trước đây mỗi Text trong CGAME::setupUI() lặp lại cùng 1 pattern:
//   setFont / setString / setCharacterSize / setFillColor / getLocalBounds /
//   setOrigin / setPosition
// Gói lại thành 1-2 hàm giúp setupUI() ngắn hơn nhiều và tránh copy-paste sai.
namespace ui {

inline void centerOrigin(sf::Text& text) {
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
}

// Tạo text đã canh giữa tại (x, y).
inline sf::Text makeCenteredText(const sf::Font& font,
                                  const std::string& str,
                                  unsigned int size,
                                  sf::Color color,
                                  float x, float y) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    centerOrigin(text);
    text.setPosition(x, y);
    return text;
}

// Cập nhật nội dung 1 text đã tồn tại rồi canh giữa lại (dùng khi text đổi
// theo runtime, ví dụ "SCORE: " + std::to_string(score)).
inline void setCenteredString(sf::Text& text, const std::string& str) {
    text.setString(str);
    centerOrigin(text);
}

} // namespace ui
