#include "SineWave.hpp"
#include "build/sintable.c"
#include <math.h>

SineWave::SineWave(uint32_t period): period(period), phase(0) {}

// compute the index into the sine table, ignoring phase
uint32_t compute_index(uint32_t time, uint32_t period)  {
   return ((SINRESOLUTION * (uint64_t)time) / (uint64_t)period) % SINRESOLUTION;
}

uint32_t add_modulo_sin_res(uint32_t a, uint32_t b) {
    uint32_t result = a + b;
    if (result >= SINRESOLUTION) {
        result -= SINRESOLUTION;
    }
    return result;
}

uint32_t sub_modulo_sin_res(uint32_t a, uint32_t b) {
    return add_modulo_sin_res(a, SINRESOLUTION - b);
}

uint16_t SineWave::sample(uint32_t time) {
    uint32_t index = add_modulo_sin_res(this->phase, compute_index(time, this->period));
    return sinetable[index];
}

uint16_t SineWave::sampleAndChangePeriod(uint32_t time, uint32_t newperiod) {
    if (newperiod == this->period) {
        return sample(time);
    } else {
        uint32_t phaseless = compute_index(time, newperiod);
        this->phase = add_modulo_sin_res(
            this->phase,
            sub_modulo_sin_res(
                compute_index(time, this->period),
                phaseless
            )
        );
        this->period = newperiod;
        return sinetable[add_modulo_sin_res(this->phase, phaseless)];
    }
}
