/*
 * adc.c
 *
 * Created: 7/19/2020 4:47:37 PM
 *  Author: Adam
 */ 

#include "attiny85_hw_adc.h"

static hw_adc_peripheral_data hwAdcPeriphData;

static hw_adc_inst *first_inst = (void *)0;
static hw_adc_inst *last_inst = (void *)0;
static hw_adc_inst *turn_inst = (void *)0;
static uint8_t inst_count = 0;

static void hw_adc_start_measure(hw_adc_inst *inst) {
	
	ADMUX &= ~0x0F;
	ADMUX |= (uint8_t)inst->pin_num;
	
	ADCSRA |= (1<<ADSC);
}

static void hw_adc_stop_measure(hw_adc_inst *inst) {
	(void)inst;
}

static void hw_adc_default_cb(void *handle) {
	(void)handle;
}

void hw_adc_init(hw_adc_peripheral_param *param) {
	hwAdcPeriphData = *param;
	ADCSRA = (uint8_t)param->prescale;
	
	ADCSRA |= (1<<ADIE);
	sei();
	
	ADCSRA |= (1 << ADEN);
	
	ADMUX = (uint8_t)hwAdcPeriphData.vref | HW_ADC_ADJUSTMENT_RIGHT;
	ADCSRB = 0;
}

void hw_adc_init_inst(hw_adc_inst *inst, hw_adc_param *param) {
	
	hw_adc_inst *temp = first_inst;
	
	while(temp) {
		if(param->pin_num == temp->pin_num) return;
		temp = temp->next_inst;
	}
	
	inst_count++;
	if(inst_count > HW_ADC_INST_COUNT) return;
	
	if(last_inst)
	last_inst->next_inst = inst;
	if(!first_inst)
	first_inst = inst;
	last_inst = inst;
	
	inst->pin_num = param->pin_num;
	inst->cb = hw_adc_default_cb;
}

void hw_adc_set_cb(hw_adc_inst *inst, lo_adc_cb cb, void *handle) {
	inst->cb = cb;
	inst->handle = handle;
}


void hw_adc_enable(hw_adc_inst *inst, bool enable) {
	if(enable) {
		inst->enabled = true;
		if(!turn_inst) {
			turn_inst = inst;
			hw_adc_start_measure(inst);
		}
	}
	else {
		hw_adc_stop_measure(inst);
		inst->enabled = false;
		if(turn_inst == inst) {
			hw_adc_inst *temp_inst = first_inst;
			while(temp_inst) {
				if(temp_inst->enabled) {
					turn_inst = temp_inst;
					return;
				}
				temp_inst = temp_inst->next_inst;
			}
			turn_inst = (void *)0;
		}
	}
}


uint16_t hw_adc_get_raw(hw_adc_inst *inst) {
	if(!inst->measure_complete) return -1;
	inst->measure_complete = false;
	return HW_ADC_CONV_XBITS(inst->raw_val_10bits, inst->resolution);
}

double hw_adc_get_volts(hw_adc_inst *inst) {
	if(!inst->measure_complete) return -1;
	inst->measure_complete = false;
	return HW_ADC_CONV_VOLTS(inst->raw_val_10bits, hwAdcPeriphData.vmax);
}

bool hw_adc_val_available(hw_adc_inst *inst) {
	return inst->measure_complete;
}

ISR(ADC_vect, ISR_BLOCK) {
	cli();
	turn_inst->raw_val_10bits = ADC;
	
	hw_adc_stop_measure(turn_inst);
	turn_inst->measure_complete = true;
	turn_inst->cb(turn_inst->handle);
	
	for(uint8_t i = 0; i < inst_count; i++) {
		turn_inst = turn_inst->next_inst;
		if(!turn_inst) turn_inst = first_inst;
		if(turn_inst->enabled) {
			hw_adc_start_measure(turn_inst);
			break;
		}
	}
	
	ADCSRA |= (1<<ADIF);
	sei();
}