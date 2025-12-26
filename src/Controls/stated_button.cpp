#include "Controls/stated_button.h"
#include <debug.h>

StatedButton::StatedButton(int pin, unsigned long debounceMs, unsigned long clickThresholdMs, unsigned long doubleClickThresholdMs)
    : 
    _pin(pin), 
    _debounceMs(debounceMs), 
    _clickThresholdMs(clickThresholdMs), 
    _doubleClickThresholdMs(doubleClickThresholdMs) {
    pinMode(pin, INPUT_PULLUP);
    _isPressed = readRaw();
    _lastClickMs = 0;
}

bool StatedButton::readRaw() const {
    return digitalRead(_pin) == LOW;
}

void StatedButton::update() {
    _changedThisFrame = false;
    _isClicked = false;

    auto raw = readRaw();
    auto now = millis();

    if (raw == _isPressed)
        return;

    if (raw != _lastRaw) {
        _lastRaw = raw;
        _lastChangeMs = now;
        return;
    }

    auto rawDuration = now - _lastChangeMs;
    if (rawDuration < _debounceMs) {
        return;
    }

    if (_isPressed && !raw)
    {
        auto pressedDuration = now - _lastStableChangeMs;
        if (pressedDuration < _clickThresholdMs) {
            _isClicked = true;
        }
    }

    _isPressed = raw;
    _lastStableChangeMs = now;
    _changedThisFrame = true;
}