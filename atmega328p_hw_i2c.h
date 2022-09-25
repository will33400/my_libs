/*
 * HW_I2C.h
 *
 * Created: 3/23/2020 9:35:59 AM
 *  Author: Adam
 */ 

#ifndef HW_I2C_H_
#define HW_I2C_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "atmega328p_hw_defines.h"

#define HW_I2C_SLAVE 0
#define HW_I2C_MASTER 1
#define HW_I2C_DISABLE 0
#define HW_I2C_ENABLE 1

#ifndef HW_I2C_MODE
#warning HW_I2C_MODE undefined
#define HW_I2C_MODE HW_I2C_SLAVE
#endif

#if HW_I2C_MODE == HW_I2C_MASTER

#ifndef F_CPU
#warning F_CPU undefined
#define F_CPU (8000000UL)
#endif

#ifndef HW_I2C_FREQ
#warning HW_I2C_FREQ undefined
#define HW_I2C_FREQ (250000UL)
#endif

#if F_CPU / 16 < HW_I2C_FREQ
#warning HW_I2C_FREQ too high
#endif

#define SCL_DIV_FACTOR ((F_CPU/(2*HW_I2C_FREQ))-8)

#if SCL_DIV_FACTOR < 0
#warning SCL_DIV_FACTOR too low
#endif

#if SCL_DIV_FACTOR > 255 / 64
#if SCL_DIV_FACTOR > 255 / 16
#if SCL_DIV_FACTOR > 255 / 4
#if SCL_DIV_FACTOR > 255 / 1
#warning SCL_DIV_FACTOR too high
#else
#define TWBR_MASK SCL_DIV_FACTOR
#define TWPS_MASK 0
#endif
#else
#define TWBR_MASK SCL_DIV_FACTOR * 4
#define TWPS_MASK 1
#endif
#else
#define TWBR_MASK SCL_DIV_FACTOR * 16
#define TWPS_MASK 2
#endif
#else
#define TWBR_MASK SCL_DIV_FACTOR * 64
#define TWPS_MASK 3
#endif

#else
#ifndef HW_I2C_SLAVE_ADDRESS
#warning HW_I2C_SLAVE_ADDRESS undefined
#define HW_I2C_SLAVE_ADDRESS 0x01
#endif
#ifndef HW_I2C_SLAVE_ADD_MASK
#warning HW_I2C_SLAVE_ADD_MASK undefined
#define HW_I2C_SLAVE_ADD_MASK 0
#endif
#endif

#ifndef HW_I2C_PULL_UP
#warning pull-up resistors undefined
#define HW_I2C_PULL_UP HW_I2C_ENABLE
#endif

#define HW_I2C_MST_TR_START		0x08
#define HW_I2C_MST_TR_RSTART		0x10
#define HW_I2C_MST_TR_SLAW_ACK		0x18
#define HW_I2C_MST_TR_SLAW_NAK		0x20
#define HW_I2C_MST_TR_DATA_ACK		0x28
#define HW_I2C_MST_TR_DATA_NAK		0x30
#define HW_I2C_MST_TR_ARB_LOST		0x38

#define HW_I2C_MST_RC_SLAR_ACK		0x40
#define HW_I2C_MST_RC_SLAR_NAK		0x48
#define HW_I2C_MST_RC_DATA_ACK		0x50
#define HW_I2C_MST_RC_DATA_NAK		0x58

#define HW_I2C_SLV_RC_SLAW_ACK		0x60 // (1<<TWEA)
#define HW_I2C_SLV_RC_SLWAR_LO		0x68 // (1<<TWEA)
#define HW_I2C_SLV_RC_GEN_CALL		0x70 // (1<<TWEA)
#define HW_I2C_SLV_RC_ARB_L_GE		0x78 // (1<<TWEA)
#define HW_I2C_SLV_RC_SW_P_ACK		0x80 // (1<<TWEA)
#define HW_I2C_SLV_RC_SW_P_NAK 	0x88 // ~(1<<TWEA)
#define HW_I2C_SLV_RC_GC_P_ACK		0x90 // (1<<TWEA)
#define HW_I2C_SLV_RC_GC_P_NAK		0x98 // ~(1<<TWTEA)
#define HW_I2C_SLV_RC_RSTA_STO		0xa0

#define HW_I2C_SLV_RC_SALR_NAK		0xa8 // (1<<TWEA)
#define HW_I2C_SLV_RC_SLRAR_LO		0xb0 // (1<<TWEA)
#define HW_I2C_SLV_TR_DATA_ACK		0xb8
#define HW_I2C_SLV_TR_DATA_NAK		0xc0
#define HW_I2C_SLV_TR_DA_TR		0xc8 // ~(1<<TWEA)

#define HW_I2C_HW_I2C_TC_TWINT_0		0xf8
#define HW_I2C_HW_I2C_ILLEGAL_SP		0x00

typedef void(*hw_i2c_cb)(void *handle, uint8_t last_byte);

typedef struct {
	uint8_t *data;
	uint16_t len;
	uint8_t flag;
	hw_i2c_cb cb;
	void *handle;
}hw_i2c_data;

void hw_i2c_init(void);
void hw_i2c_set_callback(hw_i2c_cb cb, void *handle);

void hw_i2c_write(uint8_t addr, uint8_t *data, uint16_t len);
void hw_i2c_read(uint8_t addr, uint8_t *data, uint16_t len);
void hw_i2c_write_read(uint8_t addr, uint8_t data_in, uint8_t *data_out, uint16_t len_out);
void hw_i2c_write_write(uint8_t addr, uint8_t data_in, uint8_t *data_out, uint16_t len_out);

#endif /* HW_I2C_H_ */