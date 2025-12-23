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

static CRGB leds[NUM_LEDS];
static State state;

MatrixMap matrixMap(NUM_LEDS_X, NUM_LEDS_Y);
DebouncedButton effectButton(BUTTON_PIN);
SimpleEncoder effectEncoder(ENCODER_PIN_A, ENCODER_PIN_B);

Effect* currentEffect = nullptr;

Effect* createCurrentEffect() {
    switch (state.currentEffectIndex)
    {
    case 0:
        return new RainbowEffect(NUM_LEDS_X, NUM_LEDS_Y);
    case 1:
        return new RainbowRainEffect(NUM_LEDS_X, NUM_LEDS_Y);
    default:
        return new RainbowEffect(NUM_LEDS_X, NUM_LEDS_Y);
    }
}

void setNextEffect() {
    delete currentEffect;
    state.currentEffectIndex++;
    if (state.currentEffectIndex >= EFFECTS_COUNT)
        state.currentEffectIndex = 0;

    state.save();
    currentEffect = createCurrentEffect();
    currentEffect->Reset();    

    DBG_PRINTF("Set effect %d\n", state.currentEffectIndex);
}

void SetupLed()
{
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.setBrightness(state.brightness);
    FastLED.show();
}

void setup() 
{
    DBG_BEGIN(115200);
    DBG_PRINTLN("Starting up...");

    SetupLed();

    state.load();
    FastLED.setBrightness(state.brightness);
    currentEffect = createCurrentEffect();
    currentEffect->Reset();
}

void loop()
{
    Time::Update(millis());
    auto isButtonChanged = effectButton.update();
    auto encoderDelta = effectEncoder.readDelta();

    if (encoderDelta != 0) {
        if (effectButton.isPressed()) {
            // adjust speed
            float step = 0.05f * abs(encoderDelta);
            if (encoderDelta > 0) state.speed += step;
            else state.speed -= step;
            if (state.speed < SPEED_MIN) state.speed = SPEED_MIN;
            if (state.speed > SPEED_MAX) state.speed = SPEED_MAX;
            DBG_PRINTF("Speed: %.2f\n", state.speed);
            state.save();
        } else {
            // adjust brightness
            int step = 4 * encoderDelta;
            int b = (int)state.brightness + step;
            if (b < BRIGHTNESS_MIN) b = BRIGHTNESS_MIN;
            if (b > BRIGHTNESS_MAX) b = BRIGHTNESS_MAX;
            state.brightness = static_cast<uint8_t>(b);
            FastLED.setBrightness(state.brightness);
            DBG_PRINTF("Brightness: %u\n", state.brightness);
            state.save();
        }
    }

    static bool lastBtn = false;
    if (isButtonChanged) {
        bool newBtn = effectButton.isPressed();
        if (!newBtn && lastBtn)
            setNextEffect();
    
        lastBtn = newBtn;
    }

    float deltaTime = Time::GetDeltaTimeSec() * state.speed;
    if (currentEffect != nullptr) {
        currentEffect->Update(deltaTime);
        currentEffect->Render(leds, matrixMap);

        FastLED.show();
    }

    delay(2);
}