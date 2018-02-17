// Begin sad hack https://github.com/esp8266/Arduino/issues/2787
extern "C" {
#include "user_interface.h"
#include "i2s_reg.h"
#include "slc_register.h"
#include "esp8266_peri.h"
void rom_i2c_writeReg_Mask(int, int, int, int, int, int);
}
// End sad hack

#include "slc_register.h"
#include "i2s_reg.h"

#include "build/sampletable.c"

struct registration_list
{
	uint32	blocksize:12;
	uint32	datalen:12;
	uint32	unused:5;
	uint32	sub_sof:1;
	uint32 	eof:1;
	uint32	owner:1;

	uint32	buf_ptr;
	uint32	next_link_ptr;
};

static struct registration_list reglist[1];
static const uint32_t buffersize = (1 << BITDEPTH) / 32;
static uint32_t samplebuffer[buffersize];

// Turn a control bit on and off to indicate a reset.
inline void reset_peri_reg(uint32_t reg, uint32_t mask) {
    SET_PERI_REG_MASK(reg, mask);
    CLEAR_PERI_REG_MASK(reg, mask);
}

// Start the SLC module for FIFO transmission of I2S.
void begin_slc_i2s(struct registration_list* reglist) {
    // This mostly follows sections 10.2.3 of the ESP8266 technical reference.

    // Note:
    // CLEAR_PERI_REG_MASK clears the given mask bits
    // SET_PERI_REG_MASK enables the given mask bits

    // Not in the reference, but in the example code.
    // Reset DMA and clear DMA int flags (but why?)
    reset_peri_reg(SLC_CONF0, SLC_RXLINK_RST);
    reset_peri_reg(SLC_INT_CLR, 0xffffffff);

    // 10.2.3.1
    CLEAR_PERI_REG_MASK(SLC_CONF0, (SLC_MODE<<SLC_MODE_S));
    SET_PERI_REG_MASK(SLC_CONF0, (1 << SLC_MODE_S));
    SET_PERI_REG_MASK(SLC_RX_DSCR_CONF, SLC_INFOR_NO_REPLACE | SLC_TOKEN_NO_REPLACE);

    // Not in the reference, but is in the example code (suspicious!)
    CLEAR_PERI_REG_MASK(SLC_RX_DSCR_CONF, SLC_RX_FILL_EN | SLC_RX_EOF_MODE | SLC_RX_FILL_MODE);

    // 10.2.3.2
    CLEAR_PERI_REG_MASK(SLC_RX_LINK,SLC_RXLINK_DESCADDR_MASK);
	SET_PERI_REG_MASK(SLC_RX_LINK, ((uint32)reglist) & SLC_RXLINK_DESCADDR_MASK);

    // 10.2.3.3
    SET_PERI_REG_MASK(SLC_RX_LINK, SLC_RXLINK_START);
}

// Start the I2S module.
void begin_i2s(uint32_t input_clock_prescaler, uint32_t communication_clock_freq_divider) {
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_I2SO_BCK);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_I2SO_DATA);

    // 10.2.1.1.12S
    reset_peri_reg(I2SCONF, I2S_I2S_RESET_MASK);

    // 10.2.1.2.12S
    i2c_writeReg_Mask_def(i2c_bbpll, i2c_bbpll_en_audio_clock_out, 1);

    // 10.2.1.3 fifo mode: Clear Tx so it's left at 16 bits per channel (left, right).
    // Leave access mode untouched since it defaults to DMA anyway.
    CLEAR_PERI_REG_MASK(I2S_FIFO_CONF, I2S_I2S_TX_FIFO_MOD<<I2S_I2S_TX_FIFO_MOD_S);
 
    // 10.2.1.4 channel mode: Clear leaving it as dual channel.
	CLEAR_PERI_REG_MASK(I2SCONF_CHAN, I2S_TX_CHAN_MOD<<I2S_TX_CHAN_MOD_S);

    // This is mostly copy pasted from the channel3 code.
    // Removed the I2S_RECE_SLAVE_MOD because we're not receiving.
    // Removed the I2S_RECE_MSB_SHIFT because we're not receiving.
	CLEAR_PERI_REG_MASK(I2SCONF, I2S_TRANS_SLAVE_MOD|
						(I2S_BITS_MOD<<I2S_BITS_MOD_S)|
						(I2S_BCK_DIV_NUM <<I2S_BCK_DIV_NUM_S)|
						(I2S_CLKM_DIV_NUM<<I2S_CLKM_DIV_NUM_S));
	SET_PERI_REG_MASK(I2SCONF,
						(((communication_clock_freq_divider)&I2S_BCK_DIV_NUM ) << I2S_BCK_DIV_NUM_S)|
						(((input_clock_prescaler)&I2S_CLKM_DIV_NUM) << I2S_CLKM_DIV_NUM_S));

    SET_PERI_REG_MASK(I2SCONF, I2S_I2S_TX_START);
}

// Load a sample from 0 to SAMPLEBITS exclusive into the buffer
void load_sample(uint16_t sample) {
    for (int i=0; i<buffersize; ++i) {
        samplebuffer[i] = sampletable[sample][i];
    }
}

void load_float_sample(double sample) {
    if (sample < 0.0) {
        sample = 0.0;
    }
    const uint32_t maximum = (1 << BITDEPTH) - 1;
    uint32_t scaled = (uint32_t)(sample * maximum);
    if (scaled > maximum) {
        scaled = maximum;
    }
    load_sample(scaled);
}

void init_reglist() {
    load_sample(1024);
    reglist[0].blocksize = (1 << BITDEPTH) / 8;
    reglist[0].datalen = (1 << BITDEPTH) / 8;
    reglist[0].unused = 0;
    reglist[0].sub_sof = 0;
    reglist[0].eof = 1;
    reglist[0].owner = 1; // DMA
    reglist[0].buf_ptr = (uint32_t)&samplebuffer[0];
    reglist[0].next_link_ptr = (uint32_t)&reglist;
}

void setup() {
    init_reglist();
    begin_slc_i2s(reglist);
    begin_i2s(1, 1);

    pinMode(D5, OUTPUT); // 440Hz for oscilloscope
    pinMode(D6, OUTPUT); // detect how often we change sin
}

int toggle = false;
void loop() {
    if (toggle) {
        digitalWrite(D6, HIGH);
    } else {
        digitalWrite(D6, LOW);
    }
    toggle = !toggle;

    uint32_t cycles = ESP.getCycleCount();
    double secondprogress = (double)ESP.getCycleCount() / 80000000.0;
    double freq = 440.0;
    double sineoutput = sin(freq * secondprogress * 2.0 * M_PI);

    if (sineoutput > 0.0) {
        digitalWrite(D5, HIGH);
    } else {
        digitalWrite(D5, LOW);
    }

    load_float_sample(1.0 + 0.5 * sineoutput);
}
