/*
 * mlx90632.c
 *
 *  Created on: Mar 11, 2022
 *      Author: falls
 */
#include "mlx90632.h"

/**
 * @brief Write data to register
 *
 * @param devAddr Address of device
 * @param regAddr Address of register
 * @param data Data to write in register
 * @param hi2c Handler of I2C
 * @return void
 */
void MLX90632_WriteReg(uint8_t devAddr, uint8_t regAddr, uint16_t data, I2C_HandleTypeDef hi2c)
{
	uint8_t i2cdata[4], temp[4];

	temp[0] = (devAddr << 1);
	temp[1] = regAddr;

	temp[2] = 0x00;
	temp[3] = 0x00;

	// For a write word command, in the crc8 calculus, you have to include [SA_W, Command, LSB, MSB]

	i2cdata[0] = temp[1]; //EEPROM-address
	i2cdata[1] = temp[2]; //Delete-Byte, low
	i2cdata[2] = temp[3]; //Delete-Byte, high
	i2cdata[3] = CRC8_Calc(temp, 4);

	HAL_I2C_Master_Transmit(&hi2c, (devAddr << 1), i2cdata, 4, 0xFFFF);
	HAL_Delay(10);

	MLX90632_SendDebugMsg(MLX90632_DBG_MSG_W, devAddr, i2cdata[0], (i2cdata[1] <<8 | i2cdata[2]), i2cdata[3], 0x00);

	temp[2] = data & 0xFF; //Getting LSB first
	temp[3] = data >> 8;   //Getting MSB after

	i2cdata[0] = temp[1]; //EEPROM-address
	i2cdata[1] = temp[2]; //Delete-Byte, low
	i2cdata[2] = temp[3]; //Delete-Byte, high
	i2cdata[3] = CRC8_Calc(temp, 4);

	HAL_I2C_Master_Transmit(&hi2c, (devAddr << 1), i2cdata, 4, 0xFFFF);
	HAL_Delay(10);
	MLX90632_SendDebugMsg(MLX90632_DBG_MSG_W, devAddr, i2cdata[0], data, i2cdata[3], 0x00);
}

uint16_t MLX90632_ReadReg(uint8_t devAddr, uint16_t regAddr, uint8_t dbg_lvl, I2C_HandleTypeDef hi2c)
{
	uint8_t data[2];
	int16_t ret;

	HAL_I2C_Mem_Read(&hi2c, (devAddr<<1), regAddr, 2, data, sizeof(data), 100);

	ret = data[1]|(data[0]<<8);

	return ret;
}

/**
 * @brief Read data from register
 *
 * @param devAddr Device address
 * @param regAddr Register address
 * @param dbg_lvl Debug level
 * @param hi2c Handler of I2C
 * @return uint16_t Register data
 */
//uint16_t MLX90632_ReadReg(uint8_t devAddr, uint8_t regAddr, uint8_t dbg_lvl, I2C_HandleTypeDef hi2c)
//{
//	uint16_t data;
//	uint8_t in_buff[3], crc_buff[5], crc;
//
//	HAL_I2C_Mem_Read(&hi2c, (devAddr<<1), regAddr, 1, in_buff, 3, 100);
//
//	// For a read word command, in the crc8 calculus, you have to include [SA_W, Command, SA_R, LSB, MSB]
//	crc_buff[0] = (devAddr<<1);
//	crc_buff[1] = regAddr;
//	crc_buff[2] = (devAddr<<1) + 1;
//	crc_buff[3] = in_buff[0];
//	crc_buff[4] = in_buff[1];
//	crc = CRC8_Calc(crc_buff, 5);
//
//	data = (in_buff[1] <<8 | in_buff[0]);
//
//	//TODO: implement CRC8 check on data received
//	if (crc != in_buff[2]) {
//		data = 0x0000;
//	}
//	if(dbg_lvl == MLX90632_DBG_ON)	MLX90632_SendDebugMsg(MLX90632_DBG_MSG_R, devAddr, regAddr, data, in_buff[2], crc);
//
//	HAL_Delay(1);
//	return data;
//}

/**
 * @brief Read temperature from sensor
 *
 * @param devAddr Device address
 * @param regAddr Register address
 * @param hi2c Handler of I2C
 * @return float Temperature value
 */
float MLX90632_ReadTemp(uint8_t devAddr, I2C_HandleTypeDef hi2c)
{
	float temp = 20.0;

	uint16_t reg = MLX90632_ReadReg(devAddr, MLX90632_RAM_6, MLX90632_DBG_OFF, hi2c);

	return (float)reg;
}

/**
 * @brief Send debug message
 *
 * @param op_type Operation mode with error (write/read)
 * @param devAddr Device address
 * @param regAddr Register address
 * @param data Input data
 * @param crc_in Checksum in
 * @param crc_calc Calculated checksum
 * @return void
 */
void MLX90632_SendDebugMsg(uint8_t op_type, uint8_t devAddr, uint8_t regAddr, uint16_t data, uint8_t crc_in, uint8_t crc_calc)
{
	if(op_type == MLX90632_DBG_MSG_W) {
		// TODO: Do something if error
	}
	else if (op_type == MLX90632_DBG_MSG_R) {
		// TODO: Do something if error
	}
}




