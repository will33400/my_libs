/*
 * hw_spi.c
 *
 * Created: 7/18/2022 7:48:24 AM
 *  Author: Adam
 */ 

#include "atmega328p_hw_spi.h"

static hw_spi_data hwSpiData;

void hw_spi_default_cb(void *handle) {
	(void)handle;
}

void hw_spi_init(void) {
	SPCR = 0 << SPE              /* SPI module enable: enabled */
	| 0 << DORD                  /* Data order: disabled */
#if HW_SPI_MODE_SEL == HW_SPI_MODE_MASTER
	| 1 << MSTR                  /* Master/Slave select: enabled */
#elif HW_SPI_MODE_SEL == HW_SPI_MODE_SLAVE
	| 0 << MSTR                  /* Master/Slave select: enabled */
#endif
#if HW_SPI_CPOL_SEL	== HW_SPI_CPOL_POS
	| 0 << CPOL                  /* Clock polarity: disabled */
#elif HW_SPI_CPOL_SEL == HW_SPI_CPOL_NEG
	| 1 << CPOL                  /* Clock polarity: disabled */
#endif
#if HW_SPI_CPOL_SEL	== HW_SPI_CPHA_RIS
	| 0 << CPHA                  /* Clock phase: disabled */
#elif HW_SPI_CPOL_SEL == HW_SPI_CPHA_FAL
	| 1 << CPHA                  /* Clock phase: disabled */
#endif
	| 1 << SPIE                  /* SPI interrupt enable: enabled */
#if (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_2) || (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_4)
	| (0 << SPR1) | (0 << SPR0)
#elif (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_8) || (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_16)
	| (0 << SPR1) | (1 << SPR0)
#elif (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_32) || (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_64)
	| (1 << SPR1) | (0 << SPR0)
#elif (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_128)
	| (1 << SPR1) | (1 << SPR0)
#endif
;

#if (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_2) || (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_8) || (HW_SPI_CLOCK_DIV_SEL == HW_SPI_CLOCK_DIV_32)
	SPSR = (1 << SPI2X);
#else
	SPSR = (0 << SPI2X);
#endif
	
	DDRB |= (1<<PORTB2) | (1<<PORTB3) | (1<<PORTB5);
	DDRB &=  ~(1<<PORTB4);
	PORTB &= ~(1<<PORTB2) | (1<<PORTB3) | (1<<PORTB5);
	
	sei();

	hwSpiData.status = HW_SPI_STATUS_FREE;
	hwSpiData.cb = hw_spi_default_cb;
}

void hw_spi_enable(bool enable) {
	if(enable) {
		SPCR |= (1 << SPE);
	}
	else {
		SPCR &= ~(1 << SPE);
	}
}

uint8_t hw_spi_exchange_byte(uint8_t data) {
	hwSpiData.data = &data;
	hwSpiData.size = 1;
	hwSpiData.type = HW_SPI_MODE_READ;
	hwSpiData.status = HW_SPI_STATUS_BUSY;

	SPDR = *hwSpiData.data;
	while (hw_spi_is_busy());
	return data;
}

void hw_spi_exchange(uint8_t *data, uint8_t size) {
	hwSpiData.data = data;
	hwSpiData.size = size;
	hwSpiData.type = HW_SPI_MODE_EXCHANGE;
	hwSpiData.status = HW_SPI_STATUS_BUSY;

	SPDR = *hwSpiData.data;
}

void hw_spi_write(uint8_t *data, uint8_t size) {
	hwSpiData.data = data;
	hwSpiData.size = size;
	hwSpiData.type = HW_SPI_MODE_WRITE;
	hwSpiData.status = HW_SPI_STATUS_BUSY;

	SPDR = *hwSpiData.data;
}

void hw_spi_read(uint8_t *data, uint8_t size) {
	hwSpiData.data = data;
	hwSpiData.size = size;
	hwSpiData.type = HW_SPI_MODE_READ;
	hwSpiData.status = HW_SPI_STATUS_BUSY;

	SPDR = 0xff;
}

void hw_spi_set_cb(hw_spi_tx_done_cb cb, void *handle) {
	hwSpiData.cb = cb;
	hwSpiData.handle = handle;
}

bool hw_spi_is_free(void) {
	return (hwSpiData.status == HW_SPI_STATUS_FREE);
}

bool hw_spi_is_idle(void) {
	return (hwSpiData.status == HW_SPI_STATUS_IDLE);
}

bool hw_spi_is_busy(void) {
	return (hwSpiData.status == HW_SPI_STATUS_BUSY);
}

bool hw_spi_is_done(void) {
	return (hwSpiData.status == HW_SPI_STATUS_DONE);
}

ISR(SPI_STC_vect) {
	uint8_t rdata = SPDR;
	uint8_t wdata = 0xff;

	if (hwSpiData.type == HW_SPI_MODE_READ) {
		*hwSpiData.data = rdata;
	}

	hwSpiData.data++;

	if (hwSpiData.type == HW_SPI_MODE_WRITE) {
		wdata = *hwSpiData.data;
	}

	hwSpiData.size--;
	
	if (hwSpiData.size != 0) {
		SPDR = wdata;
	}
	else {
		hwSpiData.status = HW_SPI_STATUS_DONE;
		hwSpiData.cb(hwSpiData.handle);
	}
}