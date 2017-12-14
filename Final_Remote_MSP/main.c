// INCLUDE ALL HEADER FILES
#include "msp.h"
#include "port.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "circbuf.h"
#include "functions.h"

/**
 * main.c
 */

CircBuf_t Ranged_Controls;
CircBuf_t* Ranged = &Ranged_Controls;
volatile uint8_t timer_count = 0;
volatile uint8_t data = 0;
volatile uint8_t health = 3;
volatile uint8_t flags = 0; // BIT0: melee = 0, ranged = 1, BIT2: Calls the integer send function (for distance in ranged mode)
                            // BIT1: dead flag, BIT3: ready to take damage flag (1 = ready for damage)
                            // BIT4: button 1 flag, BIT5: button 2 flag

void main(void)
{
    __disable_irq();
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// Stop watchdog timer
	port_config();                                  // Configure port settings and gpio interrupts
	timer_config();                                 // Configure timer interrupts
	uart_config();                                  // Configure UART settings
	adc_config();                                   // Configure ADC settings
	CB_initialize_buffer(&Ranged, 5);               // Create a buffer capable of holding up to 5 chars
	__enable_irq();                                 // Enable interrupts
	data = 0x00;                                    // Clear this nonsense because port interrupts hate initializing
	flags = 0x00;
	health = 3;                                     // Assure the value of health is set

	while(1){
	    if(flags & BIT0){                           // When in ranged mode
	        NVIC_DisableIRQ(PORT3_IRQn);
	        NVIC_DisableIRQ(PORT5_IRQn);
	        NVIC_DisableIRQ(TA0_0_IRQn);
	        NVIC_DisableIRQ(ADC14_IRQn);            // Disable all melee mode interrupts
	    }
	    else{                                       // When in melee mode
	        NVIC_EnableIRQ(PORT3_IRQn);
	        NVIC_EnableIRQ(PORT5_IRQn);
	        NVIC_EnableIRQ(TA0_0_IRQn);
	        NVIC_EnableIRQ(ADC14_IRQn);             // Enable all melee mode interrupts
	    }

	    if(flags & BIT1){
	        __disable_irq();
	        Bluetooth_send_byte(STOP + NO_ATTACK);  // Send a stop all command
	    }

	    ADC14->CTL0 |= ADC14_CTL0_SC;               // Start an accelerometer conversion
	}
}
