/*
 * mlx_common.c
 *
 *  Created on: Jun 7, 2022
 *      Author: falls
 */
#include "mlx_common.h"

void MLX_Device_ctor(
    MLX_Device* const me,
    I2C_HandleTypeDef* hi2c,
    uint16_t address
)
{
    me->address = address;
}

void MLX_Device_get_current_temp(
    MLX_Device* const me,
    float* output
)
{
    *output = me->current_temp;
}

void MLX_Device_get_comp_temp(
    MLX_Device* const me,
    float* output
)
{
    *output = me->comp_temp;
}

void MLX_Device_get_hi2c(
    MLX_Device* const me,
    I2C_HandleTypeDef* output
)
{
    output = me->hi2c;
}

void MLX_Device_get_address(
    MLX_Device* const me,
    uint16_t* output
)
{
    *output = me->address;
}

void MLX_Device_set_current_temp(
    MLX_Device* const me,
    float input
)
{
    me->current_temp = input;
}

void MLX_Device_set_comp_temp(
    MLX_Device* const me,
    float input
)
{
    me->comp_temp = input;
}