/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC16F628A_HW_USART_H_
#define	PIC16F628A_HW_USART_H_

 
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>

#include "pic16f628a_hw_defines.h"


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
#define HW_USART_X(fosc, baudrate, mode) ((fosc / (mode * baudrate)) - 1)
#define HW_USART_ERROR(fosc, usart_x, baudrate, mode) (((fosc/(mode*(usart_x + 1))) - baudrate) / baudrate)

#define HW_USART_ASYNC (64UL)
#define HW_USART_SYNC (4UL)

#define HW_USART_ASYNC_MASK (0)
#define HW_USART_SYNC_MASK (1)

#define HW_USART_LENGHT_8_BITS (0)
#define HW_USART_LENGHT_9_BITS (1)

#define HW_USART_IO_IN (0)
#define HW_USART_IO_OUT (1)
#define HW_USART_IO_IO (2)

#define HW_USART_SLAVE (0)
#define HW_USART_MASTER (1)

#ifndef HW_USART_MODE
#warning HW_USART_MODE undefined
#define HW_USART_MODE HW_USART_ASYNC
#endif

#if HW_USART_MODE == HW_USART_SYNC
#ifndef HW_USART_SYNC_SIDE
#warning HW_USART_SYNC_SIDE undefined for synchronous mode
#define HW_USART_SYNC_SIDE HW_USART_MASTER
#endif
#endif

#ifndef HW_USART_IO
#warning HW_USART_IO undefined
#define HW_USART_IO HW_USART_IO_IO
#endif

#ifndef HW_USART_LENGHT
#warning HW_USART_LENGHT undefined
#define HW_USART_LENGHT HW_USART_LENGHT_8_BITS
#endif

#ifndef HW_USART_BAUD
#warning HW_USART_BAUD undefined
#define HW_USART_BAUD 9600
#endif

#ifndef FOSC
#warning FOSC undefined
#define FOSC 16000000
#endif

/*

PIR1 //interrupt flag bits
PIE1 //interrupt enable bits
RCSTA //receive status and control register
TXSTA //transmit status and control register
TRISB //GPIO for TX / RX
SPBRG //baud rate generator register
RCREG //USART transmit data register
TXREG //USART receive data register

 */

typedef void(*hw_usart_rx_cb)(void *handle);

typedef struct {
    uint8_t last_char;
    hw_usart_rx_cb cb;
    void *handle;
}hw_usart_data;


void hw_usart_init();
void hw_usart_set_rx_cb(hw_usart_rx_cb rx_cb, void *rx_handle);
void hw_usart_putchar(uint16_t data, uint32_t timeout);
void hw_usart_puts(void *data, uint16_t size, uint32_t timeout);
void hw_usart_get(void *data, uint16_t size, uint32_t timeout);
uint16_t hw_usart_getchar();

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

