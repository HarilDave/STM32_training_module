#ifndef INC_TMP007_H_
#define INC_TMP007_H_

#include <stdint.h>

#define TMP007_ADDR         0x40

// TMP007 Registers
#define TMP007_VOBJ_REG     0x00
#define TMP007_TDIE_REG     0x01
#define TMP007_CONFIG_REG   0x02
#define TMP007_STATUS_REG   0x04

// Function Prototypes
void TMP007_Init(void);
void TMP007_ReadTemp(void);
void TMP007_UART_Init(void);
void TMP007_UART_Write(char c);
void TMP007_UART_Print(char *s);

#endif /* INC_TMP007_H_ */
