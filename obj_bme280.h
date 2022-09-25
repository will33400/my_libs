/*!
 * \file obj_bme280.h
 * \brief humidity pressure temperature sensor device header
 * \author Adam LUCAS
 * \date 13/01/2022
 */
 
#ifndef OBJ_BME280_H_
#define OBJ_BME280_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#define OBJ_BME280_REG_CAL_0								0x88
/*
0, 1..25
*/
#define OBJ_BME280_REG_CAL_25								0xA1
#define OBJ_BME280_REG_ID										0xD0
#define OBJ_BME280_REG_RESET								0xE0
#define OBJ_BME280_REG_CAL_26								0xE1
/*
26, 27..41
*/
#define OBJ_BME280_REG_CAL_41								0xF0

#define OBJ_BME280_REG_CTRL_HUM							0xF2
#define OBJ_BME280_REG_CTRL_HUM_MASK				0x03

#define OBJ_BME280_REG_STATUS								0xF3

#define OBJ_BME280_REG_CTRL_MEAS						0xF4

#define OBJ_BME280_REG_CONFIG								0xF5

#define OBJ_BME280_REG_PRESS_0							0xF7
#define OBJ_BME280_REG_PRESS_1							0xF8
#define OBJ_BME280_REG_PRESS_2							0xF9
#define OBJ_BME280_REG_TEMP_0								0xFA
#define OBJ_BME280_REG_TEMP_1								0xFB
#define OBJ_BME280_REG_TEMP_2								0xFC
#define OBJ_BME280_REG_HUM_MSB							0xFD
#define OBJ_BME280_REG_HUM_LSB							0xFE


#define OBJ_BME280_CONV_DEGREE_KELVIN(degree)	(degree - 273.0)
#define OBJ_BME280_CONV_DEGREE_FAHREN(degree) ((degree * 1.8) + 32.0)

#define OBJ_BME280_CONV_HPA_PA(hectoPascal)	(hectoPascal * 100.0)
#define OBJ_BME280_CONV_HPA_BAR(hectoPascal)(hectoPascal / 100000.0)


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum obj_bme280_address_sel
 * \brief device address selection. Check if SDO pin of your device is tied to GND (0x76) or to VCC (0x77)
 */
 
typedef enum {
	OBJ_BME280_ADDR0 = (0x76<<1),
	OBJ_BME280_ADDR1 = (0x77<<1),
}obj_bme280_address_sel;

/*!
 * \enum obj_bme280_measure_mode
 * \brief measuring characteristic selection. to enable to default mode use OBJ_BME280_MEASURING_1X_OVERSAMPLING else OBJ_BME280_MEASURING_DISABLED
 */

typedef enum {
	OBJ_BME280_MEASURING_DISABLED,
	OBJ_BME280_MEASURING_1X_OVERSAMPLING,
	OBJ_BME280_MEASURING_2X_OVERSAMPLING,
	OBJ_BME280_MEASURING_4X_OVERSAMPLING,
	OBJ_BME280_MEASURING_8X_OVERSAMPLING,
	OBJ_BME280_MEASURING_16X_OVERSAMPLING,
}obj_bme280_measure_mode;

/*!
 * \enum obj_bme280_start_mode
 * \brief sensor start mode
 */

typedef enum {
	OBJ_BME280_SLEEP = 0,				//!< sleep mode. Consumption greatly reduced but cannot measuring
	OBJ_BME280_START_FORCED = 1,//!< forced mode. Get any measure once before returning in sleep mode
	OBJ_BME280_START_NORMAL = 3,//!< normal mode. The sensor is contantly working even if it gets measuring requests
}obj_bme280_start_mode;

/*!
 * \enum obj_bme280_flag
 * \brief return flag for all protocol transfers
 */

typedef enum {
	OBJ_BME280_OK,						//!< transfers proceed correctly
	OBJ_BME280_TEMP_DISABLED, //!< attempting to use temperature measurement without initializing it first
	OBJ_BME280_PRES_DISABLED, //!< attempting to use pressure measurement without initializing it first
	OBJ_BME280_HUM_DISABLED,	//!< attempting to use humidity measurement without initializing it first
	OBJ_BME280_ERROR,					//!< any transfer returned an error
}obj_bme280_flag;

/*!
 * \enum obj_bme280_meas_period
 * \brief device internal refresh measure period
 */

typedef enum {
	OBJ_BME280_PERIOD_0_5MS,
	OBJ_BME280_PERIOD_62_5MS,
	OBJ_BME280_PERIOD_125MS,
	OBJ_BME280_PERIOD_250MS,
	OBJ_BME280_PERIOD_500MS,
	OBJ_BME280_PERIOD_1000MS,
	OBJ_BME280_PERIOD_10MS,
	OBJ_BME280_PERIOD_20MS,
}obj_bme280_meas_period;

