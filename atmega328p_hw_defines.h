/*
 * atmega328p_hw_defines.h
 *
 * Created: 9/14/2022 6:50:39 AM
 *  Author: Adam
 */ 


#ifndef ATMEGA328P_HW_DEFINES_H_
#define ATMEGA328P_HW_DEFINES_H_


#define F_CPU							8000000LL


#define HW_UART_MODE					HW_UART_ASYNC
#define HW_UART_BAUD					9600LL
#define HW_UART_IO						HW_UART_IN_OUT
#define HW_UART_DATA_SIZE				HW_UART_8_BITS
#define HW_UART_PARITY					HW_UART_NONE
#define HW_UART_STOP_BIT				HW_UART_1_STOP
#define HW_UART_BLOCKING_MODE			HW_UART_IRQ



#define HW_I2C_MODE						HW_I2C_MASTER
#define HW_I2C_FREQ						250000UL
#define HW_I2C_PULL_UP					HW_I2C_ENABLE



#define HW_TC0_MODE_SEL					HW_TC_CTC
#define HW_TC1_MODE_SEL					HW_TC1_CTC
#define HW_TC2_MODE_SEL					HW_TC_DISABLED

#define HW_TC0_PSCALE_SEL				HW_TC_NO_PRESCALE
#define HW_TC1_PSCALE_SEL				HW_TC_NO_PRESCALE



#define HW_SPI_MODE_SEL					HW_SPI_MODE_MASTER
#define HW_SPI_CPOL_SEL					HW_SPI_CPOL_POS
#define HW_SPI_CPHA_SEL					HW_SPI_CPHA_RIS
#define HW_SPI_CLOCK_DIV_SEL			HW_SPI_CLOCK_DIV_128


#endif /* ATMEGA328P_HW_DEFINES_H_ */