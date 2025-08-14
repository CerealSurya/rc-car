#include <ti/devices/msp/msp.h>
#include "Clock.h"
#include "Motor.h"

uint32_t static P = 4000000;//Arbitrarily defining this constant r
uint32_t  H; // H (time set high)
uint32_t  L; //L (time set low)

void static PWMCalc(uint32_t adc)
{
    int targetInt = 50; //Default incase of edge case error
    float targetDutyCycle;
    // H+L = P, Duty Cycle = (H/(H+L))
            
    //Range 510 --> 1023, 0% --> 100%, EX: ADC = 750, speed = (750-510) / (1023-510) ~ 46%
    if (adc > 520)
    {
        targetInt = (int)(((adc - 520.0f) / (1023 - 520)) * 100);
    }
    //Range 495 --> 0, 0% --> 100%
    else if (adc < 496 && adc != 0)
    {
        targetInt = (int)(((495.0f - adc) / (495 - 0)) * 100); 
    }
    targetDutyCycle = targetInt / 100.0f; //two decimal places, Range 0 --> 1

    // H + L = 200000
    // (H/(H+L)) = targetDutyCycle --> Solve system of equation
    H = (uint32_t)(targetDutyCycle * P);
    L = (uint32_t)(P - H);
}
void motorControl(uint32_t adcValue32, uint32_t pin1, uint32_t pin2) //Each motor has two pins connected to it
{
    if (adcValue32 > 1023 || adcValue32 < 0){return;} //Remove noise

    if (adcValue32 > 485 && adcValue32 < 520) //Joystick is in middle
    {
        GPIOB->DOUT31_0 &= ~(pin1);
        GPIOB->DOUT31_0 &= ~(pin2);
    } 
    else if (adcValue32 == 1023) //MAX FORWARD
    { ///Edge case
        GPIOB->DOUT31_0 |= pin1;
        GPIOB->DOUT31_0 &= ~(pin2);
    }
    else if (adcValue32 == 0) //MAX REVERSE
    { ///Edge case
        GPIOB->DOUT31_0 &= ~(pin1);
        GPIOB->DOUT31_0 |= pin2;
    }
    else if (adcValue32 > 520) //PWM Going forward direction
    {  // 1 | 0 (Forward)
        PWMCalc(adcValue32);
        GPIOB->DOUT31_0 |= pin1; // Set H-Bridge high
        GPIOB->DOUT31_0 &= ~(pin2);
        clockDelay(H); 

        GPIOB->DOUT31_0 &= ~(pin1); //Turns H-Bridge off
        GPIOB->DOUT31_0 &= ~(pin2);
        clockDelay(L); 
    }
    else if (adcValue32 < 500) //PWM Going reverse direction
    { // 0 | 1 (Reverse)
        PWMCalc(adcValue32);
        GPIOB->DOUT31_0 |= pin2; // Set H-bridge high
        GPIOB->DOUT31_0 &= ~(pin1);
        clockDelay(H); 

        GPIOB->DOUT31_0 &= ~(pin2); //Turns H-Bridge off
        GPIOB->DOUT31_0 &= ~(pin1);
        clockDelay(L); 
    }
}

void testMotor(uint32_t adcValue32, uint32_t pin1)
{
    if (adcValue32 > 1023 || adcValue32 < 0){return;} //Remove noise

    if (adcValue32 > 495 && adcValue32 < 520) //Joystick is in middle
    {
        GPIOB->DOUT31_0 &= ~(pin1);
    } 
    else if (adcValue32 == 1023) //MAX FORWARD
    { ///Edge case
        GPIOB->DOUT31_0 |= pin1;
    }
    else if (adcValue32 == 0) //MAX REVERSE
    { ///Edge case
        GPIOB->DOUT31_0 &= ~(pin1);
    }
    else if (adcValue32 > 510)
    { 
        PWMCalc(adcValue32);
        GPIOB->DOUT31_0 |= pin1; 
        clockDelay(H); 

        GPIOB->DOUT31_0 &= ~(pin1);
        clockDelay(L); 
    }
}