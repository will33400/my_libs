/*
 * hw_uart.c
 *
 * Created: 11/8/2021 6:37:45 PM
 *  Author: Adam
 */ 

#include "atmega328p_hw_uart.h"

static hw_uart_data hwUartData;

static void hw_uart_receive_refault_cb(void *handle) {
	(void)handle;
}

void hw_uart_init(void) {
	hwUartData.transmit_complete = false;
	
	PRR &= ~(1 << PRUSART0);
	
	UBRR0 = UBRR_VAL;
	UCSR0B = 0x00;
	
	UCSR0C = 0x06;
	UCSR0C |= (HW_UART_PARITY<<UPM00) | (HW_UART_STOP_BIT<<USBS0) |
	((HW_UART_DATA_SIZE&3)<<UCSZ00);
#if HW_UART_MODE == HW_UART_ASYNC
	UCSR0C &= ~((1<<UMSEL01) | (1<<UMSEL00));
#elif HW_UART_MODE == HW_UART_ASYNC_FAST
	UCSR0C &= ~((1<<UMSEL01) | (1<<UMSEL00));
	UCSR0A |= (1<<U2X0);
#elif HW_UART_MODE == HW_UART_SYNC
	UCSR0C |= (0<<UMSEL01) | (1<<UMSEL00);
#endif
#if HW_UART_BLOCKING_MODE == HW_UART_IRQ
	UCSR0B |= (
					((HW_UART_IO == HW_UART_INPUT || HW_UART_IO == HW_UART_IN_OUT) ? 1 : 0)
					<<RXCIE0
			) /*| (
					((HW_UART_IO == HW_UART_OUTPUT || HW_UART_IO == HW_UART_IN_OUT) ? 1 : 0)
					<<TXCIE0
			)*/;
	sei();
	
	hwUartData.rx_cb = hw_uart_receive_refault_cb;
	hwUartData.rx_handle = (void *)0;
#elif HW_UART_BLOCKING_MODE == HW_UART_BLOCKING
	UCSR0B &= ~((1<<RXCIE0) | (1<<TXCIE0));
#endif

	#if HW_UART_IO == HW_UART_INPUT
	UCSR0B |= (1<<RXEN0) | (((HW_UART_DATA_SIZE&4)>>2)<<UCSZ02);
	#elif HW_UART_IO == HW_UART_OUTPUT
	UCSR0B |= (1<<TXEN0) | (((HW_UART_DATA_SIZE&4)>>2)<<UCSZ02);
	#else
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (((HW_UART_DATA_SIZE&4)>>2)<<UCSZ02);
	#endif
}

#if HW_UART_BLOCKING_MODE == HW_UART_IRQ

void hw_uart_set_rx_cb(hw_uart_rx_cb rx_cb, void *rx_handle) {
	hwUartData.rx_cb = rx_cb;
	hwUartData.rx_handle = rx_handle;
}
#endif

void hw_uart_putchar(const int8_t data, uint32_t timeout) {

	#if HW_UART_BLOCKING_MODE == HW_UART_BLOCKING
	while (!(UCSR0A&(1<<UDRE0)));
	#elif HW_UART_BLOCKING_MODE == HW_UART_IRQ
	uint32_t timeout_temp = timeout;
	while (!(UCSR0A&(1<<UDRE0)) && timeout_temp > 0) {
		timeout_temp--;
	}
	if(timeout_temp == 0) return;
	#endif

	UDR0 = data;
	
	#if HW_UART_BLOCKING_MODE == HW_UART_BLOCKING
	#elif HW_UART_BLOCKING_MODE == HW_UART_IRQ
	timeout_temp = timeout;
	while (!(UCSR0A&(1<<TXC0)) && timeout_temp > 0) {
		timeout_temp--;
	}
	#endif
	
}

void hw_uart_puts(const int8_t *data, uint16_t size, uint32_t timeout) {
	for(uint16_t i = 0; i < size; i++) {
		hw_uart_putchar(data[i], timeout);
	}
}

char hw_uart_getchar(void) {
#if HW_UART_BLOCKING_MODE == HW_UART_IRQ
	return hwUartData.last_char;
#elif HW_UART_BLOCKING_MODE == HW_UART_BLOCKING
	while(!(UCSR0A&(1<<RXC0)));
	hwUartData.last_char = UDR0;
	return hwUartData.last_char;
#endif
}

#if HW_UART_BLOCKING_MODE == HW_UART_IRQ

ISR(USART_RX_vect, ISR_BLOCK) {
	UCSR0A |= (1<<RXC0);

	hwUartData.last_char = UDR0;

	hwUartData.rx_cb(hwUartData.rx_handle);
	
}
#endif