/*!
 * \struct obj_bme280_dig_T_desc
 * \brief device temperature callibration registers descriptive
 */

typedef struct {
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
}obj_bme280_dig_T_desc;

/*!
 * \struct obj_bme280_dig_P_desc
 * \brief device pression callibration registers descriptive
 */

typedef struct {
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
}obj_bme280_dig_P_desc;

/*!
 * \struct obj_bme280_dig_H_desc
 * \brief device humidity callibration registers descriptive
 */

typedef struct {
	uint8_t dig_H1;
	int16_t dig_H2;
	uint8_t dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;
	int8_t dig_H6;
}obj_bme280_dig_H_desc;

/*!
 * \union obj_bme280_dig_T
 * \brief device temperature callibration registers array/descriptive type
 */

typedef union {
	obj_bme280_dig_T_desc desc;
	uint8_t array[sizeof(obj_bme280_dig_T_desc)];
}obj_bme280_dig_T;

/*!
 * \union obj_bme280_dig_P
 * \brief device pression callibration registers array/descriptive type
 */

typedef union {
	obj_bme280_dig_P_desc desc;
	uint8_t array[sizeof(obj_bme280_dig_P_desc)];
}obj_bme280_dig_P;

/*!
 * \union obj_bme280_dig_H
 * \brief device humidity callibration registers array/descriptive type
 */

typedef union {
	obj_bme280_dig_H_desc desc;
	uint8_t array[sizeof(obj_bme280_dig_H_desc)];
}obj_bme280_dig_H;

/*!
 * \fn obj_bme280_flag(* obj_bme280_i2c_write)(void *handle, uint8_t *data, uint16_t len);
 * \brief user defined function pointer to write through I2C
 * \param[in] handle user defined argument pointer
 * \param[in] data I2C data to transmit pointer
 * \param[in] len data length in bytes
 * \return flag indicating status. Refer to obj_bme280_flag
 */

typedef obj_bme280_flag(* obj_bme280_i2c_write)(void *handle, uint8_t *data, uint16_t len);

/*!
 * \fn obj_bme280_flag(* obj_bme280_i2c_write)(void *handle, uint8_t *data, uint16_t len);
 * \brief user defined function pointer to read through I2C
 * \param[in] handle user defined argument pointer
 * \param[in] reg device register location address
 * \param[out] data I2C data to receive containing slave address pointer
 * \param[in] len data length in bytes
 * \return flag indicating status. Refer to obj_bme280_flag
 */

typedef obj_bme280_flag(* obj_bme280_i2c_read_reg)(void *handle, uint8_t reg, uint8_t *data, uint16_t len);

/*!
 * \struct obj_bme280_data
 * \brief object oriented instance data
 */

typedef struct {
	obj_bme280_i2c_write i2c_write; 		//!< I2C write routine. Defined by user
	obj_bme280_i2c_read_reg i2c_read; 	//!< I2C receive data from slave memory location
	void *handle;												//!< user defined data for i2c_write and i2c_read routines
	obj_bme280_meas_period period;			//!< device internal values acquirement period. Refer to obj_bme280_meas_period
	obj_bme280_measure_mode temp_enable;//!< enable temperature measurement for obj_bme280_init() routine
	obj_bme280_measure_mode hum_enable;	//!< enable humidity measurement for obj_bme280_init() routine
	obj_bme280_measure_mode pres_enable;//!< enable pression measurement for obj_bme280_init() routine
	obj_bme280_address_sel address;			//!< device address selection. Refer to obj_bme280_address_sel
	bool device_init;										//!< DO NOT MODIFY use obj_bme280_init() instead
	obj_bme280_start_mode start_mode;		//!< DO NOT MODIFY use obj_bme280_start() instead
	obj_bme280_flag flag;								//!< DO NOT MODIFY use obj_bme280_get_flag() instead
	obj_bme280_dig_T dig_T;							//!< DO NOT MODIFY
	obj_bme280_dig_P dig_P;							//!< DO NOT MODIFY
	obj_bme280_dig_H dig_H;							//!< DO NOT MODIFY
	int32_t t_fine;											//!< DO NOT MODIFY
}obj_bme280_data;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

uint8_t obj_bme280_get_id(obj_bme280_data *data_obj);
void obj_bme280_start(obj_bme280_data *data_obj, obj_bme280_start_mode start_mode);
void obj_bme280_reset(obj_bme280_data *data_obj);
void obj_bme280_init(obj_bme280_data *data_obj);
double obj_bme280_get_temp(obj_bme280_data *data_obj);
double obj_bme280_get_pres(obj_bme280_data *data_obj);
double obj_bme280_get_hum(obj_bme280_data *data_obj);
bool obj_bme280_is_ready(obj_bme280_data *data_obj);

