/*
 * debug_mode.c
 *
 *  Created on: Feb 8, 2022
 *      Author: falls
 */
#include <mlx90614_debug.h>
#include "stm32f4xx_hal.h"
#include "mlx90614.h"
#include "ssd1306.h"

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

/**
 * @brief Start debugging for MLX90614 sensor
 *
 * @param mlx_addr I2C Address of MLX
 * @param hi2c I2C handler
 * @return void
 */
void mlx90614_start_debugging(int mlx_addr, I2C_HandleTypeDef hi2c)
{
	  char debug_status[] = "Debug mode";

	  SSD1306_GotoXY (0,0);
	  SSD1306_Puts (debug_status, &Font_11x18, 1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(1000);
	  SSD1306_Clear();

	  // To max
	  char char_to_max[6];
	  int int_to_max = MLX90614_ReadReg(mlx_addr, MLX90614_TOMAX, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_to_max, char_to_max);

	  // To min
	  char char_to_min[6];
	  int int_to_min = MLX90614_ReadReg(mlx_addr, MLX90614_TOMIN, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_to_min, char_to_min);

	  // PWM Configuration register
	  char char_pwm_reg[6];
	  int int_pwm_reg = MLX90614_ReadReg(mlx_addr, MLX90614_PWMCTRL, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_pwm_reg, char_pwm_reg);

	  // Ta Range
	  char char_ta_range[6];
	  int int_ta_range = MLX90614_ReadReg(mlx_addr, MLX90614_TARANGE, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_ta_range, char_ta_range);

	  // Emissivity
//	  char char_emissivity[7];
//	  float emissivity = MLX90614_ReadReg(mlx_addr, MLX90614_EMISSIVITY, MLX90614_DBG_OFF, hi2c) / 65535; // max 65535
//	  int_emissivity_to_char_emissivity(emissivity, char_emissivity);
	  char char_emissivity[6];
	  int emissivity = MLX90614_ReadReg(mlx_addr, MLX90614_EMISSIVITY, MLX90614_DBG_OFF, hi2c); // max 65535
	  int_address_to_char_address(emissivity, char_emissivity);

	  // Configutation register
	  char char_conf_reg[7];
	  int int_conf_reg = MLX90614_ReadReg(mlx_addr, MLX90614_CFG1, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_conf_reg, char_conf_reg);

	  // Slave address
	  char char_slave_address[6];
	  int int_slave_address = MLX90614_ReadReg(mlx_addr, MLX90614_SA, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_slave_address, char_slave_address);

	  // ID1 Reg
	  char char_id1[6];
	  int int_id1 = MLX90614_ReadReg(mlx_addr, MLX90614_ID1, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_id1, char_id1);

	  // ID2 Reg
	  char char_id2[6];
	  int int_id2 = MLX90614_ReadReg(mlx_addr, MLX90614_ID2, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_id2, char_id2);

	  // ID3 Reg
	  char char_id3[6];
	  int int_id3 = MLX90614_ReadReg(mlx_addr, MLX90614_ID3, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_id3, char_id3);

	  // ID4 Reg
	  char char_id4[6];
	  int int_id4 = MLX90614_ReadReg(mlx_addr, MLX90614_ID4, MLX90614_DBG_OFF, hi2c);
	  int_address_to_char_address(int_id4, char_id4);

	  while(1)
	  {
		  display_output(TO_MAX_STATUS, char_to_max, 1);
		  display_output(TO_MIN_STATUS, char_to_min, 1);
		  display_output(PWM_CF_REG_STATUS, char_pwm_reg, 1);
		  display_output(TA_RANGE_STATUS, char_ta_range, 1);
		  display_output(EMISSIVITY_STATUS, char_emissivity, 0);
		  display_output(CONFIG_REGISTER_STATUS, char_conf_reg, 1);
		  display_output(SLAVE_ADDRESS_STATUS, char_slave_address, 1);
		  display_output(ID1_STATUS, char_id1, 1);
		  display_output(ID2_STATUS, char_id2, 1);
		  display_output(ID3_STATUS, char_id3, 1);
		  display_output(ID4_STATUS, char_id4, 1);
	  }
}












