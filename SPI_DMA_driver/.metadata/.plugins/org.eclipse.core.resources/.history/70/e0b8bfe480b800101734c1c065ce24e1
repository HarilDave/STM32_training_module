/*
 * spi_driver.h
 *
 *  Created on: Oct 30, 2025
 *      Author: Himshree
 */

#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include "STM32_L0538xx.h"
#include <stdint.h>

/* SPI mode settings */
#define SPI_MODE_0   0  // CPOL=0, CPHA=0
#define SPI_MODE_1   1  // CPOL=0, CPHA=1
#define SPI_MODE_2   2  // CPOL=1, CPHA=0
#define SPI_MODE_3   3  // CPOL=1, CPHA=1

/* SPI device mode */
#define SPI_DEVICE_MODE_MASTER   1
#define SPI_DEVICE_MODE_SLAVE    0

/* =================== SPI Device Mode =================== */
#define SPI_MASTER_MODE   1
#define SPI_SLAVE_MODE    0

/* SPI baud rate control (fPCLK / x) */
#define SPI_SCLK_DIV2     0
#define SPI_SCLK_DIV4     1
#define SPI_SCLK_DIV8     2
#define SPI_SCLK_DIV16    3
#define SPI_SCLK_DIV32    4
#define SPI_SCLK_DIV64    5
#define SPI_SCLK_DIV128   6
#define SPI_SCLK_DIV256   7

/* SPI Data Frame Format */
#define SPI_DFF_8BIT   0
#define SPI_DFF_16BIT  1

/* SPI Bus Configurations */
#define SPI_BUS_FULL_DUPLEX   1
#define SPI_BUS_HALF_DUPLEX   2
#define SPI_BUS_SIMPLEX_RX    3

/* SPI NSS Management */
#define SPI_SSM_DISABLE   0
#define SPI_SSM_ENABLE    1

/* =================== SPI Clock Polarity (CPOL) =================== */
#define SPI_CPOL_LOW   0
#define SPI_CPOL_HIGH  1

/* =================== SPI Clock Phase (CPHA) =================== */
#define SPI_CPHA_LOW   0
#define SPI_CPHA_HIGH  1

/* =================== SPI Slave Management =================== */
#define SPI_SSM_HW   1
#define SPI_SSM_SW   0

#define SPI_SSM_EN   1
#define SPI_SSM_DI   0

typedef struct
{
    uint8_t DeviceMode;         // Master/Slave
    uint8_t BusConfig;          // Full/Half/Simplex
    uint8_t SclkSpeed;          // Baud rate
    uint8_t DFF;                // 8/16-bit frame
    uint8_t CPOL;               // Clock polarity
    uint8_t CPHA;               // Clock phase
    uint8_t SSM;                // Software slave management
} SPI_Config_t;

typedef struct
{
    SPI_RegDef_t *pSPIx;
    SPI_Config_t  SPIConfig;

    /* DMA config using base addresses (bare-metal safe) */
    struct
    {
        uint32_t pDMA_TxAddr;   /* base address of DMA channel (e.g. (uint32_t)DMA1_Channel3_BASE) */
        uint32_t pDMA_RxAddr;   /* base address of DMA channel */
        uint8_t  Channel_Tx;    /* channel number (if needed) */
        uint8_t  Channel_Rx;    /* channel number (if needed) */
    } DMACfg;

    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;
    uint32_t TxLen;
    uint32_t RxLen;
} SPI_Handle_t;


/* ================= DMA Register Definition ================= */
typedef struct
{
    volatile uint32_t CCR;      // DMA channel configuration register
    volatile uint32_t CNDTR;    // DMA channel number of data register
    volatile uint32_t CPAR;     // DMA channel peripheral address register
    volatile uint32_t CMAR;     // DMA channel memory address register
    volatile uint32_t RESERVED; // Reserved
} DMA_Channel_TypeDef;

typedef struct
{
    volatile uint32_t ISR;      // DMA interrupt status register
    volatile uint32_t IFCR;     // DMA interrupt flag clear register
    DMA_Channel_TypeDef Channel[7]; // 7 DMA channels
} DMA_TypeDef;

/* Peripheral Clock Control */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/* Initialization & Control */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_ClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_Enable(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
/* Data Send and Receive */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/* Status Flag Check */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);

/* SPI Flag Macros */
#define SPI_TXE_FLAG   (1 << 1)
#define SPI_RXNE_FLAG  (1 << 0)
#define SPI_BSY_FLAG   (1 << 7)

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

#define SPI_CR1_SSI     8      // SSI bit position in CR1 register

/* ========== DMA API (bare-metal) ========== */
/* Offsets in DMA channel register block */
#define DMA_CCR_OFFSET    0x00U
#define DMA_CNDTR_OFFSET  0x04U
#define DMA_CPAR_OFFSET   0x08U
#define DMA_CMAR_OFFSET   0x0CU

/* helper macros to access DMA registers using base address */
#define DMA_REG32(addr, off)   (*((volatile uint32_t *)((uint32_t)(addr) + (off))))

void SPI_DMA_TxInit(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
void SPI_DMA_RxInit(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);
void SPI_DMA_Enable(SPI_Handle_t *pSPIHandle);
void SPI_DMA_Disable(SPI_Handle_t *pSPIHandle);
void SPI_DMA_TxCompleteCallback(SPI_Handle_t *pSPIHandle);
void SPI_DMA_RxCompleteCallback(SPI_Handle_t *pSPIHandle);


#define DMA1                ((DMA_TypeDef *) DMA1_BASE)

#endif /* SPI_DRIVER_H_ */
