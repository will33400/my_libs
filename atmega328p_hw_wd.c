/*
 * hw_wd.c
 *
 * Created: 7/13/2022 6:55:46 AM
 *  Author: Adam
 */ 

#include "atmega328p_hw_wd.h"

void hw_wd_init(hw_wd_counter_ovf ovf) {
	uint8_t temp = (1<<WDE) | ((ovf>>3)<<WDP3) | ((ovf & 7)<<WDP0);
	cli();
	wdr();
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = temp;
	sei();
}