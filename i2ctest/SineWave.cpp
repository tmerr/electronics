#include "SineWave.hpp"
#include <math.h>

SineWave::SineWave(double newfreq): freq(newfreq), phase(0) {}

void SineWave::setFrequency(double newfreq, double time) {
    phase += 2.0 * M_PI * time * (freq - newfreq);
    phase = fmod(phase, 2 * M_PI);
    freq = newfreq;
}

double SineWave::sample(double time) {
    return 0.5 * (1.0 + sin(freq * time * 2.0 * M_PI + phase));
}
