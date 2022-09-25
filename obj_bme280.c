/*!
 * \file obj_bme280.c
 * \brief humidity pressure temperature sensor device code
 * \author Adam LUCAS
 * \date 13/01/2022
 */
 
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_bme280.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn static void obj_bme280_get_temp_dig(obj_bme280_data *data_obj);
* \brief get temperature callibration values
* \param[in] data_obj object oriented parameters
* \return compensated value
*/

static void obj_bme280_get_temp_dig(obj_bme280_data *data_obj) {
	
	uint8_t t_dig_data[] = {
		data_obj->address,
		0x00
	};
	
	for(uint8_t i = 0; i < sizeof(obj_bme280_dig_T_desc); i++) {
		
		data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_CAL_0 + i,
			t_dig_data, 
			sizeof(t_dig_data)
		);
			
		data_obj->dig_T.array[i] = t_dig_data[1];
	}
}

//*****************************************************************************/
/*!
* \fn static void obj_bme280_get_pression_dig(obj_bme280_data *data_obj);
* \brief get pression callibration values
* \param[in] data_obj object oriented parameters
* \return None
*/

static void obj_bme280_get_pression_dig(obj_bme280_data *data_obj) {
	
	uint8_t p_dig_data[] = {
		data_obj->address,
		0x00
	};
	
	for(uint8_t i = 0; i < sizeof(obj_bme280_dig_P_desc); i++) {
		
		data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_CAL_0 + i + 6,
			p_dig_data, 
			sizeof(p_dig_data)
		);
			
		data_obj->dig_P.array[i] = p_dig_data[1];
	}
}

//*****************************************************************************/
/*!
* \fn static void obj_bme280_get_humid_dig(obj_bme280_data *data_obj);
* \brief get humidity callibration values
* \param[in] data_obj object oriented parameters
* \return None
*/

static void obj_bme280_get_humid_dig(obj_bme280_data *data_obj) {
	
	uint8_t p_dig_data[] = {
		data_obj->address,
		0x00
	};
		
	data_obj->flag = data_obj->i2c_read(
		data_obj->handle, 
		0xA1,
		p_dig_data, 
		sizeof(p_dig_data)
	);
		
	data_obj->dig_H.desc.dig_H1 = p_dig_data[1];
	
	data_obj->flag = data_obj->i2c_read(
		data_obj->handle, 
		0xE1,
		p_dig_data, 
		sizeof(p_dig_data)
	);
		
	data_obj->dig_H.desc.dig_H2 = 0;
	data_obj->dig_H.desc.dig_H2 = p_dig_data[1];
		
	data_obj->flag = data_obj->i2c_read(
		data_obj->handle, 
		0xE2,
		p_dig_data, 
		sizeof(p_dig_data)
	);
		
	data_obj->dig_H.desc.dig_H2 |= p_dig_data[1]<<8;
	
	data_obj->flag = data_obj->i2c_read(
		data_obj->handle, 
		0xE3,
		p_dig_data, 
		sizeof(p_dig_data)
	);
	
	data_obj->dig_H.desc.dig_H3 = p_dig_data[1];
		
	data_obj->flag = data_obj->i2c_read(
		data_obj->handle, 
		0xE4,
		p_dig_data, 
		sizeof(p_dig_data)
	);
	
	data_obj->dig_H.desc.dig_H4 = 0;
	data_obj->dig_H.desc.dig_H4 |= (p_dig_data[1]<<4);
		
	data_obj->flag = data_obj->i2c_read(
		data_obj->handle, 
		0xE5,
		p_dig_data, 
		sizeof(p_dig_data)
	);
	
	data_obj->dig_H.desc.dig_H5 = 0;
		
	data_obj->dig_H.desc.dig_H4 |= (p_dig_data[1] & 0x0F);
	data_obj->dig_H.desc.dig_H5 |= ((p_dig_data[1] & 0xF0)>>4);
		
		
	data_obj->flag = data_obj->i2c_read(
		data_obj->handle, 
		0xE6,
		p_dig_data, 
		sizeof(p_dig_data)
	);
	
	data_obj->dig_H.desc.dig_H5 |= (p_dig_data[1]<<4);
		
	data_obj->flag = data_obj->i2c_read(
		data_obj->handle, 
		0xE7,
		p_dig_data, 
		sizeof(p_dig_data)
	);
	
	data_obj->dig_H.desc.dig_H6 = (int8_t)p_dig_data[1];
}

