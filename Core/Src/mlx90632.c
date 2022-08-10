/*
 * mlx90632_.c
 *
 *  Created on: Jun 7, 2022
 *      Author: falls
 */
#include <mlx90632.h>

#define POW10 10000000000LL

void MLX90632_ctor(
    MLX90632* const me,
    I2C_HandleTypeDef* hi2c,
    uint16_t address
)
{
    MLX_Device_ctor(
        &me->super_device,
        hi2c,
        address
    );

    me->PR = 0x00587f5b;

    me->PG = 0x04a10289;

    me->PT = 0xfff966f8;

    me->PO = 0x00001e0f;

    me->Ea = 4859535;

    me->Eb = 5686508;

    me->Fa = 53855361;

    me->Fb = 42874149;

    me->Ga = -14556410;

    me->Gb = 9728;

    me->Ha = 16384;

    me->Hb = 0;

    me->Ka = 10752;
}

int MLX90632_read_eeprom(
    MLX90632* const me
)
{
    int32_t ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_P_R,
        &me->PR
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_P_G,
        &me->PG
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_P_O,
        &me->PO
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_P_T,
        &me->PT
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Ea,
        &me->Ea
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Eb,
        &me->Eb
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Fa,
        &me->Fa
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Fb,
        &me->Fb
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Ga,
        &me->Ga
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Gb,
        &me->Gb
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Ha,
        &me->Ha
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Hb,
        &me->Hb
    );

    if(ret < 0)
        return ret;

    ret = MLX90632_i2c_read32(
        me,
        MLX90632_EE_Ka,
        &me->Ka
    );

    if(ret < 0)
        return ret;

    return 0;
}

int32_t MLX90632_set_meas_type(
    MLX90632* const me,
    uint8_t type
)
{
    int32_t ret;
    uint16_t reg_ctrl;

    if (
        (type != MLX90632_MTYP_MEDICAL) &
        (type != MLX90632_MTYP_EXTENDED) &
        (type != MLX90632_MTYP_MEDICAL_BURST) &
        (type != MLX90632_MTYP_EXTENDED_BURST)
    )
        return -EINVAL;

    ret = MLX90632_addressed_reset(me);
    if (ret < 0)
        return ret;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_REG_CTRL,
        &reg_ctrl
    );
    if (ret < 0)
        return ret;

    reg_ctrl = reg_ctrl & (~MLX90632_CFG_MTYP_MASK & ~MLX90632_CFG_PWR_MASK);
    reg_ctrl |= (MLX90632_MTYP_STATUS(MLX90632_MEASUREMENT_TYPE_STATUS(type)) | MLX90632_PWR_STATUS_HALT);

    ret = MLX90632_i2c_write(
        me,
        MLX90632_REG_CTRL,
        reg_ctrl
    );
    if (ret < 0)
        return ret;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_REG_CTRL,
        &reg_ctrl
    );
    if (ret < 0)
        return ret;

    reg_ctrl = reg_ctrl & ~MLX90632_CFG_PWR_MASK;
    if (MLX90632_MEASUREMENT_BURST_STATUS(type)) {
        reg_ctrl |= MLX90632_PWR_STATUS_SLEEP_STEP;
    } else {
        reg_ctrl |= MLX90632_PWR_STATUS_CONTINUOUS;
    }

    ret = MLX90632_i2c_write(
        me,
        MLX90632_REG_CTRL, reg_ctrl
    );

    return ret;
}

void MLX90632_set_emissivity(
    MLX90632* const me,
    double emissivity
)
{
    me->emissivity = emissivity;
}

double MLX90632_get_emissivity(
    MLX90632* const me
)
{
    return me->emissivity;
}

int32_t MLX90632_read_temp_raw_extended(
    MLX90632* const me
)
{
    int32_t ret, start_measurement_ret;
    int tries = 3;

    while (tries --> 0)
    {
        start_measurement_ret = MLX90632_start_measurement(me);

        if (start_measurement_ret < 0)
            return start_measurement_ret;

        if (start_measurement_ret == 19)
            break;
    }

    ret = MLX90632_read_temp_raw_extended(me);

    if (ret < 0)
        return ret;

    ret = MLX90632_read_temp_object_raw_extended(
        me
    );

    return ret;
}

