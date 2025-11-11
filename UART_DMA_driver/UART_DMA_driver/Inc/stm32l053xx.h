/*
 * stm32l053xx.h
 * Minimal STM32L053 register map for UART + DMA
 */

#ifndef STM32L053XX_H_
#define STM32L053XX_H_

#include <stdint.h>

/* ----------------- Peripheral Base Addresses ----------------- */
#define PERIPH_BASE           0x40000000UL
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000UL)
#define IOPORT_BASE           (PERIPH_BASE + 0x10000000UL)

#define GPIOA_BASE            (IOPORT_BASE + 0x0000)
#define GPIOC_BASE            (IOPORT_BASE + 0x0800)
#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
#define DMA1_BASE             (AHBPERIPH_BASE + 0x00000000UL)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)

/* ----------------- Peripheral Structures ----------------- */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t ISR;
    volatile uint32_t IFCR;
    struct {
        volatile uint32_t CCR;
        volatile uint32_t CNDTR;
        volatile uint32_t CPAR;
        volatile uint32_t CMAR;
        volatile uint32_t RESERVED;
    } CH[7];
} DMA_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    volatile uint32_t IOPRSTR;
    volatile uint32_t AHBRSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t IOPENR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
} RCC_TypeDef;

typedef struct {
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
} USART_TypeDef;

/* ----------------- Peripheral Definitions ----------------- */
#define GPIOA      ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOC      ((GPIO_TypeDef *) GPIOC_BASE)
#define RCC        ((RCC_TypeDef  *) RCC_BASE)
#define DMA1       ((DMA_TypeDef  *) DMA1_BASE)
#define USART2     ((USART_TypeDef*) USART2_BASE)

#define DMA1_Channel4  (&DMA1->CH[3]) // USART2_TX
#define DMA1_Channel5  (&DMA1->CH[4]) // USART2_RX

#endif /* STM32L053XX_H_ */
