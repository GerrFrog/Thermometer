/*
 * common.c
 *
 *  Created on: Mar 14, 2022
 *      Author: falls
 */
#include <common.h>

uint8_t CRC8_Calc (uint8_t *p, uint8_t len)
{
	uint16_t i;
	uint16_t crc = 0x0;

	while (len--)
	{
			i = (crc ^ *p++) & 0xFF;
			crc = (crc_table[i] ^ (crc << 8)) & 0xFF;
	}

	return crc & 0xFF;
}

/**
 * @brief Scan device address
 *
 * @param hi2c Handler of I2C
 * @return int Device address
 */
uint16_t device_scanner(I2C_HandleTypeDef hi2c)
{
	HAL_StatusTypeDef result;

	for (uint16_t i = 0; i<256; i++)
	{
		result = HAL_I2C_IsDeviceReady(&hi2c, (uint16_t) (i<<1), 2, 2);

		if (result != HAL_OK)
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);

		if (result == HAL_OK)
		{
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		  HAL_Delay(1000);
		  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		  return i;
		}
	}

	return 0;
}


