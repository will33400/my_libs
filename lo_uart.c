/*
 * lo_uart.c
 *
 * Created: 9/11/2022 11:13:51 AM
 *  Author: Adam
 */ 

#include "lo_uart.h"


void lo_uart_init(lo_uart_inst *inst, lo_uart_param *param) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_uart_init();
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_uart_param temp_param = {
		.baud = param->baudrate <= 2400 ? HW_UART_BAUD_2400 :
				param->baudrate <= 4800 ? HW_UART_BAUD_4800 :
				param->baudrate <= 9600 ? HW_UART_BAUD_9600 :
				param->baudrate <= 19200 ? HW_UART_BAUD_19200 :
				param->baudrate <= 57600 ? HW_UART_BAUD_57600 :
				param->baudrate <= 115200 ? HW_UART_BAUD_115200 :
				param->baudrate <= 230769 ? HW_UART_BAUD_230769 :
				param->baudrate <= 461538 ? HW_UART_BAUD_461538 :
				param->baudrate <= 923076 ? HW_UART_BAUD_923076 :
				param->baudrate <= 2250000 ? HW_UART_BAUD_2250000 : 
				HW_UART_BAUD_4500000,
		.uart = param->periph_sel == 0 ? USART1 :
				param->periph_sel == 1 ? USART2 : USART3,
	};
	hw_uart_init(inst, &temp_param);
#endif
}

void lo_uart_set_rx_cb(lo_uart_inst *inst, lo_uart_rx_cb rx_cb, void *rx_handle) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_uart_set_rx_cb(rx_cb, rx_handle);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_uart_set_rx_cb(inst, rx_cb, rx_handle);
#endif
}

void lo_uart_putchar(lo_uart_inst *inst, uint16_t data, uint32_t timeout) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_uart_putchar(data, timeout);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_uart_putchar(inst, (int8_t)data, timeout);
#endif
}

void lo_uart_puts(lo_uart_inst *inst, void *data, uint16_t size, uint32_t timeout) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_uart_puts(data, size, timeout);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_uart_puts(inst, (int8_t *)data, size, timeout);
#endif
}

uint16_t lo_uart_getchar(lo_uart_inst *inst) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	return hw_uart_getchar();
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    return 0;
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	return hw_uart_getchar(inst);
#endif
}
