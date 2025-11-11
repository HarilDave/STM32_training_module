/*
 * spi_driver.c
 *
 *  Created on: Oct 30, 2025
 *      Author: Himshree
 */

#include "spi_driver.h"

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


	SPI1->CR1 |= (1 << 8);   // SSM = 1  (software slave management)
	SPI1->CR1 |= (1 << 9);   // SSI = 1  (internal slave select high)
	SPI1->CR1 |= (1 << 6);   // SPE = 1  (enable SPI)


	// Configure device mode
	tempreg |= (pSPIHandle->SPIConfig.DeviceMode << 2);

	// Configure bus config
	if (pSPIHandle->SPIConfig.BusConfig == SPI_BUS_FULL_DUPLEX)
	{
		tempreg &= ~(1 << 15);
	}
	else if (pSPIHandle->SPIConfig.BusConfig == SPI_BUS_HALF_DUPLEX)
	{
		tempreg |= (1 << 15);
	}
	else if (pSPIHandle->SPIConfig.BusConfig == SPI_BUS_SIMPLEX_RX)
	{
		tempreg &= ~(1 << 15);
		tempreg |= (1 << 10);
	}

	// Configure baud rate
	tempreg |= (pSPIHandle->SPIConfig.SclkSpeed << 3);

	// Configure DFF
	tempreg |= (pSPIHandle->SPIConfig.DFF << 11);

	// Configure CPOL and CPHA
	tempreg |= (pSPIHandle->SPIConfig.CPOL << 1);
	tempreg |= (pSPIHandle->SPIConfig.CPHA << 0);

	// Configure SSM
	tempreg |= (pSPIHandle->SPIConfig.SSM << 9);

	pSPIHandle->pSPIx->CR1 = tempreg;
}

void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if (pSPIx == SPI1) RCC->APB2RSTR |= (1 << 12);
	else if (pSPIx == SPI2) RCC->APB1RSTR |= (1 << 14);
}

void SPI_Enable(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi)
		pSPIx->CR1 |= (1 << 6);   // Enable SPI
	else
		pSPIx->CR1 &= ~(1 << 6);  // Disable SPI
}

void SPI_ClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
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
		pSPIx->DR = *pTxBuffer;
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
		*pRxBuffer = pSPIx->DR;
		pRxBuffer++;
		Len--;
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}
