/*
 * adc.h
 *
 *  Created on: Nov 18, 2017
 *      Author: Nathan
 *
 *  Accelerometer: X = J3.23 = P6.1 = A14, Y = J3.24 = P4.0 = A13
 */

#ifndef ADC_H_
#define ADC_H_

/* The setup function which configures all
 * adc specifications.
 */
void adc_config(void);

/* The interrupt handler function which is
 * called for any adc conversion interrupt.
 */
extern void ADC14_IRQHandler(void);

#endif /* ADC_H_ */
