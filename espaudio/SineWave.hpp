#ifndef SINEWAVE_HPP
#define SINEWAVE_HPP
#include "Arduino.h"

// A sine wave using fixed point arithmetic. Keeps track of some state for
// quickly updating the phase so the wave doesn't jump with a change in
// period.
class SineWave {
public:
    SineWave(uint32_t period);
    uint16_t sample(uint32_t time);
    uint16_t sampleAndChangePeriod(uint32_t time, uint32_t period);

private:
    uint32_t period;
    uint32_t phase;
};

#endif
