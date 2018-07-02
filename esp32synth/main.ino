#include "driver/i2s.h"

typedef float flt;

const int DEPTH = 16;
const int SAMPLERATE = 44100;
const flt INVSAMPLERATE = (flt)1.0/(flt)SAMPLERATE;

static const i2s_config_t i2s_config = {
    .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLERATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
};

static const i2s_pin_config_t pin_config = {
    .bck_io_num = 26,
    .ws_io_num = 25,
    .data_out_num = 22,
    .data_in_num = I2S_PIN_NO_CHANGE
};

// some sine-looking function for testing. waveform is from r = -1.0 to 1.0.
flt sinP(flt r) {
    flt rr = r*r;
    return r * ((flt)3.141591
        + rr * ((flt)-5.1662729
        + rr * ((flt)2.5422065
        + rr * ((flt)-0.5811243
        + rr * (flt)0.0636716))));
}

static const i2s_port_t i2s_num = I2S_NUM_0;

void setup() {
    pinMode(26, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(22, OUTPUT);
    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
    i2s_set_pin(i2s_num, &pin_config);
    i2s_set_sample_rates(i2s_num, SAMPLERATE/2);
}

void write_sample(flt sample) {
    int MAG = (1 << (DEPTH - 1)) - 1;
    int32_t s = MAG * sample;
    int16_t clamped;
    if (s > MAG) {
        clamped = (int16_t)MAG;
    } else if (s < -MAG) {
        clamped = (int16_t)-MAG;
    } else {
        clamped = (int16_t)s;
    }
    int16_t ss[2] = {clamped, clamped}; // stereo!
    i2s_write_bytes(i2s_num, (char*)ss, 4, portMAX_DELAY);
}

void loop() {
    flt amplitude = (flt)1.0;
    flt freq = (flt)220.0;
    flt delta = (flt)2.0 * freq * INVSAMPLERATE;
    flt cursor = -(flt)1.0;
    while (true) {
        write_sample(amplitude * sinP(cursor));
        cursor += delta;
        if (cursor > (flt)1.0) {
            cursor -= (flt)2.0;
        }
    }
}
