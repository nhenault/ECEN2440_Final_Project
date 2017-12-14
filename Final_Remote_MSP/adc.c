/*
 * adc.c
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
uint16_t accelX = 0;
uint16_t accelY = 0;

void adc_config(void){
    // PREPARE FOR CONFIGURATION
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);                      // Wait until ref generator isn't busy
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;              // Enable internal 1.2V ref
    REF_A->CTL0 |= REF_A_CTL0_TCOFF;                              // Turn off temperature sensor
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;                               // Disable conversion

    // CONFIGURE CTL0 REGISTER
    ADC14->CTL0 |= ADC14_CTL0_SHT0_5;                             // Sample and hold time = 96 clock cycles for MEM0-7 and MEM24-31
    ADC14->CTL0 |= ADC14_CTL0_ON;                                 // Enables the ADC to turn on when valid input is received
    ADC14->CTL0 |= ADC14_CTL0_SHP;                                // Source signal from the sampling timer
    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;                           // Scan all channels when called

    // CONFIGURE CTL1 REGISTER
    ADC14->CTL1 &= ~ADC14_CTL1_RES__14BIT;                        // Clear the resolution bits
    ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;                         // Put ADC14 into 14-bit resolution mode
    ADC14->CTL1 &= ~ADC14_CTL1_CSTARTADD_MASK;                    // Start sequence scan at channel 0

    // CONFIGURE ACCELEROMETER X AND Y
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_14 | ADC14_MCTLN_VRSEL_0;   // Maps accelerometer X to MEM0 and sets Vref to 3.3V
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_0;   // Maps accelerometer Y to MEM1 and sets Vref to 3.3V
    ADC14->IER0 = BIT0 | BIT1;                                    // Enable interrupt request for IFG0,1, the MEM0,1 flags

    // EXIT CONFIGURATION
    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY));                    // Wait for ref generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC;                                // Reenable conversion
    NVIC_EnableIRQ(ADC14_IRQn);                                   // Enable interrupts in the NVIC
}

extern void ADC14_IRQHandler(void){           // Complete conversion interrupt (accelerometer)
    __disable_irq();
    accelX = ADC14->MEM[0];                   // Set accelX to its conversion value
    accelY = ADC14->MEM[1];                   // Set accelY to its conversion value
    data &= 0xF0;                             // Clear the data motion bits

    if(accelY < 7400){                        // If tilted backwards
        data |= SLOW_BACKWARD;
        if(accelY < 6400){
            data |= FAST_BACKWARD;
        }
    }

    else if(accelY > 9000){                   // If tilted forwards
        data |= SLOW_FORWARD;
        if(accelY > 10000){
            data |= FAST_FORWARD;
        }
    }

    else{
        if(accelX < 7400){                    // If tilted left
            data |= SLOW_LEFT;
            if(accelX < 6400){
                data |= FAST_LEFT;
            }
        }

        else if(accelX > 9000){               // If tilted right
            data |= SLOW_RIGHT;
            if(accelX > 10000){
                data |= FAST_RIGHT;
            }
        }

        else
            data |= STOP;                     // If nothing gets set, load a stop
    }
    __enable_irq();
}
