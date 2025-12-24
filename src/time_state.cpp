#include "time_state.h"

unsigned long TimeState::timeMs = 0;
float TimeState::deltaTimeSec = 0.0f;

void TimeState::Update(unsigned long millis) {
    deltaTimeSec = (millis - timeMs) / 1000.0f;
    timeMs = millis;
}