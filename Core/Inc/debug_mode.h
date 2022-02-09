/*
 * debug_mode.h
 *
 *  Created on: Feb 8, 2022
 *      Author: falls
 */

#ifndef INC_DEBUG_MODE_H_
#define INC_DEBUG_MODE_H_

#include "stm32f4xx_hal.h"
#include "ssd1306.h"

//char emissivity_status[] = "Emissivity: ";
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

void int_emissivity_to_char_emissivity(int emissivity, char* arr);

void int_address_to_char_address(int address, char* arr);

void display_output(char* message, char* char_emissivity, char hex);


#endif /* INC_DEBUG_MODE_H_ */
