#include "TextureManager.h"
#include <cstdio>

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

const sf::Texture& TextureManager::getTexture(const std::string& filename) {
    auto it = mTextures.find(filename);
    if (it != mTextures.end()) {
        return *it->second;
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filename)) {
        printf("TextureManager ERROR: Failed to load %s\n", filename.c_str());
    }

    auto& entry = mTextures[filename];
    entry = std::move(texture);
    return *entry;
}

void TextureManager::clearCache() {
    mTextures.clear();
}
