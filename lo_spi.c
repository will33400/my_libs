/*
 * lo_spi.c
 *
 * Created: 9/17/2022 10:34:13 AM
 *  Author: Adam
 */ 

#include "lo_spi.h"


void lo_spi_init(lo_spi_inst *inst, lo_spi_param *param) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	(void)inst;
	(void)param;
	hw_spi_init();
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	(void)inst;
	(void)param;
#endif
}

void lo_spi_deinit(lo_spi_inst *inst) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	(void)inst;
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	(void)inst;
#endif
}

void lo_spi_set_cb(lo_spi_inst *inst, lo_spi_cb cb, void *handle) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	(void)inst;
	hw_spi_set_cb(cb, handle);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	(void)inst;
	(void)cb;
	(void)handle;
#endif
}

void lo_spi_exchange(lo_spi_inst *inst, uint8_t *data, uint16_t len) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_spi_exchange(data, len);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	(void)inst;
	(void)data;
	(void)len;
#endif
}

void lo_spi_write(lo_spi_inst *inst, uint8_t *data, uint16_t len) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_spi_write(data, len);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	(void)inst;
	(void)data;
	(void)len;
#endif
}

void lo_spi_read(lo_spi_inst *inst, uint8_t *data, uint16_t len) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_spi_read(data, len);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	(void)inst;
	(void)data;
	(void)len;
#endif
}
