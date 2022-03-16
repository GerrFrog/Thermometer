/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifdef USB_SEND
	#include "usb_device.h"
	#include "usbd_cdc_if.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

/* USER CODE BEGIN PV */
uint8_t buffer[64];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * @brief Convert temperature float type to temperature char type
 *
 * @param digit Temp value
 * @param arr Pointer to output array
 * @return void
 */
#if defined(MLX90614) || defined(MLX90632)
	void float_temp_to_char_temp(double digit, char* arr)
	{
//		if (digit <= 10.0)
//		{
//			arr[0] = 'l';
//			arr[1] = 'e';
//			arr[2] = 's';
//			arr[3] = 's';
//			arr[4] = '\0';
//			arr[5] = '\0';
//			return;
//		}
		int l_digit = digit * 100.0;
		arr[7] = '\0';
		arr[6] = '\0';
		arr[5] = '\0';
		arr[4] = l_digit % 10 + '0';
		l_digit /= 10;
		arr[3] = l_digit % 10 + '0';
		l_digit /= 10;
		arr[2] = ',';
		arr[1] = l_digit % 10 + '0';
		l_digit /= 10;
		arr[0] = l_digit % 10 + '0';
	}
#endif
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
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USB_DEVICE_Init();
  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */
  /**
   * 12 - green, 13 - orange, 14 - red, 15 - blue
   *
   * Display 10x12 - 10 to right, 12 to below
   *
   * Temp sensor 1 - I2C1, temp sensor 2 - I2C2, Display - I2C3
   *
   *
   */
#ifdef USB_SEND
  uint8_t *message_1 = "Temperature from 1st Sensor: \0";
  uint8_t *message_2 = "Temperature from 2nd Sensor: \0";
  uint8_t *cap_mess_1 = "Captured temperature from 1st Sensor: \0";
  uint8_t *cap_mess_2 = "Captured temperature from 2st Sensor: \0";
  uint8_t *end = "\n\0";
#endif

  // Initialize Display
#ifdef SSD1306_DISPLAY
  {
	  if (SSD1306_Init(hi2c3) != 1)
	  {
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		  HAL_Delay(1000);
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	  }

	  SSD1306_GotoXY (0,0);
	  SSD1306_Puts ("initialize", &Font_11x18, 1);
	  SSD1306_UpdateScreen();
  }
#endif

#if defined(MLX90614) || defined(MLX90632)
  int mlx_addr_1 = ScanDevices(hi2c1);
  int mlx_addr_2 = ScanDevices(hi2c2);

  float float_temp_1 = 0.0;
  float float_temp_2 = 0.0;

  char char_temp_1[8];
  char char_temp_2[8];
#endif

#ifdef SSD1306_DISPLAY
  SSD1306_Clear();
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
#ifdef MLX90614_FLASH_MODE
    mlx90614_flash(mlx_addr_1, MLX90614_EMISSIVITY, 0xF332, hi2c1);
#elif defined(MLX90632_FLASH_MODE)

#endif

#if defined(MLX90614_DEBUG_MODE)
    mlx90614_start_debugging(mlx_addr_1, hi2c1);
#elif defined(MLX90632_DEBUG_MODE)
    mlx90632_start_debugging(mlx_addr_1, hi2c1);
#endif

  while (1)
  {
#if defined(MLX90614)
	float_temp_1 = MLX90614_ReadTemp(mlx_addr_1, MLX90614_TOBJ1, hi2c1);
	float_temp_2 = MLX90614_ReadTemp(mlx_addr_2, MLX90614_TOBJ1, hi2c2);

	float_temp_to_char_temp(float_temp_1, char_temp_1);
	float_temp_to_char_temp(float_temp_2, char_temp_2);
#elif defined(MLX90632)
//	float_temp_1 = MLX90632_ReadTemp(mlx_addr_1, hi2c1);
	float_temp_1 = MLX90632_ReadReg(MLX90632_DEFAULT_SA, MLX90632_RAM_6, MLX90632_DBG_OFF, hi2c1);
	float_temp_2 = MLX90632_ReadTemp(mlx_addr_2, hi2c2);

	float_temp_to_char_temp(float_temp_1, char_temp_1);
	float_temp_to_char_temp(float_temp_2, char_temp_2);
#endif


	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_SET)
	{
#ifdef SSD1306_DISPLAY
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts(char_temp_1, &Font_11x18, 1);

		SSD1306_GotoXY(70, 0);
		SSD1306_Puts(char_temp_2, &Font_11x18, 1);
#endif

#ifdef USB_SEND
		{
			CDC_Transmit_FS(cap_mess_1, strlen(cap_mess_1));
			CDC_Transmit_FS((uint8_t*)char_temp_1, strlen((uint8_t*)char_temp_1));
			CDC_Transmit_FS(end, strlen(end));

			CDC_Transmit_FS(cap_mess_2, strlen(cap_mess_2));
			CDC_Transmit_FS((uint8_t*)char_temp_2, strlen((uint8_t*)char_temp_2));
			CDC_Transmit_FS(end, strlen(end));
		}
#endif
	}

#ifdef SSD1306_DISPLAY
	SSD1306_GotoXY(0, 29);
	SSD1306_Puts(char_temp_1, &Font_11x18, 1);

	SSD1306_GotoXY(70, 29);
	SSD1306_Puts(char_temp_2, &Font_11x18, 1);

	SSD1306_UpdateScreen();
#endif

#ifdef USB_SEND
	{
		CDC_Transmit_FS(message_1, strlen(message_1));
		CDC_Transmit_FS((uint8_t*)char_temp_1, strlen((uint8_t*)char_temp_1));
		CDC_Transmit_FS(end, strlen(end));

		CDC_Transmit_FS(message_2, strlen(message_2));
		CDC_Transmit_FS((uint8_t*)char_temp_2, strlen((uint8_t*)char_temp_2));
		CDC_Transmit_FS(end, strlen(end));
	}
#endif

	HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 400000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
