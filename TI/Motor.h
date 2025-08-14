/**
 * Calculate PWM high and low times based on ADC value
 * @param uint32_t adcValue
 * @return none
 */
void static PWMCalc(uint32_t adc);

/**
 * control H-Bridge of motor based on PWM and ADC value
 * @param uint32_t adcValue32, uint32_t pin1, uint32_t pin2
 * @return none
 */
void motorControl(uint32_t adcValue32, uint32_t pin1, uint32_t pin2);

void testMotor(uint32_t adcValue32, uint32_t pin);
