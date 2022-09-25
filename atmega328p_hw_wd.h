/*
 * hw_wd.h
 *
 * Created: 7/13/2022 6:55:31 AM
 *  Author: Adam
 */ 


#ifndef HW_WD_H_
#define HW_WD_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "atmega328p_hw_defines.h"

#define wdr()		__asm__ __volatile__ ("wdr " ::)

typedef enum {
	HW_WD_COUNTER_OVF_16MS = 0,
	HW_WD_COUNTER_OVF_32MS = 1,
	HW_WD_COUNTER_OVF_64MS = 2,
	HW_WD_COUNTER_OVF_12_5MS = 3,
	HW_WD_COUNTER_OVF_25MS = 4,
	HW_WD_COUNTER_OVF_50MS = 5,
	HW_WD_COUNTER_OVF_1S = 6,
	HW_WD_COUNTER_OVF_2S = 7,
	HW_WD_COUNTER_OVF_4S = 8,
	HW_WD_COUNTER_OVF_8S = 9,
}hw_wd_counter_ovf;

void hw_wd_init(hw_wd_counter_ovf ovf);


#endif /* HW_WD_H_ */