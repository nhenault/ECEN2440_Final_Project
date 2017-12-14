/*
 * uart.h
 *
 *  Created on: Nov 18, 2017
 *      Author: Nathan
 *
 *  Bluetooth UART (EUSCI_A2): RX = J1.3 = P3.2, TX = J1.4 = P3.3
 *  Computer UART (EUSCI_A0):  RX = P1.2, TX = P1.3
 */

#ifndef UART_H_
#define UART_H_

/* The setup function which configures all
 * uart specifications.
 */
void uart_config(void);

/* The interrupt handler function which is
 * called for any UART receive interrupt
 * from the computer.
 */
extern void EUSCIA0_IRQHandler(void);

/* The interrupt handler function which is
 * called for any UART receive interrupt
 * from the robot mounted MSP.
 */
extern void EUSCIA2_IRQHandler(void);

/* A function which sends multiple bytes
 * through computer terminal.
 * @param *data = pointer to data being sent.
 * @param length = number of bytes being sent.
 */
void Terminal_send_n(uint8_t* data, uint32_t length);

/* A function which prepares a single
 * byte to be transmitted to the
 * computer terminal.
 * @param data = byte being sent.
 */
void Terminal_send_byte(uint8_t data);

/* A function which sends multiple bytes
 * through bluetooth.
 * @param *data = pointer to data being sent.
 * @param length = number of bytes being sent.
 */
void Bluetooth_send_n(uint8_t* data, uint32_t length);

/* A function which prepares a single
 * byte to be transmitted through bluetooth.
 * @param data = byte being sent.
 */
void Bluetooth_send_byte(uint8_t data);

#endif /* UART_H_ */
