#include "TMP007.h"
#include <stdio.h>
#include <stdint.h>

/* ---------- Peripheral base addresses ---------- */
#define RCC_BASE        0x40021000UL
#define GPIOA_BASE      0x48000000UL
#define GPIOB_BASE      0x48000400UL
#define I2C1_BASE       0x40005400UL
#define USART2_BASE     0x40004400UL

/* ---------- RCC ---------- */
#define RCC_AHB2ENR     (*(volatile uint32_t*)(RCC_BASE + 0x4C))
#define RCC_APB1ENR1    (*(volatile uint32_t*)(RCC_BASE + 0x58))

/* ---------- GPIO ---------- */
#define GPIOA_MODER     (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL      (*(volatile uint32_t*)(GPIOA_BASE + 0x20))
#define GPIOA_OSPEEDR   (*(volatile uint32_t*)(GPIOA_BASE + 0x08))
#define GPIOA_OTYPER    (*(volatile uint32_t*)(GPIOA_BASE + 0x04))
#define GPIOA_PUPDR     (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))

#define GPIOB_MODER     (*(volatile uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_AFRL      (*(volatile uint32_t*)(GPIOB_BASE + 0x20))
#define GPIOB_OTYPER    (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_PUPDR     (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))

/* ---------- I2C1 ---------- */
#define I2C1_CR1        (*(volatile uint32_t*)(I2C1_BASE + 0x00))
#define I2C1_CR2        (*(volatile uint32_t*)(I2C1_BASE + 0x04))
#define I2C1_TIMINGR    (*(volatile uint32_t*)(I2C1_BASE + 0x10))
#define I2C1_ISR        (*(volatile uint32_t*)(I2C1_BASE + 0x18))
#define I2C1_ICR        (*(volatile uint32_t*)(I2C1_BASE + 0x1C))
#define I2C1_RXDR       (*(volatile uint32_t*)(I2C1_BASE + 0x24))
#define I2C1_TXDR       (*(volatile uint32_t*)(I2C1_BASE + 0x28))

/* ---------- USART2 ---------- */
#define USART2_CR1      (*(volatile uint32_t*)(USART2_BASE + 0x00))
#define USART2_BRR      (*(volatile uint32_t*)(USART2_BASE + 0x0C))
#define USART2_ISR      (*(volatile uint32_t*)(USART2_BASE + 0x1C))
#define USART2_TDR      (*(volatile uint32_t*)(USART2_BASE + 0x28))

/* ---------- Utility ---------- */
static void delay_ms(uint32_t ms){
    for(uint32_t i=0;i<ms*4000;i++) __asm__("nop");
}

/* ---------- UART ---------- */
void TMP007_UART_Init(void){
    RCC_AHB2ENR |= (1<<0);   // GPIOA clk
    RCC_APB1ENR1 |= (1<<17); // USART2 clk

    GPIOA_MODER &= ~(3<<(2*2));
    GPIOA_MODER |=  (2<<(2*2));
    GPIOA_AFRL  |=  (7<<(4*2));

    USART2_BRR = 4000000/115200;
    USART2_CR1 = (1<<3)|(1<<0); // TE + UE
}

void TMP007_UART_Write(char c){
    while(!(USART2_ISR & (1<<7)));
    USART2_TDR = c;
}

void TMP007_UART_Print(char *s){
    while(*s) TMP007_UART_Write(*s++);
}

/* ---------- I2C ---------- */
static void i2c1_init(void){
    RCC_AHB2ENR |= (1<<1);   // GPIOB clk
    RCC_APB1ENR1 |= (1<<21); // I2C1 clk

    GPIOB_MODER &= ~((3<<(8*2)) | (3<<(9*2)));
    GPIOB_MODER |=  ((2<<(8*2)) | (2<<(9*2)));
    GPIOB_AFRL  |=  (4<<((8-8)*4)) | (4<<((9-8)*4));
    GPIOB_OTYPER |= (1<<8) | (1<<9);
    GPIOB_PUPDR  |= (1<<(8*2)) | (1<<(9*2));

    I2C1_CR1 &= ~(1<<0);
    I2C1_TIMINGR = 0x20303E5D; // 100kHz
    I2C1_CR1 |= (1<<0);
}

static void i2c1_write16(uint8_t addr, uint8_t reg, uint16_t val){
    uint8_t buf[2];
    buf[0] = (val >> 8) & 0xFF;
    buf[1] = val & 0xFF;

    I2C1_CR2 = (addr<<1)|(3<<16); // 2 bytes
    I2C1_CR2 |= (1<<13);          // START
    while(!(I2C1_ISR & (1<<1)));  // TXIS
    I2C1_TXDR = reg;
    while(!(I2C1_ISR & (1<<1)));
    I2C1_TXDR = buf[0];
    while(!(I2C1_ISR & (1<<6)));
    I2C1_TXDR = buf[1];
    while(!(I2C1_ISR & (1<<6)));
    I2C1_CR2 |= (1<<14);          // STOP
}

static uint16_t i2c1_read16(uint8_t addr, uint8_t reg){
    uint8_t val[2];

    I2C1_CR2 = (addr<<1)|(1<<16);
    I2C1_CR2 |= (1<<13);
    while(!(I2C1_ISR & (1<<1)));
    I2C1_TXDR = reg;
    while(!(I2C1_ISR & (1<<6)));

    I2C1_CR2 = (addr<<1)|(1<<10)|(2<<16);
    I2C1_CR2 |= (1<<13);
    while(!(I2C1_ISR & (1<<2)));
    val[0] = I2C1_RXDR;
    while(!(I2C1_ISR & (1<<2)));
    val[1] = I2C1_RXDR;
    I2C1_CR2 |= (1<<14);

    return (val[0]<<8)|val[1];
}

/* ---------- TMP007 ---------- */
void TMP007_Init(void){
    TMP007_UART_Init();
    i2c1_init();
    delay_ms(100);

    i2c1_write16(TMP007_ADDR, TMP007_CONFIG_REG, 0x1000);
    delay_ms(500);
    TMP007_UART_Print("TMP007 Initialized\r\n");
}

void TMP007_ReadTemp(void){
    uint16_t raw = i2c1_read16(TMP007_ADDR, TMP007_TDIE_REG);
    int16_t temp_raw = (int16_t)(raw>>2);
    float tempC = temp_raw * 0.03125f;

    char buf[50];
    sprintf(buf, "TMP007 Temp: %.2f C\r\n", tempC);
    TMP007_UART_Print(buf);
}
