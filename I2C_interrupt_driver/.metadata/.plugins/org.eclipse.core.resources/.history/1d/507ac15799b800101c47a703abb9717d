#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include "STM32_L0538xx.h"

#ifndef I2C_CR1_TXDMAEN
#define I2C_CR1_TXDMAEN   (1U << 15)
#endif

#ifndef I2C_CR1_RXDMAEN
#define I2C_CR1_RXDMAEN   (1U << 14)
#endif

#ifndef RCC_AHBENR_DMA1EN
#define RCC_AHBENR_DMA1EN   (1U << 0)
#endif

#ifndef DMA1
#define DMA1_BASE        (0x40020000UL)
#define DMA1             ((DMA_RegDef_t *)DMA1_BASE)
#endif

typedef struct
{
    uint32_t ClockSpeed;
    uint8_t AddressMode;
    uint16_t OwnAddress;
} I2C_Config_t;

typedef struct
{
    I2C_RegDef_t *Instance;
    I2C_Config_t Init;
} I2C_Handle_t;

#define I2C_7BIT_ADDR   0
#define I2C_10BIT_ADDR  1

#define I2C_WRITE 0
#define I2C_READ  1

void I2C_Init(I2C_Handle_t *hi2c);
void I2C_Start(I2C_Handle_t *hi2c, uint8_t address, uint8_t direction);
void I2C_WriteData(I2C_Handle_t *hi2c, uint8_t data);
uint8_t I2C_ReadData(I2C_Handle_t *hi2c);
void I2C_Stop(I2C_Handle_t *hi2c);

/* ===================================================================== */
/* ========================== DMA REGISTER STRUCT ======================= */
/* ===================================================================== */

typedef struct
{
    volatile uint32_t CCR;
    volatile uint32_t CNDTR;
    volatile uint32_t CPAR;
    volatile uint32_t CMAR;
    volatile uint32_t RESERVED;
} DMA_Channel_RegDef_t;

typedef struct
{
    volatile uint32_t ISR;
    volatile uint32_t IFCR;
    DMA_Channel_RegDef_t CH[7];
} DMA_RegDef_t;

/* ===================================================================== */
/* ========================== I2C DMA SUPPORT ========================== */
/* ===================================================================== */

typedef struct
{
    DMA_RegDef_t *Instance;
    uint32_t Channel;
    uint8_t *Buffer;
    uint16_t Length;
} I2C_DMA_Handle_t;

#define I2C_DMA_TX     0
#define I2C_DMA_RX     1

#define I2C_DMA_READY   0
#define I2C_DMA_BUSY    1
#define I2C_DMA_ERROR   2

void I2C_DMA_Init(I2C_Handle_t *hi2c, I2C_DMA_Handle_t *hdma, uint8_t direction);
void I2C_DMA_Start(I2C_Handle_t *hi2c, I2C_DMA_Handle_t *hdma);
void I2C_DMA_Stop(I2C_Handle_t *hi2c, I2C_DMA_Handle_t *hdma);
void I2C_DMA_IRQHandler(I2C_Handle_t *hi2c, I2C_DMA_Handle_t *hdma);

void I2C_DMA_TxCpltCallback(I2C_Handle_t *hi2c);
void I2C_DMA_RxCpltCallback(I2C_Handle_t *hi2c);
void I2C_DMA_ErrorCallback(I2C_Handle_t *hi2c);

/* ===================================================================== */
/* ========================= I2C INTERRUPT SUPPORT ===================== */
/* ===================================================================== */

/* Interrupt Enable Bits */
#ifndef I2C_CR1_TXIE
#define I2C_CR1_TXIE      (1U << 1)
#endif
#ifndef I2C_CR1_RXIE
#define I2C_CR1_RXIE      (1U << 2)
#endif
#ifndef I2C_CR1_ADDRIE
#define I2C_CR1_ADDRIE    (1U << 3)
#endif
#ifndef I2C_CR1_NACKIE
#define I2C_CR1_NACKIE    (1U << 4)
#endif
#ifndef I2C_CR1_STOPIE
#define I2C_CR1_STOPIE    (1U << 5)
#endif
#ifndef I2C_CR1_TCIE
#define I2C_CR1_TCIE      (1U << 6)
#endif
#ifndef I2C_CR1_ERRIE
#define I2C_CR1_ERRIE     (1U << 7)
#endif

