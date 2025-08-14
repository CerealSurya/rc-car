#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1 //Calculation for setting Baud Rate Register comes from ATmega328 datasheet

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Peripherals.h"

int main(){
    
    adc_init();
    uart_init(MYUBRR);
    DDRC &= ~(1 << DDC0); //Set port as input for Joystick  

    DDRC &= ~(1 << DDC1); //Set port as input for second joystick
    
    uint8_t highByte;
    uint8_t lowByte;
    uint16_t inp;
    
    while (1) { 
      
        inp = adc_read(0); // Read PC0 (ADC0)
        uart_transmit(0xAA); // Framing byte Indicating LEFT
    
        highByte = (inp >> 8) & 0xFF; 
        lowByte  = inp & 0xFF;
        
        _delay_ms(5);
        uart_transmit(lowByte);
        _delay_ms(5);
        uart_transmit(highByte);
       _delay_ms(90);
        
        
        inp = adc_read(1); // Read PC1 (ADC1)
        uart_transmit(0xBB); // Framing byte Indicating RIGHT
        highByte = (inp >> 8) & 0xFF; 
        lowByte  = inp & 0xFF;
        _delay_ms(5);
        uart_transmit(lowByte);
        _delay_ms(5);
        uart_transmit(highByte);
       _delay_ms(5);
        //switcher ^= 0x01;
    }
    return 1;
}

