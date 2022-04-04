/*
 * mlx90614_flash.c
 *
 *  Created on: Mar 16, 2022
 *      Author: falls
 */
#include "mlx90614_flash.h"

/**
 * @brief Flash the value to MLX90614
 *
 * @param mlx_addr I2C address of MLX sensor
 * @param mem_addr Memory to be flashed
 * @param value Value to flash to memory
 * @param hi2c I2C handler
 */
void mlx90614_flash(uint16_t mlx_addr, uint16_t mem_addr, uint16_t value, I2C_HandleTypeDef hi2c)
{
	  MLX90614_WriteReg(mlx_addr, mem_addr, value, hi2c);
	  while(1)
	  {
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

		  HAL_Delay(100);
	  }
}
