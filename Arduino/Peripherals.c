#include <avr/io.h>
#include <stdio.h>

void adc_init() {
        PRR &= ~(1 << PRADC);
        ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR);   // Sets AVcc as reference (5V in joystick case, b/c that is the analog voltage supplying joystick)            // Right-adjust result
        
        DIDR0 = 0x03; //Disable digital input on ADC 1, 0 to reduce power consumption
        ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);  // Enable ADC, prescaler set as 8
    }

void uart_init(unsigned int ubrr) {
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr >> 8); //ubrr is a 16bit number bc the baud rate register is 16bits split into two 8bits the high then low, so the register looks like this: 15-8 High bits, 7-0 Low bits
    UBRR0L = (unsigned char)ubrr & 0xFF;

    // Enable transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    // Set frame format: 8 data, 1 stop bit
    UCSR0C = (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00); //Set 1 stop bit and then format to 8bit according to datasheet
    UCSR0C &= ~(1 << UMSEL01);
    UCSR0C &= ~(1 << UMSEL00); //Makes sure USART is asynchornous
}
void uart_transmit(unsigned char data) {
    //while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer (implementing this line made transmission wayyyy to slow)
    
    UDR0 = data; // Put data into buffer, sends the data
}

uint16_t adc_read(uint8_t channel) { 
    // Select ADC channel (keep reference bits)
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // First dummy conversion to let the S/H capacitor settle
    ADCSRA |= (1 << ADSC);              // Start conversion
    while (ADCSRA & (1 << ADSC));       // Wait until complete
    (void)ADC;                          // Read and discard result

    // Actual conversion
    ADCSRA |= (1 << ADSC);              // Start conversion again
    while (ADCSRA & (1 << ADSC));       // Wait until complete
    uint16_t result = ADC;              // Read final value

    return result;
}