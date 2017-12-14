/*
 * timer.c
 *
 *  Created on: Nov 19, 2017
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

extern volatile uint8_t timer_count;
extern volatile uint8_t data;
extern volatile uint8_t health;
extern volatile uint8_t flags;

void timer_config(void){
    // RESET TIMER REGISTERS
    TIMER_A0->R = 0x0000;                         // Clear the current timer count
    TIMER_A0->CTL = TIMER_A_CTL_CLR;              // Clear all timer A0 settings
    TIMER_A1->R = 0x0000;                         // Clear the current timer count
    TIMER_A1->CTL = TIMER_A_CTL_CLR;              // Clear all timer A1 settings

    // CONFIGURE TIMER COUNTING SETTINGS
    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK;     // Sets SMCLK as clock source, 3MHz
    TIMER_A0->CTL |= TIMER_A_CTL_ID__8;           // Sets clock divider to /8, 375kHz
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;          // Sets Timer A0 to up mode
    TIMER_A0->EX0 |= TIMER_A_EX0_IDEX__6;         // Sets extension divider to /6, 62500
    TIMER_A1->CTL |= TIMER_A_CTL_SSEL__SMCLK;     // Sets SMCLK as clock source, 3MHz
    TIMER_A1->CTL |= TIMER_A_CTL_ID__8;           // Sets clock divider to /8, 375kHz
    TIMER_A1->CTL |= TIMER_A_CTL_MC__UP;          // Sets Timer A0 to up mode
    TIMER_A1->EX0 |= TIMER_A_EX0_IDEX__6;         // Sets extension divider to /6, 62500

    // CONFIGURE TIMER_A0 INTERRUPT
    TIMER_A0->CCR[0] = 6250;                      // Count to 6250, 0.1 second per interrupt
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;      // Enables the capture/compare interrupt
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;    // Clear the capture/compare interrupt flag

    // CONFIGURE TIMER_A1 INTERRUPT
    TIMER_A1->CCR[0] = 62500;                     // Count to 62500, 1 second per interrupt
    TIMER_A1->CCTL[0] |= TIMER_A_CCTLN_CCIE;      // Enables the capture/compare interrupt
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;    // Clear the capture/compare interrupt flag

    NVIC_EnableIRQ(TA0_0_IRQn);                   // Enable Timer_A0 interrupts in NVIC
    NVIC_EnableIRQ(TA1_0_IRQn);                   // Enable Timer_A1 interrupts in NVIC
}

extern void TA0_0_IRQHandler(void){
    __disable_irq();
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG){
        if(!(data & 0xF0)){                           // If no attack information
            data |= NO_ATTACK;                        // Store no attack
        }
        if(data & 0x80){                              // If attack is a spin
            data &= 0x8F;                             // Clear non-spin attack bits
        }
        Bluetooth_send_byte(data);                    // Transmit data
        data = 0x00;                                  // Clear data
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;    // Clear the capture/compare interrupt flag
    }
    __enable_irq();
}

extern void TA1_0_IRQHandler(void){
    __disable_irq();
    if(TIMER_A1->CCTL[0] & TIMER_A_CCTLN_CCIFG){
        char CurrentHealth[1];
        char Time[2];
        timer_count++;
        flags &= ~BIT4;
        flags &= ~BIT5;
        if(timer_count >= 2){
            flags |= BIT3;
        }
        if(timer_count >= 15){
            if(P2->OUT == 3){                         // If LED is yellow
                P2->OUT = 2;                          // Turn it green
                health++;                             // Increment health
            }
            else if(P2->OUT == 1){                    // If LED is red
                P2->OUT = 3;                          // Turn LED yellow
                health++;                             // Increment health
            }
            timer_count = 0;                          // reset timer_count
        }
        itoa((int)health, CurrentHealth);
        itoa((int)(15 - timer_count), Time);
        Terminal_send_n("\n\r\n\rHealth: ", 12);
        Terminal_send_byte(*(uint8_t*)CurrentHealth);
        Terminal_send_n("\n\rTime Until Heal: ", 19);
        Terminal_send_n((uint8_t*)Time, 2);
        TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;    // Clear the capture/compare interrupt flag
    }
    __enable_irq();
}
