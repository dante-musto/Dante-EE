#include <i2c.h>
#include "msp.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

volatile uint32_t i;
uint8_t SlaveAddress = 0x3C;
uint8_t TXByteCtr;
uint8_t Flag = 0;
unsigned char data [2];
unsigned char *TI_transmit_field;
unsigned char *dataBuffer;
void i2c_init () {
    // Using pins 6.4 and 6.5 for I2C
    P6->SEL0 |= BIT4 | BIT5;

    // Enable eUSCIB0 interrupt
    NVIC->ISER[0] = 1 << ((EUSCIB1_IRQn) & 31);

    // Configure USCI_B0 for I2C mode
    // put eUSCI_B in reset state
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST;
    // Remain eUSCI_B in reset state
    EUSCI_B1->CTLW0 = EUSCI_B_CTLW0_SWRST |
            EUSCI_B_CTLW0_MODE_3 |          // I2C mode
            EUSCI_B_CTLW0_MST |             // I2C master mode
            EUSCI_B_CTLW0_SSEL__SMCLK;      // using smclk
    //sets the baudrate = SMCLK /30
    EUSCI_B1->BRW = 0x8;
    // clear reset register
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
    // Enable transmit interrupt
    EUSCI_B1->IE |= EUSCI_B_IE_TXIE0 |
            // Enable NACK interrupt
            EUSCI_B_IE_NACKIE;

    // Enable global interrupt
    __enable_irq();
}
//******************************************************************************************************************************************
void i2c_write (unsigned char *params, unsigned char flag) {

    TI_transmit_field = params;

    // Don't wake up on exit from ISR
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();

    for (i = 1000; i > 0; i--);

    // configure slave address
    EUSCI_B1->I2CSA = SlaveAddress;

    // Load TX byte counter
    TXByteCtr = flag; //<---------------------------------------------------

    // Ensure stop condition got sent
    while (EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTP);

    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR | EUSCI_B_CTLW0_TXSTT;        // I2C TX | Start condition

    // Enter LPM0
    __sleep();
    __no_operation();

    if (Flag > flag - 1) {
        Flag = 0;
    }
}

// I2C interrupt service routine
void EUSCIB1_IRQHandler(void) {
    if (EUSCI_B1->IFG & EUSCI_B_IFG_NACKIFG) {
        //checks if flag has been cleared if not clears it
        EUSCI_B1->IFG &= ~EUSCI_B_IFG_NACKIFG;
    }
    if (EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0) {
        //checks if flag has been cleared if not clears it
        EUSCI_B1->IFG &= ~EUSCI_B_IFG_TXIFG0;

        // Check TX byte counter
        if (TXByteCtr) {
            // Load TX buffer
            EUSCI_B1->TXBUF = *TI_transmit_field;

            // Decrement TX byte counter
            TI_transmit_field++;
            TXByteCtr--;
        } else {
            // I2C stop condition
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            // Clear USCI_B0 TX int flag
            EUSCI_B1->IFG &= ~EUSCI_B_IFG_TXIFG;
            // Wake up on exit from ISR
            SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
            // Ensures SLEEPONEXIT takes effect immediately
            __DSB();
        }
    }
}
