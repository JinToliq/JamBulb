#pragma once

#include <Arduino.h>

class StatedButton {
public:
    StatedButton(int pin, unsigned long debounceMs, unsigned long clickThresholdMs, unsigned long doubleClickThresholdMs);

    void update();
    bool isPressed() const { return _isPressed; }
    bool isClicked() const { return _isClicked; }
    bool isDoubleClicked() const { return _isDoubleClicked; }
    bool changedThisFrame() const { return _changedThisFrame; }

private:
    const int _pin;
    const unsigned long _debounceMs;
    const unsigned long _clickThresholdMs;
    const unsigned long _doubleClickThresholdMs;
    unsigned long _lastChangeMs;
    unsigned long _lastStableChangeMs;
    unsigned long _pendingClickMs;

    bool _changedThisFrame;
    bool _isPressed;
    bool _isClicked;
    bool _isDoubleClicked;
    bool _lastRaw;
    bool _waitingForDoubleClick;

    bool readRaw() const;
};
