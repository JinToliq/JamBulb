#pragma once

class Time {
    private:
    static unsigned long timeMs;
    static float deltaTimeSec;

    public:
    Time() = delete;

    static void Update(unsigned long millis);
    static const float GetDeltaTimeSec() { return deltaTimeSec; }
};