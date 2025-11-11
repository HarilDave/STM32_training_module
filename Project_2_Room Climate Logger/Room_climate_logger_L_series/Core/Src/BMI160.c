/*
 * BMI160.c
 *
 *  Created on: Nov 7, 2025
 *      Author: Himshree
 */

#include "BMI160.h"
#include "main.h"
#include "usart.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

static void BMI160_WriteReg(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&hi2c1, BMI160_I2C_ADDR << 1, reg, 1, &data, 1, 100);
}

static void BMI160_ReadRegs(uint8_t reg, uint8_t *buf, uint8_t len)
{
    HAL_I2C_Mem_Read(&hi2c1, BMI160_I2C_ADDR << 1, reg, 1, buf, len, 100);
}

void BMI160_Init(void)
{
    uint8_t cmd;

    cmd = BMI160_CMD_SOFTRESET;
    BMI160_WriteReg(BMI160_CMD_REG, cmd);
    HAL_Delay(100);

    cmd = BMI160_CMD_ACC_NORMAL;
    BMI160_WriteReg(BMI160_CMD_REG, cmd);
    HAL_Delay(50);

    cmd = BMI160_CMD_GYR_NORMAL;
    BMI160_WriteReg(BMI160_CMD_REG, cmd);
    HAL_Delay(50);
}

uint8_t BMI160_ReadID(void)
{
    uint8_t id;
    BMI160_ReadRegs(BMI160_CHIP_ID, &id, 1);
    return id;
}

void BMI160_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t reg = BMI160_ACC_X_L;
    uint8_t buf[6] = {0};

    if (HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMI160_I2C_ADDR << 1, &reg, 1, 100))
    {
        if (HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMI160_I2C_ADDR << 1, buf, 6, 100))
        {
            *ax = (int16_t)((buf[1] << 8) | buf[0]);
            *ay = (int16_t)((buf[3] << 8) | buf[2]);
            *az = (int16_t)((buf[5] << 8) | buf[4]);

            char msg[80];
            int len = sprintf(msg, "Accel: X=%d Y=%d Z=%d\r\n", (int)(*ax), (int)(*ay), (int)(*az));
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
        }
    }
}

void BMI160_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t reg = BMI160_GYR_X_L;
    uint8_t buf[6] = {0};

    if (HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMI160_I2C_ADDR << 1, &reg, 1, 100))
    {
        if (HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMI160_I2C_ADDR << 1, buf, 6, 100))
        {
            *gx = (int16_t)((buf[1] << 8) | buf[0]);
            *gy = (int16_t)((buf[3] << 8) | buf[2]);
            *gz = (int16_t)((buf[5] << 8) | buf[4]);

            char msg[80];
            int len = sprintf(msg, "Gyro: X=%d Y=%d Z=%d\r\n", (int)(*gx), (int)(*gy), (int)(*gz));
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
        }
    }
}
