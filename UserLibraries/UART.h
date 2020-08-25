/*
 * UART.h
 *
 *  Created on: Oct 17, 2019
 *      Author: Minh Thuan
 */

#ifndef USERLIBRARIES_UART_H_
#define USERLIBRARIES_UART_H_

#define RX0_PIN          GPIO_PIN_0
#define TX0_PIN          GPIO_PIN_1

void UART_Init(void);

void UART_SendSpeed(int16_t i16_Speed);

#endif /* USERLIBRARIES_UART_H_ */
