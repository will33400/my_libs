/*
 * lo_timer.h
 *
 * Created: 9/17/2022 10:57:14 AM
 *  Author: Adam
 */ 


#ifndef LO_TIMER_H_
#define LO_TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "lo_defines.h"

#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
#include "atmega328p/atmega328p_hw_tc.h"
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
#include "attiny85/attiny85_hw_timer.h"
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
#include "stm32f103c8_hw_timer.h"
#endif

typedef enum {
	LO_TIMER_MODE_COUNT_INCR,
	LO_TIMER_MODE_COUNT_DECR,
	LO_TIMER_MODE_PWM,	
}lo_timer_mode;

typedef void(*lo_timer_cb)(void *handle);

typedef struct {
	uint8_t channel;
	lo_timer_cb cb;
	void *handle;
}lo_timer_chan_param;

typedef
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
void *
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
void *
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
int
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
int
#endif
lo_timer_chan_inst;

typedef
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
hw_tc_data
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
hw_tc_data
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
int
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
hw_timer_inst
#endif
lo_timer_periph;

typedef struct {
	uint32_t frequency;
	uint8_t resolution;
	lo_timer_mode mode;
	uint8_t periph_sel;
}lo_timer_periph_param;

void lo_timer_periph_init(lo_timer_periph *inst, lo_timer_periph_param *param);
void lo_timer_enable(lo_timer_periph *inst, bool enable);
void lo_timer_init_chan(lo_timer_periph *inst, lo_timer_chan_param *param);
void lo_timer_chan_set_val(lo_timer_periph *inst, uint8_t channel, uint64_t val);
uint64_t lo_timer_chan_get_val(lo_timer_periph *inst, uint8_t channel);

#endif /* LO_TIMER_H_ */
