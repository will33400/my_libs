/*
 * hw_spi.h
 *
 * Created: 7/18/2022 7:48:08 AM
 *  Author: Adam
 */ 


#ifndef HW_SPI_H_
#define HW_SPI_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdbool.h>
#include <stdint.h>

#include "atmega328p_hw_defines.h"

#define HW_SPI_MODE_MASTER		0
#define HW_SPI_MODE_SLAVE		1

#ifndef HW_SPI_MODE_SEL
#warning HW_SPI_MODE_SEL undefined
#define HW_SPI_MODE_SEL			HW_SPI_MODE_MASTER
#endif

#define HW_SPI_CPOL_POS			0
#define HW_SPI_CPOL_NEG			1

#ifndef HW_SPI_CPOL_SEL
#warning HW_SPI_CPOL_SEL undefined
#define HW_SPI_CPOL_SEL			HW_SPI_CPOL_POS
#endif

#define HW_SPI_CPHA_RIS			0
#define HW_SPI_CPHA_FAL			1

#ifndef HW_SPI_CPHA_SEL
#warning HW_SPI_CPHA_SEL undefined
#define HW_SPI_CPHA_SEL			HW_SPI_CPHA_RIS
#endif

#define HW_SPI_CLOCK_DIV_2		0
#define HW_SPI_CLOCK_DIV_4		1
#define HW_SPI_CLOCK_DIV_8		2
#define HW_SPI_CLOCK_DIV_16		3
#define HW_SPI_CLOCK_DIV_32		4
#define HW_SPI_CLOCK_DIV_64		5
#define HW_SPI_CLOCK_DIV_128	6

#ifndef HW_SPI_CLOCK_DIV_SEL
#warning HW_SPI_CLOCK_DIV_SEL undefined
#define HW_SPI_CLOCK_DIV_SEL	HW_SPI_CLOCK_DIV_128
#endif

/** Function pointer to callback function called by SPI IRQ.
    NULL=default value: No callback function is to be used.
*/
typedef void (*hw_spi_tx_done_cb)(void *handle);

/** Specify whether the SPI transfer is to be uni- or bidirectional.
    A bidirectional transfer (=SPI_EXCHANGE) causes the received data
    to overwrite the buffer with the data to transmit.
*/
typedef enum  {
	HW_SPI_MODE_EXCHANGE, ///< SPI transfer is bidirectional
	HW_SPI_MODE_READ,     ///< SPI transfer reads, writes only 0s
	HW_SPI_MODE_WRITE     ///< SPI transfer writes, discards read data
} hw_spi_mode;

/** Status of the SPI hardware and SPI bus.*/
typedef enum {
	HW_SPI_STATUS_FREE, ///< SPI hardware is not open, bus is free.
	HW_SPI_STATUS_IDLE, ///< SPI hardware has been opened, no transfer ongoing.
	HW_SPI_STATUS_BUSY, ///< SPI hardware has been opened, transfer ongoing.
	HW_SPI_STATUS_DONE  ///< SPI hardware has been opened, transfer complete.
} hw_spi_status;

typedef struct {
	hw_spi_status status;
	uint8_t *data;
	uint8_t size;
	hw_spi_mode type;
	hw_spi_tx_done_cb cb;
	void *handle;
} hw_spi_data;

void hw_spi_init(void);
void hw_spi_enable(bool enable);

uint8_t hw_spi_exchange_byte(uint8_t data);
void hw_spi_exchange(uint8_t *data, uint8_t size);
void hw_spi_write(uint8_t *data, uint8_t size);
void hw_spi_read(uint8_t *data, uint8_t size);

void hw_spi_set_cb(hw_spi_tx_done_cb cb, void *handle);

bool hw_spi_is_free(void);
bool hw_spi_is_idle(void);
bool hw_spi_is_busy(void);
bool hw_spi_is_done(void);

#endif /* HW_SPI_H_ */