//*****************************************************************************/
/*!
* \fn static int32_t obj_bme280_compensate_temp(obj_bme280_data *data_obj, int32_t val);
* \brief get compensated temperature value from device ADC values
* \param[in] data_obj object oriented parameters
* \param[in] val device temperature raw value in ADC resolution unit
* \return compensated value
*/

static int32_t obj_bme280_compensate_temp(obj_bme280_data *data_obj, int32_t val) {
	int32_t var1, var2, T;
	var1 = (
			(
					(((uint32_t)val>>3) - ((uint32_t)data_obj->dig_T.desc.dig_T1<<1))
			) * 
			((uint32_t)data_obj->dig_T.desc.dig_T2)
	) >> 11;
	
	var2 = (
			(
					((((uint32_t)val>>4) - ((uint32_t)data_obj->dig_T.desc.dig_T1)) * 
					(((uint32_t)val>>4) - ((uint32_t)data_obj->dig_T.desc.dig_T1))
			)>>12 * 
			((uint32_t)data_obj->dig_T.desc.dig_T3)))>>14;
	data_obj->t_fine = var1 + var2;
	T = (data_obj->t_fine * 5 + 128)>>8;
	
	return T;
}

//*****************************************************************************/
/*!
* \fn static uint32_t obj_bme280_compensate_pression(obj_bme280_data *data_obj, int32_t val);
* \brief get compensated pression value from device ADC values
* \param[in] data_obj object oriented parameters
* \param[in] val device pression raw value in ADC resolution unit
* \return compensated value
*/

static uint32_t obj_bme280_compensate_pression(obj_bme280_data *data_obj, int32_t val) {
	int64_t var1, var2, P;
	
	var1 = ((int64_t)data_obj->t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)data_obj->dig_P.desc.dig_P6;
	var2 = var2 + ((var1*(int64_t)data_obj->dig_P.desc.dig_P5)<<17);
	var2 = var2 + (((int64_t)data_obj->dig_P.desc.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)data_obj->dig_P.desc.dig_P3)>>8) + ((var1 * (int64_t)data_obj->dig_P.desc.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)data_obj->dig_P.desc.dig_P1)>>33;
	if(var1 == 0) {
		return 0;
	}
	P = 1048576 - val;
	P = (((P<<31)-var2)*3125)/var1;
	var1 = (((int64_t)data_obj->dig_P.desc.dig_P9) * (P>>13) * (P>>13)) >>25;
	var2 = (((int64_t)data_obj->dig_P.desc.dig_P8) * P)>>19;
	P = ((P + var1 + var2)>>8) + (((int64_t)data_obj->dig_P.desc.dig_P7)<<4);
	return (uint32_t)P;
}

//*****************************************************************************/
/*!
* \fn static uint32_t bme280_compensate_humid(obj_bme280_data *data_obj, int32_t val);
* \brief get compensated humidity value from device ADC values
* \param[in] data_obj object oriented parameters
* \param[in] val device humidity raw value in ADC resolution unit
* \return compensated value
*/

static uint32_t bme280_compensate_humid(obj_bme280_data *data_obj, int32_t val) {
	int32_t v_x1u32_r;
	
	v_x1u32_r = (data_obj->t_fine - ((int32_t)76800));
	v_x1u32_r = (((((val<<14) - (((int32_t)data_obj->dig_H.desc.dig_H4)<<20) - (((int32_t)data_obj->dig_H.desc.dig_H5) *
	v_x1u32_r)) + ((int32_t)16384))>>15) * (((((((v_x1u32_r *
	((int32_t)data_obj->dig_H.desc.dig_H6))>>10) * (((v_x1u32_r * ((int32_t)data_obj->dig_H.desc.dig_H3))>>11) +
	((int32_t)32768)))>>10) + ((int32_t)2097152)) * ((int32_t)data_obj->dig_H.desc.dig_H2) +
	8192)>>14));
	v_x1u32_r = (v_x1u32_r - (((((v_x1u32_r>>15) * (v_x1u32_r>>15))>>7) *
	((int32_t)data_obj->dig_H.desc.dig_H1))>>4));
	v_x1u32_r = (v_x1u32_r < 0 ? 0 : v_x1u32_r);
	v_x1u32_r = (v_x1u32_r > 419430400 ? 419430400 : v_x1u32_r);
	return (uint32_t)(v_x1u32_r>>12);
}

//*****************************************************************************/
/*!
* \fn uint8_t obj_bme280_get_id(obj_bme280_data *data_obj);
* \brief get device ID routine
* \param[in] data_obj object oriented parameters
* \return device ID
*/

uint8_t obj_bme280_get_id(obj_bme280_data *data_obj) {
	
	uint8_t id_data[] = {
		data_obj->address,
		0x00
	};
	
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_ID,
			id_data, 
			sizeof(id_data)
	);
	
	return id_data[1];
}

