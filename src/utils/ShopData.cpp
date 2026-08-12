#include "ShopData.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>

int ShopData::sActiveSlot = 1;

void ShopData::setActiveSlot(int slot) {
  if (slot >= 1 && slot <= 3) {
    sActiveSlot = slot;
  }
}

int ShopData::getActiveSlot() {
  return sActiveSlot;
}

std::string ShopData::getFilePath(int slot) {
  int targetSlot = (slot >= 1 && slot <= 3) ? slot : sActiveSlot;
  if (targetSlot < 1 || targetSlot > 3) targetSlot = 1;

  std::string path = "saves/shopdata_slot" + std::to_string(targetSlot) + ".txt";

  // Backward compatibility migration: If legacy shopdata.txt exists and slot1 file doesn't, copy it
  if (targetSlot == 1 && !std::filesystem::exists(path) && std::filesystem::exists("saves/shopdata.txt")) {
    try {
      std::filesystem::copy_file("saves/shopdata.txt", path);
    } catch (...) {}
  }
  return path;
}

namespace {
std::string trimStr(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}
}

ShopState ShopData::load(int slot) {
  ShopState state;
  std::ifstream file(getFilePath(slot));
  if (!file.is_open()) {
    save(state, slot); // Create initial save file for this slot
    return state;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string key, value;
    if (!std::getline(ss, key, '=')) continue;
    std::getline(ss, value);
    key = trimStr(key);
    value = trimStr(value);
    if (value.empty())
      continue;

    try {
      if (key == "coins")
        state.coins = std::stoi(value);
      else if (key == "shieldCount")
        state.shieldCount = std::stoi(value);
      else if (key == "speedCount")
        state.speedCount = std::stoi(value);
      else if (key == "timeCount")
        state.timeCount = std::stoi(value);
      else if (key == "radarCount")
        state.radarCount = std::stoi(value);
      else if (key == "heartCount")
        state.heartCount = std::stoi(value);
      else if (key == "hasShield")
        state.hasShield = (std::stoi(value) != 0);
      else if (key == "hasSpeedBoots")
        state.hasSpeedBoots = (std::stoi(value) != 0);
      else if (key == "hasTimeExtender")
        state.hasTimeExtender = (std::stoi(value) != 0);
      else if (key == "hasRadar")
        state.hasRadar = (std::stoi(value) != 0);
      else if (key == "hasSpeedSkill")
        state.hasSpeedSkill = (std::stoi(value) != 0);
    } catch (...) {
    }
  }

  // Backwards compatibility migration
  if (state.shieldCount == 0 && state.hasShield) state.shieldCount = 1;
  if (state.speedCount == 0 && state.hasSpeedBoots) state.speedCount = 1;
  if (state.timeCount == 0 && state.hasTimeExtender) state.timeCount = 1;
  if (state.radarCount == 0 && state.hasRadar) state.radarCount = 1;

  state.hasShield = (state.shieldCount > 0);
  state.hasSpeedBoots = (state.speedCount > 0);
  state.hasTimeExtender = (state.timeCount > 0);
  state.hasRadar = (state.radarCount > 0);

  return state;
}

void ShopData::save(const ShopState &state, int slot) {
  std::filesystem::create_directories("saves");
  std::ofstream file(getFilePath(slot));
  if (!file.is_open())
    return;

  file << "coins=" << state.coins << "\n";
  file << "shieldCount=" << state.shieldCount << "\n";
  file << "speedCount=" << state.speedCount << "\n";
  file << "timeCount=" << state.timeCount << "\n";
  file << "radarCount=" << state.radarCount << "\n";
  file << "heartCount=" << state.heartCount << "\n";
  file << "hasShield=" << (state.shieldCount > 0 ? 1 : 0) << "\n";
  file << "hasSpeedBoots=" << (state.speedCount > 0 ? 1 : 0) << "\n";
  file << "hasTimeExtender=" << (state.timeCount > 0 ? 1 : 0) << "\n";
  file << "hasRadar=" << (state.radarCount > 0 ? 1 : 0) << "\n";
  file << "hasSpeedSkill=" << (state.hasSpeedSkill ? 1 : 0) << "\n";
}

void ShopData::resetSlot(int slot) {
  ShopState freshState;
  save(freshState, slot);
  std::string path = getFilePath(slot);
  std::remove(path.c_str());
}

int ShopData::getCoins(int slot) { return load(slot).coins; }

void ShopData::addCoins(int amount, int slot) {
  if (amount <= 0)
    return;
  ShopState state = load(slot);
  state.coins += amount;
  save(state, slot);
}

bool ShopData::spendCoins(int amount, int slot) {
  ShopState state = load(slot);
  if (state.coins < amount)
    return false;
  state.coins -= amount;
  save(state, slot);
  return true;
}

int ShopData::getItemCount(const std::string &itemId, int slot) {
  ShopState state = load(slot);
  if (itemId == "shield")
    return state.shieldCount;
  if (itemId == "speed")
    return state.speedCount;
  if (itemId == "time")
    return state.timeCount;
  if (itemId == "radar")
    return state.radarCount;
  if (itemId == "heart")
    return state.heartCount;
  if (itemId == "speed_skill")
    return state.hasSpeedSkill ? 1 : 0;
  return 0;
}

bool ShopData::isItemPurchased(const std::string &itemId, int slot) {
  return getItemCount(itemId, slot) > 0;
}

bool ShopData::buyItem(const std::string &itemId, int price, int slot) {
  ShopState state = load(slot);
  if (state.coins < price)
    return false;

  state.coins -= price;
  if (itemId == "shield")
    state.shieldCount++;
  else if (itemId == "speed")
    state.speedCount++;
  else if (itemId == "time")
    state.timeCount++;
  else if (itemId == "radar")
    state.radarCount++;
  else if (itemId == "heart")
    state.heartCount++;
  else if (itemId == "speed_skill")
    state.hasSpeedSkill = true;

  state.hasShield = (state.shieldCount > 0);
  state.hasSpeedBoots = (state.speedCount > 0);
  state.hasTimeExtender = (state.timeCount > 0);
  state.hasRadar = (state.radarCount > 0);

  save(state, slot);
  return true;
}

void ShopData::consumeShield(int slot) {
  ShopState state = load(slot);
  if (state.shieldCount > 0) {
    state.shieldCount--;
  }
  state.hasShield = (state.shieldCount > 0);
  save(state, slot);
}

