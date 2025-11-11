/*
 * servo.h
 *
 *  Created on: Nov 7, 2025
 *      Author: Himshree
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "main.h"
#include <stdint.h>

void Servo_Init(TIM_HandleTypeDef *htim, uint32_t channel, UART_HandleTypeDef *huart);
void Servo_SetAngle(float angle);

#endif /* INC_SERVO_H_ */
