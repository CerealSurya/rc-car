#include <ti/devices/msp/msp.h>
#include "Clock.h"
#include "Pins.h"

void adcInit(uint32_t channel)
{
    //Reset peripheral of adc / voltage reference
    ADC0->ULLMEM.GPRCM.RSTCTL = 0xB1000003;
    
    //Power adc 
    ADC0->ULLMEM.GPRCM.PWREN = 0x26000001;
    
    clockDelay(24); // time for ADC to power up
    ADC0->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // ULPCLK for clock

    ADC0->ULLMEM.CLKFREQ = 7; // 40 to 48 MHz (Theoretically 80mhz clock shouldn't work but it seems like it is)
    ADC0->ULLMEM.CTL0 = 0x00010000; // no divide on clock source, bit 16 PWRDN=1 for manual, =0 power down on completion, if no pending trigger

    ADC0->ULLMEM.CTL1 = 0x00000000;
    
    ADC0->ULLMEM.CTL2 = 0x00000000;

    ADC0->ULLMEM.MEMCTL[0] = 0x000000+channel; //VDDA as voltage selection, should take input 0-3.3V

    ADC0->ULLMEM.SCOMP0 = 0; // 8 sample clocks
}

uint32_t adcIn(void)
{
    ADC0->ULLMEM.CTL0 |= 0x00000001; // enable conversions
    ADC0->ULLMEM.CTL1 |= 0x00000100; // start ADC
    uint32_t volatile delay=ADC0->ULLMEM.STATUS; // time to let ADC start
    while((ADC0->ULLMEM.STATUS&0x01)==0x01){} // wait for completion
    return ADC0->ULLMEM.MEMRES[0];
}