//*****************************************************************************/
/*!
* \fn void obj_bme280_start(obj_bme280_data *data_obj, obj_bme280_start_mode start_mode);
* \brief device start measuring routine
* \param[in] data_obj object oriented parameters
* \param[in] start device start in sleep, forced or normal mode
* \return None
*/

void obj_bme280_start(obj_bme280_data *data_obj, obj_bme280_start_mode start_mode) {
	
	uint8_t init_data[] = {
		data_obj->address,
		OBJ_BME280_REG_CTRL_HUM,
		(uint8_t)(data_obj->hum_enable<<0),
		OBJ_BME280_REG_CTRL_MEAS,
		(
				(uint8_t)(data_obj->pres_enable<<5) |
				(uint8_t)(data_obj->temp_enable<<2) |
				(uint8_t)(start_mode<<0)
		),
		OBJ_BME280_REG_CONFIG,
		(uint8_t)(data_obj->period<<5),
	};
	
	data_obj->flag = data_obj->i2c_write(
			data_obj->handle, 
			init_data, 
			sizeof(init_data)
	);
	if(obj_bme280_get_flag(data_obj) == OBJ_BME280_OK)data_obj->start_mode = start_mode;
}

//*****************************************************************************/
/*!
* \fn void obj_bme280_reset(obj_bme280_data *data_obj);
* \brief device software reset routine
* \param[in] data_obj object oriented parameters
* \return None
*/

void obj_bme280_reset(obj_bme280_data *data_obj) {
	
	uint8_t reset_data[] = {
		data_obj->address,
		OBJ_BME280_REG_RESET,
		0xB6,
	};
	
	data_obj->flag = data_obj->i2c_write(
			data_obj->handle, 
			reset_data, 
			sizeof(reset_data)
	);
}

//*****************************************************************************/
/*!
* \fn void obj_bme280_init(obj_bme280_data *data_obj);
* \brief device init routine. Modify in data_obj temp_enable, hum_enable and pres_enable parameters
* \param[in] data_obj object oriented parameters
* \return None
*/

void obj_bme280_init(obj_bme280_data *data_obj) {
			
	obj_bme280_data init_temp = *data_obj; 	//copy user defined object into temporary object
	init_temp.temp_enable = OBJ_BME280_MEASURING_1X_OVERSAMPLING; //force temperature measuring
	obj_bme280_start(&init_temp, OBJ_BME280_START_FORCED); //start device with temporary object to get t_fine value
	init_temp.device_init = true;
	obj_bme280_get_temp_dig(&init_temp);		//get all callibration registers contents
	obj_bme280_get_pression_dig(&init_temp);
	obj_bme280_get_humid_dig(&init_temp);
	
	obj_bme280_get_temp(&init_temp); 				//temperature measurement required to update t_fine
	data_obj->t_fine = init_temp.t_fine; 		//update t_fine variable and callibration vairables to the user object
	data_obj->dig_H = init_temp.dig_H;		
	data_obj->dig_T = init_temp.dig_T;		
	data_obj->dig_P = init_temp.dig_P;		
	
	//obj_bme280_reset(data_obj);							//software reset device
	obj_bme280_start(data_obj, false); 			//reinit device considering user parameters
			
	if(data_obj->flag == OBJ_BME280_OK) data_obj->device_init = true; //if no errors detected, device init flag returned true
	
}

//*****************************************************************************/
/*!
* \fn double obj_bme280_get_temp(obj_bme280_data *data_obj);
* \brief device get temperature routine
* \param[in] data_obj object oriented parameters
* \return temperature in degrees celsius
*/

double obj_bme280_get_temp(obj_bme280_data *data_obj) {
	
	if(
			!data_obj->device_init || 
			(data_obj->temp_enable == OBJ_BME280_MEASURING_DISABLED) || 
			(data_obj->start_mode == OBJ_BME280_SLEEP)
	) {
		data_obj->flag = OBJ_BME280_TEMP_DISABLED;
		return 0;
	}
	
	uint8_t temp_data[] = {
		data_obj->address,
		0x00
	};
	
	int32_t conv_temp = 0;
	
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_TEMP_0,
			temp_data, 
			sizeof(temp_data)
	);
	
	conv_temp |= (uint32_t)(temp_data[1]<<12);
			
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_TEMP_1,
			temp_data, 
			sizeof(temp_data)
	);	
	
	conv_temp |= (uint32_t)(temp_data[1]<<4);
			
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_TEMP_2,
			temp_data, 
			sizeof(temp_data)
	);
	
	conv_temp |= (uint32_t)(temp_data[1]>>4);
	
	if(
			data_obj->start_mode == OBJ_BME280_START_FORCED && 
			data_obj->flag == OBJ_BME280_OK
	) data_obj->start_mode = OBJ_BME280_SLEEP;
			
	return obj_bme280_compensate_temp(data_obj, conv_temp) / 100.0;
}

