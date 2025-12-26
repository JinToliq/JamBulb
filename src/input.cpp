#include "input.h"
#include <Config/pins.h>
#include "Config/runtime_config.h"

Input::Input(uint8_t buttonPin, uint8_t encoderPinA, uint8_t encoderPinB)
    : _button(buttonPin, BTN_DEBOUNCE_MS, BTN_CLICK_THRESHOLD_MS), _encoder(encoderPinA, encoderPinB) {
}

void Input::update() {
    _button.update();

    if (_button.isClicked()) {
        setNextEffectRequest();
        return;
    }

    auto encoderDelta = _encoder.readDelta();
    if (encoderDelta  != 0) {
        if (_button.isPressed()) {
            _speedDelta += encoderDelta * SPEED_ENCODER_MULTIPLIER;
        } else {
            _brightnessDelta += encoderDelta * BRIGHTNESS_ENCODER_MULTIPLIER;
        }

        return;
    }
}

InputEvents Input::get() {
    InputEvents events;
    
    if (_isNextEffectRequested) {
        _isNextEffectRequested = false;
        events.isNextEffectRequested = true;
    }
    
    auto handledHardwareInput = false;
    if (_brightnessDelta != 0) {
        events.brightnessDelta = _brightnessDelta;
        _brightnessDelta = 0;
        handledHardwareInput = true;
    }

    if (_speedDelta != 0.0f) {
        events.speedDelta = _speedDelta;
        _speedDelta = 0.0f;
        handledHardwareInput = true;
    }

    if (handledHardwareInput) {
        return events;
    }

    if (_isBrightnessRequested) {
        _isBrightnessRequested = false;
        events.hasNewBrightness = true;
        events.newBrightness = _brightness;
    }

    if (_isSpeedRequested) {
        _isSpeedRequested = false;
        events.hasNewSpeed = true;
        events.newSpeed = _speed;
    }

    return events;
}

void Input::setNextEffectRequest() {
    _isNextEffectRequested = true;
}

void Input::setBrightness(uint8_t value) {
    _brightness = value;
    _isBrightnessRequested = true;
}

void Input::setSpeed(float value) {
    _speed = value;
    _isSpeedRequested = true;
}
