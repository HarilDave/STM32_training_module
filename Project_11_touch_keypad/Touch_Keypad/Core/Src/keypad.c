/*
 * keypad.c
 *
 *  Created on: Nov 10, 2025
 *      Author: Himshree
 */

#include "keypad.h"

static uint8_t status_lo = 0;
static uint8_t status_hi = 0;

volatile uint8_t keypad_event = 0;

void KEYPAD_Init(void)
{
}

void KEYPAD_ReadStatus(void)
{
    uint8_t reg = 0x00;
    uint8_t buf[4] = {0};

    // Use shifted address for I2C (HAL expects 8-bit address)
    if (HAL_I2C_Master_Transmit(&hi2c1, (KEYPAD_ADDR << 1), &reg, 1, 100) == HAL_OK)
    {
        if (HAL_I2C_Master_Receive(&hi2c1, (KEYPAD_ADDR << 1), buf, 4, 100) == HAL_OK)
        {
            // Debug output (remove later if needed)
            char debug[40];
            sprintf(debug, "BUF: %02X %02X %02X %02X\r\n", buf[0], buf[1], buf[2], buf[3]);
            HAL_UART_Transmit(&huart2, (uint8_t*)debug, strlen(debug), 100);

            status_lo = buf[2];
            status_hi = buf[3];
        }
    }

    // Optional: clear event flag if your keypad IC needs it
    uint8_t clear_cmd[2] = {0x00, 0x00};
    HAL_I2C_Master_Transmit(&hi2c1, (KEYPAD_ADDR << 1), clear_cmd, 2, 100);
}

char KEYPAD_GetKey(void)
{
    char key = 0;

    if(status_lo & 0x01) key = '1';
    else if(status_lo & 0x02) key = '2';
    else if(status_lo & 0x04) key = '3';
    else if(status_lo & 0x08) key = '4';
    else if(status_hi & 0x01) key = '5';
    else if(status_hi & 0x02) key = '6';
    else if(status_hi & 0x04) key = '7';
    else if(status_hi & 0x08) key = '8';
    else if(status_hi & 0x10) key = '9';
    else if(status_hi & 0x20) key = 'A';
    else if(status_hi & 0x40) key = 'B';
    else if(status_hi & 0x80) key = 'C';

    // Reset after read
    status_lo = 0;
    status_hi = 0;

    return key;
}

void KEYPAD_SendUART(UART_HandleTypeDef *huart)
{
    char key = KEYPAD_GetKey();
    if(key)
    {
        HAL_UART_Transmit(huart, (uint8_t*)&key, 1, 100);
        HAL_UART_Transmit(huart, (uint8_t*)"\r\n", 2, 100);
    }
}
