/*
 * timer.h
 *
 *  Created on: Nov 19, 2017
 *      Author: Nathan
 */

#ifndef TIMER_H_
#define TIMER_H_

/* The setup function which configures all
 * timer specifications.
 */
void timer_config(void);

/* The interrupt handler function which is
 * called for the Timer_A0 interrupt.
 */
extern void TA0_0_IRQHandler(void);

/* The interrupt handler function which is
 * called for the Timer_A1 interrupt.
 */
extern void TA1_0_IRQHandler(void);

#endif /* TIMER_H_ */
