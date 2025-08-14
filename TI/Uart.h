/**
 * Initialize UART1, used for communication with joystick
 * @param none
 * @return none
 */
void uart1Init(void); 

/**
 * Initialize UART0, used for communication with debugger
 * @param none
 * @return none
 */
void uart0Init(void); 

/**
 * Copy data from RXDATA to software receive fifo
 * @param UARTn
 * @return 1 | 0
 */
int static receiveFifoRX(UART_Regs *uart);

/**
 * Copy data from software transmit fifo to TXDATA
 * @param UARTn
 * @return 1 | 0
 */
int static transmitFifoTX(UART_Regs *uart);

/**
 * Output num to debugger UART
 * @param uint32_t data
 * @return none
 */
void uartOutNum(uint32_t data);



