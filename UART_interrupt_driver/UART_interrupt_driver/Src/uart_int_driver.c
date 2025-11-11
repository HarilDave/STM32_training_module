#include "uart_int_driver.h"
#include "stm32l.h"

volatile uint8_t rx_data;

void USART2_Init(uint32_t baudrate)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;      // Enable GPIOA clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;   // Enable USART2 clock

    // PA2 = TX, PA3 = RX (AF4)
    GPIOA->MODER &= ~((3U << (2 * 2)) | (3U << (3 * 2))); // Clear MODER for PA2, PA3
    GPIOA->MODER |=  ((2U << (2 * 2)) | (2U << (3 * 2))); // Set alternate function
    GPIOA->AFRL  &= ~((0xF << (2 * 4)) | (0xF << (3 * 4)));
    GPIOA->AFRL  |=  ((4U << (2 * 4)) | (4U << (3 * 4)));  // AF4 = USART2

    USART2->BRR = (32000000U / baudrate);   // Assuming 32 MHz clock
    USART2->CR1 = (1U << 3) | (1U << 2) | (1U << 0);  // TE, RE, UE enable
}

void USART2_SendChar(char c)
{
    while (!(USART2->ISR & (1U << 7)));     // Wait for TXE
    USART2->TDR = c;
}

void USART2_EnableInterrupt(void)
{
    USART2->CR1 |= (1U << 5);               // RXNE interrupt enable
    NVIC_EnableIRQ(USART2_IRQn);            // Enable USART2 interrupt in NVIC
}

void USART2_IRQHandler(void)
{
    if (USART2->ISR & (1U << 5))            // RXNE flag
    {
        rx_data = USART2->RDR;              // Read data
        USART2_SendChar(rx_data);           // Echo back
    }
}