/* Interrupt Event Flags */
#ifndef I2C_ISR_TXIS
#define I2C_ISR_TXIS      (1U << 1)
#endif
#ifndef I2C_ISR_RXNE
#define I2C_ISR_RXNE      (1U << 2)
#endif
#ifndef I2C_ISR_TC
#define I2C_ISR_TC        (1U << 6)
#endif
#ifndef I2C_ISR_STOPF
#define I2C_ISR_STOPF     (1U << 5)
#endif
#ifndef I2C_ISR_NACKF
#define I2C_ISR_NACKF     (1U << 4)
#endif
#ifndef I2C_ISR_ADDR
#define I2C_ISR_ADDR      (1U << 3)
#endif
#ifndef I2C_ISR_TXE
#define I2C_ISR_TXE       (1U << 0)
#endif
#ifndef I2C_ISR_BERR
#define I2C_ISR_BERR      (1U << 8)
#endif
#ifndef I2C_ISR_ARLO
#define I2C_ISR_ARLO      (1U << 9)
#endif
#ifndef I2C_ISR_OVR
#define I2C_ISR_OVR       (1U << 10)
#endif

/* I2C Interrupt Control and Handling */
void I2C_IT_Enable(I2C_Handle_t *hi2c);
void I2C_IT_Disable(I2C_Handle_t *hi2c);
void I2C_Transmit_IT(I2C_Handle_t *hi2c, uint8_t address, uint8_t *data, uint16_t size);
void I2C_Receive_IT(I2C_Handle_t *hi2c, uint8_t address, uint8_t *buffer, uint16_t size);
void I2C_EV_IRQHandler(I2C_Handle_t *hi2c);
void I2C_ER_IRQHandler(I2C_Handle_t *hi2c);

/* Optional user callbacks */
void I2C_TxCpltCallback(I2C_Handle_t *hi2c);
void I2C_RxCpltCallback(I2C_Handle_t *hi2c);
void I2C_ErrorCallback(I2C_Handle_t *hi2c);

/* ===================================================================== */
/* ===================== I2C INTERRUPT SUPPORT ========================= */
/* ===================================================================== */

/* Common I2C interrupt flags (for STM32L0xx) */
#ifndef I2C_ICR_NACKCF
#define I2C_ICR_NACKCF      (1U << 4)
#endif
#ifndef I2C_ICR_STOPCF
#define I2C_ICR_STOPCF      (1U << 5)
#endif
#ifndef I2C_ISR_TXIS
#define I2C_ISR_TXIS        (1U << 1)
#endif
#ifndef I2C_ISR_RXNE
#define I2C_ISR_RXNE        (1U << 2)
#endif
#ifndef I2C_ISR_STOPF
#define I2C_ISR_STOPF       (1U << 5)
#endif
#ifndef I2C_ISR_NACKF
#define I2C_ISR_NACKF       (1U << 4)
#endif

/* Interrupt enable bits */
#ifndef I2C_CR1_TXIE
#define I2C_CR1_TXIE        (1U << 1)
#endif
#ifndef I2C_CR1_RXIE
#define I2C_CR1_RXIE        (1U << 2)
#endif
#ifndef I2C_CR1_STOPIE
#define I2C_CR1_STOPIE      (1U << 5)
#endif
#ifndef I2C_CR1_NACKIE
#define I2C_CR1_NACKIE      (1U << 4)
#endif
#ifndef I2C_CR1_ERRIE
#define I2C_CR1_ERRIE       (1U << 7)
#endif

#ifndef I2C1_IRQn
#define I2C1_IRQn 29
#endif

/* Function prototypes */
void I2C_IT_Init(I2C_Handle_t *hi2c);
void I2C_EnableInterrupts(I2C_Handle_t *hi2c);
void I2C_DisableInterrupts(I2C_Handle_t *hi2c);
void I2C_IRQHandler(I2C_Handle_t *hi2c);

/* Weak callback prototypes for user override */
void I2C_IT_TxCpltCallback(I2C_Handle_t *hi2c);
void I2C_IT_RxCpltCallback(I2C_Handle_t *hi2c);
void I2C_IT_StopCallback(I2C_Handle_t *hi2c);
void I2C_IT_ErrorCallback(I2C_Handle_t *hi2c);


#endif /* I2C_DRIVER_H_ */
