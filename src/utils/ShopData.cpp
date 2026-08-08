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
    std::getline(ss, key, '=');
    std::getline(ss, value);
    if (value.empty())
      continue;

    try {
      if (key == "coins")
        state.coins = std::stoi(value);
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
  return state;
}

void ShopData::save(const ShopState &state, int slot) {
  std::filesystem::create_directories("saves");
  std::ofstream file(getFilePath(slot));
  if (!file.is_open())
    return;

  file << "coins=" << state.coins << "\n";
  file << "hasShield=" << (state.hasShield ? 1 : 0) << "\n";
  file << "hasSpeedBoots=" << (state.hasSpeedBoots ? 1 : 0) << "\n";
  file << "hasTimeExtender=" << (state.hasTimeExtender ? 1 : 0) << "\n";
  file << "hasRadar=" << (state.hasRadar ? 1 : 0) << "\n";
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

bool ShopData::isItemPurchased(const std::string &itemId, int slot) {
  ShopState state = load(slot);
  if (itemId == "shield")
    return state.hasShield;
  if (itemId == "speed")
    return state.hasSpeedBoots;
  if (itemId == "time")
    return state.hasTimeExtender;
  if (itemId == "radar")
    return state.hasRadar;
  if (itemId == "speed_skill")
    return state.hasSpeedSkill;
  return false;
}

bool ShopData::buyItem(const std::string &itemId, int price, int slot) {
  ShopState state = load(slot);
  if (isItemPurchased(itemId, slot))
    return false;
  if (state.coins < price)
    return false;

  state.coins -= price;
  if (itemId == "shield")
    state.hasShield = true;
  else if (itemId == "speed")
    state.hasSpeedBoots = true;
  else if (itemId == "time")
    state.hasTimeExtender = true;
  else if (itemId == "radar")
    state.hasRadar = true;
  else if (itemId == "speed_skill")
    state.hasSpeedSkill = true;

  save(state, slot);
  return true;
}

void ShopData::consumeShield(int slot) {
  ShopState state = load(slot);
  state.hasShield = false;
  save(state, slot);
}

