/*
 * port_final.h
 *
 *  Created on: Nov 22, 2017
 *      Author: Nathan
 *
 *  J PINS MAY BE WRONG
 *
 *  Interrupt Peripherals:
 *      Right Encoder: J2.12 = P5.2 = PCB pin 9
 *      Left Encoder:  J2.11 = P1.6 = PCB pin 10
 *      Left Bumper:   J?.?? = P3.5
 *      Middle Bumper: J?.?? = P3.6
 *      Right Bumper:  J?.?? = P3.7
 *
 *  Output Peripherals:
 *      Nerf Gun (PWM):            J?.?? = P2.7 = CCR[4]
 *      Sword (PWM):               J?.?? = P2.6 = CCR[3]
 *      Right Reference (PWM):     J2.19 = P2.5 = CCR[2] = PCB pin 8
 *      Left Reference (PWM):      J4.38 = P2.4 = CCR[1] = PCB pin 3
 *      CW Left (Forward):         J3.30 = P5.4 = PCB pin 2
 *      CC Left (Backward):        J3.29 = P5.5 = PCB pin 1
 *      CW Right (Backward):       J2.14 = P1.7 = PCB pin 7
 *      CC Right (Forward):        J2.13 = P5.0 = PCB pin 6
 *      Bluetooth UART (EUSCI_A2): RX = J1.3 = P3.2, TX = J1.4 = P3.3
 */

#ifndef PORT_FINAL_H_
#define PORT_FINAL_H_

/* The setup function which configures all
 * port specifications.
 */
void port_config(void);

/* The interrupt handler function which is
 * called for any port 1 interrupt.
 */
extern void PORT1_IRQHandler(void);

/* The interrupt handler function which is
 * called for any port 3 interrupt.
 */
extern void PORT3_IRQHandler(void);

/* The interrupt handler function which is
 * called for any port 5 interrupt.
 */
extern void PORT5_IRQHandler(void);


#endif /* PORT_FINAL_H_ */
