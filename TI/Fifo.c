#include <ti/devices/msp/msp.h>
#include <stdio.h>
#include <string.h>
#include "Fifo.h"

// can hold 0 to FIFOSIZE-1 elements
uint32_t volatile txPutIndex; // where to put next
uint32_t volatile txGetIndex; // where to get next
uint8_t static txFifo[FIFOSIZE];

uint32_t volatile rxPutIndex; 
uint32_t volatile rxGetIndex; 
uint8_t static rxFifo[FIFOSIZE]; //Static hides the variable from outside files


void rxFifoInit(void)
{
    rxPutIndex = rxGetIndex = 0;
}
void txFifoInit(void)
{
    txPutIndex = txGetIndex = 0;
}

uint32_t fifoRxSize()
{
    if (rxPutIndex >= rxGetIndex)
    {
        return (rxPutIndex - rxGetIndex)&(FIFOSIZE-1);
    }

    return (UINT32_MAX - rxGetIndex) + rxPutIndex; // Incase if putIndex wraps around
}

uint32_t fifoTxSize() 
{
    if (txPutIndex >= txGetIndex) 
    {
        return (txPutIndex - txGetIndex)&(FIFOSIZE-1);
    }
    return (UINT32_MAX - txGetIndex) + txPutIndex; // Incase if putIndex wraps around
}

int fifoTxPut(uint8_t data)
{
    uint32_t nextIndex = (txPutIndex + 1) ;
    if (nextIndex == txGetIndex){return 0;} //Queue is full (Having this if statement here sacrifices one index in our queue which gives it the FIFOSIZE-1 total storage capacity)
    txFifo[txPutIndex & (FIFOSIZE-1)] = data; //Bitwise & to == (FIFOSIZE-1wrap index around so it never exceeds (FIFOSIZE-1)
    txPutIndex = nextIndex; //Automatically goes to 0 if counter exceeds 32 bits
    return 1;
}

uint8_t fifoTxGet(void)
{
    if(txGetIndex == txPutIndex){return 0;} //Queue is empty
    uint8_t data = txFifo[txGetIndex  & (FIFOSIZE-1)];
    txGetIndex = (txGetIndex + 1);
    return data;
}

int fifoRxPut(uint8_t data)
{
    uint32_t nextIndex = rxPutIndex + 1;
    if (nextIndex == rxGetIndex){return 0;} //Queue is full (Having this if statement here sacrifices one index in our queue which gives it the FIFOSIZE-1 total storage capacity)
    rxFifo[rxPutIndex & (FIFOSIZE-1)] = data; //Bitwise & to wrap index around so it never exceeds (FIFOSIZE-1)
    rxPutIndex = nextIndex; //Automatically goes to 0 if counter exceeds 32 bits
    return 1;
}

uint8_t fifoRxGet(void) 
{
    if(rxGetIndex == rxPutIndex){return 0;} //Queue is empty
    uint8_t data = rxFifo[rxGetIndex & (FIFOSIZE-1)];
    rxGetIndex++;
    return data;
}