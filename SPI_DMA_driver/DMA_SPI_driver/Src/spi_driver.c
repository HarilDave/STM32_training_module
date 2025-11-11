/*
 * spi_driver.c
 *
 *  Created on: Oct 30, 2025
 *      Author: Himshree
 */

#include "spi_driver.h"

/* =================== SPI Basic Functions =================== */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi)
	{
		if (pSPIx == SPI1) RCC->APB2ENR |= (1 << 12);
		else if (pSPIx == SPI2) RCC->APB1ENR |= (1 << 14);
	}
	else
	{
		if (pSPIx == SPI1) RCC->APB2ENR &= ~(1 << 12);
		else if (pSPIx == SPI2) RCC->APB1ENR &= ~(1 << 14);
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	uint32_t tempreg = 0;

	tempreg |= ((uint32_t)(pSPIHandle->SPIConfig.DeviceMode & 0x1) << 2);
	tempreg |= ((uint32_t)(pSPIHandle->SPIConfig.SclkSpeed & 0x7) << 3);
	tempreg |= ((uint32_t)(pSPIHandle->SPIConfig.CPOL & 0x1) << 1);
	tempreg |= ((uint32_t)(pSPIHandle->SPIConfig.CPHA & 0x1) << 0);
	tempreg |= ((uint32_t)(pSPIHandle->SPIConfig.DFF & 0x1) << 11);
	tempreg |= ((uint32_t)(pSPIHandle->SPIConfig.SSM & 0x1) << 9);

	if (pSPIHandle->SPIConfig.BusConfig == SPI_BUS_HALF_DUPLEX)
		tempreg |= (1U << 15);
	else
		tempreg &= ~(1U << 15);

	pSPIHandle->pSPIx->CR1 = tempreg;

	if (pSPIHandle->SPIConfig.SSM == SPI_SSM_EN)
		pSPIHandle->pSPIx->CR1 |= (1U << SPI_CR1_SSI);

	pSPIHandle->pSPIx->CR1 |= (1U << 6);
}

void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if (pSPIx == SPI1) RCC->APB2RSTR |= (1 << 12);
	else if (pSPIx == SPI2) RCC->APB1RSTR |= (1 << 14);
}

void SPI_Enable(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi)
		pSPIx->CR1 |= (1 << 6);
	else
		pSPIx->CR1 &= ~(1 << 6);
}

void SPI_ClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi)
	{
		if (pSPIx == SPI1) RCC->APB2ENR |= (1 << 12);
		else if (pSPIx == SPI2) RCC->APB1ENR |= (1 << 14);
	}
	else
	{
		if (pSPIx == SPI1) RCC->APB2ENR &= ~(1 << 12);
		else if (pSPIx == SPI2) RCC->APB1ENR &= ~(1 << 14);
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if (pSPIx->SR & FlagName)
		return 1;
	return 0;
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while (Len > 0)
	{
		while (!SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG));
		*((volatile uint8_t *)&(pSPIx->DR)) = *pTxBuffer;
		pTxBuffer++;
		Len--;
	}
	while (SPI_GetFlagStatus(pSPIx, SPI_BSY_FLAG));
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while (Len > 0)
	{
		while (!SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG));
		*pRxBuffer = *((volatile uint8_t *)&(pSPIx->DR));
		pRxBuffer++;
		Len--;
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	else
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
}

/* =================== SPI DMA (bare-metal, address-based) =================== */

static inline void dma_disable_channel(uint32_t base)
{
    DMA_REG32(base, DMA_CCR_OFFSET) &= ~1U;
}

static inline void dma_enable_channel(uint32_t base)
{
    DMA_REG32(base, DMA_CCR_OFFSET) |= 1U;
}

static inline void dma_set_cpar(uint32_t base, uint32_t periph_addr)
{
    DMA_REG32(base, DMA_CPAR_OFFSET) = periph_addr;
}

static inline void dma_set_cmar(uint32_t base, uint32_t mem_addr)
{
    DMA_REG32(base, DMA_CMAR_OFFSET) = mem_addr;
}

static inline void dma_set_cndtr(uint32_t base, uint32_t n)
{
    DMA_REG32(base, DMA_CNDTR_OFFSET) = n;
}

static inline void dma_set_dir_mem2per(uint32_t base)
{
    DMA_REG32(base, DMA_CCR_OFFSET) |= (1U << 4);
}

static inline void dma_set_dir_per2mem(uint32_t base)
{
    DMA_REG32(base, DMA_CCR_OFFSET) &= ~(1U << 4);
}

static inline void dma_enable_mem_inc(uint32_t base)
{
    DMA_REG32(base, DMA_CCR_OFFSET) |= (1U << 7);
}

static inline void dma_set_size_8bit(uint32_t base)
{
    uint32_t v = DMA_REG32(base, DMA_CCR_OFFSET);
    v &= ~((1U << 8) | (1U << 10));
    DMA_REG32(base, DMA_CCR_OFFSET) = v;
}

static inline void dma_enable_tc_interrupt(uint32_t base)
{
    DMA_REG32(base, DMA_CCR_OFFSET) |= (1U << 1);
}

