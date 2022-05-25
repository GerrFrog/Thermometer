/**
 * @file mlx90632_depends.h
 * @author GerrFrog (ghaghal93@gmail.com)
 * @brief File contains functions with I2C realizations for MLX90632
 * @version 1.0
 * @date 2022-05-25
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef INC_MLX90632_DEPENDS_H_
#define INC_MLX90632_DEPENDS_H_

#include "stm32f4xx_hal.h"
#include "mlx90632.h"

/** 
 * @brief Read the register_address 16-bit value from the mlx90632
 *
 * i2c read is processor specific and this function expects to have address of mlx90632 known, as it operates purely on
 * register addresses.
 *
 * @note Needs to be implemented externally
 * @param[in] register_address Address of the register to be read from
 * @param[out] *value pointer to where read data can be written

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_i2c_read(
    int16_t register_address, 
    uint16_t *value, 
    I2C_HandleTypeDef hi2c
);

/** 
 * @brief Read the register_address 32-bit value from the mlx90632
 *
 * i2c read is processor specific and this function expects to have address of mlx90632 known, as it operates purely on
 * register addresses.
 *
 * @note Needs to be implemented externally
 * @param[in] register_address Address of the register to be read from
 * @param[out] *value pointer to where read data can be written

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_i2c_read32(
    int16_t register_address, 
    uint32_t *value, 
    I2C_HandleTypeDef hi2c
);

/** 
 * @brief Write value to register_address of the mlx90632
 *
 * i2c write is processor specific and this function expects to have address of mlx90632 known, as it operates purely
 * on register address and value to be written to.
 *
 * @note Needs to be implemented externally
 * @param[in] register_address Address of the register to be read from
 * @param[in] value value to be written to register address of mlx90632

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_i2c_write(
    int16_t register_address, 
    uint16_t value, 
    I2C_HandleTypeDef hi2c
);

/** 
 * @brief Blocking function for sleeping in microseconds
 *
 * Range of microseconds which are allowed for the thread to sleep. This is to avoid constant pinging of sensor if the
 * data is ready.
 *
 * @note Needs to be implemented externally
 * @param[in] min_range Minimum amount of microseconds to sleep
 * @param[in] max_range Maximum amount of microseconds to sleep
 */
void usleep(
    int min_range,
    int max_range
);

/** 
 * @brief Blocking function for sleeping in milliseconds
 *
 * milliseconds which are allowed for the thread to sleep. This is to avoid constant pinging of sensor
 * while the measurement is ongoing in sleeping step mode.
 *
 * @note Needs to be implemented externally
 * @param[in] msecs Amount of milliseconds to sleep
 */
void msleep(
    int msecs
);


#endif /* INC_MLX90632_DEPENDS_H_ */
