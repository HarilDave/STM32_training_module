/*
 * gpio_config.h
 *
 *  Created on: Nov 6, 2025
 *      Author: Himshree
 */

#ifndef INC_GPIO_CONFIG_H_
#define INC_GPIO_CONFIG_H_

#include "stm32f411xe.h"

#define BUTTON_PIN   13
#define LED_PIN      5

void GPIO_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void LED_Blink(uint8_t times, uint32_t delay_ms);
uint8_t Button_Read(void);

#endif /* INC_GPIO_CONFIG_H_ */
