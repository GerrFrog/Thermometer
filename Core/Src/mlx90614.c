/*
 * mlx90614.c
 *
 *  Created on: Oct 5, 2021
 *      Author: falls
 */
#include "mlx90614.h"

//extern I2C_HandleTypeDef hi2c1;
//extern I2C_HandleTypeDef hi2c2;
char temp_buff[128] = {};

/**
 * @brief Write data to register
 *
 * @param devAddr Address of device
 * @param regAddr Address of register
 * @param data Data to write in register
 * @param hi2c Handler of I2C
 * @return void
 */
void MLX90614_WriteReg(uint8_t devAddr, uint8_t regAddr, uint16_t data, I2C_HandleTypeDef hi2c)
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

	MLX90614_SendDebugMsg(MLX90614_DBG_MSG_W, devAddr, i2cdata[0], (i2cdata[1] <<8 | i2cdata[2]), i2cdata[3], 0x00);

	temp[2] = data & 0xFF; //Getting LSB first
	temp[3] = data >> 8;   //Getting MSB after

	i2cdata[0] = temp[1]; //EEPROM-address
	i2cdata[1] = temp[2]; //Delete-Byte, low
	i2cdata[2] = temp[3]; //Delete-Byte, high
	i2cdata[3] = CRC8_Calc(temp, 4);

	HAL_I2C_Master_Transmit(&hi2c, (devAddr << 1), i2cdata, 4, 0xFFFF);
	HAL_Delay(10);
	MLX90614_SendDebugMsg(MLX90614_DBG_MSG_W, devAddr, i2cdata[0], data, i2cdata[3], 0x00);
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
uint16_t MLX90614_ReadReg(uint8_t devAddr, uint8_t regAddr, uint8_t dbg_lvl, I2C_HandleTypeDef hi2c)
{
	uint16_t data;
	uint8_t in_buff[3], crc_buff[5], crc;

	HAL_I2C_Mem_Read(&hi2c, (devAddr<<1), regAddr, 1, in_buff, 3, 100);

	// For a read word command, in the crc8 calculus, you have to include [SA_W, Command, SA_R, LSB, MSB]
	crc_buff[0] = (devAddr<<1);
	crc_buff[1] = regAddr;
	crc_buff[2] = (devAddr<<1) + 1;
	crc_buff[3] = in_buff[0];
	crc_buff[4] = in_buff[1];
	crc = CRC8_Calc(crc_buff, 5);

	data = (in_buff[1] <<8 | in_buff[0]);

	//TODO: implement CRC8 check on data received
	if (crc != in_buff[2]) {
		data = 0x0000;
	}
	if(dbg_lvl == MLX90614_DBG_ON)	MLX90614_SendDebugMsg(MLX90614_DBG_MSG_R, devAddr, regAddr, data, in_buff[2], crc);

	HAL_Delay(1);
	return data;
}

/**
 * @brief Read temperature from sensor
 *
 * @param devAddr Device address
 * @param regAddr Register address
 * @param hi2c Handler of I2C
 * @return float Temperature value
 */
float MLX90614_ReadTemp(uint8_t devAddr, uint8_t regAddr, I2C_HandleTypeDef hi2c)
{
	float temp;
	uint16_t data;

	data = MLX90614_ReadReg(devAddr, regAddr, MLX90614_DBG_OFF, hi2c);

	temp = data*0.02 - 273.15; // from datasheet

	return temp;
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
void MLX90614_SendDebugMsg(uint8_t op_type, uint8_t devAddr, uint8_t regAddr, uint16_t data, uint8_t crc_in, uint8_t crc_calc)
{
	if(op_type == MLX90614_DBG_MSG_W) {
		// TODO: Do something if error
	}
	else if (op_type == MLX90614_DBG_MSG_R) {
		// TODO: Do something if error
	}
}

/**
 * @brief Temperatur compensation for value
 * 
 * @note https://arachnoid.com/polysolve/
 *
 * @param x Value
 * @return double
 */
double MLX90614_temp_compensation(double x)
{
	double terms[] = {
		4.5285558056644653e+001,
		-7.7381865153331808e-001,
		1.4986086918853544e-002
	};

	size_t csz = sizeof terms / sizeof *terms;

	double t = 1;
	double r = 0;
	for (int i = 0; i < csz;i++)
	{
		r += terms[i] * t;
		t *= x;
	}
	return r;
}
