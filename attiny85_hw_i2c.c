#if 0
/*!
 * \file lo_i2c.c
 * \brief low level i2c functions
 * \author Adam LUCAS
 * \date 8/1/2021 9:35:13 PM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "attiny85_hw_i2c.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

LO_I2C_DATA loI2cData;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn static void lo_i2c_master_gpio_get_state_sda_callback(
*		uint8_t pin,
*		bool state
* );
* \brief low level i2c sda gpio pin get state callback
* \param[in] pin pin number. refer to lo_gpio.h
* \param[in] state pin state (false : LOW, true : HIGH)
* \return None
*/

static void lo_i2c_master_gpio_get_state_sda_callback(uint8_t pin, bool state) {
	if(loI2cData.handle.gpio_sda->pin == pin) {
		OBJ_I2C_SDA_GET_STATE(state);
	}
}

//*****************************************************************************/
/*!
* \fn static uint8_t lo_i2c_master_start(void);
* \brief low level i2c master start any message routine
* \param None
* \return i2c start error message if start completes
*/

static uint8_t lo_i2c_master_start(void) {
	uint8_t delay_val = OBJ_I2C_DELAY_PERIOD(20);
	
	OBJ_I2C_SDA_LOW();
	
	_delay_us(delay_val);
	
	loI2cData.ctx.error_code = OBJ_I2C_MST_TR_START;
	
	return loI2cData.ctx.error_code;
}

//*****************************************************************************/
/*!
* \fn static uint8_t lo_i2c_master_restart(void);
* \brief low level i2c master restart any message routine
* \param None
* \return i2c start error message if start completes
*/
/*
static uint8_t obj_i2c_master_restart(void) {
	uint8_t delay_val = OBJ_I2C_DELAY_PERIOD(10);
	
	OBJ_I2C_SCL_LOW();
	OBJ_I2C_SDA_HIGH();
	
	_delay_us(delay_val);
	
	OBJ_I2C_SCL_HIGH();
	
	_delay_us(delay_val);
	
	OBJ_I2C_SDA_LOW();
	
	return loI2cData.ctx.error_code;
}
*/
//*****************************************************************************/
/*!
* \fn static uint8_t lo_i2c_master_restart(void);
* \brief low level i2c master acknowledge from slave routine
* \param None
* \return i2c ack error message if slave recognized precedent routine
*/

static uint8_t lo_i2c_master_ack(void) {
	uint8_t delay_val = OBJ_I2C_DELAY_PERIOD(10);
	
	OBJ_I2C_SCL_LOW();
	#ifdef OBJ_I2C_ENABLE_ACK
	OBJ_I2C_SDA_HIGH();
	#else
	OBJ_I2C_SDA_LOW();
	#endif
	_delay_us(delay_val);
	
	OBJ_I2C_SCL_HIGH();
	
	_delay_us(delay_val);
	
	OBJ_I2C_SCL_LOW();
	OBJ_I2C_SDA_LOW();
	
	_delay_us(delay_val);
	
	OBJ_I2C_SDA_LOW();

	return 0;
}

//*****************************************************************************/
/*!
* \fn static uint8_t lo_i2c_master_restart(void);
* \brief low level i2c master stop any message
* \param None
* \return None
*/

static void lo_i2c_master_stop(void) {
	uint8_t delay_val = OBJ_I2C_DELAY_PERIOD(10);
	
	OBJ_I2C_SCL_HIGH();
	
	_delay_us(delay_val);
	
	OBJ_I2C_SDA_HIGH();
}

//*****************************************************************************/
/*!
* \fn void lo_i2c_master_init(
*		lo_gpio_parameters *gpio_scl,
*		lo_gpio_parameters *gpio_sda,
*		uint8_t slave_address,
*		uint32_t frequency
* );
* \brief low level i2c instance init routine
* \param[in] gpio_scl scl gpio pin paramters pointer
* \param[in] gpio_sda sda gpio pin paramters pointer
* \param[in] slave_address i2c instance device slave address (shifted)
* \param[in] frequency i2c instance working frequency. Recommended : 100 000Hz
* \return None
*/

