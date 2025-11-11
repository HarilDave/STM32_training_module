#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "STM32_L0538xx.h"
#include <stdint.h>

#include <stdint.h>

/* ========================== USER STRUCTURE FROM MAIN FILE ========================== */
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
} USART_RegDef_t;

/* ========================== BASE ADDRESSES ========================== */
#define USART1_BASEADDR      (APB2_BASE + 0x3800UL)
#define USART2_BASEADDR      (APB1_BASE + 0x4400UL)

#define USART1    ((USART_RegDef_t *)USART1_BASEADDR)
#define USART2    ((USART_RegDef_t *)USART2_BASEADDR)

/* ========================== CLOCK CONTROL ========================== */
#define USART1_PCLK_EN()     (RCC->APB2ENR |= (1 << 14))
#define USART2_PCLK_EN()     (RCC->APB1ENR |= (1 << 17))

#define USART1_PCLK_DI()     (RCC->APB2ENR &= ~(1 << 14))
#define USART2_PCLK_DI()     (RCC->APB1ENR &= ~(1 << 17))

/* ========================== ENUM-LIKE DEFINITIONS ========================== */
#define UART_READY            0
#define UART_BUSY_IN_TX       1
#define UART_BUSY_IN_RX       2

#define UART_WORDLEN_8BITS    0
#define UART_WORDLEN_9BITS    1

#define UART_STOPBITS_1       0
#define UART_STOPBITS_2       2

#define UART_PARITY_NONE      0
#define UART_PARITY_EVEN      1
#define UART_PARITY_ODD       2

#define UART_MODE_TX          1
#define UART_MODE_RX          2
#define UART_MODE_TXRX        3

/* ========================== FLAG DEFINITIONS ========================== */
#define UART_FLAG_TXE         (1U << 7)
#define UART_FLAG_TC          (1U << 6)
#define UART_FLAG_RXNE        (1U << 5)
#define UART_FLAG_IDLE        (1U << 4)
#define UART_FLAG_ORE         (1U << 3)
#define UART_FLAG_NE          (1U << 2)
#define UART_FLAG_FE          (1U << 1)

/* ========================== CONFIGURATION STRUCTURE ========================== */
typedef struct
{
    uint32_t BaudRate;
    uint8_t  WordLength;
    uint8_t  StopBits;
    uint8_t  Parity;
    uint8_t  Mode;
} UART_Config_t;

/* ========================== HANDLE STRUCTURE ========================== */
typedef struct
{
    USART_RegDef_t *pUSARTx;
    UART_Config_t   Init;
    uint8_t        *pTxBuffer;
    uint8_t        *pRxBuffer;
    uint32_t        TxLen;
    uint32_t        RxLen;
    uint8_t         TxState;
    uint8_t         RxState;
} UART_Handle_t;

/* ========================== FUNCTION PROTOTYPES ========================== */

/* Clock control */
void UART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

/* Init / DeInit */
void UART_Init(UART_Handle_t *pUARTHandle);
void UART_DeInit(USART_RegDef_t *pUSARTx);

/* Blocking Data Send / Receive */
void UART_SendData(UART_Handle_t *pUARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void UART_ReceiveData(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/* Interrupt-based Data Send / Receive */
uint8_t UART_SendDataIT(UART_Handle_t *pUARTHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t UART_ReceiveDataIT(UART_Handle_t *pUARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/* IRQ Config / Priority / Handler */
void UART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void UART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void UART_IRQHandling(UART_Handle_t *pUARTHandle);

/* Peripheral enable/disable */
void UART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

/* Flag status check */
uint8_t UART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);

/* Error flag clear */
void UART_ClearOREFlag(USART_RegDef_t *pUSARTx);
void UART_ClearNEFlag(USART_RegDef_t *pUSARTx);
void UART_ClearFEFlag(USART_RegDef_t *pUSARTx);

#endif /* UART_DRIVER_H */
