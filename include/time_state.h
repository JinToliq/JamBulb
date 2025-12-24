#pragma once

class TimeState {
    private:
    static unsigned long timeMs;
    static float deltaTimeSec;

    public:
    TimeState() = delete;

    static void Update(unsigned long millis);
    static const float GetDeltaTimeSec() { return deltaTimeSec; }
};