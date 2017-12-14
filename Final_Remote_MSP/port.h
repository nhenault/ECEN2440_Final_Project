/*
 * port.h
 *
 *  Created on: Nov 18, 2017
 *      Author: Nathan
 *
 *  Interrupt Peripherals:
 *      Booster Button 1 (top):    J4.33 = P5.1
 *      Booster Button 2 (bottom): J4.32 = P3.5
 *
 *  Other Peripherals:
 *      Launchpad RGB LED:         Red = P2.0, Green = P2.1, Blue = P2.2
 *      Accelerometer:             X = J3.23 = P6.1, Y = J3.24 = P4.0
 *      Bluetooth UART (EUSCI_A2): RX = J1.3 = P3.2, TX = J1.4 = P3.3
 *      Computer UART (EUSCI_A0):  RX = P1.2, TX = P1.3
 */

#ifndef PORT_H_
#define PORT_H_

/* The setup function which configures all
 * port specifications.
 */
void port_config(void);

/* The interrupt handler function which is
 * called for any port 3 interrupt.
 */
extern void PORT3_IRQHandler(void);

/* The interrupt handler function which is
 * called for any port 5 interrupt.
 */
extern void PORT5_IRQHandler(void);

#endif /* PORT_H_ */
