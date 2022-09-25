/*
 * HW_I2C.c
 *
 * Created: 3/23/2020 9:36:18 AM
 *  Author: Adam
 */ 

#include "atmega328p_hw_i2c.h"

static hw_i2c_data hwI2cData;

static void hw_i2c_default_cb(void *handle, uint8_t last_byte) {
	(void)handle;
	(void)last_byte;
}

void hw_i2c_init(void) {
	DDRC &= ~((1<<PINC4) | (1<<PINC5));
	#if HW_I2C_MODE == HW_I2C_MASTER
	TWBR = TWBR_MASK;
	TWSR = TWPS_MASK;
	#else
	TWAR = HW_I2C_SLAVE_ADDRESS;
	TWAMR = HW_I2C_SLAVE_ADD_MASK;
	#endif
	#if HW_I2C_PULL_UP == HW_I2C_ENABLE
	PORTC |= (1<<PINC4) | (1<<PINC5);
	#else
	PORTC &= ~((1<<PINC4) | (1<<PINC5));
	#endif
	
	hwI2cData.cb = hw_i2c_default_cb;
}

void hw_i2c_set_callback(hw_i2c_cb cb, void *handle) {
	
}

#if HW_I2C_MODE == HW_I2C_MASTER

static inline void hw_i2c_start(void) {
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT);
	while(!(TWCR &(1<<TWINT)));	hwI2cData.flag = TWSR & 0xF8;	if(hwI2cData.flag != HW_I2C_MST_TR_START) {
		while(1);
	}
}

static inline void hw_i2c_restart(void) {
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT);
	while(!(TWCR &(1<<TWINT)));	hwI2cData.flag = TWSR & 0xF8;	if(hwI2cData.flag != HW_I2C_MST_TR_RSTART) {
		while(1);
	}
}

static inline void hw_i2c_slave_addr_w(uint8_t addr) {	TWDR = addr & 0xFE;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));	hwI2cData.flag = TWSR & 0xF8;	if(hwI2cData.flag != HW_I2C_MST_TR_SLAW_ACK) {
		while(1);
	}
}

static inline void hw_i2c_data_byte_w(uint8_t data) {	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));	hwI2cData.flag = TWSR & 0xF8;	if(hwI2cData.flag != HW_I2C_MST_TR_DATA_ACK) {
		while(1);
	}
}

static inline void hw_i2c_slave_addr_r(uint8_t addr) {	TWDR = addr | 1;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));	hwI2cData.flag = TWSR & 0xF8;	if(hwI2cData.flag != HW_I2C_MST_RC_SLAR_ACK) {
		while(1);
	}
}

static inline uint8_t hw_i2c_data_byte_r(void) {
	uint8_t temp = 0;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR &(1<<TWINT)));	temp = TWDR;		hwI2cData.flag = TWSR & 0xF8;	if(		hwI2cData.flag != HW_I2C_MST_RC_DATA_ACK &&		hwI2cData.flag != HW_I2C_MST_RC_DATA_NAK	) {		while(1);	}
	return temp;
}

static inline void hw_i2c_stop(void) {	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	while(!(TWCR&(1<<TWSTO)));
	
	for(uint32_t i = 0; i < 3200; i++);
}

void hw_i2c_write(uint8_t addr, uint8_t *data, uint16_t len) {
	hw_i2c_start();	hw_i2c_slave_addr_w(addr);		for(uint16_t i = 0; i < len; i++) {		hw_i2c_data_byte_w(data[i]);	}		hw_i2c_stop();
}

void hw_i2c_read(uint8_t addr, uint8_t *data, uint16_t len) {
	hw_i2c_start();	hw_i2c_slave_addr_r(addr);
	
	for(uint16_t i = 0; i < len; i++) {		data[i] = hw_i2c_data_byte_r();		if(hwI2cData.flag == HW_I2C_MST_RC_DATA_NAK) break;	}
		hw_i2c_stop();
}

void hw_i2c_write_read(uint8_t addr, uint8_t data_in, uint8_t *data_out, uint16_t len_out) {
	hw_i2c_start();
	hw_i2c_slave_addr_w(addr);
	hw_i2c_data_byte_w(data_in);
	hw_i2c_restart();
	hw_i2c_slave_addr_r(addr);
	
	for(uint16_t i = 0; i < len_out; i++) {
		data_out[i] = hw_i2c_data_byte_r();		if(hwI2cData.flag == HW_I2C_MST_RC_DATA_NAK) break;
	}
		hw_i2c_stop();
}

void hw_i2c_write_write(uint8_t addr, uint8_t data_in, uint8_t *data_out, uint16_t len_out) {
	
}

#else


#endif
/*
ISR(TWI_vect, ISR_BLOCK) {
	
}*/