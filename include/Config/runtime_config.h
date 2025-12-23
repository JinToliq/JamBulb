#pragma once

// Brightness limits
constexpr uint8_t BRIGHTNESS_MIN = 8;
constexpr uint8_t BRIGHTNESS_MAX = 210;
constexpr uint8_t BRIGHTNESS_DEFAULT = 120;

// Speed limits
constexpr float SPEED_MIN = 0.1f;
constexpr float SPEED_MAX = 8.0f;
constexpr float SPEED_DEFAULT = 1.0f;

// Persistence keys
constexpr char STORAGE_NAMESPACE[] = "main";
constexpr char STORAGE_KEY_EFFECT[] = "eff";
constexpr char STORAGE_KEY_BRIGHT[] = "brt";
constexpr char STORAGE_KEY_SPEED[] = "spd";

// Effects
constexpr int EFFECTS_COUNT = 2;
