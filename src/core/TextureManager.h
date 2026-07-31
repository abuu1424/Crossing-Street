#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class TextureManager {
public:
    static TextureManager& getInstance();

    // Trả về reference tới sf::Texture đã cache. Nếu chưa có, nạp từ file đĩa.
    const sf::Texture& getTexture(const std::string& filename);

    // Xóa bộ nhớ tạm khi cần
    void clearCache();

private:
    TextureManager() = default;
    ~TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> mTextures;
};
