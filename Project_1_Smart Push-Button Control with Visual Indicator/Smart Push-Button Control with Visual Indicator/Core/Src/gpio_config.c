/*
 * gpio_config.c
 *
 *  Created on: Nov 6, 2025
 *      Author: Himshree
 */

#include "gpio_config.h"

// Simple software delay (~1ms per iteration)
void delay_ms(volatile uint32_t ms)
{
    for(volatile uint32_t i = 0; i < ms * 4000; i++);
}

// Initialize PA5 as output and PC13 as input with pull-up
void GPIO_Init(void)
{
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

    // PA5 output
    GPIOA->MODER &= ~(0x3 << (LED_PIN*2));
    GPIOA->MODER |=  (0x1 << (LED_PIN*2));   // output
    GPIOA->OTYPER &= ~(1 << LED_PIN);        // push-pull
    GPIOA->OSPEEDR |= (0x3 << (LED_PIN*2));  // high speed
    GPIOA->PUPDR &= ~(0x3 << (LED_PIN*2));   // no pull

    // PC13 input pull-up
    GPIOC->MODER &= ~(0x3 << (BUTTON_PIN*2)); // input
    GPIOC->PUPDR &= ~(0x3 << (BUTTON_PIN*2));
    GPIOC->PUPDR |=  (0x1 << (BUTTON_PIN*2)); // pull-up
}

// LED controls
void LED_On(void)      { GPIOA->ODR |=  (1 << LED_PIN); }
void LED_Off(void)     { GPIOA->ODR &= ~(1 << LED_PIN); }
void LED_Toggle(void)  { GPIOA->ODR ^=  (1 << LED_PIN); }

void LED_Blink(uint8_t times, uint32_t delay)
{
    for(uint8_t i=0; i<times; i++)
    {
        LED_On();
        delay_ms(delay);
        LED_Off();
        delay_ms(delay);
    }
}

// Read button state, return 1=pressed, 0=released (active-low)
uint8_t Button_Read(void)
{
    return (GPIOC->IDR & (1 << BUTTON_PIN)) ? 0 : 1;
}

