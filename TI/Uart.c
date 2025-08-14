#include <ti/devices/msp/msp.h>
#include "Pins.h"
#include "Clock.h"
#include "Uart.h"
#include "Fifo.h"
#include <stdio.h>
#include <string.h>

// power Domain PD0
// for 32MHz bus clock, bus clock is 32MHz
// for 40MHz bus clock, bus clock is ULPCLK (ultra low power clock) 20MHz
// for 80MHz bus clock, bus clock is ULPCLK 40MHz
// assume 40MHz bus clock, bus clock = 20MHz
void uart1Init(void)
{    
    //Reset peripheral
    UART1->GPRCM.RSTCTL = 0xB1000003;
    //Power UART peripheral
    UART1->GPRCM.PWREN = 0x26000001;
    //clockDelay(24);

    //Configure IO Pins
    IOMUX->SECCFG.PINCM[PA8INDEX] |= 0x00000082; //output (Tx)
    IOMUX->SECCFG.PINCM[PA9INDEX] |= 0x00040082; //Input (Rx)
    
    //Select UART clock source and divide
    UART1->CLKSEL = 0x08; //Bus clock is source
    UART1->CLKDIV = 0x00; // no divide
    //Disable UART to implement changes
    UART1->CTL0 &= ~(0x01);
    UART1->CTL0 = 0x00020018;
    //UART0->CTL0 &= ~(0x20000); //Flush FEN bit to turn off fifo as per datasheet instruction
    //Set Baud Rate Divsor

    //UART Clock --> 40mhz (about half of main clock), Oversampling 16, Desired Baud Rate 9600 || 19200
    UART1->IBRD = 260;//130; 
    UART1->FBRD = 27; //13; 
    
    //UART SETTINGS: Least significant bit first, FIFO Enabled, Default operation
    //each fifo has 12 bits of value (8 data bits, 4 error bits
    //8 bit transfer
    UART1->LCRH = 0x00000030;
    
    UART1->CPU_INT.IMASK = 0x0C01; //Enable interrupts
    UART1->IFLS = 0x0445; //Controls trigger point at which interrupts are generated, basically when queue reaches half capacity interrupt is triggered

    NVIC->ICPR[0] = 1<<13; // Clear pending interrupt for UART1 if there is any
    NVIC->ISER[0] = 1<<13; //Set the interrupt 
    NVIC->IP[3] = (NVIC->IP[3]&(~0xFF000000))|(2<<30); // set priority (bits 31,30) IRQ 13 (Lowest priority allowed)
  
    UART1->CTL0 |= 0x01; //Enabling UART
}

void uart0Init(void) //Used for debugging (doesn't really work rn tho idk why)
{
    //Reset peripheral
    UART0->GPRCM.RSTCTL = 0xB1000003;
    //Power UART peripheral
    UART0->GPRCM.PWREN = 0x26000001;
    clockDelay(24);
    IOMUX->SECCFG.PINCM[PA10INDEX]  = 0x00000082;

    //Select UART clock source and divide
    UART0->CLKSEL = 0x08; //Bus clock is source
    UART0->CLKDIV = 0x00; //No divide, use 80mhz to calculate baud rate
    //Disable UART to implement changes
    UART0->CTL0 &= ~(0x01);
    UART0->CTL0 = 0x00020018;
    //UART0->CTL0 &= ~(0x20000); //Flush FEN bit to turn off fifo as per datasheet instruction
    //Set Baud Rate Divsor

    //UART Clock --> 80mhz, Oversampling 16, Desired Baud Rate 115200

    UART0->IBRD = 43;
    UART0->FBRD = 26;
    // UART0->IBRD = 21;
    // UART0->FBRD = 45;

    //UART SETTINGS: Least significant bit first, FIFO Enabled, Default operation
    //each fifo has 12 bits of value (8 data bits, 4 error bits
    //8 bit transfer
    UART0->LCRH = 0x00000030;

    UART0->CTL0 |= 0x01; //Enabling UART
}

int static transmitFifoTX(UART_Regs *uart) //Used by UART0 for debugging
{
    while(fifoTxSize() > 0)
    {
        if ((uart->STAT&0x80) == 1){break;} //Wait until next transmission can be sent

        uint8_t transmitData = fifoTxGet();
        uart->TXDATA = transmitData;
    }
    
    return 1;
}

int static receiveFifoRX(UART_Regs *uart) //Used to receive data from arduino
{

    while ((uart->STAT&0x04) == 0 && fifoRxSize() < FIFOSIZE) //fifoRxSize() < FIFOSIZE
    {
        uint8_t data = (uart->RXDATA)&0xFF;
        // if (data == 0xAA){
        //     GPIOB->DOUT31_0 ^= 0x01;
        // }
        // if (data == 0xBB){
        //     GPIOB->DOUT31_0 ^= 0x01;
        // }
        
        //else{GPIOB->DOUT31_0 |= 0x01;}
        fifoRxPut(data);  
    }
    return 1;
}

void uartOutNum(uint32_t data)
{
    uint8_t highByte = (data >> 24) & 0xFF;
    uint8_t midHighByte = (data >> 16) & 0xFF;
    uint8_t midLowByte = (data >> 8)  & 0xFF;
    uint8_t lowByte = data & 0xFF;

    while(fifoTxPut(lowByte) == 0){}; //Wait until open space
    while(fifoTxPut(midLowByte) == 0){}; //Wait until open space
    while(fifoTxPut(midHighByte) == 0){}; //Wait until open space
    while(fifoTxPut(highByte) == 0){}; //Wait until open space

    transmitFifoTX(UART0);
}

void UART1_IRQHandler(void) //UART1 interrupt handler
{
    
    uint32_t status = UART1->CPU_INT.IIDX; // reading clears bit in RIS
    // if (status == 0x0C) //No transmission required bc we are solely receiving from arduino
    // {transmitFifoTX();}
    if(status == 0x0B || status == 0x01)
    {receiveFifoRX(UART1);}
}