void lo_i2c_master_init(
		lo_gpio_parameters *gpio_scl,
		lo_gpio_parameters *gpio_sda,
		uint8_t slave_address,
		uint32_t frequency
) {
	gpio_scl->dir = PORT_DIR_OUT;
	gpio_sda->dir = PORT_DIR_OUT;
	
	loI2cData.handle.gpio_sda = gpio_sda;
	loI2cData.handle.gpio_scl = gpio_scl;
	
	lo_gpio_init(gpio_scl->inst, gpio_scl->pin, gpio_scl->mode, gpio_scl->dir);
	lo_gpio_init(gpio_sda->inst, gpio_sda->pin, gpio_sda->mode, gpio_sda->dir);
	lo_gpio_set_callback(gpio_sda->inst, lo_i2c_master_gpio_get_state_sda_callback);
	
	LO_TIMER_Initialize();
	
	loI2cData.ctx.step = OBJ_I2C_STEP_IDLE;
	loI2cData.ctx.slave_address = slave_address;
	loI2cData.ctx.frequency = frequency;
	loI2cData.ctx.slave_address = slave_address;
	
	OBJ_I2C_SCL_HIGH();
	OBJ_I2C_SDA_HIGH();
}

//*****************************************************************************/
/*!
* \fn uint8_t lo_i2c_master_transmit(
*		uint8_t command,
*		uint8_t *data,
*		uint16_t length
* );
* \brief low level i2c transmit data routine
* \param[in] command slave device first data byte to transmit after slave address
* \param[in] data pointer to send to the slave device
* \param[in] length data length
* \return i2c error byte. refer to obj_i2c_master.h error messages defines
*/

uint8_t lo_i2c_master_transmit(
		uint8_t command, 
		uint8_t *data, 
		uint16_t length
) {
	uint8_t data_temp = loI2cData.ctx.slave_address & ~0x01;
	uint8_t delay_half_period = OBJ_I2C_DELAY_PERIOD(10);
	
	OBJ_I2C_SCL_HIGH();
	OBJ_I2C_SDA_HIGH();
	
	lo_i2c_master_start();
	
	for(uint8_t i = 0; i < 8; i++) {
		
		OBJ_I2C_SCL_LOW();
		
		if(data_temp & 0x80) {
			OBJ_I2C_SDA_HIGH();
		}
		else {
			OBJ_I2C_SDA_LOW();
		}
		
		_delay_us(delay_half_period);
		
		OBJ_I2C_SCL_HIGH();
		
		_delay_us(delay_half_period);
		
		data_temp <<= 1;
	}
	OBJ_I2C_SCL_LOW();
	lo_i2c_master_ack();
	
	data_temp = command;
	
	for(uint8_t i = 0; i < 8; i++) {
		
		OBJ_I2C_SCL_LOW();
		
		if(data_temp & 0x80) {
			OBJ_I2C_SDA_HIGH();
		}
		else {
			OBJ_I2C_SDA_LOW();
		}
		
		_delay_us(delay_half_period);
		
		OBJ_I2C_SCL_HIGH();
		
		_delay_us(delay_half_period);
		
		data_temp <<= 1;
	}
	OBJ_I2C_SCL_LOW();
	lo_i2c_master_ack();
	
	for(uint8_t i = 0; i < length; i++) {
		
		lo_i2c_master_start();
		data_temp = data[i];
		
		for(uint8_t i = 0; i < 8; i++) {
			OBJ_I2C_SCL_LOW();
			
			if(data_temp & 0x80) {
				OBJ_I2C_SDA_HIGH();
			}
			else {
				OBJ_I2C_SDA_LOW();
			}
			
			_delay_us(delay_half_period);
			
			OBJ_I2C_SCL_HIGH();
			
			_delay_us(delay_half_period);
			
			data_temp <<= 1;
		}
		OBJ_I2C_SCL_LOW();
		lo_i2c_master_ack();
	}
	
	lo_i2c_master_stop();
	
	return 0;
}

//*****************************************************************************/
/*!
* \fn uint8_t lo_i2c_master_receive(
*		uint8_t command,
*		uint8_t *data,
*		uint16_t length
* );
* \brief low level i2c receive data routine
* \param[in] command slave device first data byte to transmit after slave address
* \param[out] data pointer to send to the slave device
* \param[in] length data length
* \return i2c error byte. refer to obj_i2c_master.h error messages defines
*/

uint8_t lo_i2c_master_receive(
		uint8_t command,
		uint8_t *data,
		uint16_t length
) {
	return 0;
}
#endif