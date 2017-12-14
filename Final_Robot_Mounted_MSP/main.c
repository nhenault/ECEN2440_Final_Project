// INCLUDE ALL HEADER FILES
#include "msp.h"
#include "port_final.h"
#include "timer_final.h"
#include "uart_final.h"
#include "functions_final.h"
#include "circbuf_final.h"


/**
 * main.c
 */

CircBuf_t Ranged_Controls;
CircBuf_t* Ranged = &Ranged_Controls;
volatile uint32_t LeftEnc = 0;          // Counts left encoder pulses
volatile uint32_t RightEnc = 0;         // Counts right encoder pulses
volatile uint8_t flags = 0;             // BIT0: melee = 0, ranged = 1; BIT1: Indicates a receive;
                                        // BIT2: flags on first bumper interrupt to count every other one

void main(void)
{
    __disable_irq();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // Stop watchdog timer
    port_config();
    timer_config();
    uart_config();
    CB_initialize_buffer(&Ranged, 100);
    __enable_irq();

    while(1){
        if (flags & BIT1){
            Receive_Function();
            flags &= ~BIT1;
        }
    }
}
