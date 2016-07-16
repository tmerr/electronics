#include "mbed.h"

// Changes in LPCXPresso Rev C:
// D0: P2_11
// D1: P2_12
// A4: P0_23
// A5: P0_11

AnalogIn   knob0(A0);
AnalogIn   knob1(A1);
AnalogIn   knob2(A2);
AnalogIn   knob3(A3);

Serial     midi(P2_12, P2_11);


void sendMIDI(int byte0, int byte1, int byte2) {
    midi.putc(byte0);
    midi.putc(byte1);
    midi.putc(byte2);
}


int main() {
    midi.baud(31250);

    int last[4] = {255, 255, 255, 255};

    while (1) {
        int current[4];
        current[0] = (int)(knob0.read()*127.0 + 0.5);
        current[1] = (int)(knob1.read()*127.0 + 0.5);
        current[2] = (int)(knob2.read()*127.0 + 0.5);
        current[3] = (int)(knob3.read()*127.0 + 0.5);

        for (size_t i=0; i < 4; ++i) {
            if (current[i] != last[i]) {
                sendMIDI(0xB0, 70 + i, current[i]);
                last[i] = current[i];
            }
        }
    }
}
