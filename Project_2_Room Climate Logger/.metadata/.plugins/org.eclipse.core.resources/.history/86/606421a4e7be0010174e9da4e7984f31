/*
 * BMI160.h
 *
 *  Created on: Nov 7, 2025
 *      Author: Himshree
 */

#ifndef INC_BMI160_H_
#define INC_BMI160_H_

#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "usart.h"

// I2C Address
#define BMI160_I2C_ADDR         0x69

// Registers
#define BMI160_CHIP_ID          0x00
#define BMI160_CMD_REG          0x7E
#define BMI160_ACC_X_L          0x12
#define BMI160_GYR_X_L          0x0C

// Commands
#define BMI160_CMD_SOFTRESET    0xB6
#define BMI160_CMD_ACC_NORMAL   0x11
#define BMI160_CMD_GYR_NORMAL   0x15

// Functions
void BMI160_Init(void);
void BMI160_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);
void BMI160_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz);
uint8_t BMI160_ReadID(void);

#endif /* INC_BMI160_H_ */
