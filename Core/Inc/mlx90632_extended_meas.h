/**
 * @file mlx90632_extended_meas.h
 * @author GerrFrog (ghaghal93@gmail.com)
 * @brief File contains function for extended mode of MLX90632
 * @version 1.0
 * @date 2022-05-25
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef INC_MLX90632_EXTENDED_MEAS_H_
#define INC_MLX90632_EXTENDED_MEAS_H_

#include "stm32f4xx_hal.h"

/** 
 * @brief Read raw ambient and object temperature for extended range
 *
 * Trigger and read raw ambient and object temperatures. This values still need
 * to be pre-processed via @link mlx90632_preprocess_temp_ambient_extended @endlink and @link
 * mlx90632_preprocess_temp_object_extended @endlink functions and then processed via @link
 * mlx90632_calc_temp_ambient_extended @endlink and @link mlx90632_calc_temp_object_extended @endlink
 * to retrieve values in milliCelsius
 *
 * @param[out] ambient_new_raw Pointer to where new raw ambient temperature is written
 * @param[out] object_new_raw Pointer to where new raw object temperature is written
 * @param[out] ambient_old_raw Pointer to where old raw ambient temperature is written
 * @param[out] object_old_raw Pointer to where old raw object temperature is written
 *
 * @retval 0 Successfully read both temperatures
 * @retval <0 Something went wrong. Check errno.h for more details
 */
int32_t mlx90632_read_temp_raw_extended(
    int16_t *ambient_new_raw, 
    int16_t *ambient_old_raw, 
    int16_t *object_new_raw, 
    I2C_HandleTypeDef hi2c
);

/** 
 * @brief Read raw ambient and object temperature for extended range sleeping step mode
 *
 * Trigger and read raw ambient and object temperatures. This values still need
 * to be pre-processed via @link mlx90632_preprocess_temp_ambient_extended @endlink and @link
 * mlx90632_preprocess_temp_object_extended @endlink functions and then processed via @link
 * mlx90632_calc_temp_ambient_extended @endlink and @link mlx90632_calc_temp_object_extended @endlink
 * to retrieve values in milliCelsius
 *
 * @param[out] ambient_new_raw Pointer to where new raw ambient temperature is written
 * @param[out] object_new_raw Pointer to where new raw object temperature is written
 * @param[out] ambient_old_raw Pointer to where old raw ambient temperature is written
 * @param[out] object_old_raw Pointer to where old raw object temperature is written
 *
 * @retval 0 Successfully read both temperatures
 * @retval <0 Something went wrong. Check errno.h for more details
 */
int32_t mlx90632_read_temp_raw_extended_burst(
    int16_t *ambient_new_raw, 
    int16_t *ambient_old_raw, 
    int16_t *object_new_raw, 
    I2C_HandleTypeDef hi2c
);

/** 
 * @brief Calculation of raw ambient output for the extended range
 *
 * Preprocessing of the raw ambient value
 *
 * @param[in] ambient_new_raw ambient temperature from @link MLX90632_RAM_3 @endlink from meas num 17
 * @param[in] ambient_old_raw ambient temperature from @link MLX90632_RAM_3 @endlink from meas num 18
 * @param[in] Gb Register value on @link MLX90632_EE_Gb @endlink
 *
 * @return Calculated ambient raw output
 */
double mlx90632_preprocess_temp_ambient_extended(
    int16_t ambient_new_raw, 
    int16_t ambient_old_raw, 
    int16_t Gb
);

/** 
 * @brief Calculation of raw object output for the extended range
 *
 * Preprocessing of the raw object value
 *
 * @param[in] object_new_raw object temperature from @link MLX90632_RAM_1 @endlink and @link MLX90632_RAM_2 @endlink
 *                              from meas_number 17, 18 and 19
 * @param[in] ambient_new_raw ambient temperature from @link MLX90632_RAM_3 @endlink from meas num 17
 * @param[in] ambient_old_raw ambient temperature from @link MLX90632_RAM_3 @endlink from meas num 18
 * @param[in] Ka Register value on @link MLX90632_EE_Ka @endlink
 *
 * @return Calculated object raw output
 */
double mlx90632_preprocess_temp_object_extended(
    int16_t object_new_raw, 
    int16_t ambient_new_raw,
    int16_t ambient_old_raw, 
    int16_t Ka
);

/** 
 * @brief Calculation of ambient temperature for the extended range
 *
 * @param[in] ambient_new_raw ambient temperature from @link MLX90632_RAM_3 @endlink from meas num 17
 * @param[in] ambient_old_raw ambient temperature from @link MLX90632_RAM_3 @endlink from meas num 18
 * @param[in] P_T Register value on @link MLX90632_EE_P_T @endlink
 * @param[in] P_R Register value on @link MLX90632_EE_P_R @endlink
 * @param[in] P_G Register value on @link MLX90632_EE_P_G @endlink
 * @param[in] P_O Register value on @link MLX90632_EE_P_O @endlink
 * @param[in] Gb Register value on @link MLX90632_EE_Gb @endlink
 *
 * @return Calculated ambient temperature degrees Celsius
 */
