#include "Controls/debounced_button.h"

DebouncedButton::DebouncedButton(int pin, unsigned long debounceMs)
    : pin(pin), debounceMs(debounceMs), state(false), lastStable(false), lastRaw(false), lastChange(0) {
    pinMode(pin, INPUT_PULLUP);
    lastStable = digitalRead(pin) == LOW;
    state = lastStable;
}

bool DebouncedButton::update() {
    bool raw = digitalRead(pin) == LOW;
    unsigned long now = millis();
    if (raw != lastRaw) {
        lastChange = now;
        lastRaw = raw;
    }
    if ((now - lastChange) > debounceMs) {
        if (raw != lastStable) {
            lastStable = raw;
            state = lastStable;
            return true;
        }
    }
    return false;
}

bool DebouncedButton::isPressed() const {
    return state;
}
