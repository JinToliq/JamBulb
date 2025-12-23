#include "time.h"

unsigned long Time::timeMs = 0;
float Time::deltaTimeSec = 0.0f;

void Time::Update(unsigned long millis) {
    deltaTimeSec = (millis - timeMs) / 1000.0f;
    timeMs = millis;
}