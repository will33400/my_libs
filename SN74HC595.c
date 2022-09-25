/*
 * SN74HC595.c
 *
 * Created: 5/15/2020 6:42:54 PM
 *  Author: Adam
 */ 

#include "SN74HC595.h"

void shift_init_pins(shift_reg_t *shift_reg, uint8_t pin_1, uint8_t pin_2, uint8_t pin_3) {
	shift_reg->ser_pin = pin_1;
	shift_reg->rclr_pin = pin_2;
	shift_reg->srclr_pin = pin_3;
	pin_set_state(shift_reg->ser_pin, LOW);
	pin_set_state(shift_reg->rclr_pin, LOW);
	pin_set_state(shift_reg->srclr_pin, LOW);
	pin_set_mode(shift_reg->ser_pin, OUT);
	pin_set_mode(shift_reg->rclr_pin, OUT);
	pin_set_mode(shift_reg->srclr_pin, OUT);
}

void shift_byte_transmission(shift_reg_t *shift_reg, uint8_t *data, side_t side) {
	for (int i = 0; i < 8; i++) {
		pin_set_state(shift_reg->srclr_pin, LOW);
		while (pin_get_state(shift_reg->srclr_pin));
		
		if (
		((*(data)>>i)&0x01 && side == RIGHT) ||
		((*(data)<<i)&0x80 && side == LEFT)
		) {
			pin_set_state(shift_reg->ser_pin, HIGH);
			while (!pin_get_state(shift_reg->ser_pin));
		}
		else
			_delay_us(110);
			
		_delay_us(20);
		
		pin_set_state(shift_reg->srclr_pin, HIGH);
		while (!pin_get_state(shift_reg->srclr_pin));
		_delay_us(20);
		
		pin_set_state(shift_reg->ser_pin, LOW);
		while (pin_get_state(shift_reg->ser_pin));
		_delay_us(20);
	}
	pin_set_state(shift_reg->rclr_pin, HIGH);
	while (!pin_get_state(shift_reg->rclr_pin));
	_delay_us(160);
	
	pin_set_state(shift_reg->rclr_pin, LOW);
	while (pin_get_state(shift_reg->rclr_pin));
	_delay_us(500);
}