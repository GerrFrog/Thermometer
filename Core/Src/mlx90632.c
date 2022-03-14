/*
 * mlx90632.c
 *
 *  Created on: Mar 11, 2022
 *      Author: falls
 */
#include "mlx90632.h"

int MLX90632_ScanDevices(I2C_HandleTypeDef hi2c)
{
	HAL_StatusTypeDef result;
	for (int i = 0; i<126; i++)
	{
		result = HAL_I2C_IsDeviceReady(&hi2c, (uint16_t) (i<<1), 2, 2);
		if (result != HAL_OK)
		{
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		}
		if (result == HAL_OK)
		{
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		  HAL_Delay(1000);
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		  return i;
		}
	}
}
