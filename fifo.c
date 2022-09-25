/*
 * fifo.c
 *
 * Created: 4/22/2020 5:58:22 PM
 *  Author: Adam
 */ 

#include "fifo.h"


void fifo_init(fifo_inst *inst, fifo_param *param) {
	inst->array = param->array;
	inst->array_size = param->array_size;
	inst->array_default_val = param->array_default_val;
	fifo_reset_all(inst);
}

void fifo_write(fifo_inst *inst, uint8_t data) {
	if(inst->read_index != inst->write_index) {
		inst->array[inst->write_index] = data;
		inst->write_index++;
		inst->write_index %= inst->array_size;
	}
}

uint8_t fifo_read(fifo_inst *inst) {
	uint8_t temp = inst->array[inst->read_index];
	if(inst->write_index != inst->read_index) {
		inst->read_index++;
		inst->read_index %= inst->array_size;
		temp = inst->array[inst->read_index];
	}
	return temp;
}

void fifo_set_read_index(fifo_inst *inst, uint32_t val) {
	val %= inst->array_size;
	inst->read_index = val;
}

void fifo_set_write_index(fifo_inst *inst, uint32_t val) {
	val %= inst->array_size;
	inst->write_index = val;
}

uint32_t fifo_get_read_index(fifo_inst *inst) {
	return inst->read_index;
}

uint32_t fifo_get_write_index(fifo_inst *inst) {
	return inst->write_index;
}

void fifo_reset_array(fifo_inst *inst) {
	for(uint32_t i = 0; i < inst->array_size; i++) {
		inst->array[i] = inst->array_default_val;
	}
}

void fifo_reset_all(fifo_inst *inst) {
	inst->read_index = inst->array_size - 1;
	inst->write_index = 0;
	fifo_reset_array(inst);
}
