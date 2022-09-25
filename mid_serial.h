/*
 * mid_serial.h
 *
 * Created: 11/11/2021 8:28:00 AM
 *  Author: Adam
 */ 


#ifndef MID_SERIAL_H_
#define MID_SERIAL_H_

#include "lo_uart.h"
#include "fifo.h"
#include "vt100.h"

#define MID_SERIAL_DEFAULT_TO			1000000

#define MID_SERIAL_KEY_ENTER			'\r'
#define MID_SERIAL_KEY_NEW_LINE			'\n'
#define MID_SERIAL_KEY_RETURN			MID_HMI_KEY_ENTER
#define MID_SERIAL_KEY_ESCAPE			VT100_ESC
#define MID_SERIAL_KEY_BACKSPACE		0x7f

typedef void(*mid_serial_receive_cb)(void *handle);

typedef struct mid_serial_inst_{
	mid_serial_receive_cb user_receive_cb;
	void *user_receive_cb_handle;
	VT100_CTX vt100_inst;
	bool echo_enabled;
	int8_t echo_char;
	uint8_t pos_x;
	uint8_t pos_y;
	uint16_t buffer_index;
	int8_t last_char;
	uint8_t fifo_array[16];
	
	fifo_inst fifo_inst;
	lo_uart_inst uart_inst;
	
	struct mid_serial_inst_ *next_inst;
}mid_serial_inst;

void mid_serial_init(mid_serial_inst *inst, lo_uart_param *uart_param);
void mid_serial_cb_set(mid_serial_inst *inst, mid_serial_receive_cb receive_cb, void *handle);

void mid_serial_font_set(
		mid_serial_inst *inst,
		vt100_font_setting setting,
		vt100_font_foreground foreground, 
		vt100_font_background background
);
void mid_serial_transmit(mid_serial_inst *inst, uint8_t *data, uint16_t length);
uint8_t mid_serial_receive_last_char(mid_serial_inst *inst);
void mid_serial_receive(mid_serial_inst *inst, uint8_t *data, uint16_t length);
void mid_serial_echo_enable(mid_serial_inst *inst, bool enable);
void mid_serial_echo_process(mid_serial_inst *inst);
void mid_serial_reset_buffer(mid_serial_inst *inst);

void mid_serial_reset_screen(mid_serial_inst *inst);
void mid_serial_vt100_move(mid_serial_inst *inst, uint8_t x, uint8_t y);

#endif /* MID_SERIAL_H_ */
