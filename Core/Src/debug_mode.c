/*
 * debug_mode.c
 *
 *  Created on: Feb 8, 2022
 *      Author: falls
 */
#include "debug_mode.h"

/**
 * @brief Convert emissivity float type to emissivity char type
 *
 * @param emissivity Emissivity value (max 65535)
 * @param arr Pointer to output array
 * @return void
 */
void int_emissivity_to_char_emissivity(int emissivity, char* arr)
{
	emissivity *= 10000;
	arr[6] = '\0';
	arr[5] = emissivity % 10 + '0';
	emissivity /= 10;
	arr[4] = emissivity % 10 + '0';
	emissivity /= 10;
	arr[3] = emissivity % 10 + '0';
	emissivity /= 10;
	arr[2] = emissivity % 10 + '0';
	emissivity /= 10;
	arr[1] = ',';
	arr[0] = emissivity % 10 + '0';
}

/**
 * @brief Convert address (e.g. 0xFF) to char type
 *
 * @param address Address to convert
 * @param arr Pointer to output char array
 */
void int_address_to_char_address(int address, char* arr)
{
	arr[5] = '\0';
	arr[4] = address % 10 + '0';
	address /= 10;
	arr[3] = address % 10 + '0';
	address /= 10;
	arr[2] = address % 10 + '0';
	address /= 10;
	arr[1] = address % 10 + '0';
	address /= 10;
	arr[0] = address % 10 + '0';
}

/**
 * @brief Output message about debug to display
 *
 * @param message Pointer to message (what parameter)
 * @param char_param Parameter to output
 * @return void
 */
void display_output(char* message, char* char_param, char hex)
{
	SSD1306_GotoXY (0,0);
	SSD1306_Puts (message, &Font_11x18, 1);
	SSD1306_UpdateScreen();

	HAL_Delay(1500);
	SSD1306_Clear();

	SSD1306_GotoXY (0,0);
	SSD1306_Puts (char_param, &Font_11x18, 1);
	if (hex > 0)
	{
		SSD1306_GotoXY (0,20);
		SSD1306_Puts (HEX2DEX, &Font_11x18, 1);
	}
	SSD1306_UpdateScreen();

	HAL_Delay(1500);
	SSD1306_Clear();
}













