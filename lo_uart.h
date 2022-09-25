/*
 * lo_uart.h
 *
 * Created: 9/11/2022 11:14:13 AM
 *  Author: Adam
 */ 


#ifndef LO_UART_H_
#define LO_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "lo_defines.h"

#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
#include "atmega328p/atmega328p_hw_uart.h"
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
#include "pic16f628a_hw_usart.h"
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
#include "stm32f103c8_hw_uart.h"
#endif

typedef void(*lo_uart_rx_cb)(void *handle);

typedef enum {
	LO_UART_STOP_BIT_0,
	LO_UART_STOP_BIT_1,
	LO_UART_STOP_BIT_1_5,
	LO_UART_STOP_BIT_2,
}lo_uart_stop_bit;

typedef enum {
	LO_UART_FRAME_8BITS,
	LO_UART_FRAME_9BITS,
	LO_UART_FRAME_10BITS,
	LO_UART_FRAME_12BITS,
	LO_UART_FRAME_16BITS,
}lo_uart_frame;

typedef
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
void *
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
int
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
hw_uart_inst
#endif
lo_uart_inst;

typedef struct {
	uint32_t baudrate;
	lo_uart_stop_bit stop_bit;
	lo_uart_frame frame;
	bool parity;
	uint16_t rx_pin_num;
	uint16_t tx_pin_num;
	uint8_t periph_sel;
}lo_uart_param;


void lo_uart_init(lo_uart_inst *inst, lo_uart_param *param);
void lo_uart_set_rx_cb(lo_uart_inst *inst, lo_uart_rx_cb rx_cb, void *rx_handle);
void lo_uart_putchar(lo_uart_inst *inst, uint16_t data, uint32_t timeout);
void lo_uart_puts(lo_uart_inst *inst, void *data, uint16_t size, uint32_t timeout);
uint16_t lo_uart_getchar(lo_uart_inst *inst);


#endif /* LO_UART_H_ */
