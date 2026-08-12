#pragma once
#include <string>

struct ShopState {
  int coins = 1000;             // Coins balance
  int shieldCount = 0;          // Energy Shield quantity
  int speedCount = 0;           // Speed Boots quantity
  int timeCount = 0;            // Time Extender quantity
  int radarCount = 0;           // Hazard Radar quantity
  int heartCount = 0;           // Extra Heart quantity
  bool hasShield = false;       // Energy Shield (legacy helper)
  bool hasSpeedBoots = false;   // Permanent (legacy helper)
  bool hasTimeExtender = false; // Permanent (legacy helper)
  bool hasRadar = false;        // Permanent (legacy helper)
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

  static int getItemCount(const std::string &itemId, int slot = 0);
  static bool isItemPurchased(const std::string &itemId, int slot = 0);
  static bool buyItem(const std::string &itemId, int price, int slot = 0);
  static void consumeShield(int slot = 0);

private:
  static std::string getFilePath(int slot = 0);
};

