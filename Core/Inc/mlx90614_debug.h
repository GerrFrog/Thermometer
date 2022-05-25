/**
 * @file mlx90614_debug.h
 * @author GerrFrog (ghaghal93@gmail.com)
 * @brief File contains debugs functions for MLX90614
 * @version 1.0
 * @date 2022-05-25
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef INC_MLX90614_DEBUG_H_
#define INC_MLX90614_DEBUG_H_

#include "stm32f4xx_hal.h"
#include "ssd1306.h"

#define HEX2DEX "hex2dec"
#define TO_MAX_STATUS "To max:"
#define TO_MIN_STATUS "To min:"
#define PWM_CF_REG_STATUS "PWM Cf Reg:"
#define TA_RANGE_STATUS "Ta range"
#define EMISSIVITY_STATUS "Emissivity:"
#define CONFIG_REGISTER_STATUS "Config Reg:"
#define SLAVE_ADDRESS_STATUS "Slave addr:"
#define ID1_STATUS "ID1 Reg:"
#define ID2_STATUS "ID2 Reg:"
#define ID3_STATUS "ID3 Reg:"
#define ID4_STATUS "ID4 Reg:"

/**
 * @brief Convert emissivity float type to emissivity char type
 *
 * @param emissivity Emissivity value (max 65535)
 * @param arr Pointer to output array
 * @return void
 */
void int_emissivity_to_char_emissivity(
    int emissivity, 
    char* arr
);

/**
 * @brief Convert address (e.g. 0xFF) to char type
 *
 * @param address Address to convert
 * @param arr Pointer to output char array
 */
void int_address_to_char_address(
    int address, 
    char* arr
);

/**
 * @brief Output message about debug to display
 *
 * @param message Pointer to message (what parameter)
 * @param char_param Parameter to output
 * @return void
 */
void display_output(
    char* message, 
    char* char_emissivity, 
    char hex
);

/**
 * @brief Start debugging for MLX90614 sensor
 *
 * @param mlx_addr I2C Address of MLX
 * @param hi2c I2C handler
 * @return void
 */
void mlx90614_start_debugging(
    int mlx_addr, 
    I2C_HandleTypeDef hi2c
);

#endif /* INC_MLX90614_DEBUG_H_ */
