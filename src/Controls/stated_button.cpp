#include "Controls/stated_button.h"
#include <debug.h>

StatedButton::StatedButton(int pin, unsigned long debounceMs, unsigned long clickThresholdMs, unsigned long doubleClickThresholdMs)
    : 
    _pin(pin), 
    _debounceMs(debounceMs), 
    _clickThresholdMs(clickThresholdMs), 
    _doubleClickThresholdMs(doubleClickThresholdMs),
    _pendingClickMs(0),
    _waitingForDoubleClick(false) {
    pinMode(pin, INPUT_PULLUP);
    _isPressed = readRaw();
}

bool StatedButton::readRaw() const {
    return digitalRead(_pin) == LOW;
}

void StatedButton::update() {
    _changedThisFrame = false;
    _isClicked = false;
    _isDoubleClicked = false;

    auto raw = readRaw();
    auto now = millis();

    if (_waitingForDoubleClick) {
        if (now - _pendingClickMs >= _doubleClickThresholdMs) {
            _isClicked = true;
            _waitingForDoubleClick = false;
        }
    }

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
            if (_waitingForDoubleClick) {
                _isDoubleClicked = true;
                _waitingForDoubleClick = false;
            } else {
                _pendingClickMs = now;
                _waitingForDoubleClick = true;
            }
        }
    }

    _isPressed = raw;
    _lastStableChangeMs = now;
    _changedThisFrame = true;
}