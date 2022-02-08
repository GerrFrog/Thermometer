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
#define EMISSIVITY_STATUS "Emissivity:"
#define SLAVE_ADDRESS_STATUS "SA:"

void int_emissivity_to_char_emissivity(int emissivity, char* arr);

void int_address_to_char_address(int address, char* arr);

void display_output(char* message, char* char_emissivity);


#endif /* INC_DEBUG_MODE_H_ */
