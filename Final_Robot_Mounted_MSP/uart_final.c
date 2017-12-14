/*
 * uart_final.c
 *
 *  Created on: Nov 22, 2017
 *      Author: Nathan
 */

// INCLUDE ALL HEADER FILES
#include "msp.h"
#include "port_final.h"
#include "timer_final.h"
#include "uart_final.h"
#include "functions_final.h"
#include "circbuf_final.h"

extern volatile uint8_t flags;             // BIT0: melee = 0, ranged = 1
extern CircBuf_t* Ranged;                  // Ranged command queue
uint8_t command = 0;                       // Stores previous commands in ranged mode

void uart_config(void){
    // PREPARE TO CONFIGURE UART
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;               // Put eUSCIA2 in reset

    // CONFIGURE UART PROTOCOL
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_PEN                // Parity disabled
                       | EUSCI_A_CTLW0_MSB                // LSB first
                       | EUSCI_A_CTLW0_SEVENBIT           // 8-bit mode
                       | EUSCI_A_CTLW0_SPB                // 1 stop bit
                       | EUSCI_A_CTLW0_MODE0              // UART Mode
                       | EUSCI_A_CTLW0_MODE1);            // UART Mode

    // SET BAUD RATE TO 115200
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;         // Clock source SMCLK
    EUSCI_A2->BRW = 1;                                    // UCBRx = 1
    EUSCI_A2->MCTLW |= 0x00A0 | EUSCI_A_MCTLW_OS16;       // UCBRSx = 0.0417, UCBRFx = 10, OS16 = 1

    // STOP CONFIGURING UART
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;              // Initialize eUSCIA2

    // ENABLE RECEIVE INTERRUPT
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;                      // Enable receive interrupt
    NVIC_EnableIRQ(EUSCIA2_IRQn);                         // Enable interrupt in NVIC
}

extern void EUSCIA2_IRQHandler(void){
    if(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG){
        EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;
        flags |= BIT1;
    }
}

void Bluetooth_send_n(uint8_t* data, uint32_t length){
    int i = 0;
    for (i=0;i<length;i++){
        Bluetooth_send_byte(data[i]);        // Load one byte at a time
    }
}

void Bluetooth_send_byte(uint8_t data){
    while (!(EUSCI_A2->IFG & BIT1));      // Wait for previous transmission to finish
    EUSCI_A2->TXBUF = data;               // Load data to be transmitted
}
