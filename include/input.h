#pragma once
#include <Arduino.h>
#include <atomic>
#include "Controls/stated_button.h"
#include "Controls/simple_encoder.h"

struct InputEvents {
    bool switchIsActive = false;
    bool isNextEffectRequested = false;
    int brightnessDelta = 0;
    float speedDelta = 0.0f;
    
    bool hasNewBrightness = false;
    uint8_t newBrightness = 0;
    
    bool hasNewSpeed = false;
    float newSpeed = 0.0f;
};

class Input {
public:
    Input(uint8_t effectButtonPin, uint8_t effectEncoderPinA, uint8_t effectEncoderPinB);
    
    void update();
    InputEvents get();
    void setSwitchIsActive();
    void setNextEffectRequest();
    void setBrightness(uint8_t value);
    void setSpeed(float value);

private:
    StatedButton _button;
    SimpleEncoder _encoder;
    
    int _brightnessDelta = 0;
    float _speedDelta = 0.0f;

    std::atomic<bool> _switchIsActive{false};
    std::atomic<bool> _isNextEffectRequested{false};
    std::atomic<bool> _isBrightnessRequested{false};
    std::atomic<uint8_t> _brightness{0};
    std::atomic<bool> _isSpeedRequested{false};
    std::atomic<float> _speed{0.0f};
};
