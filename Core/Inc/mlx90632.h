/*
 * mlx90632.h
 *
 *  Created on: Mar 11, 2022
 *      Author: falls
 */

#ifndef INC_MLX90632_H_
#define INC_MLX90632_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "common.h"

/* DEFAULT SLAVE ADDRESS */
#define MLX90632_DEFAULT_SA 0x003A

/* OPCODE DEFINES */
#define MLX90632_OP_EEPROM	 0x2400
#define MLX90632_OP_REGISTER 0x3000
#define MLX90632_OP_RAM		 0x4000

/* EEPROM offsets with 16-bit data */
#define MLX90632_CHIP_VERSION_1 (MLX90632_OP_EEPROM | 0x0005)
#define MLX90632_CHIP_VERSION_2 (MLX90632_OP_EEPROM | 0x0006)
#define MLX90632_CHIP_VERSION_3 (MLX90632_OP_EEPROM | 0x0007)
#define MLX90632_CRC (MLX90632_OP_EEPROM | 0x0008)
#define MLX90632_SENSOR_INFORMATION (MLX90632_OP_EEPROM | 0x0009)

/* REGISTER offsets with 16-bit data */
#define MLX90632_REG_I2C_ADDRESS (MLX90632_OP_REGISTER | 0x0000)
#define MLX90632_REG_CONTROL (MLX90632_OP_REGISTER | 0x0001)
#define MLX90632_REG_STATUS (MLX90632_OP_REGISTER | 0x0FFF)

/* RAM offsets with 16-bit data */
#define MLX90632_RAM_1 (MLX90632_OP_RAM | 0x0000)
#define MLX90632_RAM_2 (MLX90632_OP_RAM | 0x0001)
#define MLX90632_RAM_3 (MLX90632_OP_RAM | 0x0002)
#define MLX90632_RAM_4 (MLX90632_OP_RAM | 0x0003)
#define MLX90632_RAM_5 (MLX90632_OP_RAM | 0x0004)
#define MLX90632_RAM_6 (MLX90632_OP_RAM | 0x0005)
#define MLX90632_RAM_7 (MLX90632_OP_RAM | 0x0006)
#define MLX90632_RAM_8 (MLX90632_OP_RAM | 0x0007)
#define MLX90632_RAM_9 (MLX90632_OP_RAM | 0x0008)
#define MLX90632_RAM_52 (MLX90632_OP_RAM | 0x0033)
#define MLX90632_RAM_53 (MLX90632_OP_RAM | 0x0034)
#define MLX90632_RAM_54 (MLX90632_OP_RAM | 0x0035)
#define MLX90632_RAM_55 (MLX90632_OP_RAM | 0x0036)
#define MLX90632_RAM_56 (MLX90632_OP_RAM | 0x0037)
#define MLX90632_RAM_57 (MLX90632_OP_RAM | 0x0038)
#define MLX90632_RAM_58 (MLX90632_OP_RAM | 0x0039)
#define MLX90632_RAM_59 (MLX90632_OP_RAM | 0x003A)
#define MLX90632_RAM_60 (MLX90632_OP_RAM | 0x003B)

#define MLX90632_DBG_OFF 0
#define MLX90632_DBG_ON 1
#define MLX90632_DBG_MSG_W 0
#define MLX90632_DBG_MSG_R 1

void MLX90632_WriteReg(uint8_t devAddr, uint8_t regAddr, uint16_t data, I2C_HandleTypeDef hi2c);

uint16_t MLX90632_ReadReg(uint8_t devAddr, uint16_t regAddr, uint8_t dbg_lvl, I2C_HandleTypeDef hi2c);

float MLX90632_ReadTemp(uint8_t devAddr, I2C_HandleTypeDef hi2c);

void MLX90632_SendDebugMsg(uint8_t op_type, uint8_t devAddr, uint8_t regAddr, uint16_t data, uint8_t crc_in, uint8_t crc_calc);

#endif /* INC_MLX90632_H_ */
