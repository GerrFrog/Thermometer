/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* If SSD1306 display is on */
#define SSD1306_DISPLAY 

/* If MLX90614 termometer is on */
#define MLX90614

/* If MLX90632 termometer is on */
#define MLX90632

/* If program runs as debug for MLX90614 */
// #define MLX90614_DEBUG_MODE 

/* If program runs as debug for MLX90632 */
// #define MLX90632_DEBUG_MODE 

/* If program runs as flash for MLX90614 */
// #define MLX90614_FLASH_MODE 

/* If program runs as flash for MLX90632 */
// #define MLX90632_FLASH_MODE 

/* If temperature has to be send to USB port */
#define USB_SEND

#ifdef SSD1306_DISPLAY
#   include "ssd1306.h"
#endif

#if defined(MLX90614_DEBUG_MODE)
#	include <mlx90614_debug.h>
#elif defined(MLX90632_DEBUG_MODE)
#	include <mlx90632_debug.h>
#endif

//#if defined(MLX90614)
//#	include "mlx90614.h"
//#elif defined(MLX90632)
//#	include "mlx90632.h"
//#	include "mlx90632_depends.h"
//#endif
#include "mlx90614.h"
#include "mlx90632.h"
#include "mlx90632_depends.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/**
 * @brief Convert double temp in celcius to char array
 * 
 * @param digit Double temperature
 * @param arr Array pointer
 */
void float_temp_to_char_temp(
    double digit, 
    char* arr
);
// #endif

//#if defined(MLX90632)
/**
 * @brief Read EEPROM memory for parameters initializing
 * 
 * @param PR PR parameter
 * @param PG PG parameter
 * @param PO PO parameter
 * @param PT PT parameter
 * @param Ea Ea parameter
 * @param Eb Eb parameter
 * @param Fa Fa parameter
 * @param Fb Fb parameter
 * @param Ga Ga parameter
 * @param Gb Gb parameter
 * @param Ha Ha parameter
 * @param Hb Hb parameter
 * @param Ka Ka parameter
 * @return int 
 */
static int mlx90632_read_eeprom(
	int32_t *PR, 
	int32_t *PG, 
	int32_t *PO, 
	int32_t *PT, 
	int32_t *Ea, 
	int32_t *Eb, 
	int32_t *Fa, 
	int32_t *Fb, 
	int32_t *Ga, 
	int16_t *Gb, 
	int16_t *Ha, 
	int16_t *Hb, 
	int16_t *Ka, 
	I2C_HandleTypeDef hi2c
);

/**
 * @brief Start work for MLX9632 in standard mode
 */
void mlx90632_start_standard_mode();

/**
 * @brief Start work for MLX90632 in extended mode
 */
void mlx90632_start_extended_mode();

/**
 * @brief Start work for MLX90632 in extended burst mode
 */
void mlx90632_start_extended_burst_mode();
//#elif defined(MLX90614)
/**
 * @brief Start work for MLX90614 in standard mode
 */
void mlx90614_start_standard_mode();
//#endif

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
