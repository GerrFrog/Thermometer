/*
 * mlx90632_debug_mode.h
 *
 *  Created on: Mar 16, 2022
 *      Author: falls
 */

#ifndef INC_MLX90632_DEBUG_MODE_H_
#define INC_MLX90632_DEBUG_MODE_H_

#include "stm32f4xx_hal.h"
#include "ssd1306.h"

void mlx90632_start_debugging(int mlx_addr, I2C_HandleTypeDef hi2c);

#endif /* INC_MLX90632_DEBUG_MODE_H_ */