void MLX90632_calc_temp_ambient_extended(
    MLX90632* const me
)
{
    double Asub, Bsub, Ablock, Bblock, Cblock, AMB;

    MLX90632_preprocess_temp_ambient_extended(me);

    AMB = me->pre_ambient;

    Asub = ((double)me->PT) / (double)17592186044416.0;
    Bsub = AMB - ((double)me->PR / (double)256.0);
    Ablock = Asub * (Bsub * Bsub);
    Bblock = (Bsub / (double)me->PG) * (double)1048576.0;
    Cblock = (double)me->PO / (double)256.0;

    me->ambient = Bblock + Ablock + Cblock;
}

void MLX90632_preprocess_temp_ambient_extended(
    MLX90632* const me
)
{
    double VR_Ta, kGb;

    kGb = ((double)me->Gb) / 1024.0;

    VR_Ta = me->ambient_old_raw + kGb *
        (me->ambient_new_raw / (MLX90632_REF_3));

    me->pre_ambient = ((me->ambient_new_raw / (MLX90632_REF_3)) / VR_Ta) * 524288.0;
}

void MLX90632_preprocess_temp_object_extended(
    MLX90632* const me
)
{
    double VR_IR, kKa;

    kKa = ((double)me->Ka) / 1024.0;

    VR_IR = me->ambient_old_raw + kKa *
        (me->ambient_new_raw / (MLX90632_REF_3));

    me->pre_object = ((me->object_new_raw / (MLX90632_REF_12)) / VR_IR) * 524288.0;
}

void MLX90632_calc_temp_object_extended(
    MLX90632* const me
)
{
    double kEa, kEb, TAdut;
    double temp = 25.0;
    double tmp_emi = MLX90632_get_emissivity(me);
    double TaTr4;
    double ta4;
    int8_t i;

    kEa = ((double)me->Ea) / ((double)65536.0);
    kEb = ((double)me->Eb) / ((double)256.0);
    TAdut = (((double)me->ambient) - kEb) / kEa + 25;

    TaTr4 = me->ambient + 273.15;
    TaTr4 = TaTr4 * TaTr4;
    TaTr4 = TaTr4 * TaTr4;
    ta4 = TAdut + 273.15;
    ta4 = ta4 * ta4;
    ta4 = ta4 * ta4;
    TaTr4 = TaTr4 - (TaTr4 - ta4) / tmp_emi;

    //iterate through calculations
    for (i = 0; i < 5; ++i)
        temp = MLX90632_calc_temp_object_iteration_extended(
            me, temp, TAdut, TaTr4, tmp_emi
        );

    return temp;
}

int32_t MLX90632_i2c_read(
    MLX90632* const me,
    int16_t register_address,
    uint16_t* value
)
{
    uint8_t data[2];
    int32_t ret;

	uint16_t devAddr;
	I2C_HandleTypeDef hi2c;

	MLX_Device_get_address(&me->super_device, &devAddr);
	MLX_Device_get_hi2c(&me->super_device, &hi2c);

    ret = HAL_I2C_Mem_Read(
        &hi2c,
        devAddr,
        register_address,
        2,
        data,
        sizeof(data),
        100
    );
    *value = data[1]|(data[0]<<8);
    return ret;
}

int32_t MLX90632_i2c_read32(
    MLX90632* const me,
    int16_t register_address,
    uint32_t* value
)
{
    uint8_t data[4];
    int32_t ret;

	uint16_t devAddr;
	I2C_HandleTypeDef hi2c;

	MLX_Device_get_address(&me->super_device, &devAddr);
	MLX_Device_get_hi2c(&me->super_device, &hi2c);

    ret = HAL_I2C_Mem_Read(
        &hi2c,
        devAddr,
        register_address,
        2,
        data,
        sizeof(data),
        100
    );
    *value = data[2]<<24|data[3]<<16|data[0]<<8|data[1];
    return ret;
}

