/*
 * mlx90614_.c
 *
 *  Created on: Jun 7, 2022
 *      Author: falls
 */
#include <mlx90614.h>

void MLX90614_ctor(
    MLX90614* const me,
    I2C_HandleTypeDef* hi2c,
    uint16_t address
)
{
	MLX_Device_ctor(
		&me->super_device,
		hi2c,
		address
	);

    me->addr_raw1 = (MLX90614_OP_RAM | 0x04); /* raw data IR channel 1 */

    me->addr_raw2 = (MLX90614_OP_RAM | 0x05); /* raw data IR channel 2 */

    me->addr_tamb = (MLX90614_OP_RAM | 0x06); /* ambient temperature */

    me->addr_tobj1 = (MLX90614_OP_RAM | 0x07); /* object 1 temperature */

    me->addr_tobj2 = (MLX90614_OP_RAM | 0x08); /* object 2 temperature */

    me->addr_tomax = (MLX90614_OP_EEPROM | 0x00); /* object temperature min register */

    me->addr_tomin = (MLX90614_OP_EEPROM | 0x01); /* object temperature max register */

    me->addr_pwmctrl = (MLX90614_OP_EEPROM | 0x02); /* pwm configuration register */

    me->addr_tarange = (MLX90614_OP_EEPROM | 0x03); /* ambient temperature register */

    me->addr_emissivity = (MLX90614_OP_EEPROM | 0x04); /* emissivity correction register */

    me->addr_cfg1 = (MLX90614_OP_EEPROM | 0x05); /* configuration register */

    me->addr_sa = (MLX90614_OP_EEPROM | 0x0E); /* slave address register */

    me->addr_id1 = (MLX90614_OP_EEPROM | 0x1C); /*[read-only] 1 ID register */

    me->addr_id2 = (MLX90614_OP_EEPROM | 0x1D); /*[read-only] 2 ID register */

    me->addr_id3 = (MLX90614_OP_EEPROM | 0x1E); /*[read-only] 3 ID register */

    me->addr_id4 = (MLX90614_OP_EEPROM | 0x1F); /*[read-only] 4 ID register */
}

void MLX90614_write_reg(
    MLX90614* const me,
    uint16_t regAddr,
    uint16_t data
)
{
	uint8_t i2cdata[4], temp[4];
	uint16_t devAddr;
	I2C_HandleTypeDef hi2c;

	MLX_Device_get_address(&me->super_device, &devAddr);
	MLX_Device_get_hi2c(&me->super_device, &hi2c);

	temp[0] = (devAddr << 1);
	temp[1] = regAddr;

	temp[2] = 0x00;
	temp[3] = 0x00;

	// For a write word command, in the crc8 calculus, you have to include [SA_W, Command, LSB, MSB]

	i2cdata[0] = temp[1]; //EEPROM-address
	i2cdata[1] = temp[2]; //Delete-Byte, low
	i2cdata[2] = temp[3]; //Delete-Byte, high
	i2cdata[3] = CRC8_Calc(temp, 4);

	// HAL_I2C_Master_Transmit(&hi2c, (devAddr << 1), i2cdata, 4, 0xFFFF);
	HAL_I2C_Master_Transmit(
		&hi2c,
		(devAddr << 1),
		i2cdata,
		4,
		0xFFFF
	);
	HAL_Delay(10);

	temp[2] = data & 0xFF; //Getting LSB first
	temp[3] = data >> 8;   //Getting MSB after

	i2cdata[0] = temp[1]; //EEPROM-address
	i2cdata[1] = temp[2]; //Delete-Byte, low
	i2cdata[2] = temp[3]; //Delete-Byte, high
	i2cdata[3] = CRC8_Calc(temp, 4);

	HAL_I2C_Master_Transmit(
		&hi2c,
		(devAddr << 1),
		i2cdata, 4,
		0xFFFF
	);
	HAL_Delay(10);
}

