/*
 * lo_uart.h
 *
 * Created: 11/8/2021 6:37:31 PM
 *  Author: Adam
 */ 


#ifndef HW_UART_H_
#define HW_UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "atmega328p_hw_defines.h"

#define HW_UART_SYNC		0
#define HW_UART_ASYNC		1
#define HW_UART_ASYNC_FAST 2

#define HW_UART_SPI_MASTER 0
#define HW_UART_SPI_SLAVE	1

#ifndef HW_UART_MODE
#warning HW_UART_MODE undefined
#define HW_UART_MODE HW_UART_ASYNC
#endif

#define HW_UART_BLOCKING		0
#define HW_UART_IRQ				1

#ifndef HW_UART_BLOCKING_MODE
#warning HW_UART_BLOCKING_MODE undefined
#define HW_UART_BLOCKING_MODE HW_UART_BLOCKING
#endif

#ifndef F_CPU
#warning F_CPU undefined
#define F_CPU 8000000LL
#endif

#ifndef HW_UART_BAUD
#warning HW_UART_BAUD undefined
#define HW_UART_BAUD 9600LL
#endif

#define HW_UART_INPUT		0
#define HW_UART_OUTPUT		1
#define HW_UART_IN_OUT		2

#ifndef HW_UART_IO
#warning HW_UART_IO undefined
#define HW_UART_IO HW_UART_IN_OUT
#endif

#define HW_UART_5_BITS		0
#define HW_UART_6_BITS		1
#define HW_UART_7_BITS		2
#define HW_UART_8_BITS		3
#define HW_UART_9_BITS		7

#ifndef HW_UART_DATA_SIZE
#warning HW_UART_DATA_SIZE undefined
#define HW_UART_DATA_SIZE HW_UART_8_BITS
#endif

#define HW_UART_NONE		0
#define HW_UART_EVEN		2
#define HW_UART_ODD			3

#ifndef HW_UART_PARITY
#warning HW_UART_PARITY undefined
#define HW_UART_PARITY HW_UART_NONE
#endif

#define HW_UART_1_STOP		0
#define HW_UART_2_STOP		1

#ifndef HW_UART_STOP_BIT
#warning HW_UART_STOP_BIT undefined
#define HW_UART_STOP_BIT HW_UART_1_STOP
#endif

