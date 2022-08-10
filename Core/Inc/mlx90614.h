/*
 * mlx90614_.h
 *
 *  Created on: Jun 7, 2022
 *      Author: falls
 */

#ifndef INC_MLX90614_H_
#define INC_MLX90614_H_

#include "mlx_common.h"
#include "ssd1306.h"

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

/* OPCODE DEFINES */
#define MLX90614_OP_RAM		0x00
#define MLX90614_OP_EEPROM	0x20
#define MLX90614_OP_SLEEP	0xFF

typedef struct
{
    MLX_Device super_device;

    uint16_t  addr_raw1; /* raw data IR channel 1 */

    uint16_t  addr_raw2; /* raw data IR channel 2 */

    uint16_t  addr_tamb; /* ambient temperature */

    uint16_t  addr_tobj1; /* object 1 temperature */

    uint16_t  addr_tobj2; /* object 2 temperature */

    uint16_t  addr_tomax; /* object temperature min register */

    uint16_t  addr_tomin; /* object temperature max register */

    uint16_t  addr_pwmctrl; /* pwm configuration register */

    uint16_t  addr_tarange; /* ambient temperature register */

    uint16_t  addr_emissivity; /* emissivity correction register */

    uint16_t  addr_cfg1; /* configuration register */

    uint16_t  addr_sa; /* slave address register */

    uint16_t  addr_id1; /*[read-only] 1 ID register */

    uint16_t  addr_id2; /*[read-only] 2 ID register */

    uint16_t  addr_id3; /*[read-only] 3 ID register */

    uint16_t  addr_id4; /*[read-only] 4 ID register */

} MLX90614;

void MLX90614_ctor(
    MLX90614* const me,
    I2C_HandleTypeDef* hi2c,
    uint16_t address
);

void MLX90614_write_reg(
    MLX90614* const me,
    uint16_t regAddr,
    uint16_t data
);

uint16_t MLX90614_read_reg(
    MLX90614* const me,
    uint16_t regAddr
);

void MLX90614_read_temp(
    MLX90614* const me
);

void MLX90614_temp_compensation(
    MLX90614* const me
);

float MLX90614_get_current_temp(
    MLX90614* const me
);

float MLX90614_get_comp_temp(
    MLX90614* const me
);

void MLX90614_debug_mode(
    MLX90614* const me
);

void MLX90614_flash_mode(
    MLX90614* const me
);

void MLX90614_start_standard_mode();

#endif /* INC_MLX90614_H_ */