obj_bme280_flag obj_bme280_get_flag(obj_bme280_data *data_obj);

#endif /*OBJ_BME280_H_*/

/************ THIS SECTION IS FOR DOCUMENTATION ONLY !**********************//**
 * \addtogroup OBJ_BME280
 * \brief Object oriented Temperature, pression and humidity sensor BME280
 * \{

   \details
   \li \ref bme280_intro
   \li \ref bme280_api_description
   \li \ref bme280_data_types
	 \li \ref bme280_macros
	 \li \ref bme280_routines
	 \li \ref bme280_example

   # Introduction {#bme280_intro}
	 
	 This driver handles almost all specifications required to use the BME280 except :
	 
	 -# 3/4Wire SPI interface mode
	 -# oversampling frequency delay handling
	 
	 (these specifications will be implemented in further versions)
	 
	 This sensor is able measure three weather caracteristics :
	 
	 -# Temperature in celsius degrees (0.01 resolution)
	 -# Pressure in hPa (24 integer bits and 8 fractional bits resolution)
	 -# Humidity in percent (22 interger bits and 10 fractionl bitsresolution)
	 
	 Each caracteristics can be enabled from 1x to 16x oversampling or disabled
	 
	 Tree modes are available :
	 
	 -# Sleep mode (measuring disabled but low power consumption)
	 -# Forced mode (return in sleep mode after measuring request from driver)
	 -# Normal mode (always active)
	 
	 It is also possible to apply to the device internal measuring frequency from 0.5ms to 1s period
	 
	 The sensor 7-bits address LSB through I2C interface can vary by putting SDO pin to GND (0x76) or to VCC (0x77)
	 usefull to ensure 2 sensors into a same I2C network.
	 
   # API_Description {#bme280_api_description}
	 
	 This driver contains :
	 
	 -# get_id()
	 -# start()
	 -# reset()
	 -# init()
	 -# get_temp()
	 -# get_pres()
	 -# get_hum()
	 -# is_ready()
	 -# get_flag()
	 
	 these routines are described further in this document
	 
	 # Data_Types {#bme280_data_types}
	 
	 This driver contains these enums :
	 
	 -# address_sel
	 -# measure_mode
	 -# start_mode
	 -# flag
	 -# meas_period
	 
	 exept flag, all of these enums describes component address selection, mode disabled or enable with oversampling, 
	 start in sleep, forced or normal mode and internal measure period of the sensor.
	 
	 flag enum is useful du determine any error through I2C protocol or due to wrong initialisation
	 
	 These structs and unions are used to get callibration stored in the NVM of the sensor:
	 
	 -# obj_bme280_dig_T_desc
	 -# obj_bme280_dig_P_desc
	 -# obj_bme280_dig_H_desc
	 -# obj_bme280_dig_T
	 -# obj_bme280_dig_P
	 -# obj_bme280_dig_H
	 
	 obj_bme280_dig_X_desc structs describes memory mapping of the dig variables used for compensation formulas. Theses types and formulas are specified in datasheet.
	 obj_bme280_dig_X unions are usefull to attribute to a variable array and struct variables properties to simplify code affectation
	 
	 I2C interface mode functions pointers :
	 
	 -# obj_bme280_i2c_write
	 -# obj_bme280_i2c_read
	 
	 These functions have to be defined in user application before to be assigned in these variables.
	 ! data[] argument first element contains 7-bits slave address else are data contents to send or receive through I2C !
	 
	 # Macros {#bme280_macros}
	 
	 OBJ_BME280_REG_CAL_0 to OBJ_BME280_REG_HUM_LSB correspond to registers listed in datasheet
	 
	 These macros convert celsius into another unit :
	 
	 -# OBJ_BME280_CONV_DEGREE_KELVIN()
	 -# OBJ_BME280_CONV_DEGREE_FAHREN()
	 
	 These macros convert pressure in hPa into another unit :
	 
	 -# OBJ_BME280_CONV_HPA_PA()
	 -# OBJ_BME280_CONV_HPA_BAR()
	 
	 Example of use : double kelvin_var = OBJ_BME280_CONV_DEGREE_KELVIN(obj_bme280_get_temp(&device));
	 
	 # Routines {#bme280_routines}
	 
	 configuration routines :
	 
	 -# obj_bme280_start()
	 -# obj_bme280_reset()
	 -# obj_bme280_init()
	 
	 It is recommended to proceed as it's explained below :
	 
	 -# define a global variable that contains object contents obj_bme280_data and affect all desired parameters
	 -# use obj_bme280_reset() (glitches detected do not use if there are problems with sensor values)
	 -# use obj_bme280_init() to initialise correctly all variables and sensor (enters sleep mode by default)
	 -# in last use obj_bme280_start() to select start mode
	 
	 getters routines :
	 
	 -# obj_bme280_get_id()
	 -# obj_bme280_get_temp()
	 -# obj_bme280_get_pres()
	 -# obj_bme280_get_hum()
	 -# obj_bme280_is_ready()
	 -# obj_bme280_get_flag()
	 
	 obj_bme280_get_id() is used to get device internal ID (8-bits long)
	 obj_bme280_get_temp() is used to get device temperature in celsius degrees
	 obj_bme280_get_pres() is used to get device pressure value in hPa
	 obj_bme280_get_hum() is used to get device humidity concentration in percent
	 obj_bme280_is_ready() is used to check if the sensor has finished to get internal ADC values
	 obj_bme280_get_flag() is used to check protocol working state by getting error flags
	 
	 recommented use example for temperature (same for pressure or humidity) :
	 
	 -# first use obj_bme280_init() with OBJ_BME280_MEASURING_1X_OVERSAMPLING affected to temp_enable from obj_bme280_data variable
	 -# wait that the sensor is ready with : while(!obj_bme280_is_ready(&sensor));
	 -# then get temperature as this : double temp_celsius = obj_bme280_get_temp(&sensor);
	 
	 it is also possible to enable all caracteristics by affecting else than OBJ_BME280_MEASURING_DISABLED to 
	 temp_enable, pres_enable or hum_enable from obj_bme280_data typed variable.
	 
	 # Example {#bme280_Example}
	 
	 this example uses HAL functions from Keil. Refer to HAL I2C documentation
	 
	 #include "obj_bme280.h"
	 
	 static obj_bme280_flag i2c_write(void *handle, uint8_t *data, uint16_t len) {
			lo_weather_sensor_data *handle_temp = (lo_weather_sensor_data *)handle;
			if(HAL_I2C_Master_Transmit(&handle_temp->hi2c,  data[0], &data[1], len - 1, 2000) == HAL_OK) return OBJ_BME280_OK;
			return OBJ_BME280_ERROR;
		}
		
		static obj_bme280_flag i2c_read(void *handle, uint8_t reg, uint8_t *data, uint16_t len) {
			lo_weather_sensor_data *handle_temp = (lo_weather_sensor_data *)handle;
			if(HAL_I2C_Mem_Read(&handle_temp->hi2c, data[0], reg, I2C_MEMADD_SIZE_8BIT, &data[1], len - 1, 2000) == HAL_OK) return OBJ_BME280_OK;
			return OBJ_BME280_ERROR;
		}
		
		static I2C_HandleTypeDef hi2c = {
			.Instance = I2C1,
			.Init.ClockSpeed = 400000,
			.Init.DutyCycle = I2C_DUTYCYCLE_2,
			.Init.OwnAddress1 = 0x0,
			.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
			.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
			.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
			.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE,
		};
		
		
		static obj_bme280_data sensor = {
			.address = OBJ_BME280_ADDR0,
			.period = OBJ_BME280_PERIOD_500MS,
			.temp_enable = OBJ_BME280_MEASURING_1X_OVERSAMPLING,
			.pres_enable = OBJ_BME280_MEASURING_1X_OVERSAMPLING,
			.hum_enable = OBJ_BME280_MEASURING_1X_OVERSAMPLING,
			.handle = NULL,
			.i2c_read = i2c_read,
			.i2c_write = i2c_write,
		};
		
		double temp;
		double pres;
		double hum;
		
		int main(void) {
				__HAL_RCC_GPIOB_CLK_ENABLE();
				
				GPIO_InitTypeDef IOI2C;

				IOI2C.Pin = GPIO_PIN_8 | GPIO_PIN_7;
				IOI2C.Mode = GPIO_MODE_AF_OD;
				IOI2C.Pull = GPIO_PULLUP;
				IOI2C.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
				IOI2C.Alternate = GPIO_AF4_I2C1;
				
				HAL_GPIO_Init(GPIOB, &IOI2C);
				
				__HAL_RCC_I2C1_CLK_ENABLE();
				HAL_I2C_Init(&hi2c);
		
				obj_bme280_reset(&sensor);
				obj_bme280_init(&sensor);
				
				while(1) {
					
					while(!obj_bme280_is_ready(&sensor));
					
					temp = obj_bme280_get_temp(&sensor);
					pres = obj_bme280_get_pres(&sensor);
					hum = obj_bme280_get_hum(&sensor);
					
					//implement code
				
				}
		}
	 
 * \} end group obj_bme280 ****************************************************/
