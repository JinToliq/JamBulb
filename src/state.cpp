#include "state.h"
#include "Config/runtime_config.h"
#include <Preferences.h>
#include <algorithm>

void State::load() {
    Preferences prefs;
    prefs.begin(STORAGE_NAMESPACE, true);

    currentEffectIndex = prefs.getUInt(STORAGE_KEY_EFFECT, 0);
    speed = prefs.getFloat(STORAGE_KEY_SPEED, SPEED_DEFAULT);
    brightness = prefs.getUChar(STORAGE_KEY_BRIGHT, BRIGHTNESS_DEFAULT);
    prefs.end();
}

void State::save() const {
    Preferences prefs;
    prefs.begin(STORAGE_NAMESPACE, false);
    prefs.putUInt(STORAGE_KEY_EFFECT, static_cast<unsigned int>(currentEffectIndex));
    prefs.putFloat(STORAGE_KEY_SPEED, speed);
    prefs.putUChar(STORAGE_KEY_BRIGHT, brightness);
    prefs.end();
}
