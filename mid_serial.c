/*
 * mid_serial.c
 *
 * Created: 11/11/2021 8:27:41 AM
 *  Author: Adam
 */ 

#include "mid_serial.h"

static mid_serial_inst *first_inst = (void *)0;
static mid_serial_inst *last_inst = (void *)0;

static void mid_serial_vt100_write_cb(void *handle, uint8_t *data, uint16_t length) {
	mid_serial_inst *handle_temp = (mid_serial_inst *)handle;
	lo_uart_puts(&handle_temp->uart_inst, (int8_t *)data, length, MID_SERIAL_DEFAULT_TO);
}

static void mid_serial_uart_receive_cb(void *handle) {
	mid_serial_inst *handle_temp = (mid_serial_inst *)handle;
	handle_temp->echo_char = 
			(int8_t)lo_uart_getchar(&handle_temp->uart_inst);
	handle_temp->last_char = handle_temp->echo_char;
	if(
		handle_temp->echo_char > 0x1f && 
		handle_temp->echo_char < MID_SERIAL_KEY_BACKSPACE
	){
		fifo_write(&handle_temp->fifo_inst, (uint8_t)handle_temp->echo_char);
	} else if(
			handle_temp->echo_char >= MID_SERIAL_KEY_BACKSPACE && 
			handle_temp->buffer_index > 0
	) {
		uint32_t index_temp = fifo_get_write_index(&handle_temp->fifo_inst);
		if(index_temp > 0) {
			index_temp--;
			fifo_set_write_index(&handle_temp->fifo_inst, index_temp);
		}
	}
	
	handle_temp->user_receive_cb(handle_temp->user_receive_cb_handle);
}

static void mid_serial_default_cb(void *handle) {
	(void)handle;
}


void mid_serial_init(mid_serial_inst *inst, lo_uart_param *uart_param) {
	
	if(last_inst)
		last_inst->next_inst = inst;
	if(!first_inst)
		first_inst = inst;
	last_inst = inst;
	
	inst->user_receive_cb_handle = (void *)0;
	inst->user_receive_cb = mid_serial_default_cb;
	inst->vt100_inst.uart_write = mid_serial_vt100_write_cb;
	inst->vt100_inst.handle = inst;
	inst->echo_enabled = false;
	inst->echo_char = -1;
	
	lo_uart_init(&inst->uart_inst, uart_param);
	lo_uart_set_rx_cb(
			&inst->uart_inst, 
			mid_serial_uart_receive_cb, 
			inst
	);
	
	fifo_param f_param = {
		.array = inst->fifo_array,
		.array_size = 16,
		.array_default_val = 0xff,
	};
	
	fifo_init(&inst->fifo_inst, &f_param);
}

void mid_serial_cb_set(mid_serial_inst *inst, mid_serial_receive_cb receive_cb, void *handle) {
	inst->user_receive_cb = receive_cb;
	inst->user_receive_cb_handle = handle;
}

void mid_serial_transmit(mid_serial_inst *inst, uint8_t *data, uint16_t length) {
	lo_uart_puts(
			&inst->uart_inst, 
			(int8_t *)data, 
			length, 
			MID_SERIAL_DEFAULT_TO
	);
}

void mid_serial_font_set(
		mid_serial_inst *inst,
		vt100_font_setting setting,
		vt100_font_foreground foreground, 
		vt100_font_background background
) {
	vt100_font_set(
			&inst->vt100_inst, 
			setting, 
			foreground, 
			background
	);
}

uint8_t mid_serial_receive_last_char(mid_serial_inst *inst) {
	
	uint8_t temp = (uint8_t)inst->last_char;
	inst->last_char = -1;
	return temp;
}

void mid_serial_receive(mid_serial_inst *inst, uint8_t *data, uint16_t length) {
	for(uint16_t i = 0; i < length; i++) {
		data[i] = fifo_read(&inst->fifo_inst);
	}
}

void mid_serial_echo_enable(mid_serial_inst *inst, bool enable) {
	inst->echo_enabled = enable;
	inst->echo_char = -1;
	inst->buffer_index = 0;
}

void mid_serial_echo_process(mid_serial_inst *inst) {
	if(inst->echo_enabled && inst->echo_char != -1) {
		if(inst->echo_char == '\r') {
			mid_serial_transmit(inst, (uint8_t *)&inst->echo_char, 1);
			inst->pos_x = 0;
		}
		else if(inst->echo_char == '\n') {
			mid_serial_transmit(inst, (uint8_t *)&inst->echo_char, 1);
			inst->pos_y++;
			inst->buffer_index++;
		}
		else if(
				inst->echo_char == MID_SERIAL_KEY_BACKSPACE
		) {
			if(inst->buffer_index > 0) {
				mid_serial_transmit(inst, (uint8_t *)&inst->echo_char, 1);
				inst->buffer_index--;
			}
		}
		else {
			mid_serial_transmit(inst, (uint8_t *)&inst->echo_char, 1);
			inst->pos_x++;
			if(inst->pos_x >= VT100_SCREEN_WIDTH) {
				inst->pos_x %= VT100_SCREEN_WIDTH;
				inst->pos_y++;
			}
			inst->buffer_index++;
		}
		inst->echo_char = -1;
	}
}

void mid_serial_reset_buffer(mid_serial_inst *inst) {
	fifo_reset_all(&inst->fifo_inst);
	inst->buffer_index = 0;
}

void mid_serial_reset_screen(mid_serial_inst *inst) {
	vt100_clear_screen(&inst->vt100_inst);
	mid_serial_vt100_move(inst, 0, 0);
}

void mid_serial_vt100_move(mid_serial_inst *inst, uint8_t x, uint8_t y) {
	vt100_move(&inst->vt100_inst, x, y);
	inst->pos_x = x;
	inst->pos_y = y;
}

