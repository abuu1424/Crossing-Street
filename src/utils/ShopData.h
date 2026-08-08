#pragma once
#include <string>

struct ShopState {
  int coins = 1000;             // Coins balance
  bool hasShield = false;       // Energy Shield
  bool hasSpeedBoots = false;   // Permanent
  bool hasTimeExtender = false; // Permanent
  bool hasRadar = false;        // Permanent
  bool hasSpeedSkill = false;   // Speed Surge Skill 'E'
};

class ShopData {
private:
  static int sActiveSlot;

public:
  static void setActiveSlot(int slot);
  static int getActiveSlot();

  static ShopState load(int slot = 0);
  static void save(const ShopState &state, int slot = 0);
  static void resetSlot(int slot = 0);

  static int getCoins(int slot = 0);
  static void addCoins(int amount, int slot = 0);
  static bool spendCoins(int amount, int slot = 0);

  static bool isItemPurchased(const std::string &itemId, int slot = 0);
  static bool buyItem(const std::string &itemId, int price, int slot = 0);
  static void consumeShield(int slot = 0);

private:
  static std::string getFilePath(int slot = 0);
};

