#include <Arduino.h>
#include <FastLED.h>

#include "debug.h"
#include "Config/pins.h"
#include "Config/runtime_config.h"
#include "Controls/debounced_button.h"
#include "Controls/simple_encoder.h"
#include "matrix_map.h"
#include "state.h"
#include "Effects/effect.h"
#include "Effects/rainbow_effect.h"
#include "Effects/rainbow_rain_effect.h"

// LED buffer
static CRGB leds[NUM_LEDS];

// Globals
MatrixMap matrixMap(NUM_LEDS_X, NUM_LEDS_Y);
static State state;

// Effect management
Effect *effects[2]; // expand later
int effectsCount = 0;

// Inputs
DebouncedButton effectButton(BUTTON_PIN);
SimpleEncoder effectEncoder(ENCODER_PIN_A, ENCODER_PIN_B);

void setup() {
    DBG_BEGIN(115200);
    DBG_PRINTLN("Starting up...");

    // Setup LEDs
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.setBrightness(state.brightness); // initial (may be overwritten by loaded settings)
    FastLED.show();

    // Create effects
    effectsCount = 0;
    effects[effectsCount++] = new RainbowEffect(NUM_LEDS_X, NUM_LEDS_Y);
    effects[effectsCount++] = new RainbowRainEffect(NUM_LEDS_X, NUM_LEDS_Y);
    // TODO: push more effects here

    // Load saved state
    state.load(effectsCount);
    FastLED.setBrightness(state.brightness); // apply loaded brightness
    if (effectsCount > 0) effects[state.currentEffectIndex]->Reset();

    // Configure input pins - constructors already did
}

void loop() {
    static unsigned long lastMs = millis();
    unsigned long now = millis();
    float dtReal = (now - lastMs) / 1000.0f;
    lastMs = now;

    // Read button and encoder
    bool changed = effectButton.update();
    bool buttonPressed = effectButton.isPressed();

    int delta = effectEncoder.readDelta();
    if (delta != 0) {
        if (buttonPressed) {
            // adjust speed
            float step = 0.05f * abs(delta);
            if (delta > 0) state.speed += step;
            else state.speed -= step;
            if (state.speed < SPEED_MIN) state.speed = SPEED_MIN;
            if (state.speed > SPEED_MAX) state.speed = SPEED_MAX;
            DBG_PRINTF("Speed: %.2f\n", state.speed);
            state.save();
        } else {
            // adjust brightness
            int step = 4 * delta;
            int b = (int)state.brightness + step;
            if (b < BRIGHTNESS_MIN) b = BRIGHTNESS_MIN;
            if (b > BRIGHTNESS_MAX) b = BRIGHTNESS_MAX;
            state.brightness = static_cast<uint8_t>(b);
            FastLED.setBrightness(state.brightness);
            DBG_PRINTF("Brightness: %u\n", state.brightness);
            state.save();
        }
    }

    // If button pressed and released quickly -> cycle effect
    static bool lastBtn = false;
    if (changed) {
        bool newBtn = buttonPressed;
        if (!newBtn && lastBtn) {
            // released -> treat as a click
            if (effectsCount > 0) {
                state.currentEffectIndex = (state.currentEffectIndex + 1) % effectsCount;
                DBG_PRINTF("Switching to effect %d\n", state.currentEffectIndex);
                effects[state.currentEffectIndex]->Reset();
                state.save();
            }
        }
        lastBtn = newBtn;
    }

    float dt = dtReal * state.speed;
    if (effectsCount > 0) 
        effects[state.currentEffectIndex]->Update(dt);

    if (effectsCount > 0) 
        effects[state.currentEffectIndex]->Render(leds, matrixMap);

    FastLED.show();
    delay(2); // small delay to avoid busy loop (frame cap ~200Hz)
}