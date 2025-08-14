/* main.c
 * your name
 * date
 */

#include <ti/devices/msp/msp.h>
#include "Clock.h"
#include "Pins.h"
#include "Uart.h"
#include "Adc.h"
#include "Fifo.h"
#include "Motor.h"

#define leftPin1 0x01 //PB0
#define leftPin2 0x02 //PB1

#define rightPin1 0x10 //PB4
#define rightPin2 0x40 //PB6


void GPIOInit(void)
{
    GPIOA->GPRCM.RSTCTL = (uint32_t)0xB1000003;
    GPIOB->GPRCM.RSTCTL = (uint32_t)0xB1000003;
    GPIOA->GPRCM.PWREN = (uint32_t)0x26000001;
    GPIOB->GPRCM.PWREN = (uint32_t)0x26000001;
}
int main(void)
{
    clockInit();
    txFifoInit();
    rxFifoInit();
    GPIOInit();
    //adcInit(03);
    uart1Init();
    //uart0Init(); //For debugging

    
    // Set PB1 to GPIO mode (PF = 0x01), connect to IO logic, and optionally enable pull-up
    IOMUX->SECCFG.PINCM[PB0INDEX] = 0x00000081; //Designates output, Input = 0x00040081
    IOMUX->SECCFG.PINCM[PB1INDEX] = 0x00000081;

    IOMUX->SECCFG.PINCM[PB4INDEX] = 0x00000081; //Designates output, Input = 0x00040081
    IOMUX->SECCFG.PINCM[PB6INDEX] = 0x00000081;
    // Enable PB0 as an output
    GPIOB->DOE31_0 |= leftPin1;  // Bit 1 corresponds to PB0
    GPIOB->DOE31_0 |= leftPin2;

    GPIOB->DOE31_0 |= rightPin1;  // Bit 1 corresponds to PB0
    GPIOB->DOE31_0 |= rightPin2;
    //PWM variables
    // GPIOB->DOUT31_0 |= 0x01;
    // GPIOB->DOUT31_0 &= ~(0x02);
    GPIOB->DOUT31_0 &= ~(leftPin1);
    GPIOB->DOUT31_0 &= ~(leftPin2);

    GPIOB->DOUT31_0 &= ~(rightPin1);
    GPIOB->DOUT31_0 &= ~(rightPin2);
    while(1) 
    { 
        uint8_t byte;
        do {
            byte = fifoRxGet(); // discard until sync byte found
        } while (byte != 0xAA && byte != 0xBB);

        uint8_t dataLow = fifoRxGet();
        uint8_t dataHigh = fifoRxGet();
        
        uint32_t adcValue32 = ((uint32_t)dataHigh << 8) | dataLow; //Range 0->1023
        if (byte == 0xAA) //LEFT COMMAND
        {
            motorControl(adcValue32, leftPin1, leftPin2);
        }
        else if (byte == 0xBB) //RIGHT COMMAND
        {
            motorControl(adcValue32, rightPin1, rightPin2);
        }
        
        clockDelay1ms(20); //Required delay for everything to work. 
    }
}