int32_t MLX90632_i2c_write(
    MLX90632* const me,
    int16_t register_address,
    uint16_t value
)
{
    uint8_t data[2];

	uint16_t devAddr;
	I2C_HandleTypeDef hi2c;

	MLX_Device_get_address(&me->super_device, &devAddr);
	MLX_Device_get_hi2c(&me->super_device, &hi2c);

    data[0] = value >> 8;
    data[1] = value;
    return HAL_I2C_Mem_Write(
        &hi2c,
        devAddr,
        register_address,
        2,
        data,
        2,
        100
    );
}

int32_t MLX90632_addressed_reset(
    MLX90632* const me
)
{
    int32_t ret;
    uint16_t reg_ctrl;
    uint16_t reg_value;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_REG_CTRL,
        &reg_value
    );
//    if (ret < 0)
//        return ret;

    reg_ctrl = reg_value & ~MLX90632_CFG_PWR_MASK;
    reg_ctrl |= MLX90632_PWR_STATUS_STEP;
    ret = MLX90632_i2c_write(
        me,
        MLX90632_REG_CTRL,
        reg_ctrl
    );
    if (ret < 0)
        return ret;

    ret = MLX90632_i2c_write(
        me,
        0x3005,
        MLX90632_RESET_CMD
    );
//    if (ret < 0)
//        return ret;

    usleep(150, 200);

    ret = MLX90632_i2c_write(
        me,
        MLX90632_REG_CTRL, reg_value
    );

    return ret;
}

int MLX90632_start_measurement(
    MLX90632* const me
)
{
    int ret, tries = MLX90632_MAX_NUMBER_MESUREMENT_READ_TRIES;
    uint16_t reg_status;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_REG_STATUS,
        &reg_status
    );

    if (ret < 0)
        return ret;

    ret = MLX90632_i2c_write(
        me,
        MLX90632_REG_STATUS,
        reg_status & (~MLX90632_STAT_DATA_RDY)
    );

    if (ret < 0)
        return ret;

    while (tries --> 0)
    {
        ret = MLX90632_i2c_read32(
            me,
            MLX90632_REG_STATUS,
            &reg_status
        );

        if (ret < 0)
            return ret;
        if (reg_status && MLX90632_STAT_DATA_RDY)
            break;

        usleep(10000, 11000);
    }

    if (tries < 0)
        return -ETIMEDOUT;

    return (reg_status & MLX90632_STAT_CYCLE_POS) >> 2;
}

int32_t MLX90632_read_temp_ambient_raw_extended(
    MLX90632* const me
)
{
    int32_t ret;
    uint16_t read_temp;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_RAM_3(17),
        &read_temp
    );

    if (ret < 0)
        return ret;

    me->ambient_new_raw = (int16_t)read_temp;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_RAM_3(18),
        &read_temp
    );

    if (ret < 0)
        return ret;

    me->ambient_old_raw = (int16_t)read_temp;

    return ret;
}

int32_t mlx90632_read_temp_object_raw_extended(
    MLX90632* const me
)
{
    int32_t ret;
    uint16_t read_tmp;
    int32_t read;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_RAM_1(17),
        &read_tmp
    );

    if (ret < 0)
        return ret;

    read = (int16_t)read_tmp;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_RAM_2(17),
        &read_tmp
    );

    if (ret < 0)
        return ret;

    read = read - (int16_t)read_tmp;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_RAM_1(18),
        &read_tmp
    );

    if (ret < 0)
        return ret;

    read = read - (int16_t)read_tmp;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_RAM_2(18),
        &read_tmp
    );

    if (ret < 0)
        return ret;

    read = (read + (int16_t)read_tmp) / 2;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_RAM_1(19),
        &read_tmp
    );

    if (ret < 0)
        return ret;

    read = read + (int16_t)read_tmp;

    ret = MLX90632_i2c_read(
        me,
        MLX90632_RAM_2(19),
        &read_tmp
    );

    if (ret < 0)
        return ret;

    read = read + (int16_t)read_tmp;

    if (read > 32767 || read < -32768)
        return -EINVAL;

    me->object_new_raw = (int16_t)read;

    return ret;
}

