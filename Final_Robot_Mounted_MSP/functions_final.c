/*
 * action.c
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

// INCLUDE EXTRA HEADERS FOR FUNCTIONS
#include <math.h>     // For pow and round
#include <string.h>   // For strlen

extern CircBuf_t* Ranged;                  // Ranged command queue
extern volatile uint32_t LeftEnc;          // Left encoder count
extern volatile uint32_t RightEnc;         // Right encoder count
uint8_t first = 0;                         // First value for Ranged_Commands (command)
uint8_t second = 0;                        // Second value for Ranged_Commands (distance/degrees)
extern volatile uint8_t flags;

void Robot_Action_Melee(uint8_t attack, uint8_t motion){
    if(attack > NO_ATTACK){                   // If an attack has been received
        if(attack == SLASH){
            // Slash left or right based on current position
            if (TIMER_A0->CCR[3] == 6000){
                TIMER_A0->CCR[3] = 3000;
            }
            else if (TIMER_A0->CCR[3] == 3000){
                TIMER_A0->CCR[3] = 6000;
            }
        }

        else if(attack == FIRE){
            __disable_irq();
            if (TIMER_A0->CCR[4] == 2500){                // if trigger ready
                int i;
                TIMER_A0->CCR[4] = 6500;                  // pull trigger
                for (i = 0;i < 200000;i++);
                TIMER_A0->CCR[4] = 2500;                  // reset trigger
                for(i = 0;i < 150000;i++);                // Delay for the shoot to reset
            }
            __enable_irq();
        }

        else if (attack == SPIN){
            int i;
            int j;
            TIMER_A0->CCR[1] = 59999;  // 100% Speed
            TIMER_A0->CCR[2] = 59899;
            P1->OUT &= ~BIT7;          // CW Right off
            P5->OUT &= ~BIT4;          // CW Left off
            P5->OUT |= BIT0;           // CC Right on
            P5->OUT |= BIT5;           // CC Left on
            for(i = 0;i < 10;i++){
                if (TIMER_A0->CCR[3] == 6000){
                    TIMER_A0->CCR[3] = 3000;
                }
                else if (TIMER_A0->CCR[3] == 3000){
                    TIMER_A0->CCR[3] = 6000;
                }
                for(j = 0;j < 70000;j++);  // Small delay to slash
            }
            P5->OUT &= ~BIT0;           // CC Right on
            P5->OUT &= ~BIT5;           // CC Left on
        }
    }

    if(motion <= FAST_FORWARD){               // If the motion is Forward
        P5->OUT &= ~BIT5;                     // Set CC Left off
        P1->OUT &= ~BIT7;                     // Set CW Right off
        if(motion == SLOW_FORWARD){
            TIMER_A0->CCR[1] = 20000;         // Set Left Ref to ~33% speed
            TIMER_A0->CCR[2] = 20000;         // Set Right Ref to ~33% speed
        }
        else if(motion == FAST_FORWARD){
            TIMER_A0->CCR[1] = 40000;         // Set Left Ref to ~66% speed
            TIMER_A0->CCR[2] = 40000;         // Set Right Ref to ~66% speed
        }
        P5->OUT |= BIT4;                      // Set CW Left on
        P5->OUT |= BIT0;                      // Set CC Right on
    }

    else if(motion <= FAST_BACKWARD){         // If the motion is Backward
        P5->OUT &= ~BIT4;                     // Set CW Left off
        P5->OUT &= ~BIT0;                     // set CC Right off
        if(motion == SLOW_BACKWARD){
            TIMER_A0->CCR[1] = 20000;         // Set Left Ref to ~33% speed
            TIMER_A0->CCR[2] = 20000;         // Set Right Ref to ~33% speed
        }
        else if(motion == FAST_BACKWARD){
            TIMER_A0->CCR[1] = 40000;         // Set Left Ref to ~66% speed
            TIMER_A0->CCR[2] = 40000;         // Set Right Ref to ~66% speed
        }
        P5->OUT |= BIT5;                     // Set CC Left on
        P1->OUT |= BIT7;                     // Set CW Right on
    }

    else if(motion <= FAST_LEFT){            // If the motion is right
        P5->OUT &= ~BIT4;                    // Set CW Left off
        P1->OUT &= ~BIT7;                    // Set CW Right off
        if(motion == SLOW_LEFT){
            TIMER_A0->CCR[1] = 10000;         // Set Left Ref to low turn speed
            TIMER_A0->CCR[2] = 10000;         // Set Right Ref to low turn speed
        }
        else if(motion == FAST_LEFT){
            TIMER_A0->CCR[1] = 20000;        // Set Left Ref to low turn speed
            TIMER_A0->CCR[2] = 20000;        // Set Right Ref to low turn speed
        }
        P5->OUT |= BIT5;                     // Set CC Left on
        P5->OUT |= BIT0;                     // Set CC Right on
    }

    else if(motion <= FAST_RIGHT){           // if the motion is left
        P5->OUT &= ~BIT5;                    // Set CC Left off
        P5->OUT &= ~BIT0;                    // Set CC Right off
        if(motion == SLOW_RIGHT){
            TIMER_A0->CCR[1] = 10000;         // Set Left Ref to low turn speed
            TIMER_A0->CCR[2] = 10000;         // Set Right Ref to low turn speed
        }
        else if(motion == FAST_RIGHT){
            TIMER_A0->CCR[1] = 20000;        // Set Left Ref to low turn speed
            TIMER_A0->CCR[2] = 20000;        // Set Right Ref to low turn speed
        }
        P5->OUT |= BIT4;                     // Set CW Left on
        P1->OUT |= BIT7;                     // Set CW Right on
    }

    else{                                    // If motion is stopped
        P5->OUT &= ~BIT5;
        P5->OUT &= ~BIT4;
        P1->OUT &= ~BIT7;
        P5->OUT &= ~BIT0;                    // Turn off all motion
    }
}

void Robot_Action_Ranged(uint8_t direction, float distance){
    LeftEnc = 0;
    RightEnc = 0;                   // Reset encoder counts
    distance /= 42.666;             // Convert distance to rotations
    distance *= 768;                // Convert rotations to encoder pulses
    float degrees = distance;
    degrees /= 360;                 // Convert degrees to positional rotations
    degrees *= 88.354;              // Convert positional rotations to distance per wheel
    degrees = truncf(degrees);      // rounds degree to the nearest integer
    distance = truncf(distance);    // rounds distance to the nearest integer

    if(direction == BACKWARD){
        TIMER_A0->CCR[1] = 10100;             // Set Left Ref to low speed
        TIMER_A0->CCR[2] = 9900;              // Set Right Ref to low speed
        P5->OUT &= ~BIT4;                     // Set CW Left off
        P5->OUT &= ~BIT0;                     // set CC Right off
        P5->OUT |= BIT5;                      // Set CC Left on
        P1->OUT |= BIT7;                      // Set CW Right on

        while((LeftEnc < distance) || (RightEnc < distance)){ // Until both wheels finish
            if (LeftEnc > distance){                          // Until left wheel finishes
                P5->OUT &= ~BIT5;                             // Stop left wheel
            }
            if (RightEnc > distance){                         // Until right wheel finishes
                P1->OUT &= ~BIT7;                             // Stop right wheel
            }
            if(((LeftEnc + 400) > distance) || ((RightEnc + 400) > distance)){    // Slows down at the end
                TIMER_A0->CCR[1] = 10100;
                TIMER_A0->CCR[2] = 9900;
            }
            else if((LeftEnc < 300) || (RightEnc < 300)){                         // Maintains slowness at beginning
                TIMER_A0->CCR[1] = 10100;
                TIMER_A0->CCR[2] = 9900;
            }
            else{                                                                 // Speeds up in the middle
                TIMER_A0->CCR[1] = 20200;
                TIMER_A0->CCR[2] = 19800;
            }
        }
        P1->OUT &= ~BIT7;                             // Stop right wheel
        P5->OUT &= ~BIT5;                             // Stop left wheel
    }

    else if(direction == FORWARD){
        TIMER_A0->CCR[1] = 10100;             // Set Left Ref to low speed
        TIMER_A0->CCR[2] = 9900;             // Set Right Ref to low speed
        P5->OUT &= ~BIT5;                     // Set CC Left off
        P1->OUT &= ~BIT7;                     // Set CW Right off
        P5->OUT |= BIT4;                      // Set CW Left on
        P5->OUT |= BIT0;                      // set CC Right on

        while((LeftEnc < distance) || (RightEnc < distance)){ // Until both wheels finish
            if (LeftEnc > distance){                          // Until left wheel finishes
                P5->OUT &= ~BIT4;                             // Stop left wheel
            }
            if (RightEnc > distance){                         // Until right wheel finishes
                P5->OUT &= ~BIT0;                             // Stop right wheel
            }
            if(((LeftEnc + 400) > distance) || ((RightEnc + 400) > distance)){    // Slows down at the end
                TIMER_A0->CCR[1] = 10100;
                TIMER_A0->CCR[2] = 9900;
            }
            else if((LeftEnc < 300) || (RightEnc < 300)){                         // Maintains slowness at beginning
                TIMER_A0->CCR[1] = 10100;
                TIMER_A0->CCR[2] = 9900;
            }
            else{                                                                 // Speeds up in the middle
                TIMER_A0->CCR[1] = 20200;
                TIMER_A0->CCR[2] = 19800;
            }
        }
        P5->OUT &= ~BIT0;                             // Stop right wheel
        P5->OUT &= ~BIT4;                             // Stop left wheel
    }

    else if(direction == LEFT){
        TIMER_A0->CCR[1] = 6060;              // Set Left Ref to 25% speed
        TIMER_A0->CCR[2] = 5940;              // Set Right Ref to 25% speed
        P1->OUT &= ~BIT7;                     // Set CW Right off
        P5->OUT &= ~BIT4;                     // Set CW Left off
        P5->OUT |= BIT5;                      // Set CC Left on
        P5->OUT |= BIT0;                      // set CC Right on

        while((LeftEnc < degrees) || (RightEnc < degrees)){ // Until both wheels finish
            if (LeftEnc > degrees){                          // Until left wheel finishes
                P5->OUT &= ~BIT5;                             // Stop left wheel
            }
            if (RightEnc > degrees){                         // Until right wheel finishes
                P5->OUT &= ~BIT0;                             // Stop right wheel
            }
            if(((LeftEnc + 400) > degrees) || ((RightEnc + 400) > degrees)){    // Slows down at the end
                TIMER_A0->CCR[1] = 6060;
                TIMER_A0->CCR[2] = 5940;
            }
            else if((LeftEnc < 300) || (RightEnc < 300)){                         // Maintains slowness at beginning
                TIMER_A0->CCR[1] = 6060;
                TIMER_A0->CCR[2] = 5940;
            }
            else{                                                                 // Speeds up in the middle
                TIMER_A0->CCR[1] = 12120;
                TIMER_A0->CCR[2] = 11880;
            }
        }
        P5->OUT &= ~BIT0;                             // Stop right wheel
        P5->OUT &= ~BIT5;                             // Stop left wheel
    }

    else if(direction == MELEE){
        if (TIMER_A0->CCR[3] == 6000){
            TIMER_A0->CCR[3] = 3000;
        }
        else if (TIMER_A0->CCR[3] == 3000){
            TIMER_A0->CCR[3] = 6000;
        }
        int i;
        for(i = 0;i < 150000;i++);           // Delay for the melee swing to run
    }

    else if(direction == RIGHT){
        TIMER_A0->CCR[1] = 6060;             // Set Left Ref to 25% speed
        TIMER_A0->CCR[2] = 5940;             // Set Right Ref to 25% speed
        P5->OUT &= ~BIT5;                     // Set CC Left off
        P5->OUT &= ~BIT0;                     // set CC Right off
        P1->OUT |= BIT7;                      // Set CW Right on
        P5->OUT |= BIT4;                      // Set CW Left on

        while((LeftEnc < degrees) || (RightEnc < degrees)){ // Until both wheels finish
            if (LeftEnc > degrees){                          // Until left wheel finishes
                P5->OUT &= ~BIT4;                             // Stop left wheel
            }
            if (RightEnc > degrees){                         // Until right wheel finishes
                P1->OUT &= ~BIT7;                             // Stop right wheel
            }
            if(((LeftEnc + 400) > degrees) || ((RightEnc + 400) > degrees)){    // Slows down at the end
                TIMER_A0->CCR[1] = 6060;
                TIMER_A0->CCR[2] = 5940;
            }
            else if((LeftEnc < 300) || (RightEnc < 300)){                         // Maintains slowness at beginning
                TIMER_A0->CCR[1] = 6060;
                TIMER_A0->CCR[2] = 5940;
            }
            else{                                                                 // Speeds up in the middle
                TIMER_A0->CCR[1] = 12120;
                TIMER_A0->CCR[2] = 11880;
            }
        }
        P1->OUT &= ~BIT7;                             // Stop right wheel
        P5->OUT &= ~BIT4;                             // Stop left wheel
    }

    else if(direction == SHOOT){
        if (TIMER_A0->CCR[4] == 2500){                // if trigger ready
            int i;
            TIMER_A0->CCR[4] = 6500;                  // pull trigger
            for (i=0; i<200000; i++);
            TIMER_A0->CCR[4] = 2500;                  // reset trigger
            for(i = 0;i < 150000;i++);                // Delay for the shoot to reset
        }
    }
    LeftEnc = 0;
    RightEnc = 0;                                     // Reset Encoder counts
}

void Ranged_Commands(void){
    while(!CB_is_buffer_empty(Ranged)){
        first = CB_remove_item_from_buffer(Ranged);     // Remove the first item from the buffer
        second = CB_remove_item_from_buffer(Ranged);    // Remove the second item from the buffer
        Robot_Action_Ranged(first, (float)second);      // Call the action function with the values
    }
}

void Receive_Function(void){
    uint8_t motion = 0;                             // The attack portion of received signals (melee)
    uint8_t attack = 0;                             // The motion portion of received signals (melee)
    uint8_t receive = EUSCI_A2->RXBUF;              // Grab the received value
    if(receive == SWITCH){
        flags ^= BIT0;                              // Toggle the mode if switch is received
        CB_clear_buffer(Ranged);                    // Clear the buffer in case any values snuck in
        if(flags & BIT0){                           // If in ranged mode
            P1->IE |= BIT6;
            P5->IE |= BIT2;                         // Enable the port interrupts for counting encoders
            P1->IFG = 0x00;
            P5->IFG = 0x00;                         // Clear flags
        }
        else{                                       // If in melee mode
            P1->IE &= ~BIT6;
            P5->IE &= ~BIT2;                        // Disable the port interrupts for counting encoders
        }
    }

    if((flags & BIT0) && (receive != SWITCH)){      // If in ranged mode
        if(receive == GO){                          // If the go command is received
            Ranged_Commands();                      // Run everything from the circbuf
        }

        else{                                       // If no go command
            CB_add_item_to_buffer(Ranged, receive); // Add received to circbuf queue
            if(receive == MELEE ||
               receive == SHOOT){                   // If the command is melee or shoot
                CB_add_item_to_buffer(Ranged, 1);   // Add a 1 to fill in the distance opening
            }
        }
    }

    else{                                           // If in melee mode
        motion = receive & 0x0F;                    // Set motion equal to the motion bits
        attack = receive & 0xF0;                    // Set attack equal to the attack bits
        Robot_Action_Melee(attack, (float)motion);  // Call melee action with attack and motion
    }
}
