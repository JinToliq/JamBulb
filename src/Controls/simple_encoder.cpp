#include "Controls/simple_encoder.h"

SimpleEncoder::SimpleEncoder(int pinA_, int pinB_) : pinA(pinA_), pinB(pinB_) {
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    lastA = digitalRead(pinA);
    lastB = digitalRead(pinB);
    position = 0;
}

int SimpleEncoder::readDelta() {
    int curA = digitalRead(pinA);
    int curB = digitalRead(pinB);
    int delta = 0;
    if (curA != lastA) {
        // A changed -> determine direction by B
        delta = (curA != curB) ? 1 : -1;
    } else if (curB != lastB) {
        // B changed -> determine direction by A
        delta = (curA == curB) ? 1 : -1;
    }
    lastA = curA;
    lastB = curB;
    position += delta;
    return delta;
}