//*****************************************************************************/
/*!
* \fn double obj_bme280_get_pres(obj_bme280_data *data_obj);
* \brief device get pression routine
* \param[in] data_obj object oriented parameters
* \return pression in hectoPascal
*/

double obj_bme280_get_pres(obj_bme280_data *data_obj) {
	
	if(
			!data_obj->device_init || 
			(data_obj->pres_enable == OBJ_BME280_MEASURING_DISABLED) || 
			(data_obj->start_mode == OBJ_BME280_SLEEP)
	) {
		data_obj->flag = OBJ_BME280_PRES_DISABLED;
		return 0;
	}
	
	uint8_t pres_data[] = {
		data_obj->address,
		0x00
	};
	
	uint32_t conv_temp = 0;
	
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_PRESS_0,
			pres_data, 
			sizeof(pres_data)
	);
	
	conv_temp |= (uint32_t)(pres_data[1]<<12);
	
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_PRESS_1,
			pres_data, 
			sizeof(pres_data)
	);
	
	conv_temp |= (uint32_t)(pres_data[1]<<4);
			
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_PRESS_2,
			pres_data, 
			sizeof(pres_data)
	);	
	
	conv_temp |= (uint32_t)(pres_data[1]>>4);
			
	if(
			data_obj->start_mode == OBJ_BME280_START_FORCED && 
			data_obj->flag == OBJ_BME280_OK
	) data_obj->start_mode = OBJ_BME280_SLEEP;
			
	return obj_bme280_compensate_pression(data_obj, (int32_t)conv_temp) / 25600.0;
}

//*****************************************************************************/
/*!
* \fn double obj_bme280_get_hum(obj_bme280_data *data_obj);
* \brief device get humidity routine
* \param[in] data_obj object oriented parameters
* \return humidity in percent
*/

double obj_bme280_get_hum(obj_bme280_data *data_obj) {
	
	if(
			!data_obj->device_init || 
			(data_obj->hum_enable == OBJ_BME280_MEASURING_DISABLED) || 
			(data_obj->start_mode == OBJ_BME280_SLEEP)
	) {
		data_obj->flag = OBJ_BME280_HUM_DISABLED;
		return 0;
	}
	
	uint8_t hum_data[] = {
		data_obj->address,
		0x00
	};
	uint16_t conv_temp = 0;
	
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_HUM_MSB,
			hum_data, 
			sizeof(hum_data)
	);
	
	conv_temp |= (hum_data[1]<<8);
	
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_HUM_LSB,
			hum_data, 
			sizeof(hum_data)
	);
	
	conv_temp |= (hum_data[1]<<0);
			
	if(
			data_obj->start_mode == OBJ_BME280_START_FORCED && 
			data_obj->flag == OBJ_BME280_OK
	) data_obj->start_mode = OBJ_BME280_SLEEP;
			
	return bme280_compensate_humid(data_obj, (int32_t)conv_temp) / 1024.0;
}

//*****************************************************************************/
/*!
* \fn bool obj_bme280_is_ready(obj_bme280_data *data_obj);
* \brief check if device is ready to get values
* \param[in] data_obj object oriented parameters
* \return true if done false else
*/

bool obj_bme280_is_ready(obj_bme280_data *data_obj) {
	
	if(!data_obj->device_init || (data_obj->start_mode == OBJ_BME280_SLEEP)) {
		data_obj->flag = OBJ_BME280_ERROR;
		return false;
	}
	
	uint8_t data[] = {
		data_obj->address,
		0x00
	};
	
	data_obj->flag = data_obj->i2c_read(
			data_obj->handle, 
			OBJ_BME280_REG_STATUS,
			data, 
			sizeof(data)
	);
			
	return !(data[1] & (1<<3)) ? true : false;
}

//*****************************************************************************/
/*!
* \fn obj_bme280_flag obj_bme280_get_flag(obj_bme280_data *data_obj);
* \brief get all routines error flag
* \param[in] data_obj object oriented parameters
* \return OBJ_BME280_OK if done OBJ_BME280_ERROR else
*/

obj_bme280_flag obj_bme280_get_flag(obj_bme280_data *data_obj) {
	return data_obj->flag;
}
