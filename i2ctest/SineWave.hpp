#ifndef SINEWAVE_HPP
#define SINEWAVE_HPP
#include "Arduino.h"

class SineWave {
public:
    SineWave(double freq);
    void setFrequency(double newfreq, double time);
    double sample(double time);

private:
    double freq;
    double phase;
};

#endif