uint16_t MLX90614_read_reg(
    MLX90614* const me,
    uint16_t regAddr
)
{
	uint16_t data;
	uint8_t in_buff[3], crc_buff[5], crc;

	uint16_t devAddr;
	I2C_HandleTypeDef hi2c;

	MLX_Device_get_address(&me->super_device, &devAddr);
	MLX_Device_get_hi2c(&me->super_device, &hi2c);

	HAL_I2C_Mem_Read(
		&hi2c,
		(devAddr << 1),
		regAddr,
		1,
		in_buff,
		3,
		100
	);

	// For a read word command, in the crc8 calculus, you have to include [SA_W, Command, SA_R, LSB, MSB]
	crc_buff[0] = (devAddr << 1);
	crc_buff[1] = regAddr;
	crc_buff[2] = (devAddr << 1) + 1;
	crc_buff[3] = in_buff[0];
	crc_buff[4] = in_buff[1];
	crc = CRC8_Calc(crc_buff, 5);

	data = (in_buff[1] <<8 | in_buff[0]);

	// TODO: implement CRC8 check on data received
	if (crc != in_buff[2])
		data = 0x0000;

	HAL_Delay(1);

	return data;
}

void MLX90614_read_temp(
    MLX90614* const me
)
{
	uint16_t data;

	data = MLX90614_read_reg(me, me->addr_tobj1);

	MLX_Device_set_current_temp(
		&me->super_device,
		data * 0.02 - 273.15
	);
}

void MLX90614_temp_compensation(
    MLX90614* const me
)
{
	double x;
	double terms[] = {
		4.5285558056644653e+001,
		-7.7381865153331808e-001,
		1.4986086918853544e-002
	};

	size_t csz = sizeof terms / sizeof *terms;

	double t = 1;
	double r = 0;

	MLX_Device_get_current_temp(
		&me->super_device,
		&x
	);

	for (int i = 0; i < csz;i++)
	{
		r += terms[i] * t;
		t *= x;
	}

	MLX_Device_set_comp_temp(
		&me->super_device,
		r
	);
}

float MLX90614_get_current_temp(
    MLX90614* const me
)
{
	float output;
	MLX_Device_get_current_temp(
		&me->super_device,
		&output
	);
	return output;
}

float MLX90614_get_comp_temp(
    MLX90614* const me
)
{
	float output;
	MLX_Device_get_comp_temp(
		&me->super_device,
		&output
	);
	return output;
}

void MLX90614_debug_mode(
    MLX90614* const me
)
{
	// char debug_status[] = "Debug mode";

	// SSD1306_GotoXY (0,0);
	// SSD1306_Puts (debug_status, &Font_11x18, 1);
	// SSD1306_UpdateScreen();
	// HAL_Delay(1000);
	// SSD1306_Clear();

	// // To max
	// char char_to_max[6];
	// int int_to_max = MLX90614_ReadReg(mlx_addr, MLX90614_TOMAX, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_to_max, char_to_max);

	// // To min
	// char char_to_min[6];
	// int int_to_min = MLX90614_ReadReg(mlx_addr, MLX90614_TOMIN, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_to_min, char_to_min);

	// // PWM Configuration register
	// char char_pwm_reg[6];
	// int int_pwm_reg = MLX90614_ReadReg(mlx_addr, MLX90614_PWMCTRL, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_pwm_reg, char_pwm_reg);

	// // Ta Range
	// char char_ta_range[6];
	// int int_ta_range = MLX90614_ReadReg(mlx_addr, MLX90614_TARANGE, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_ta_range, char_ta_range);

	// // Emissivity
	// char char_emissivity[6];
	// int emissivity = MLX90614_ReadReg(mlx_addr, MLX90614_EMISSIVITY, MLX90614_DBG_OFF, hi2c); // max 65535
	// int_address_2_char_address(emissivity, char_emissivity);

	// // Configutation register
	// char char_conf_reg[7];
	// int int_conf_reg = MLX90614_ReadReg(mlx_addr, MLX90614_CFG1, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_conf_reg, char_conf_reg);

	// // Slave address
	// char char_slave_address[6];
	// int int_slave_address = MLX90614_ReadReg(mlx_addr, MLX90614_SA, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_slave_address, char_slave_address);

	// // ID1 Reg
	// char char_id1[6];
	// int int_id1 = MLX90614_ReadReg(mlx_addr, MLX90614_ID1, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_id1, char_id1);

	// // ID2 Reg
	// char char_id2[6];
	// int int_id2 = MLX90614_ReadReg(mlx_addr, MLX90614_ID2, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_id2, char_id2);

	// // ID3 Reg
	// char char_id3[6];
	// int int_id3 = MLX90614_ReadReg(mlx_addr, MLX90614_ID3, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_id3, char_id3);

	// // ID4 Reg
	// char char_id4[6];
	// int int_id4 = MLX90614_ReadReg(mlx_addr, MLX90614_ID4, MLX90614_DBG_OFF, hi2c);
	// int_address_2_char_address(int_id4, char_id4);

//	while(1)
//	{
//		display_output(TO_MAX_STATUS, char_to_max, 1);
//		display_output(TO_MIN_STATUS, char_to_min, 1);
//		display_output(PWM_CF_REG_STATUS, char_pwm_reg, 1);
//		display_output(TA_RANGE_STATUS, char_ta_range, 1);
//		display_output(EMISSIVITY_STATUS, char_emissivity, 0);
//		display_output(CONFIG_REGISTER_STATUS, char_conf_reg, 1);
//		display_output(SLAVE_ADDRESS_STATUS, char_slave_address, 1);
//		display_output(ID1_STATUS, char_id1, 1);
//		display_output(ID2_STATUS, char_id2, 1);
//		display_output(ID3_STATUS, char_id3, 1);
//		display_output(ID4_STATUS, char_id4, 1);
//	}
}

