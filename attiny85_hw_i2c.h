/*!
 * \file lo_i2c.h
 * \brief low level i2c header
 * \author Adam LUCAS
 * \date 8/1/2021 9:35:01 PM
 */

#ifndef LO_I2C_H_
#define LO_I2C_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "attiny85_hw_gpio.h"
#include "attiny85_hw_timer.h"

#define F_CPU 8000000UL

#include <util/delay.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#define LO_I2C_MASTER_1		0

#define LO_I2C_MASTER_NUM	1

#define OBJ_I2C_ENABLE_ACK

#define	OBJ_I2C_MST_TR_START				0x08
#define	OBJ_I2C_MST_TR_RSTART				0x10
#define	OBJ_I2C_MST_TR_SLAW_ACK				0x18
#define	OBJ_I2C_MST_TR_SLAW_NAK				0x20
#define	OBJ_I2C_MST_TR_DATA_ACK				0x28
#define	OBJ_I2C_MST_TR_DATA_NAK				0x30
#define	OBJ_I2C_MST_TR_ARB_LOST				0x38

#define	OBJ_I2C_MST_RC_SLAR_ACK				0x40
#define	OBJ_I2C_MST_RC_SLAR_NAK				0x48
#define	OBJ_I2C_MST_RC_DATA_ACK				0x50
#define	OBJ_I2C_MST_RC_DATA_NAK				0x58
/*
#define	OBJ_I2C_SLV_RC_SLAW_ACK				0x60
#define	OBJ_I2C_SLV_RC_SLWAR_LO				0x68
#define	OBJ_I2C_SLV_RC_GEN_CALL				0x70
#define	OBJ_I2C_SLV_RC_ARB_L_GE				0x78
#define	OBJ_I2C_SLV_RC_SW_P_ACK				0x80
#define	OBJ_I2C_SLV_RC_SW_P_NAK 			0x88
#define	OBJ_I2C_SLV_RC_GC_P_ACK				0x90
#define	OBJ_I2C_SLV_RC_GC_P_NAK				0x98
#define	OBJ_I2C_SLV_RC_RSTA_STO				0xa0

#define	OBJ_I2C_SLV_RC_SALR_NAK				0xa8
#define	OBJ_I2C_SLV_RC_SLRAR_LO				0xb0
#define	OBJ_I2C_SLV_TR_DATA_ACK				0xb8
#define	OBJ_I2C_SLV_TR_DATA_NAK				0xc0
#define	OBJ_I2C_SLV_TR_DA_TR				0xc8
*/
#define	OBJ_I2C_WI_TC_TWINT_0				0xf8
#define	OBJ_I2C_TWI_ILLEGAL_SP				0x00

#define OBJ_I2C_DELAY_PERIOD(delay_us)		(double)delay_us

#define OBJ_I2C_SCL_HIGH()					while(!(PORTB&(1<<2))) PORTB ^= (1<<2)
#define OBJ_I2C_SCL_LOW()					while(PORTB&(1<<2)) PORTB &= ~(1<<2) 

#define OBJ_I2C_SDA_HIGH()					while(!(PORTB&(1<<0))) PORTB ^= (1<<0)
#define OBJ_I2C_SDA_LOW()					while(PORTB&(1<<0)) PORTB &= ~(1<<0)
#define OBJ_I2C_SDA_GET_STATE(state)		loI2cData.ctx.sda_state = state

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef enum {
	OBJ_I2C_STEP_IDLE,
	OBJ_I2C_STEP_START,
	OBJ_I2C_STEP_TRANSMIT,
	OBJ_I2C_STEP_TRANSMIT_RECEIVE,
	OBJ_I2C_STEP_STOP,
}lo_i2c_step;

typedef uint8_t lo_i2c_error_code;

typedef struct {
	lo_i2c_step step;
	uint8_t slave_address;
	uint32_t frequency;
	lo_i2c_error_code error_code;
	bool sda_state;
	bool waiting_ack;
}ctx_lo_i2c_master;

/*!
 * \struct lo_i2c_handle
 * \brief low level i2c protocol gpio + i2c instance parameters handle struct
 */

typedef struct {
	uint8_t lo_i2c_inst;
	lo_gpio_parameters *gpio_scl;
	lo_gpio_parameters *gpio_sda;
}lo_i2c_handle;

/*!
 * \struct LO_I2C_DATA
 * \brief low level i2c protocol instance struct
 */

typedef struct {
	lo_i2c_handle handle;
	ctx_lo_i2c_master ctx;
}LO_I2C_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void lo_i2c_master_init(
		lo_gpio_parameters *gpio_scl, 
		lo_gpio_parameters *gpio_sda,
		uint8_t slave_address,
		uint32_t frequency
);

uint8_t lo_i2c_master_transmit(
		uint8_t command, 
		uint8_t *data, 
		uint16_t length
);

uint8_t lo_i2c_master_receive(
		uint8_t command,
		uint8_t *data,
		uint16_t length
);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* LO_I2C_H_ */