/*
 * File:   pic16f628a_hw_usart.c
 * Author: Adam
 *
 * Created on September 18, 2022, 7:11 PM
 */


#include "pic16f628a_hw_usart.h"

void hw_usart_init() {
#if HW_USART_ERROR(FOSC, HW_USART_X(FOSC, HW_USART_BAUD, HW_USART_MODE), HW_USART_BAUD, HW_USART_MODE) == 0
    STATUSbits.RP = 1;
    SPBRG = HW_USART_X(FOSC, HW_USART_BAUD, HW_USART_MODE);
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    STATUSbits.RP = 0;
    RCSTAbits.SPEN = 1;
#if HW_USART_IO == HW_USART_IO_IN
    STATUSbits.RP = 1;
    PIE1bits.RCIE = 1;
    PIE1bits.TXIE = 0;
    STATUSbits.RP = 0;
    RCSTAbits.RX9 = HW_USART_LENGHT;
    RCSTAbits.CREN = 1;
#elif HW_USART_IO == HW_USART_IO_OUT
    STATUSbits.RP = 1;
    TXSTAbits.TX9 = HW_USART_LENGHT;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 0;
    STATUSbits.RP = 0;
#else
    STATUSbits.RP = 1;
    PIE1bits.RCIE = 1;
    PIE1bits.TXIE = 0;

    TXSTAbits.TX9 = HW_USART_LENGHT;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 0;

    STATUSbits.RP = 0;
    RCSTAbits.RX9 = HW_USART_LENGHT;
    RCSTAbits.CREN = 1;
#endif
#if HW_USART_MODE == HW_USART_SYNC
   RCSTAbits.SREN = 1;
   STATUSbits.RP = 1;
   TXSTAbits.SYNC = HW_USART_SYNC_MASK;
   TXSTAbits.CSRC = HW_USART_SYNC_SIDE;
   STATUSbits.RP = 0;
#else
    STATUSbits.RP = 1;
    TXSTAbits.SYNC = HW_USART_ASYNC_MASK;
    STATUSbits.RP = 0;
#if HW_USART_LENGHT == HW_USART_LENGHT_9_BITS
   RCSTAbits.ADEN = 1;
#endif
#endif
#endif
}

void hw_usart_set_rx_cb(hw_usart_rx_cb rx_cb, void *rx_handle) {
    
}

void hw_usart_putchar(uint16_t data, uint32_t timeout) {
    TXREG = (uint8_t)data;
    for (int i = 0; i < 20 * HW_USART_X(FOSC, HW_USART_BAUD, HW_USART_MODE); i++);
}

void hw_usart_puts(void *data, uint16_t size, uint32_t timeout) {
    
}

void hw_usart_get(void *data, uint16_t size, uint32_t timeout) {
    
}

uint16_t hw_usart_getchar() {
    return 0;
}