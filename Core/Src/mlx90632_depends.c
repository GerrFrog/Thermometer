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

/* Implementation of reading all calibration parameters for calucation of Ta and To */
//int mlx90632_read_eeprom(int32_t *PR, int32_t *PG, int32_t *PO, int32_t *PT, int32_t *Ea, int32_t *Eb, int32_t *Fa, int32_t *Fb, int32_t *Ga, int16_t *Gb, int16_t *Ha, int16_t *Hb, int16_t *Ka)
//{
//	int32_t ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_P_R, (uint32_t *) PR);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_P_G, (uint32_t *) PG);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_P_O, (uint32_t *) PO);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_P_T, (uint32_t *) PT);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_Ea, (uint32_t *) Ea);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_Eb, (uint32_t *) Eb);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_Fa, (uint32_t *) Fa);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_Fb, (uint32_t *) Fb);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read32(MLX90632_EE_Ga, (uint32_t *) Ga);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read(MLX90632_EE_Gb, (uint16_t *) Gb);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read(MLX90632_EE_Ha, (uint16_t *) Ha);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read(MLX90632_EE_Hb, (uint16_t *) Hb);
//	if(ret < 0)
//		return ret;
//	ret = mlx90632_i2c_read(MLX90632_EE_Ka, (uint16_t *) Ka);
//	if(ret < 0)
//		return ret;
//	return 0;
//}

void usleep(int min_range, int max_range) {
	while(--min_range);
}

