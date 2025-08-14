#define FIFOSIZE 64

/**
 * Initialize transmit Fifo software
 * @param none
 * @return none
 */
void txFifoInit(void);

/**
 * Initialize receive Fifo software
 * @param none
 * @return none
 */
void rxFifoInit(void);

/**
 * Put 8bit data into software transmit fifo
 * @param uint8_t data
 * @return 1 | 0
 */
int fifoTxPut(uint8_t data); //Add to Transmit queue

/**
 * Get size of software transmit fifo
 * @param none
 * @return uint32_t size
 */
uint32_t fifoTxSize();

/**
 * Get size of software receive fifo
 * @param none
 * @return uint32_t size
 */
uint32_t fifoRxSize();

/**
 * Get transmit data from software fifo
 * @param none
 * @return uint8_t data
 */
uint8_t fifoTxGet(void); //Get data from Tx queue 

/**
 * Put 8bit data into rx queue (should only be called from interrupt)
 * @param uint8_t data
 * @return 1 | 0
 */
int fifoRxPut(uint8_t data); //Add to receive queue

/**
 * Get data from software receive fifo
 * @param none
 * @return uint8_t data
 */
uint8_t fifoRxGet(void); //Get data from Rx queue 
