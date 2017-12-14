/*
 * port.c
 *
 *  Created on: Nov 18, 2017
 *      Author: Nathan
 */

// INCLUDE ALL HEADER FILES
#include "msp.h"
#include "port.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "circbuf.h"
#include "functions.h"

extern volatile uint8_t data;
extern volatile uint8_t flags;

void port_config(void){
    // RESET ALL PORT SELECT BITS TO DEFAULT (GPIO MODE)
    P1->SEL0 = 0x00;                     // All P1->SEL = 00
    P1->SEL1 = 0x00;                     // Reset all P1 select bits to gpio mode
    P2->SEL0 = 0x00;                     // All P2->SEL = 00
    P2->SEL1 = 0x00;                     // Reset all P2 select bits to gpio mode
    P3->SEL0 = 0x00;                     // All P3->SEL = 00
    P3->SEL1 = 0x00;                     // Reset all P3 select bits to gpio mode
    P4->SEL0 = 0x00;                     // All P4->SEL = 00
    P4->SEL1 = 0x00;                     // Reset all P4 select bits to gpio mode
    P5->SEL0 = 0x00;                     // All P5->SEL = 00
    P5->SEL1 = 0x00;                     // Reset all P5 select bits to gpio mode
    P6->SEL0 = 0x00;                     // All P6->SEL = 00
    P6->SEL1 = 0x00;                     // Reset all P6 select bits to gpio mode

    // CONFIGURE SELECT BITS
    P1->SEL1 &= ~(BIT2 | BIT3);          // P1.2,3 in UART mode
    P1->SEL0 |= BIT2 | BIT3;             // P1.2 = RX, P1.3 = TX
    P3->SEL1 &= ~(BIT2 | BIT3);          // P3.2,3 in UART mode
    P3->SEL0 |= BIT2 | BIT3;             // P3.2 = J1.3 = RX, P3.3 = J1.4 = TX
    P4->SEL1 |= BIT0;                    // P4.0 SEL = 11
    P4->SEL0 |= BIT0;                    // Accelerometer Y pin to analog
    P6->SEL1 |= BIT1;                    // P6.1 SEL = 11
    P6->SEL0 |= BIT1;                    // Accelerometer X pin to analog

    // CONFIGURE GPIO PERIPHERALS
    P3->DIR &= ~BIT5;                    // Set P3.5 to input
    P3->REN |= BIT5;                     // Enable Pullup/Pulldown on P3.5
    P3->OUT &= ~BIT5;                    // Set P3.5 to Pulldown
    P2->DIR |= BIT0 | BIT1 | BIT2;       // Set P2.0,1,2 to outputs
    P2->OUT = 2;                         // Set LED to green (full health)
    P5->DIR &= ~BIT1;                    // Set P5.1 to input
    P5->REN |= BIT1;                     // Enable Pullup/Pulldown on P5.1
    P5->OUT &= ~BIT1;                    // Set P5.1 to Pulldown

    // CONFIGURE BUTTON INTERRUPTS
    P3->IES |= BIT5;                     // Sets P3.5 to flag high-to-low transition
    P3->IE |= BIT5;                      // Enables P3.5 interrupt
    P3->IFG = 0x00;                      // Clears P3 interrupt flag
    P5->IES |= BIT1;                     // Sets P5.1 to flag high-to-low transition
    P5->IE |= BIT1;                      // Enables P5.1 interrupt
    P5->IFG = 0x00;                      // Clears P5 interrupt flag
    NVIC_EnableIRQ(PORT3_IRQn);          // Enables P3 interrupt in the NVIC
    NVIC_EnableIRQ(PORT5_IRQn);          // Enables P5 interrupt in the NVIC
}

extern void PORT3_IRQHandler(void){      // Button 2 (bottom) pressed, charge attack
    __disable_irq();
    if(flags & BIT4){
        data |= SPIN;                    // Ready a spin attack
        flags &= ~BIT4;
    }
    else if(!(flags & BIT5)){
        data |= FIRE;                  // Ready a charge attack
        flags |= BIT5;
    }
    P3->IFG = 0;                         // Clear port 3 interrupt flags
    __enable_irq();
}

extern void PORT5_IRQHandler(void){      // Button 1 (top) pressed, slash attack
    __disable_irq();
    if(flags & BIT5){
        data |= SPIN;                    // Ready a spin attack
        flags &= ~BIT5;
    }
    else if(!(flags & BIT4)){
        data |= SLASH;                   // Ready a slash attack
        flags |= BIT4;
    }
    P5->IFG = 0;                         // Clear port 5 interrupt flags
    __enable_irq();
}
