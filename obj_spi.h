/*!
 * \file obj_spi.h
 * \brief software SPI protocol header
 * \author Adam LUCAS
 * \date 24/01/2022
 */

#ifndef OBJ_SPI_H_
#define OBJ_SPI_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#ifndef NULL
#define NULL ((void *)0)
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum obj_spi_side_mode
 * \brief SPI protocol master or slave selection
 */
 
typedef enum {
	OBJ_SPI_MASTER_MODE,
	OBJ_SPI_SLAVE_MODE,
}obj_spi_side_mode;

/*!
 * \enum obj_spi_pol_pha_mode
 * \brief SPI protocol clock polarity/phase selection
 */

typedef enum {
	OBJ_SPI_CPOL0_CPHA0, //!< clock polarity rising edge phase leading edge
	OBJ_SPI_CPOL1_CPHA0, //!< clock polarity falling edge phase trailing edge
	OBJ_SPI_CPOL0_CPHA1, //!< clock polarity rising edge phase trailing edge
	OBJ_SPI_CPOL1_CPHA1, //!< clock polarity falling edge phase leading edge
}obj_spi_pol_pha_mode;

/*!
 * \fn typedef bool(*obj_spi_pin_get_state)(void *handle);
 * \brief GPIO pin get actual state function pointer
 * \param[in] handle user defined argument pointer content
 * \return pin state (false : LOW, true : HIGH)
 */

typedef bool(*obj_spi_pin_get_state)(void *handle);

/*!
 * \fn typedef void(*obj_spi_pin_set_state)(void *handle, bool state);
 * \brief GPIO pin set actual state function pointer
 * \param[in] handle user defined argument pointer content
 * \param[in] state pin state (false : LOW, true : HIGH)
 * \return None
 */

typedef void(*obj_spi_pin_set_state)(void *handle, bool state);

/*!
 * \fn typedef void(*obj_spi_timer_start)(void *handle, bool start)
 * \brief TIMER peripheral start or stop counting function pointer
 * \param[in] handle user defined argument pointer content
 * \param[in] start (false : stop, true : start)
 * \return None
 */
 
typedef void(*obj_spi_timer_start)(void *handle, bool start);

/*!
 * \union obj_spi_io
 * \brief intercompatibility for any SPI pin (MOSI, MISO, SCK, SS) considering if instance is in master or slave mode
 */

typedef union {
	obj_spi_pin_get_state get_state;
	obj_spi_pin_set_state set_state;
}obj_spi_io;

/*!
 * \struct obj_spi_init_param
 * \brief user modifiable parameters to init obj_spi_ctx structure through obj_spi_init() routine
 */

typedef struct {
	obj_spi_side_mode side;						//!< master or slave mode selection
	obj_spi_pol_pha_mode pol_pha;			//!< clock polarity and phase selection
	uint32_t frequency;								//!< protocol frequency
	obj_spi_timer_start timer_start;	//!< timer start function pointer
	obj_spi_io sck_pin;								//!< sck pin set or get state considering being in master or slave mode
	obj_spi_io miso_pin;							//!< miso pin set or get state considering being in master or slave mode
	obj_spi_io mosi_pin;							//!< mosi pin set or get state considering being in master or slave mode
	obj_spi_io *ss_pin;								//!< ss pin array set or get state considering being in master or slave mode
	uint8_t ss_pin_num;								//!< ss pin array length
	void *handle;											//!< all functions pointer user defined handle argument
}obj_spi_init_param;

/*!
 * \struct obj_spi_ctx
 * \brief user declared instance contents. DO NOT MODIFY use obj_spi_init() instead
 */

typedef struct {
	obj_spi_init_param init_param;		//!< user defined init parameters
	uint16_t data_byte_index;					//!< transmitted data index in byte
	uint8_t data_bit_index;						//!< transmitted data index n bit from n byte
	bool transmitting;								//!< transmitting flag
	bool receiving;										//!< receiving flag
	uint8_t *data;										//!< transmit data pointer
	uint16_t len;											//!< data length
	uint32_t ss_mask;									//!< active ss pins to transmit data
	bool sck_toggle_state;						//!< sck toggling actual state
	bool last_tick;										//!< glitch last tick to send correctly data
}obj_spi_ctx;


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void obj_spi_init(obj_spi_ctx *ctx, obj_spi_init_param *init_param);
void obj_spi_transmit(obj_spi_ctx *ctx, uint8_t *data, uint16_t len, uint32_t ss_mask);
void obj_spi_receive(obj_spi_ctx *ctx, uint8_t *data, uint16_t len, uint32_t ss_mask);

void obj_spi_process(obj_spi_ctx *ctx);

bool obj_spi_busy(obj_spi_ctx *ctx);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif /* OBJ_SPI_H_ */
