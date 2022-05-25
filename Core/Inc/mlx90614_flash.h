/**
 * @file mlx90614_flash.h
 * @author GerrFrog (ghaghal93@gmail.com)
 * @brief File contains flash functions for MLX90614
 * @version 1.0
 * @date 2022-05-25
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef INC_MLX90614_FLASH_H_
#define INC_MLX90614_FLASH_H_

#include "stm32f4xx_hal.h"

/**
 * @brief Flash the value to MLX90614
 *
 * @param mlx_addr I2C address of MLX sensor
 * @param mem_addr Memory to be flashed
 * @param value Value to flash to memory
 * @param hi2c I2C handler
 */
void mlx90614_flash(
    uint16_t mlx_addr, 
    uint16_t mem_addr, 
    uint16_t value, 
    I2C_HandleTypeDef hi2c
);

#endif /* INC_MLX90614_FLASH_H_ */
