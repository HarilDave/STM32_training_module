/*
 * OPT3001.h
 *
 *  Created on: Nov 7, 2025
 *      Author: Himshree
 */

#ifndef INC_OPT3001_H_
#define INC_OPT3001_H_

#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "i2c.h"

#define REG_OPT3001_CONFIG						(0x01)
#define REG_OPT3001_RES							(0x00)

void read_OPT3001_dev_ID();
uint16_t GetOPT3001LightRes(void);

#endif /* INC_OPT3001_H_ */
