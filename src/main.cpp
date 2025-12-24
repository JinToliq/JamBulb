#include <Arduino.h>
#include <FastLED.h>

#include "debug.h"
#include "input.h"
#include "web_interface.h"
#include "matrix_map.h"
#include "state.h"
#include "time_state.h"
#include "Config/pins.h"
#include "Config/runtime_config.h"
#include "Effects/effect.h"
#include "Effects/rainbow_effect.h"
#include "Effects/rainbow_rain_effect.h"

static CRGB leds[NUM_LEDS];
static State state;

Input input(BUTTON_PIN, ENCODER_PIN_A, ENCODER_PIN_B);
MatrixMap matrixMap(NUM_LEDS_X, NUM_LEDS_Y);
WebInterface webInterface(input, state);
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

void setBrightness(int brightness)
{
    if (brightness < BRIGHTNESS_MIN) 
        brightness = BRIGHTNESS_MIN;
    else if (brightness > BRIGHTNESS_MAX) 
        brightness = BRIGHTNESS_MAX;

    state.brightness = static_cast<uint8_t>(brightness);
    state.save();

    FastLED.setBrightness(state.brightness);
    DBG_PRINTF("Brightness set: %u\n", state.brightness);
}

void setSpeed(float speed)
{
    if (speed < SPEED_MIN) 
        speed = SPEED_MIN;
    else if (speed > SPEED_MAX) 
        speed = SPEED_MAX;

    state.speed = speed;
    state.save();

    DBG_PRINTF("Speed set: %.2f\n", state.speed);    
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
    webInterface.begin();
    FastLED.setBrightness(state.brightness);
    currentEffect = createCurrentEffect();
    currentEffect->Reset();
}

void loop()
{
    TimeState::Update(millis());
    input.update();
    auto events = input.get();

    if (events.isNextEffectRequested) {
        setNextEffect();
    }

    if (events.brightnessDelta != 0) {
        setBrightness(events.brightnessDelta + state.brightness);   
    }

    if (events.speedDelta != 0.0f) {
        setSpeed(state.speed + events.speedDelta);
    }

    if (events.hasNewBrightness) {
        setBrightness(events.newBrightness);
    }

    if (events.hasNewSpeed) {
        setSpeed(events.newSpeed);
    }

    float deltaTime = TimeState::GetDeltaTimeSec() * state.speed;
    if (currentEffect != nullptr) {
        currentEffect->Update(deltaTime);
        currentEffect->Render(leds, matrixMap);

        FastLED.show();
    }

    delay(2);
}