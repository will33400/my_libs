/*
 * fifo.h
 *
 * Created: 4/22/2020 5:58:36 PM
 *  Author: Adam
 */ 


#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>


typedef struct {
	uint8_t *array;
	uint16_t array_size;
	uint8_t array_default_val;
	uint32_t read_index;
	uint32_t write_index;
}fifo_inst; // TODO

typedef struct {
	uint8_t *array;
	uint16_t array_size;
	uint8_t array_default_val;
}fifo_param;

void fifo_init(fifo_inst *inst, fifo_param *param);
void fifo_write(fifo_inst *inst, uint8_t data);
uint8_t fifo_read(fifo_inst *inst);
void fifo_set_read_index(fifo_inst *inst, uint32_t val);
void fifo_set_write_index(fifo_inst *inst, uint32_t val);
uint32_t fifo_get_read_index(fifo_inst *inst);
uint32_t fifo_get_write_index(fifo_inst *inst);
void fifo_reset_array(fifo_inst *inst);
void fifo_reset_all(fifo_inst *inst);


#endif /* FIFO_H_ */
