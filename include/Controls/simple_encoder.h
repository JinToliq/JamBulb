#pragma once

#include <Arduino.h>

class SimpleEncoder {
public:
    SimpleEncoder(int pinA, int pinB);

    // Call regularly; returns delta since last call
    int readDelta();

private:
    int pinA, pinB;
    int lastA, lastB;
    int position;
};
