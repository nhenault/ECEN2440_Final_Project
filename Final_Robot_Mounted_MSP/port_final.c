/*
 * port_final.c
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

extern volatile uint32_t LeftEnc;
extern volatile uint32_t RightEnc;
extern volatile uint8_t flags;

void port_config(void){
    // RESET ALL PORT SELECT BITS TO DEFAULT (GPIO MODE)
    P1->SEL0 = 0x00;                     // All P1->SEL = 00
    P1->SEL1 = 0x00;                     // Reset all P1 select bits to gpio mode
    P2->SEL0 = 0x00;                     // All P2->SEL = 00
    P2->SEL1 = 0x00;                     // Reset all P2 select bits to gpio mode
    P3->SEL0 = 0x00;                     // All P3->SEL = 00
    P3->SEL1 = 0x00;                     // Reset all P3 select bits to gpio mode
    P5->SEL0 = 0x00;                     // All P5->SEL = 00
    P5->SEL1 = 0x00;                     // Reset all P5 select bits to gpio mode

    // CONFIGURE PWM PINS
    P2->SEL1 &= ~(BIT4 | BIT5 |
                  BIT6 | BIT7);          // P2.4,5,6,7 SEL bits to 01
    P2->SEL0 |= BIT4 | BIT5 |
                BIT6 | BIT7;             // P2.4,5,6,7 to PWM mode
    P2->DIR |= BIT4 | BIT5 |
               BIT6 | BIT7;              // Set pins as outputs

    // CONFIGURE UART PINS
    P3->SEL1 &= ~(BIT2 | BIT3);          // P3.2,3 in UART mode
    P3->SEL0 |= BIT2 | BIT3;             // P3.2 = J1.3 = RX, P3.3 = J1.4 = TX

    // CONFIGURE OUTPUT PINS
    P1->DIR |= BIT7;                     // Set P1.7 as an output
    P1->OUT &= ~BIT7;                    // Set P1.7 to low
    P5->DIR |= BIT0 | BIT4 | BIT5;       // Set P5.0,4,5 as outputs
    P5->OUT &= ~(BIT0 | BIT4 | BIT5);    // Set P5.0,4,5 to low

    // CONFIGURE ENCODER PINS AND INTERRUPTS
    P1->DIR &= ~BIT6;                    // Set P1.6 as an input
    P1->REN |= BIT6;                     // Enable Pullup/Pulldown
    P1->OUT |= BIT6;                     // Set to Pullup resistor
    P1->IES &= ~BIT6;                    // Set P1.6 to flag low-to-high transition
    P1->IE &= ~BIT6;                     // Disable P1.6 interrupt
    P1->IFG = 0x00;                      // Clears P1 interrupt flag
    P5->DIR &= ~BIT2;                    // Set P5.2 as an input
    P5->REN |= BIT2;                     // Enable Pullup/Pulldown
    P5->OUT |= BIT2;                     // Set to Pullup resistor
    P5->IES &= ~BIT2;                    // Set P5.2 to flag low-to-high transition
    P5->IE &= ~BIT2;                     // Disable P5.2 interrupt
    P5->IFG = 0x00;                      // Clears P5 interrupt flag
    NVIC_EnableIRQ(PORT1_IRQn);          // Enables P1 interrupt in the NVIC
    NVIC_EnableIRQ(PORT5_IRQn);          // Enables P5 interrupt in the NVIC

    // CONFIGURE BUMPER PINS AND INTERRUPTS
    P3->DIR &= ~(BIT5 | BIT6 | BIT7);    // Set P3.4,5,6 as inputs
    P3->REN |= BIT5 | BIT6 | BIT7;       // Enable Pullup/Pulldown
    P3->OUT &= ~(BIT5 | BIT6 | BIT7);    // Set to Pulldown resistor
    P3->IES |= BIT5 | BIT6 | BIT7;       // Set P3.4,5,6 to flag high-to-low transition
    P3->IE |= BIT5 | BIT6 | BIT7;        // Enable P3.4,5,6 interrupts
    P3->IFG = 0x00;                      // Clears P3 interrupt flags
    NVIC_EnableIRQ(PORT3_IRQn);          // Enables P3 interrupt in the NVIC
}

extern void PORT1_IRQHandler(void){
    if(P1->IFG & BIT6){
        LeftEnc++;
        P1->IFG &= ~BIT6;                    // Clears encoder interrupt flag
    }
}

extern void PORT3_IRQHandler(void){
    if(P3->IFG & (BIT5 | BIT6 | BIT7)){      // If a bumper triggers
        Bluetooth_send_byte(15);         // Send a 15 to indicate damage
        P3->IFG = 0x00;                      // Clear bumper interrupt flags
    }
}

extern void PORT5_IRQHandler(void){
    if(P5->IFG & BIT2){
        RightEnc++;
        P5->IFG &= ~BIT2;                    // Clears encoder interrupt flag
    }
}
