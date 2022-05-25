/**
 * @file mlx90632_debug.h
 * @author GerrFrog (ghaghal93@gmail.com)
 * @brief File contains all functions for MLX90632 debugging
 * @version 1.0
 * @date 2022-05-25
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef INC_MLX90632_DEBUG_H_
#define INC_MLX90632_DEBUG_H_

#include "stm32f4xx_hal.h"
#include "ssd1306.h"

/**
 * @brief Start debugging for MLX90632
 * 
 * @param mlx_addr Address of MLX sensor
 * @param hi2c Handler for I2C
 */
void mlx90632_start_debugging(
    int mlx_addr, 
    I2C_HandleTypeDef hi2c
);

#endif /* INC_MLX90632_DEBUG_H_ */
