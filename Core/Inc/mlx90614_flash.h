/*
 * mlx90614_flash.h
 *
 *  Created on: Mar 16, 2022
 *      Author: falls
 */

#ifndef INC_MLX90614_FLASH_H_
#define INC_MLX90614_FLASH_H_

#include "stm32f4xx_hal.h"

void mlx90614_flash(uint16_t mlx_addr, uint16_t mem_addr, uint16_t value, I2C_HandleTypeDef hi2c);


#endif /* INC_MLX90614_FLASH_H_ */
