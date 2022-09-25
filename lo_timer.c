/*
 * lo_timer.c
 *
 * Created: 9/17/2022 10:57:38 AM
 *  Author: Adam
 */ 

#include "lo_timer.h"



void lo_timer_periph_init(lo_timer_periph *inst, lo_timer_periph_param *param) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_tc_data_param temp = {
		.peripheral = param->periph,
	};
	hw_tc_periph_init(inst, &temp);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	hw_tc_data_param temp = {
		.peripheral = param->periph,
	};
	hw_tc_periph_init(inst, &temp);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_timer_param temp_param = {
		.timer = param->periph_sel == 0 ? TIM1 :
		param->periph_sel == 1 ? TIM2 :
		param->periph_sel == 2 ? TIM3 : TIM4,
		.cr1 = (
						param->mode == LO_TIMER_MODE_COUNT_INCR ? HW_TIMER_DIR_UP_COUNT :
						LO_TIMER_MODE_COUNT_DECR
				) | (
						(SYSCLK_FREQ_VAL / param->frequency) <= 1 ? HW_TIMER_CKD_1 :
						(SYSCLK_FREQ_VAL / param->frequency) <= 2 ? HW_TIMER_CKD_2 : HW_TIMER_CKD_4
				) | HW_TIMER_OPM_STOPPED | HW_TIMER_ARPE_BUFFERED,
		.irq = HW_TIMER_UIE
				
	};
	
	hw_timer_init(inst, &temp_param);
#endif
}

void lo_timer_enable(lo_timer_periph *inst, bool enable) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_tc_enable(inst, enable);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	hw_timer_enable(inst, enable);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_timer_enable(inst, enable);
#endif
}

void lo_timer_init_chan(lo_timer_periph *inst, lo_timer_chan_param *param) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_tc_chan_param temp = {
		.cb = param->cb,
		.handle = param->handle,
		.channel = param->channel,
	};
	hw_tc_init_chan(inst, &temp);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	hw_tc_chan_param temp = {
		.cb = param->cb,
		.handle = param->handle,
		.channel = param->channel,
	};
	hw_tc_init_chan(inst, &temp);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_timer_set_handle(inst, param->handle);
	switch(param->channel) {
		case 0:
			hw_timer_set_update_cb(inst, param->cb);
			break;
		case 1:
			hw_timer_set_cc1_cb(inst, param->cb);
			break;
		case 2:
			hw_timer_set_cc2_cb(inst, param->cb);
			break;
		case 3:
			hw_timer_set_cc3_cb(inst, param->cb);
			break;
		case 4:
			hw_timer_set_cc4_cb(inst, param->cb);
			break;
	}
#endif
}

void lo_timer_chan_set_val(lo_timer_periph *inst, uint8_t channel, uint64_t val) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_tc_set_val(inst, channel, (uint16_t)val);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	hw_tc_set_val(inst, channel, (uint16_t)val);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	switch(channel) {
		case 1:
			hw_timer_set_ccr1(inst, (uint16_t)val);
			break;
		case 2:
			hw_timer_set_ccr2(inst, (uint16_t)val);
			break;
		case 3:
			hw_timer_set_ccr3(inst, (uint16_t)val);
			break;
		case 4:
			hw_timer_set_ccr4(inst, (uint16_t)val);
			break;
	}
#endif
}

uint64_t lo_timer_chan_get_val(lo_timer_periph *inst, uint8_t channel) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	return (uint64_t)hw_tc_get_val(inst, channel);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	return (uint64_t)hw_tc_get_val(inst, channel);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    return 0;
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	uint16_t val = 0;
	switch(channel) {
		case 1:
			val = hw_timer_get_ccr1(inst);
			break;
		case 2:
			val = hw_timer_get_ccr2(inst);
			break;
		case 3:
			val = hw_timer_get_ccr3(inst);
			break;
		case 4:
			val = hw_timer_get_ccr4(inst);
			break;
	}
	return val;
#endif
}
