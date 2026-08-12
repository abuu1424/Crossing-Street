#pragma once
#include <string>

struct PlayerStats {
  int maxHp = 3;
  int currentHp = 3;

  float baseSpeed = 200.f;
  float speedMultiplier = 1.0f;

  // Energy System (Tốc độ giảm theo Năng Lượng & Score & Hazard)
  float energy = 100.f;
  float maxEnergy = 100.f;
  float energyRegenRate = 18.f;
  float baseEnergyDrain = 10.f;

  // Stamina (Sprint Shift)
  float stamina = 100.f;
  float maxStamina = 100.f;
  float staminaRegenRate = 25.f;
  float staminaDrainRate = 45.f;
  bool isSprinting = false;

  // Skill Speed Burst 'E'
  bool hasSpeedSkill = true;
  bool skillActive = false;
  float skillTimer = 0.f; // 5.0s duration
  float skillDuration = 5.0f;
  float skillCooldownTimer = 0.f; // 10.0s cooldown
  float skillCooldownDuration = 10.0f;

  // Skill EMP Sonar Radar 'Q'
  bool hasRadarSkill = false;
  bool radarActive = false;
  float radarTimer = 0.f; // 3.5s duration
  float radarDuration = 3.5f;
  float radarCooldownTimer = 0.f; // 10.0s cooldown
  float radarCooldownDuration = 10.0f;
  float radarPulseRadius = 0.f;

  // Skill Time Freeze Clock 'T'
  bool hasTimeSkill = false;
  bool timeFreezeActive = false;
  float timeFreezeTimer = 0.f; // 5.0s duration
  float timeFreezeDuration = 5.0f;
  float timeFreezeCooldownTimer = 0.f; // 12.0s cooldown
  float timeFreezeCooldownDuration = 12.0f;

  // Cooldown Warning Message ("Chưa đủ thời gian hồi")
  std::string cooldownWarningMsg = "";
  float cooldownWarningTimer = 0.f; // 1.2s display timer

  // Realtime Speed Stat Output
  float currentCalculatedSpeed = 200.f;

  int speedLevel = 1;
  int hpLevel = 1;
  int staminaLevel = 1;

  void resetHp() {
    currentHp = maxHp;
    energy = maxEnergy;
    stamina = maxStamina;
    skillActive = false;
    skillTimer = 0.f;
    skillCooldownTimer = 0.f;
    radarActive = false;
    radarTimer = 0.f;
    radarCooldownTimer = 0.f;
    timeFreezeActive = false;
    timeFreezeTimer = 0.f;
    timeFreezeCooldownTimer = 0.f;
    cooldownWarningTimer = 0.f;
  }
};