double mlx90632_preprocess_temp_ambient_extended(
    MLX90632* const me
)
{
    double VR_Ta, kGb;

    kGb = ((double)me->Gb) / 1024.0;

    VR_Ta = me->ambient_old_raw + kGb *
        (me->ambient_new_raw / (MLX90632_REF_3));
    return ((me->ambient_new_raw / (MLX90632_REF_3)) / VR_Ta) * 524288.0;
}

double MLX90632_calc_temp_object_iteration_extended(
    MLX90632* const me,
    double prev_object_temp,
    double TAdut,
    double TaTr4,
    double emissivity
)
{
    double calcedGa, calcedGb, calcedFa, first_sqrt;
    // temp variables
    double KsTAtmp, Alpha_corr;
    double Ha_customer, Hb_customer;

    Ha_customer = me->Ha / ((double)16384.0);
    Hb_customer = me->Hb / ((double)1024.0);
    calcedGa = ((double)me->Ga * (prev_object_temp - 25)) / ((double)68719476736.0);
    KsTAtmp = (double)me->Fb * (TAdut - 25);
    calcedGb = KsTAtmp / ((double)68719476736.0);
    Alpha_corr = (((double)((me->Fa / 2) * POW10)) * Ha_customer * (double)(1.0 + calcedGa + calcedGb)) /
                 ((double)70368744177664.0);
    calcedFa = me->object / (emissivity * (Alpha_corr / POW10));

    first_sqrt = sqrt(calcedFa + TaTr4);

    return sqrt(first_sqrt) - 273.15 - Hb_customer;
}

double mlx90632_calc_temp_object(
	MLX90632* const me
)
{
    double kEa, kEb, TAdut;
    double temp = 25.0;
    double tmp_emi = mlx90632_get_emissivity();
    int8_t i;

    kEa = ((double)me->Ea) / ((double)65536.0);
    kEb = ((double)me->Eb) / ((double)256.0);
    TAdut = (((double)me->ambient) - kEb) / kEa + 25;

    //iterate through calculations
    for (i = 0; i < 5; ++i)
        temp = mlx90632_calc_temp_object_iteration(temp, object, TAdut, Ga, Fa, Fb, Ha, Hb, tmp_emi);

    return temp;
}

void MLX90632_preprocess_temp_ambient(
	MLX90632* const me
)
{
    double VR_Ta, kGb;

    kGb = ((double)me->Gb) / 1024.0;

    VR_Ta = me->ambient_old_raw + kGb * (me->ambient_new_raw / (MLX90632_REF_3));

    me->ambient = ((me->ambient_new_raw / (MLX90632_REF_3)) / VR_Ta) * 524288.0;
}

void MLX90632_standard_mode_measure(
		MLX90632* const me,
		double* output
)
{
	HAL_StatusTypeDef result = HAL_I2C_IsDeviceReady(&hi2c1, 0x3a << 1, 1, 100);
	if (result != HAL_OK) {
		*output = -273.3;
		return;
	}

//	mlx90632_set_meas_type(MLX90632_MTYP_MEDICAL, hi2c1);

    MLX90632_read_eeprom(me);

	MLX90632_read_temp_raw(me);

	pre_ambient = MLX90632_preprocess_temp_ambient(me);
	pre_object = MLX90632_preprocess_temp_object(me);

	MLX90632_set_emissivity(1);

	MLX90632_calc_temp_ambient(me);

	object = MLX90632_calc_temp_object(me);

	float_temp_to_char_temp(object, char_temp_1);
}

