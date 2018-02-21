// Espressif's analogRead calls out to the closed source system_adc_read, which
// spends a few thousand cycles busy looping. This is a non blocking alternative
// thanks to someone's reverse engineering
// http://www.esp8266.com/viewtopic.php?f=13&t=800&start=12
//
// system_adc_read_fast is also worth looking into.


#include "eagle_soc.h"
#include "analogReadNonblocking.hpp"


extern "C" {
void rom_i2c_writeReg_Mask(int, int, int, int, int, int);
void read_sar_dout(void *);
}

static int state = 0;

void analogReadStart() {
    rom_i2c_writeReg_Mask(108,2,0,5,5,1);
    SET_PERI_REG_MASK(0x60000D5C,0x00200000);
    state = 1;
}

// If a reading is available write it to result and return true.
// Otherwise return false. Rearm indicates whether to start another
// analog read automatically if this one succeeds.
bool analogReadNonblocking(uint16_t *result, bool rearm) { 
    if (state == 1 && !(READ_PERI_REG(0x60000D50)&(0x7<<24))) {
        CLEAR_PERI_REG_MASK(0x60000D50,2);
        SET_PERI_REG_MASK(0x60000D50,2);
        state = 2;
        return true;
    }

    if (state == 2 && !(READ_PERI_REG(0x60000D50)&(0x7<<24))) {
        uint16_t sar_x[8];
        uint16_t z = 0;
        read_sar_dout(&sar_x[0]);
        for(int i = 0; i < 8; i++) {
            z += sar_x[i];
        }
        z += 8;
        z >>= 4;
        *result = z;
        if (rearm) {
            analogReadStart();
        } else {
            state = 0;
        }
        return true;
    }

    return false;
}
