#ifndef SINEWAVE_HPP
#define SINEWAVE_HPP
#include "Arduino.h"

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
