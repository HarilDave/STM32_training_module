#include "stm32l.h"


void SystemClock_Config(void)
{
    /* Enable HSI (16 MHz) and wait until ready */
    RCC->CR |= (1 << 8);
    while (!(RCC->CR & (1 << 10)));

    /* Switch system clock to HSI */
    RCC->CFGR &= ~(3 << 0);
    RCC->CFGR |=  (1 << 0);
    while (((RCC->CFGR >> 2) & 3) != 1);
}

void GPIO_Init(void)
{
    RCC->AHB2ENR |= (1 << 0);             // Enable GPIOA clock

    GPIOA->MODER &= ~((3 << (2 * TRIG_PIN)) | (3 << (2 * ECHO_PIN)));
    GPIOA->MODER |=  (1 << (2 * TRIG_PIN)); // TRIG as output (PA1)
    // ECHO (PA2) stays input

    GPIOA->PUPDR &= ~(3 << (2 * ECHO_PIN));
    GPIOA->PUPDR |=  (1 << (2 * ECHO_PIN)); // pull-up to stabilize input
}

void TIM2_Init(void)
{
    RCC->APB1ENR1 |= (1 << 0);   // Enable TIM2 clock
    TIM2->PSC = 16 - 1;          // 1 MHz → 1 µs tick
    TIM2->ARR = 0xFFFF;
    TIM2->CR1 = 1;               // Enable counter
}

static void delay_us(uint32_t us)
{
    TIM2->CNT = 0;
    while (TIM2->CNT < us);
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
        delay_us(1000);
}

uint32_t Ultrasonic_Read(void)
{
    uint32_t timeout, start, end;

    /* Send 10µs trigger pulse */
    GPIOA->ODR &= ~(1 << TRIG_PIN);
    delay_us(2);
    GPIOA->ODR |= (1 << TRIG_PIN);
    delay_us(10);
    GPIOA->ODR &= ~(1 << TRIG_PIN);

    /* Wait for echo HIGH */
    timeout = 60000;
    while (!(GPIOA->IDR & (1 << ECHO_PIN))) {
        if (--timeout == 0) return 0;
    }
    start = TIM2->CNT;

    /* Wait for echo LOW */
    timeout = 60000;
    while (GPIOA->IDR & (1 << ECHO_PIN)) {
        if (--timeout == 0) return 0;
    }
    end = TIM2->CNT;

    /* Calculate pulse width */
    if (end >= start)
        return end - start;
    else
        return (0xFFFF - start + end);
}

/* ---------------- MAIN ---------------- */
int main(void)
{
    SystemClock_Config();
    GPIO_Init();
    TIM2_Init();

    while (1)
    {
        uint32_t time_us = Ultrasonic_Read();
        float distance_cm = time_us * 0.017f;

        delay_ms(200);
    }
}
