/*
 * mlx90632_depends.c
 *
 *  Created on: Apr 4, 2022
 *      Author: falls
 */
#include "mlx90632_depends.h"

/* Definition of I2C address of MLX90632 */
#define CHIP_ADDRESS 0x3a << 1

/* HAL_I2C_Mem_Read()/Write() are used instead of Master_Transmit()/Receive() because repeated start condition is needed */
/* Implementation of I2C read for 16-bit values */
int32_t mlx90632_i2c_read(int16_t register_address, uint16_t *value, I2C_HandleTypeDef hi2c)
{
	uint8_t data[2];
	int32_t ret;
	ret = HAL_I2C_Mem_Read(&hi2c, CHIP_ADDRESS, register_address, 2, data, sizeof(data), 100);
	//Endianness
	*value = data[1]|(data[0]<<8);
	return ret;
}

/* Implementation of I2C read for 32-bit values */
int32_t mlx90632_i2c_read32(int16_t register_address, uint32_t *value, I2C_HandleTypeDef hi2c)
{
	uint8_t data[4];
	int32_t ret;
	ret = HAL_I2C_Mem_Read(&hi2c, CHIP_ADDRESS, register_address, 2, data, sizeof(data), 100);
	//Endianness
	*value = data[2]<<24|data[3]<<16|data[0]<<8|data[1];
	return ret;
}

/* Implementation of I2C write for 16-bit values */
int32_t mlx90632_i2c_write(int16_t register_address, uint16_t value, I2C_HandleTypeDef hi2c) {
	uint8_t data[2];
	data[0] = value >> 8;
	data[1] = value;
	return HAL_I2C_Mem_Write(&hi2c, CHIP_ADDRESS, register_address, 2, data, 2, 100);
}

void usleep(int min_range, int max_range) {
	while(--min_range);
}