void MLX90614_flash_mode(
    MLX90614* const me
)
{

}

void MLX90614_start_standard_mode()
{
    uint16_t mlx_addr_1 = device_scanner(hi2c1);
    uint16_t mlx_addr_2 = device_scanner(hi2c2);

    float float_temp_1 = 0.0;
    float float_temp_2 = 0.0;

    char char_temp_1[8];
    char char_temp_2[8];

#   ifdef SSD1306_DISPLAY
    SSD1306_Clear();
#   endif

    while(1)
    {
        float_temp_1 = MLX90614_ReadTemp(mlx_addr_1, MLX90614_TOBJ1, hi2c1);
        float_temp_1 = MLX90614_temp_compensation(float_temp_1);
        float_temp_2 = MLX90614_ReadTemp(mlx_addr_2, MLX90614_TOBJ1, hi2c2);

        float_temp_to_char_temp(float_temp_1, char_temp_1);
        float_temp_to_char_temp(float_temp_2, char_temp_2);

        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_SET)
        {
#	        ifdef SSD1306_DISPLAY
            SSD1306_GotoXY(0, 0);
            SSD1306_Puts(char_temp_1, &Font_11x18, 1);

            SSD1306_GotoXY(70, 0);
            SSD1306_Puts(char_temp_2, &Font_11x18, 1);
#	        endif

#		    ifdef USB_SEND
            CDC_Transmit_FS(cap_mess_1, strlen(cap_mess_1));
            CDC_Transmit_FS((uint8_t*)char_temp_1, strlen((uint8_t*)char_temp_1));
            CDC_Transmit_FS(end, strlen(end));

            CDC_Transmit_FS(cap_mess_2, strlen(cap_mess_2));
            CDC_Transmit_FS((uint8_t*)char_temp_2, strlen((uint8_t*)char_temp_2));
            CDC_Transmit_FS(end, strlen(end));
#		    endif
        }

#       ifdef SSD1306_DISPLAY
        SSD1306_GotoXY(0, 29);
        SSD1306_Puts(char_temp_1, &Font_11x18, 1);

        SSD1306_GotoXY(70, 29);
        SSD1306_Puts(char_temp_2, &Font_11x18, 1);

        SSD1306_UpdateScreen();
#       endif

#	    ifdef USB_SEND
        CDC_Transmit_FS(message_1, strlen(message_1));
        CDC_Transmit_FS((uint8_t*)char_temp_1, strlen((uint8_t*)char_temp_1));
        CDC_Transmit_FS(end, strlen(end));

        CDC_Transmit_FS(message_2, strlen(message_2));
        CDC_Transmit_FS((uint8_t*)char_temp_2, strlen((uint8_t*)char_temp_2));
        CDC_Transmit_FS(end, strlen(end));
#	    endif

	// HAL_Delay(10);
    }
}






