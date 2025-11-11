#ifndef STM32L_H
#define STM32L_H

#include <stdint.h>

/* ========== Base Addresses ========== */
#define PERIPH_BASE           (0x40000000UL)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)

#define GPIOA_BASE            (AHBPERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHBPERIPH_BASE + 0x0400UL)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000UL)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)

/* ========== Register Structures ========== */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t PLLSAI1CFGR;
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
    volatile uint32_t IOPSMEN;
    volatile uint32_t AHBSMENR;
    volatile uint32_t APB2SMENR;
    volatile uint32_t APB1SMENR;
    volatile uint32_t CCIPR;
    volatile uint32_t CSR;
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

/* ========== Peripheral Instances ========== */
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)

/* ========== RCC Bit Definitions ========== */
#define RCC_IOPENR_GPIOAEN   (1 << 0)
#define RCC_IOPENR_GPIOBEN   (1 << 1)
#define RCC_APB1ENR_USART2EN (1 << 17)

/* ========== NVIC & Core Helpers ========== */
#define NVIC_ISER_BASE     (0xE000E100UL)
#define NVIC_ISER          ((volatile uint32_t *)NVIC_ISER_BASE)

static inline void NVIC_EnableIRQ(uint8_t irq)
{
    NVIC_ISER[irq >> 5] = (1 << (irq & 0x1F));
}

static inline void __NOP(void)
{
    __asm volatile ("nop");
}

/* IRQ Numbers */
#define USART2_IRQn        28

#endif
