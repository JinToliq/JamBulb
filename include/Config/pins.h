#pragma once

// LEDs
constexpr int LED_PIN = 5;
constexpr int NUM_LEDS_X = 16;
constexpr int NUM_LEDS_Y = 16;
constexpr int NUM_LEDS = NUM_LEDS_X * NUM_LEDS_Y;

// Input
constexpr int BUTTON_PIN = 0;       // push button to cycle effects
constexpr int ENCODER_PIN_A = 2;    // rotary encoder pin A
constexpr int ENCODER_PIN_B = 4;    // rotary encoder pin B

constexpr int BTN_DEBOUNCE_MS = 50;
constexpr int BTN_CLICK_THRESHOLD_MS = 500;