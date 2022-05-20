/*
 * mlx90632_debug_mode.c
 *
 *  Created on: Mar 16, 2022
 *      Author: falls
 */
#include <mlx90632_debug.h>

void mlx90632_start_debugging(int mlx_addr, I2C_HandleTypeDef hi2c)
{
	  char debug_status[] = "Debug mode";

	  SSD1306_GotoXY (0,0);
	  SSD1306_Puts (debug_status, &Font_11x18, 1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(1000);
	  SSD1306_Clear();
}
