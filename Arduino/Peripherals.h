/**
 * Initializes ADC used to read in Joystick inputs
 * @param none
 * @return none
 */
void adc_init();

/**
 * Initializes UART communication to transmit joystick values
 * Note: Uses blind-cycle synchronization
 * @param unsignedint data to be transferred
 * @return none
 */
void uart_init(unsigned int ubrr);

/**
 * Puts data to be transmitted into the buffer
 * @param unsignedchar 8bit data
 * @return none
 */
void uart_transmit(unsigned char data);

/**
 * Read ADC value from joystick
 * @param uint8_t channel to read from 
 * @return uint16_t ADC value (0 -> 1023)
 */
uint16_t adc_read(uint8_t channel);