#if HW_UART_MODE == HW_UART_SYNC
#define UBRR_VAL (F_CPU/(2*HW_UART_BAUD)-1)
#ifndef HW_UART_MODE_2
#warning HW_UART_MODE_2 undefined (HW_UART_SPI_MASTER or HW_UART_SPI_SLAVE ?)
#define HW_UART_MODE_2 HW_UART_SPI_MASTER
#endif
#elif HW_UART_MODE == HW_UART_ASYNC
#define UBRR_VAL (F_CPU/(16*HW_UART_BAUD)-1)
#define REAL_BAUD (F_CPU/(16*(UBRR_VAL+1)))
#define UBRR_BAUD_ERR ((REAL_BAUD*1000)/HW_UART_BAUD-1000) // 0/00
#if HW_UART_DATA_SIZE == HW_UART_5_BITS && HW_UART_PARITY == HW_UART_NONE && (UBRR_BAUD_ERR > 30 || UBRR_BAUD_ERR < -30)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif ((HW_UART_DATA_SIZE == HW_UART_5_BITS && HW_UART_PARITY != HW_UART_NONE) || (HW_UART_DATA_SIZE == HW_UART_6_BITS && HW_UART_PARITY == HW_UART_NONE)) && (UBRR_BAUD_ERR > 25 || UBRR_BAUD_ERR < -25)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif ((HW_UART_DATA_SIZE == HW_UART_6_BITS && HW_UART_PARITY != HW_UART_NONE) || (HW_UART_DATA_SIZE == HW_UART_7_BITS && HW_UART_PARITY == HW_UART_NONE)) && (UBRR_BAUD_ERR > 20 || UBRR_BAUD_ERR < -20)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif ((HW_UART_DATA_SIZE == HW_UART_7_BITS && HW_UART_PARITY != HW_UART_NONE) || (HW_UART_DATA_SIZE == HW_UART_8_BITS && HW_UART_PARITY == HW_UART_NONE)) && (UBRR_BAUD_ERR > 20 || UBRR_BAUD_ERR < -20)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif ((HW_UART_DATA_SIZE == HW_UART_8_BITS && HW_UART_PARITY != HW_UART_NONE) || (HW_UART_DATA_SIZE == HW_UART_9_BITS && HW_UART_PARITY == HW_UART_NONE)) && (UBRR_BAUD_ERR > 15 || UBRR_BAUD_ERR < -15)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif HW_UART_DATA_SIZE == HW_UART_9_BITS && HW_UART_PARITY != HW_UART_NONE && (UBRR_BAUD_ERR > 15 || UBRR_BAUD_ERR < -15)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#endif
#else
#define UBRR_VAL (F_CPU/(8*HW_UART_BAUD)-1)
#define REAL_BAUD (F_CPU/(8*(UBRR_VAL+1)))
#define UBRR_BAUD_ERR ((REAL_BAUD*1000)/HW_UART_BAUD-1000) // 0/00
#if HW_UART_DATA_SIZE == HW_UART_5_BITS && HW_UART_PARITY == HW_UART_NONE && (UBRR_BAUD_ERR > 25 || UBRR_BAUD_ERR < -25)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif ((HW_UART_DATA_SIZE == HW_UART_5_BITS && HW_UART_PARITY != HW_UART_NONE) || (HW_UART_DATA_SIZE == HW_UART_6_BITS && HW_UART_PARITY == HW_UART_NONE)) && (UBRR_BAUD_ERR > 20 || UBRR_BAUD_ERR < -20)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif ((HW_UART_DATA_SIZE == HW_UART_6_BITS && HW_UART_PARITY != HW_UART_NONE) || (HW_UART_DATA_SIZE == HW_UART_7_BITS && HW_UART_PARITY == HW_UART_NONE)) && (UBRR_BAUD_ERR > 15 || UBRR_BAUD_ERR < -15)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif ((HW_UART_DATA_SIZE == HW_UART_7_BITS && HW_UART_PARITY != HW_UART_NONE) || (HW_UART_DATA_SIZE == HW_UART_8_BITS && HW_UART_PARITY == HW_UART_NONE)) && (UBRR_BAUD_ERR > 15 || UBRR_BAUD_ERR < -15)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif ((HW_UART_DATA_SIZE == HW_UART_8_BITS && HW_UART_PARITY != HW_UART_NONE) || (HW_UART_DATA_SIZE == HW_UART_9_BITS && HW_UART_PARITY == HW_UART_NONE)) && (UBRR_BAUD_ERR > 15 || UBRR_BAUD_ERR < -15)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#elif HW_UART_DATA_SIZE == HW_UART_9_BITS && HW_UART_PARITY != HW_UART_NONE && (UBRR_BAUD_ERR > 10 || UBRR_BAUD_ERR < -10)
#warning baud rate uncorrect. Change HW_UART_BAUD or F_CPU
#endif
#endif

typedef void(*hw_uart_rx_cb)(void *handle);

typedef struct {
	int8_t last_char;
#if HW_UART_BLOCKING_MODE == HW_UART_IRQ
	hw_uart_rx_cb rx_cb;
	void *rx_handle;
	
	bool transmit_complete;
#endif
}hw_uart_data;

void hw_uart_init(void);
#if HW_UART_BLOCKING_MODE == HW_UART_IRQ
void hw_uart_set_rx_cb(hw_uart_rx_cb rx_cb, void *rx_handle);
#endif
void hw_uart_putchar(const int8_t data, uint32_t timeout);
void hw_uart_puts(const int8_t *data, uint16_t size, uint32_t timeout);
char hw_uart_getchar(void);

#endif /* HW_UART_H_ */