void SPI_DMA_TxInit(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    pSPIHandle->pTxBuffer = pTxBuffer;
    pSPIHandle->TxLen = Len;

#ifdef RCC_AHBENR_DMA1EN
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
#else
    RCC->AHBENR |= (1U << 0);
#endif

    uint32_t base = pSPIHandle->DMACfg.pDMA_TxAddr;

    dma_disable_channel(base);
    dma_set_cpar(base, (uint32_t)&(pSPIHandle->pSPIx->DR));
    dma_set_cmar(base, (uint32_t)pTxBuffer);
    dma_set_cndtr(base, Len);
    dma_set_dir_mem2per(base);
    dma_enable_mem_inc(base);
    dma_set_size_8bit(base);
    dma_enable_tc_interrupt(base);

    pSPIHandle->pSPIx->CR2 |= (1U << 1);
}

void SPI_DMA_RxInit(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    pSPIHandle->pRxBuffer = pRxBuffer;
    pSPIHandle->RxLen = Len;

#ifdef RCC_AHBENR_DMA1EN
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
#else
    RCC->AHBENR |= (1U << 0);
#endif

    uint32_t base = pSPIHandle->DMACfg.pDMA_RxAddr;

    dma_disable_channel(base);
    dma_set_cpar(base, (uint32_t)&(pSPIHandle->pSPIx->DR));
    dma_set_cmar(base, (uint32_t)pRxBuffer);
    dma_set_cndtr(base, Len);
    dma_set_dir_per2mem(base);
    dma_enable_mem_inc(base);
    dma_set_size_8bit(base);
    dma_enable_tc_interrupt(base);

    pSPIHandle->pSPIx->CR2 |= (1U << 0);
}

void SPI_DMA_Enable(SPI_Handle_t *pSPIHandle)
{
    if (pSPIHandle->TxLen > 0)
        dma_enable_channel(pSPIHandle->DMACfg.pDMA_TxAddr);

    if (pSPIHandle->RxLen > 0)
        dma_enable_channel(pSPIHandle->DMACfg.pDMA_RxAddr);
}

void SPI_DMA_Disable(SPI_Handle_t *pSPIHandle)
{
    dma_disable_channel(pSPIHandle->DMACfg.pDMA_TxAddr);
    dma_disable_channel(pSPIHandle->DMACfg.pDMA_RxAddr);

    pSPIHandle->pSPIx->CR2 &= ~((1U << 0) | (1U << 1));
}

void SPI_DMA_TxCompleteCallback(SPI_Handle_t *pSPIHandle)
{
    dma_disable_channel(pSPIHandle->DMACfg.pDMA_TxAddr);
    pSPIHandle->pSPIx->CR2 &= ~(1U << 1);
    pSPIHandle->TxLen = 0;
}

void SPI_DMA_RxCompleteCallback(SPI_Handle_t *pSPIHandle)
{
    dma_disable_channel(pSPIHandle->DMACfg.pDMA_RxAddr);
    pSPIHandle->pSPIx->CR2 &= ~(1U << 0);
    pSPIHandle->RxLen = 0;
}

/* =================== SPI INTERRUPT MODE =================== */

void SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	pSPIHandle->pTxBuffer = pTxBuffer;
	pSPIHandle->TxLen = Len;

	pSPIHandle->pSPIx->CR2 |= SPI_CR2_TXEIE;
}

void SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	pSPIHandle->pRxBuffer = pRxBuffer;
	pSPIHandle->RxLen = Len;

	pSPIHandle->pSPIx->CR2 |= SPI_CR2_RXNEIE;
}

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	uint32_t sr = pSPIHandle->pSPIx->SR;
	uint32_t cr2 = pSPIHandle->pSPIx->CR2;

	/* TXE interrupt */
	if ((sr & SPI_TXE_FLAG) && (cr2 & SPI_CR2_TXEIE))
	{
		if (pSPIHandle->TxLen > 0)
		{
			pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
			pSPIHandle->pTxBuffer++;
			pSPIHandle->TxLen--;

			if (pSPIHandle->TxLen == 0)
			{
				pSPIHandle->pSPIx->CR2 &= ~SPI_CR2_TXEIE;
				SPI_TxCpltCallback(pSPIHandle);
			}
		}
	}

	/* RXNE interrupt */
	if ((sr & SPI_RXNE_FLAG) && (cr2 & SPI_CR2_RXNEIE))
	{
		if (pSPIHandle->RxLen > 0)
		{
			*pSPIHandle->pRxBuffer = pSPIHandle->pSPIx->DR;
			pSPIHandle->pRxBuffer++;
			pSPIHandle->RxLen--;

			if (pSPIHandle->RxLen == 0)
			{
				pSPIHandle->pSPIx->CR2 &= ~SPI_CR2_RXNEIE;
				SPI_RxCpltCallback(pSPIHandle);
			}
		}
	}
}

/* User-overridable weak callbacks */
__attribute__((weak)) void SPI_TxCpltCallback(SPI_Handle_t *pSPIHandle) {}
__attribute__((weak)) void SPI_RxCpltCallback(SPI_Handle_t *pSPIHandle) {}
