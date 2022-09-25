/*
 * lo_spi.h
 *
 * Created: 9/17/2022 10:34:22 AM
 *  Author: Adam
 */ 


#ifndef LO_SPI_H_
#define LO_SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include "lo_defines.h"

#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
#include "atmega328p/atmega328p_hw_spi.h"
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
#include "pic16f628a_hw_usart.h"
#endif

typedef void(*lo_spi_cb)(void *handle);

typedef enum {
	LO_SPI_CPOL_POS,
	LO_SPI_CPOL_NEG,	
}lo_spi_cpol;

typedef enum {
	LO_SPI_CPHA_RIS,
	LO_SPI_CPHA_FAL,	
}lo_spi_cpha;

typedef
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
void *
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
int
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
int
#endif
lo_spi_inst;

typedef struct {
	uint32_t frequency;
	lo_spi_cpol cpol;
	lo_spi_cpha cpha;
	uint16_t mosi_pin_num;
	uint16_t miso_pin_num;
	uint16_t sck_pin_num;
	uint16_t cs_pin_num;
	bool cs_enable;
	bool cs_logic;
}lo_spi_param;

void lo_spi_init(lo_spi_inst *inst, lo_spi_param *param);
void lo_spi_deinit(lo_spi_inst *inst);
void lo_spi_set_cb(lo_spi_inst *inst, lo_spi_cb cb, void *handle);
void lo_spi_exchange(lo_spi_inst *inst, uint8_t *data, uint16_t len);
void lo_spi_write(lo_spi_inst *inst, uint8_t *data, uint16_t len);
void lo_spi_read(lo_spi_inst *inst, uint8_t *data, uint16_t len);


#endif /* LO_SPI_H_ */
