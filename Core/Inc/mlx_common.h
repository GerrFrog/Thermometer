/*
 * mlx_common.h
 *
 *  Created on: Jun 7, 2022
 *      Author: falls
 */

#ifndef INC_MLX_COMMON_H_
#define INC_MLX_COMMON_H_

#include "stm32f4xx_hal.h"
#include "common.h"

typedef struct 
{
    I2C_HandleTypeDef* hi2c;

    uint16_t address;

    float current_temp;

    float comp_temp;
} MLX_Device;

void MLX_Device_ctor(
    MLX_Device* const me,
    I2C_HandleTypeDef* hi2c,
    uint16_t address
);

void MLX_Device_get_current_temp(
    MLX_Device* const me,
    float* output
);

void MLX_Device_get_comp_temp(
    MLX_Device* const me,
    float* output
);

void MLX_Device_get_hi2c(
    MLX_Device* const me,
    I2C_HandleTypeDef* output
);

void MLX_Device_get_address(
    MLX_Device* const me,
    uint16_t* output
);

void MLX_Device_set_current_temp(
    MLX_Device* const me,
    float input
);

void MLX_Device_set_comp_temp(
    MLX_Device* const me,
    float input
);

#endif /* INC_MLX_COMMON_H_ */
