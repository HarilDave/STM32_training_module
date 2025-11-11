#ifndef STM32L_H
#define STM32L_H

#include <stdint.h>

/* ---------------- Base addresses ---------------- */
#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE   (PERIPH_BASE + 0x08000000UL)
#define APB1PERIPH_BASE   (PERIPH_BASE + 0x00000000UL)

/* Peripheral base addresses */
#define RCC_BASE          (AHB1PERIPH_BASE + 0x1000UL)
#define GPIOA_BASE        (AHB2PERIPH_BASE + 0x0000UL)
#define TIM2_BASE         (APB1PERIPH_BASE + 0x0000UL)

/* ---------------- Peripheral structures ---------------- */
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
    volatile uint32_t BRR;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t PLLSAI1CFGR;
    volatile uint32_t PLLSAI2CFGR;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR1;
    volatile uint32_t APB1RSTR2;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED2;
    volatile uint32_t APB1ENR1;
    volatile uint32_t APB1ENR2;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
} TIM_TypeDef;

/* Peripheral instances */
#define RCC     ((RCC_TypeDef *) RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *) GPIOA_BASE)
#define TIM2    ((TIM_TypeDef *) TIM2_BASE)

/* Pin definitions */
#define TRIG_PIN 1
#define ECHO_PIN 2

/* ---------------- Function prototypes ---------------- */
void SystemClock_Config(void);
void GPIO_Init(void);
void TIM2_Init(void);
static void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
uint32_t Ultrasonic_Read(void);

/* ---------------- Functions ---------------- */


#endif
