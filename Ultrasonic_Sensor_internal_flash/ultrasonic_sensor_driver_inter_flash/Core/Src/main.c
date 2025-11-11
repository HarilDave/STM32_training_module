/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define TRIG_PIN     1
#define ECHO_PIN     2
#define FLASH_PAGE_ADDR  0x0807F800UL    // page number 255
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void delay_us(uint32_t us)
{
	TIM2->CNT = 0;
	while (TIM2->CNT < us);
}

void GPIO_Init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// PB1 output (TRIG)
	GPIOB->MODER &= ~(3U << (TRIG_PIN * 2));
	GPIOB->MODER |=  (1U << (TRIG_PIN * 2));

	// PB2 input (ECHO)
	GPIOB->MODER &= ~(3U << (ECHO_PIN * 2));
}

void TIM2_Init(void)
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	TIM2->PSC = 80 - 1;   // 1 MHz tick at 80 MHz system clock (1 µs)
	TIM2->ARR = 0xFFFFFFFF;
	TIM2->CR1 = TIM_CR1_CEN;
}

uint32_t Ultrasonic_Read(void)
{
	uint32_t time_us = 0;

	GPIOB->ODR &= ~(1U << TRIG_PIN);
	delay_us(2);
	GPIOB->ODR |=  (1U << TRIG_PIN);
	delay_us(10);
	GPIOB->ODR &= ~(1U << TRIG_PIN);

	while (!(GPIOB->IDR & (1U << ECHO_PIN))); // wait for high
	TIM2->CNT = 0;
	while (GPIOB->IDR & (1U << ECHO_PIN));    // wait for low
	time_us = TIM2->CNT;

	return (uint32_t)((time_us * 0.0343f) / 2.0f); // distance in cm
}

// --- Flash Functions ---
void Flash_Unlock(void)
{
	if (FLASH->CR & FLASH_CR_LOCK)
	{
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
	}
}

void Flash_Lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

void Flash_Wait(void)
{
	while (FLASH->SR & FLASH_SR_BSY);
}

void Flash_Erase_Page(uint32_t address)
{
	Flash_Unlock();
	Flash_Wait();

	FLASH->CR &= ~FLASH_CR_PNB;
	FLASH->CR |= FLASH_CR_PER;
	FLASH->CR |= ((address - 0x08000000) / 0x800) << FLASH_CR_PNB_Pos; // page index
	FLASH->CR |= FLASH_CR_STRT;
	Flash_Wait();

	FLASH->CR &= ~FLASH_CR_PER;
	Flash_Lock();
}

void Flash_Write(uint32_t address, uint64_t data)
{
	Flash_Unlock();
	Flash_Wait();

	FLASH->CR |= FLASH_CR_PG;

	*(__IO uint32_t*)address = (uint32_t)data;
	*(__IO uint32_t*)(address + 4) = (uint32_t)(data >> 32);

	Flash_Wait();
	FLASH->CR &= ~FLASH_CR_PG;
	Flash_Lock();
}

uint64_t Flash_Read(uint32_t address)
{
	return *(__IO uint64_t*)address;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		uint32_t distance = Ultrasonic_Read();

		Flash_Erase_Page(FLASH_PAGE_ADDR);
		Flash_Write(FLASH_PAGE_ADDR, (uint64_t)distance);

		uint64_t stored_value = Flash_Read(FLASH_PAGE_ADDR);

		// small delay between measurements
		for (volatile uint32_t d = 0; d < 100000; d++);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