double mlx90632_calc_temp_ambient_extended(
    int16_t ambient_new_raw, 
    int16_t ambient_old_raw, 
    int32_t P_T,
    int32_t P_R, 
    int32_t P_G, 
    int32_t P_O, 
    int16_t Gb
);

/** 
 * @brief Calculation of object temperature for the extended range
 *
 * @param[in] object object temperature from @link mlx90632_preprocess_temp_object @endlink
 * @param[in] ambient sensor ambient temperature from @link mlx90632_preprocess_temp_ambient @endlink
 * @param[in] reflected reflected (environment) temperature from a sensor different than the MLX90632 or acquired by other means
 * @param[in] Ea Register value on @link MLX90632_EE_Ea @endlink
 * @param[in] Eb Register value on @link MLX90632_EE_Eb @endlink
 * @param[in] Ga Register value on @link MLX90632_EE_Ga @endlink
 * @param[in] Fb Register value on @link MLX90632_EE_Fb @endlink
 * @param[in] Fa Register value on @link MLX90632_EE_Fa @endlink
 * @param[in] Ha Register value on @link MLX90632_EE_Ha @endlink
 * @param[in] Hb Register value on @link MLX90632_EE_Hb @endlink
 *
 * @note emissivity Value provided by user of the object emissivity
 * using @link mlx90632_set_emissivity @endlink function.
 *
 * @return Calculated object temperature in milliCelsius
 */
double mlx90632_calc_temp_object_extended(
    int32_t object, 
    int32_t ambient, 
    double reflected,
    int32_t Ea, 
    int32_t Eb, 
    int32_t Ga, 
    int32_t Fa, 
    int32_t Fb,
    int16_t Ha, 
    int16_t Hb
);

/** 
 * @brief Switch the measurement type of the MLX90632
 *
 * @param[in] type measurement type to switch to
 *
 * @note the available types are defined as @link MLX90632_MTYP_MEDICAL @endlink
 * and @link MLX90632_MTYP_EXTENDED @endlink
 *
 * @retval 0 Successfully swithed the meausurement mode
 * @retval <0 Something went wrong. Check errno.h for more details
 */
int32_t mlx90632_set_meas_type(
    uint8_t type, 
    I2C_HandleTypeDef hi2c
);

/** 
 * @brief Get the current measurement type set in the MLX90632
 *
 * @note the available types are defined as @link MLX90632_MTYP_MEDICAL @endlink
 * and @link MLX90632_MTYP_EXTENDED @endlink
 *
 * @retval = @link MLX90632_MTYP_MEDICAL @endlink - medical range measurement type
 * @retval = @link MLX90632_MTYP_EXTENDED @endlink - extended range measurement type
 * @retval <0 Something went wrong. Consult errno.h for more details.
 */
int32_t mlx90632_get_meas_type(
    I2C_HandleTypeDef hi2c
);

#ifdef TEST
/** 
 * @brief Read ambient raw old and new values for the extended range based on @link mlx90632_start_measurement @endlink return value.
 *
 * Two i2c_reads are needed to obtain necessary raw ambient values from the sensor, as they are then
 * preprocessed before going to calculation functions
 * @param[out] *ambient_new_raw Pointer to memory location where new ambient value from sensor is stored
 * @param[out] *ambient_old_raw Pointer to memory location where old ambient value from sensor is stored
 *
 * @retval 0 Successfully read both values
 * @retval <0 Something went wrong. Check errno.h for more details.
 */
int32_t mlx90632_read_temp_ambient_raw_extended(
    int16_t *ambient_new_raw, 
    int16_t *ambient_old_raw, 
    I2C_HandleTypeDef hi2c
);

/** 
 * @brief Read object raw values for the extended range based on @link mlx90632_start_measurement @endlink return value.
 *
 * Six i2c_reads are needed to obtain necessary raw object values from the sensor. These values are grouped and then
 * averaged before return of the function. After that they are then preprocessed before going to
 * calculation functions
 *
 * @param[out] *object_new_raw Pointer to memory location where average of new object values from sensor is stored
 *
 * @retval 0 Successfully read values
 * @retval <0 Something went wrong. Check errno.h for more details.
 */
int32_t mlx90632_read_temp_object_raw_extended(
    int16_t *object_new_raw, 
    I2C_HandleTypeDef hi2c
);
#endif

#endif /* INC_MLX90632_EXTENDED_MEAS_H_ */
