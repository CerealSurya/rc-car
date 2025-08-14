/**
 * Initializes ADC0
 * PA27 ADC0 channel 0 J1.8 also MKII light interrupt
 * PA26 ADC0 channel 1 J1.5 Joystick select button
 * PA25 ADC0 channel 2 J1.2 MKII Joystick X
 * PA24 ADC0 channel 3 J3.27 ***free***
 * PB25 ADC0 channel 4 J19.7 (insert 0ohm R74, no U3 OPA2365)
 * PB24 ADC0 channel 5 J1.6 MKII microphone in
 * PB20 ADC0 channel 6 J4.36 ***free***
 * PA22 ADC0 channel 7 J24 MKII Accelerometer Y
 * PA15 ADC1 channel 0 J3.30 (also DACout)
 * PA16 ADC1 channel 1 J3.29 ***free***
 * PA17 ADC1 channel 2 J3.28 ***free***
 * PA18 ADC1 channel 3 J3.26 MKII Joystick Y
 * PB17 ADC1 channel 4 J2.18 ***free***
 * PB18 ADC1 channel 5 J3.25 MKII Accelerometer Z
 * PB19 ADC1 channel 6 J3.23 MKII Accelerometer X
 * PA21 ADC1 channel 7 J17.8 (insert R20, remove R3)
 * @param uint32_t channel
 * @return 12-bit result RANGE --> as 0 to 4095.
 * @note  Assumes ADC0_Init has been called.
 * @brief  Trigger ADC measurement and wait for result.
 */
void adcInit(uint32_t channel);


/**
 * Trigger a single ADC0 measurement,
 * wait for it to complete, and return the 12-bit result
 * The ADC input voltage range is 0 to 3.3V.
 * Busy-wait synchronization used.
 * @param none
 * @return 12-bit result RANGE --> as 0 to 4095.
 * @note  Assumes ADC0_Init has been called.
 * @brief  Trigger ADC measurement and wait for result.
 */
uint32_t adcIn(void);