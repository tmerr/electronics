#include <math.h>
#define CLOCKMHZ 80000000

// CLOCKMHZ / 440
#define CLOCKS_PER_CYCLE 181818

unsigned anything = 0;
unsigned last;

void setup() {
    pinMode(D8, OUTPUT);
    analogWriteFreq(1000);
    analogWrite(D8, 512);
    last = micros();
}

void loop() {
    unsigned now = micros();
    if (now > last + 22730/1024) {
        anything = (anything + 1) % 1024;
        last = now;
        analogWrite(D8, anything);
    }

}
