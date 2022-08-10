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

void int_address_2_char_address(
    uint16_t address,
    char* output
)
{
	output[5] = '\0';
	output[4] = address % 10 + '0';
	address /= 10;
	output[3] = address % 10 + '0';
	address /= 10;
	output[2] = address % 10 + '0';
	address /= 10;
	output[1] = address % 10 + '0';
	address /= 10;
	output[0] = address % 10 + '0';
}

void float_temp_to_char_temp(double digit, char* arr)
{
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

void usleep(int min_range, int max_range) {
	while(--min_range);
}
