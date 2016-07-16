#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "AdcLib.h"
#include "DioLib.h"
#include "UrtLib.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void configureMIDI() {
   DioCfg(pADI_GP0, 0x9000); /* Configure P0.6 and P0.7 for UART */
   UrtCfg(pADI_UART, 31250, COMLCR_WLS_8BITS, 0);
   UrtMod(pADI_UART, 0, 0);
   UrtIntCfg(pADI_UART, 0);
}

void sendMIDI(uint32_t byte1, uint32_t byte2, uint32_t byte3) {
   while (!(COMLSR_THRE==(UrtLinSta(pADI_UART) & COMLSR_THRE)));
   UrtTx(pADI_UART, byte1);
   while (!(COMLSR_THRE==(UrtLinSta(pADI_UART) & COMLSR_THRE)));
   UrtTx(pADI_UART, byte2);
   while (!(COMLSR_THRE==(UrtLinSta(pADI_UART) & COMLSR_THRE)));
   UrtTx(pADI_UART, byte3);
}

#define pADI_ADCX pADI_ADC0
#define ADCXSTA_RDY ADC0STA_RDY
#define ADCXSTA_CAL ADC0STA_CAL

void configureADC() {
   // Make sure the ADC is powered on
   AdcGo(pADI_ADCX, ADCMDE_ADCMD_IDLE);

   // Set reference voltage to VDD (3.3V)
   // Set gain to 1 (nothing special)
   // Use bipolar mode because it permits a larger range of input voltages.
   AdcRng(pADI_ADCX, ADCCON_ADCREF_AVDDREF, ADCMDE_PGA_G1, ADCCON_ADCCODE_INT);

   AdcFlt(pADI_ADCX, 0, 15, FLT_NORMAL);

   // No interrupts.
   AdcMski(pADI_ADCX, 0, 1);

   AdcBuf(pADI_ADCX, ADCCFG_EXTBUF_OFF, ADCCON_BUFPOWP | ADCCON_BUFPOWN);

   // Calibrate offset
   AdcGo(pADI_ADCX, ADCMDE_ADCMD_INTOCAL);
   while ((pADI_ADCX->STA & ADCXSTA_CAL) != ADCXSTA_CAL) {
   }
}

int readADC(int pin) {
   // Hook up the negative input to ground, and the positive input to analog input pin 0 on the dev board.
   AdcPin(pADI_ADCX, ADCCON_ADCCN_AGND, pin << 5);

   // Prepare to receive a single sample
   AdcGo(pADI_ADCX, ADCMDE_ADCMD_SINGLE);

   // Block until the ADC status is set to ready
   while ((AdcSta(pADI_ADCX) & ADCXSTA_RDY) != ADCXSTA_RDY) {
   }

   // Read the data
   return AdcRd(pADI_ADCX);
}

int main(int argc, char* argv[])
{
   int32_t usedpins[4] = {0, 1, 2, 3};
   int32_t previous[4] = {0, 0, 0, 0};
   configureADC();
   configureMIDI();
   int32_t controllers[4] = {75, 76, 77, 78}; // midi controller numbers

   int32_t max = 0xFFFFFFF; // max adc reading is 2 ^ 28
   int32_t divider = max / 127;

   while (1) {
      for (size_t i=0; i<(sizeof usedpins)/(sizeof usedpins[0]); ++i) {
         int32_t reading = readADC(usedpins[i]) / divider;
         if (reading > 127) {
            reading = 127;
         }
         if (reading < 0) {
            reading = 0;
         }
         if (reading != previous[i]) {
            sendMIDI(0xB0, controllers[i], reading);

            /*debugging
            char str[30];
            snprintf(str, 30, "reading %d: %d", i, reading);
            trace_puts(str);
            */
         }

         previous[i] = reading;
      }
   }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
