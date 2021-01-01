/*
 * UART.h
 *
 *  Created on: Oct 17, 2019
 *      Author: Minh Thuan
 */

#ifndef USERLIBRARIES_UART_H_
#define USERLIBRARIES_UART_H_

#define UART_PERIPH         SYSCTL_PERIPH_UART0
#define UART_GPIO_PERIPH    SYSCTL_PERIPH_GPIOA
#define UART_GPIO_PORT      GPIO_PORTB_BASE
#define UART_PORT			UART0_BASE
#define SCL_PIN             GPIO_PIN_2
#define SDA_PIN             GPIO_PIN_3
#define SCL_GPIO_CONFIG     GPIO_PB2_UART0SCL
#define SDA_GPIO_CONFIG     GPIO_PB3_UART0SDA

#define RX0_PIN          GPIO_PIN_0
#define TX0_PIN          GPIO_PIN_1

void UART_Init(uint32_t ui32Speed);

void UART_SendSpeed(int16_t i16_Speed);

#endif /* USERLIBRARIES_UART_H_ */
