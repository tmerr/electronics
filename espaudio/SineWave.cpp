#include "SineWave.hpp"
#include "build/sintable.c"
#include <math.h>

SineWave::SineWave(double newfreq): freq(newfreq), phase(0) {}

void SineWave::setFrequency(double newfreq, double time) {
    phase += SINRESOLUTION * time * (freq - newfreq);
    phase = fmod(phase, SINRESOLUTION);
    freq = newfreq;
}

uint16_t SineWave::sample(double time) {
    uint16_t idx = (uint16_t)fmod(SINRESOLUTION * freq * time + phase, SINRESOLUTION);
    return sinetable[idx];
}
