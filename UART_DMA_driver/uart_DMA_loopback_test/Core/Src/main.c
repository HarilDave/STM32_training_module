#include "main.h"

#define TX_LEN 12
uint8_t TxBuffer[TX_LEN] = "DMA LOOPBACK";
volatile uint8_t RxBuffer[TX_LEN];

#define DMA1_CSELR   ((volatile uint32_t *)(DMA1_BASE + 0xA8))

void SystemClock_Config(void) {}
void MX_GPIO_Init(void) {}
void Error_Handler(void) { while(1); }

void DMA_Channel_Select(void)
{
    *DMA1_CSELR &= ~((0xF << (4*6)) | (0xF << (4*7)));
    *DMA1_CSELR |=  (2 << (4*6)) | (2 << (4*7)); // USART2 RX/TX = req 2
}

void UART2_GPIO_Init(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    GPIOA->MODER &= ~((3 << (2*2)) | (3 << (3*2)));
    GPIOA->MODER |=  ((2 << (2*2)) | (2 << (3*2)));   // AF mode
    GPIOA->AFR[0] |= (7 << (4*2)) | (7 << (4*3));     // AF7 = USART2
}

void UART2_Init(void)
{
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
    USART2->BRR = 0x1A1;                 // 115200 @ 80 MHz
    USART2->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;  // DMA enable
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void DMA_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    // RX = DMA1 Channel 6
    DMA1_Channel6->CCR = 0;
    DMA1_Channel6->CPAR = (uint32_t)&USART2->RDR;
    DMA1_Channel6->CMAR = (uint32_t)RxBuffer;
    DMA1_Channel6->CNDTR = TX_LEN;
    DMA1_Channel6->CCR = (1<<7) | (1<<5); // MINC + CIRC

    // TX = DMA1 Channel 7
    DMA1_Channel7->CCR = 0;
    DMA1_Channel7->CPAR = (uint32_t)&USART2->TDR;
    DMA1_Channel7->CMAR = (uint32_t)TxBuffer;
    DMA1_Channel7->CNDTR = TX_LEN;
    DMA1_Channel7->CCR = (1<<4) | (1<<7);  // DIR=1, MINC=1
}

int main(void)
{
    UART2_GPIO_Init();
    UART2_Init();
    DMA_Init();
    DMA_Channel_Select();        // ✅ Add this line!

    DMA1_Channel6->CCR |= 1;     // Enable RX
    DMA1_Channel7->CCR |= 1;     // Enable TX

    while (!(DMA1->ISR & (1 << 25)));  // Wait for TCIF7
    DMA1->IFCR |= (1 << 25);           // Clear flag

    volatile uint8_t check = RxBuffer[0]; // view in debugger
    while (1);
}
