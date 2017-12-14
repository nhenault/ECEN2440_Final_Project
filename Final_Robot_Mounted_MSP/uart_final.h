/*
 * uart_final.h
 *
 *  Created on: Nov 22, 2017
 *      Author: Nathan
 */

#ifndef UART_FINAL_H_
#define UART_FINAL_H_

/* The setup function which configures all
 * uart specifications.
 */
void uart_config(void);

/* The interrupt handler function which is
 * called for any UART receive interrupt
 * from the robot mounted MSP.
 */
extern void EUSCIA2_IRQHandler(void);

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

#endif /* UART_FINAL_H_ */
