#pragma once
#include <SFML/Graphics.hpp>
#include <string>


namespace ui {

inline void centerOrigin(sf::Text& text) {
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
}

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

inline void setCenteredString(sf::Text& text, const std::string& str) {
    text.setString(str);
    centerOrigin(text);
}

}
