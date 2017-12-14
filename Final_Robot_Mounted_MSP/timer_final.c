/*
 * timer_final.c
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


void timer_config(void){
    // RESET TIMER REGISTERS
    TIMER_A0->R = 0x0000;                          // Clear the current timer count
    TIMER_A0->CTL = TIMER_A_CTL_CLR;               // Clear all timer A settings

    // CONFIGURE TIMER COUNTING SETTINGS
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP            // Clock in up mode
                   | TIMER_A_CTL_SSEL__SMCLK       // Source clock SMCLK
                   | TIMER_A_CTL_ID__1             // Input divider /1
                   | TIMER_A_CTL_IE;               // Interrupt enabled
    TIMER_A0->R = 0;                               // Reset count value

    // CONFIGURE PWM SETTINGS
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7;   // Compare mode, reset/set mode
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_OUTMOD_7;   // Compare mode, reset/set mode
    TIMER_A0->CCTL[3] |= TIMER_A_CCTLN_OUTMOD_7;   // Compare mode, reset/set mode
    TIMER_A0->CCTL[4] |= TIMER_A_CCTLN_OUTMOD_7;   // Compare mode, reset/set mode
    TIMER_A0->CCR[0] = 59999;                      // Total period count
    TIMER_A0->CCR[1] = 0;                          // Duty cycle P2.4 (val/59999)
    TIMER_A0->CCR[2] = 0;                          // Duty cycle P2.5 (val/59999)
    TIMER_A0->CCR[3] = 3000;                       // Duty cycle P2.6 (val/59999) (Start in left position)
    TIMER_A0->CCR[4] = 2500;                       // Duty cycle P2.7 (val/59999) (Start in forward position)
}
