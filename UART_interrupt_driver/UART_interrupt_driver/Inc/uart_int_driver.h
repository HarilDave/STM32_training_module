/*
 * uart_int_driver.h
 *
 *  Created on: Nov 4, 2025
 *      Author: Himshree
 */

#ifndef UART_INT_DRIVER_H_
#define UART_INT_DRIVER_H_

#include "stm32l.h"

/* Function prototypes */
void USART1_Init(uint32_t baudrate);
void USART1_SendChar(uint8_t c);
void USART1_SendString(char *str);
void USART1_EnableInterrupt(void);
void USART1_IRQHandler(void);

/* Global buffers */
extern volatile uint8_t rx_data;
extern volatile uint8_t rx_buffer[64];
extern volatile uint8_t rx_index;


#endif /* UART_INT_DRIVER_H_ */