void MLX90632_extended_mode_measure()
{
	HAL_StatusTypeDef result = HAL_I2C_IsDeviceReady(&hi2c1, 0x3a << 1, 1, 100);
	if (result == HAL_OK) {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	} else {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}

    mlx90632_set_meas_type(MLX90632_MTYP_EXTENDED, hi2c1);

    mlx90632_read_eeprom(&PR, &PG, &PO, &PT, &Ea, &Eb, &Fa, &Fb, &Ga, &Gb, &Ha, &Hb, &Ka, hi2c1);

    while(1)
    {
    	mlx90632_set_emissivity(0.5);

    	mlx90632_read_temp_raw_extended(&ambient_new_raw, &ambient_old_raw, &object_new_raw, hi2c1);

        ambient = mlx90632_calc_temp_ambient_extended(ambient_new_raw, ambient_old_raw,
                                                      PT, PR, PG, PO, Gb);

        pre_ambient = mlx90632_preprocess_temp_ambient_extended(ambient_new_raw,
                                                                       ambient_old_raw, Gb);
        pre_object = mlx90632_preprocess_temp_object_extended(object_new_raw, ambient_new_raw,
                                                                     ambient_old_raw, Ka);

        object = mlx90632_calc_temp_object_extended(pre_object, pre_ambient, ambient, Ea, Eb, Ga, Fa, Fb, Ha, Hb);

        float_temp_1 = object;
        float_temp_to_char_temp(float_temp_1, char_temp_1);

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

//            CDC_Transmit_FS(cap_mess_2, strlen(cap_mess_2));
//            CDC_Transmit_FS((uint8_t*)char_temp_2, strlen((uint8_t*)char_temp_2));
//            CDC_Transmit_FS(end, strlen(end));
#  		    endif
        }

#       ifdef SSD1306_DISPLAY
        SSD1306_GotoXY(0, 29);
        SSD1306_Puts(char_temp_1, &Font_11x18, 1);

        SSD1306_GotoXY(70, 29);
        SSD1306_Puts(char_temp_2, &Font_11x18, 1);

        SSD1306_UpdateScreen();
#       endif
    }
}

void MLX90632_extended_burst_mode_measure()
{
	HAL_StatusTypeDef result = HAL_I2C_IsDeviceReady(&hi2c1, 0x3a << 1, 1, 100);
	if (result == HAL_OK) {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	} else {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}

    mlx90632_set_meas_type(MLX90632_MTYP_EXTENDED_BURST, hi2c1);

    mlx90632_read_eeprom(&PR, &PG, &PO, &PT, &Ea, &Eb, &Fa, &Fb, &Ga, &Gb, &Ha, &Hb, &Ka, hi2c1);

    while(1)
    {
    	mlx90632_set_emissivity(1);

        mlx90632_read_temp_raw_extended_burst(&ambient_new_raw, &ambient_old_raw, &object_new_raw, hi2c1);

        ambient = mlx90632_calc_temp_ambient_extended(ambient_new_raw, ambient_old_raw,
                                                      PT, PR, PG, PO, Gb);

        pre_ambient = mlx90632_preprocess_temp_ambient_extended(ambient_new_raw,
                                                                       ambient_old_raw, Gb);
        pre_object = mlx90632_preprocess_temp_object_extended(object_new_raw, ambient_new_raw,
                                                                     ambient_old_raw, Ka);

        object = mlx90632_calc_temp_object_extended(pre_object, pre_ambient, ambient, Ea, Eb, Ga, Fa, Fb, Ha, Hb);

        float_temp_1 = object;
        float_temp_to_char_temp(float_temp_1, char_temp_1);

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
#  		    endif
        }

#       ifdef SSD1306_DISPLAY
        SSD1306_GotoXY(0, 29);
        SSD1306_Puts(char_temp_1, &Font_11x18, 1);

        SSD1306_GotoXY(70, 29);
        SSD1306_Puts(char_temp_2, &Font_11x18, 1);

        SSD1306_UpdateScreen();
#       endif
    }
}








