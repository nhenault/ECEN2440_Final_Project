/*
 * uart.c
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

extern CircBuf_t* Ranged;
extern volatile uint8_t timer_count;
extern volatile uint8_t flags;
extern volatile uint8_t health;
uint8_t command;

void uart_config(void){
    // PREPARE TO CONFIGURE UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;               // Put eUSCIA0 in reset
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;               // Put eUSCIA2 in reset

    // CONFIGURE UART PROTOCOL
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_PEN                // Parity disabled
                       | EUSCI_A_CTLW0_MSB                // LSB first
                       | EUSCI_A_CTLW0_SEVENBIT           // 8-bit mode
                       | EUSCI_A_CTLW0_SPB                // 1 stop bit
                       | EUSCI_A_CTLW0_MODE0              // UART Mode
                       | EUSCI_A_CTLW0_MODE1);            // UART Mode
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_PEN                // Parity disabled
                       | EUSCI_A_CTLW0_MSB                // LSB first
                       | EUSCI_A_CTLW0_SEVENBIT           // 8-bit mode
                       | EUSCI_A_CTLW0_SPB                // 1 stop bit
                       | EUSCI_A_CTLW0_MODE0              // UART Mode
                       | EUSCI_A_CTLW0_MODE1);            // UART Mode

    // SET BAUD RATES TO 115200
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;         // Clock source SMCLK
    EUSCI_A0->BRW = 1;                                    // UCBRx = 1
    EUSCI_A0->MCTLW |= 0x00A0 | EUSCI_A_MCTLW_OS16;       // UCBRSx = 0.0417, UCBRFx = 10, OS16 = 1
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;         // Clock source SMCLK
    EUSCI_A2->BRW = 1;                                    // UCBRx = 1
    EUSCI_A2->MCTLW |= 0x00A0 | EUSCI_A_MCTLW_OS16;       // UCBRSx = 0.0417, UCBRFx = 10, OS16 = 1

    // STOP CONFIGURING UART
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;              // Initialize eUSCIA0
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;              // Initialize eUSCIA2

    // ENABLE RECEIVE INTERRUPTS
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;                      // Enable receive interrupt
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;                      // Enable receive interrupt
    NVIC_EnableIRQ(EUSCIA0_IRQn);                         // Enable interrupt in NVIC
    NVIC_EnableIRQ(EUSCIA2_IRQn);                         // Enable interrupt in NVIC
}


extern void EUSCIA0_IRQHandler(void){                          // Receive interrupt from computer (ranged input)
    __disable_irq();
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){
        uint8_t total = 0;
        uint8_t multiplier = 0;
        uint8_t power = 0;                                         // Create temp variables for transmitting ranged commands
        uint8_t comp_receive = EUSCI_A0->RXBUF;                    // Grab the received value
        if(comp_receive == 't'){
            flags ^= BIT0;
            Bluetooth_send_byte(SWITCH);
        }

        if(flags & BIT0){                                          // If in ranged mode
            if(comp_receive == 'g'){                               // If g is received
                Bluetooth_send_byte(GO);                           // Send the go command
            }
            if(!command){                                          // If no command is currently set

                // CHECK THE RECEIVED VALUES TO SET THE COMMAND VALUE
                if(comp_receive == 'b'){
                    command = BACKWARD;
                    Bluetooth_send_byte(command);
                    Terminal_send_n("\n\rBackward", 10);
                }
                else if(comp_receive == 'f'){
                    command = FORWARD;
                    Bluetooth_send_byte(command);
                    Terminal_send_n("\n\rForward", 9);
                }
                else if(comp_receive == 'l'){
                    command = LEFT;
                    Bluetooth_send_byte(command);
                    Terminal_send_n("\n\rLeft", 6);
                }
                else if(comp_receive == 'm'){
                    command = MELEE;
                    Bluetooth_send_byte(command);
                    Terminal_send_n("\n\rMelee", 7);
                    command = 0;                     // Melee doesn't take a follow up number
                }
                else if(comp_receive == 'r'){
                    command = RIGHT;
                    Bluetooth_send_byte(command);
                    Terminal_send_n("\n\rRight", 7);
                }
                else if(comp_receive == 's'){
                    command = SHOOT;
                    Bluetooth_send_byte(command);
                    Terminal_send_n("\n\rShoot", 7);
                    command = 0;                     // Shoot doesn't take a follow up number
                }
                else{
                    command = 0;
                }
            }

            if(command && (47 < comp_receive) && (comp_receive < 58)){  // If a command is currently set, read any following numbers
                CB_add_item_to_buffer(Ranged, comp_receive);
            }
            else{                                                       // If there's no command or no number, transmit number
                total = 0;
                Terminal_send_n("\n\r", 2);
                while(!CB_is_buffer_empty(Ranged)){
                    power = Ranged->num_items;
                    multiplier = 1;                                     // Initialize the multiplier
                    while(power - 1){
                        multiplier *= 10;                               // Set up the multiplier
                        power--;
                    }
                    command = CB_remove_item_from_buffer(Ranged);       // Grab the first number in the buffer
                    Terminal_send_byte(command);                        // Transmit numbers to the terminal
                    command -= '0';                                     // Convert the ASCII value to it's int value
                    command *= multiplier;                              // Convert the value to the proper decimal place
                    total += command;                                   // Sum up all components
                    command = 0;                                        // Reset command
                }
                Bluetooth_send_byte(total);                             // Transmit the value
            }
        }
        EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;
    }
    __enable_irq();
}

extern void EUSCIA2_IRQHandler(void){                         // Receive interrupt from robot (health status)
    __disable_irq();
    if(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG){
        uint8_t bluetooth_receive = EUSCI_A2->RXBUF;          // Grab the received value
            if(bluetooth_receive == 15 && (flags & BIT3)){    // If a 15 is received (damage taken)
                if(health == 3){                              // If LED is green
                    P2->OUT = 3;                              // Turn it yellow
                    health--;                                 // Decrement health
                }
                else if(health == 2){                         // If LED is yellow
                    P2->OUT = 1;                              // Turn LED red
                    health--;                                 // Decrement health
                }
                else if(health == 1){                         // If LED is red
                    P2->OUT = 0;                              // Turn LED off
                    flags |= BIT1;                            // Set the dead flag
                }
                timer_count = 0;                              // Reset timer_count
                flags &= ~BIT3;                               // Can't take damage for 2 seconds
            }
            EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;
    }
    __enable_irq();
}

void Terminal_send_n(uint8_t* data, uint32_t length){
    int i = 0;
    for (i=0;i<length;i++){
        Terminal_send_byte(data[i]);         // Load one byte at a time
    }
}

void Terminal_send_byte(uint8_t data){
    while (!(EUSCI_A0->IFG & BIT1));      // Wait for previous transmission to finish
    EUSCI_A0->TXBUF = data;               // Load data to be transmitted
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
