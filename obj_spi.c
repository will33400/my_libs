/*!
 * \file obj_spi.c
 * \brief software SPI protocol functions
 * \author Adam LUCAS
 * \date 24/01/2022
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_spi.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
 * \fn static bool obj_spi_get_state_default_cb(void *handle);
 * \brief GPIO pin get actual state function pointer
 * \param[in] handle user defined argument pointer content
 * \return pin state (false : LOW, true : HIGH)
 */

static bool obj_spi_get_state_default_cb(void *handle) {
	(void)handle;
	return false;
}

//*****************************************************************************/
/*!
 * \fn static void obj_spi_set_state_default_cb(void *handle, bool state);
 * \brief GPIO pin set actual state function pointer
 * \param[in] handle user defined argument pointer content
 * \param[in] state pin state (false : LOW, true : HIGH)
 * \return None
 */

static void obj_spi_set_state_default_cb(void *handle, bool state) {
	(void)handle;
	(void)state;
}

//*****************************************************************************/
/*!
 * \fn static void obj_spi_timer_start_default_cb(void *handle,bool start)
 * \brief TIMER peripheral start or stop counting function pointer
 * \param[in] handle user defined argument pointer content
 * \param[in] start (false : stop, true : start)
 * \return None
 */

static void obj_spi_timer_start_default_cb(void *handle,bool start) {
	(void)handle;
	(void)start;
}

static void inline obj_spi_master_transmit_routine(obj_spi_ctx *ctx) {
	switch(ctx->init_param.pol_pha) {
		case OBJ_SPI_CPOL0_CPHA0: {	
			
			if(ctx->last_tick == true) {
				ctx->init_param.sck_pin.set_state(
						ctx->init_param.handle, 
						ctx->sck_toggle_state
				);
				
				if(ctx->sck_toggle_state == false) {
					ctx->last_tick = false;
					ctx->transmitting = false;
					ctx->init_param.timer_start(ctx->init_param.handle, false);
					return;
				}
			}
			
			if(ctx->sck_toggle_state == false) {
				ctx->init_param.mosi_pin.set_state(
						ctx->init_param.handle, 
						(
								ctx->data[ctx->data_byte_index]>>
								(7-ctx->data_bit_index)
						)&1
				);
				ctx->data_bit_index++;
				if(ctx->data_bit_index / 8) {
					ctx->data_bit_index = 0;
					ctx->data_byte_index++;
					if(ctx->data_byte_index == ctx->len) {
						ctx->last_tick = true;
					}
				}
			}
			ctx->init_param.sck_pin.set_state(
					ctx->init_param.handle, 
					ctx->sck_toggle_state
			);
			ctx->sck_toggle_state ^= true;
		}
		break;
		case OBJ_SPI_CPOL1_CPHA0: {
			
		}
		break;
		case OBJ_SPI_CPOL0_CPHA1: {
			
		}
		break;
		case OBJ_SPI_CPOL1_CPHA1: {
			
		}
		break;
	}
}

static inline void obj_spi_slave_transmit_routine(obj_spi_ctx *ctx) {
	switch(ctx->init_param.pol_pha) {
		case OBJ_SPI_CPOL0_CPHA0: {
			
		}
		break;
		case OBJ_SPI_CPOL1_CPHA0: {
			
		}
		break;
		case OBJ_SPI_CPOL0_CPHA1: {
			
		}
		break;
		case OBJ_SPI_CPOL1_CPHA1: {
			
		}
		break;
	}
}

//*****************************************************************************/
/*!
* \fn void obj_spi_init(obj_spi_ctx *ctx, obj_spi_init_param *init_param);
* \brief object oriented SPI protocol init routine 
* \param[in] ctx user instance pointer. DO NOT MODIFY
* \param[in] init_param user defined parameters
* \return None
*/

void obj_spi_init(obj_spi_ctx *ctx, obj_spi_init_param *init_param) {
	ctx->init_param = *init_param;
	
	if(ctx->init_param.timer_start == NULL) {
		ctx->init_param.timer_start = obj_spi_timer_start_default_cb;
	}
	switch(ctx->init_param.side) {
		case OBJ_SPI_MASTER_MODE: {
			if(ctx->init_param.miso_pin.get_state == NULL) {
				ctx->init_param.miso_pin.get_state = obj_spi_get_state_default_cb;
			}
			if(ctx->init_param.mosi_pin.set_state == NULL) {
				ctx->init_param.mosi_pin.set_state = obj_spi_set_state_default_cb;
			}
			if(ctx->init_param.sck_pin.set_state == NULL) {
				ctx->init_param.sck_pin.set_state = obj_spi_set_state_default_cb;
			}
			
			for(uint8_t i = 0; i < ctx->init_param.ss_pin_num; i++) {
				if(ctx->init_param.ss_pin[i].set_state == NULL) {
					ctx->init_param.ss_pin[i].set_state = obj_spi_set_state_default_cb;
				}
			}
			
			ctx->init_param.mosi_pin.set_state(ctx->init_param.handle, false);
			
			if(
					(ctx->init_param.pol_pha == OBJ_SPI_CPOL1_CPHA0) || 
					(ctx->init_param.pol_pha == OBJ_SPI_CPOL1_CPHA1)
			) {
				ctx->init_param.sck_pin.set_state(ctx->init_param.handle, true);
			}
			else {
				ctx->init_param.sck_pin.set_state(ctx->init_param.handle, false);
			}
		}
		break;
		case OBJ_SPI_SLAVE_MODE: {
			if(ctx->init_param.miso_pin.set_state == NULL) {
				ctx->init_param.miso_pin.set_state = obj_spi_set_state_default_cb;
			}
			if(ctx->init_param.mosi_pin.get_state == NULL) {
				ctx->init_param.mosi_pin.get_state = obj_spi_get_state_default_cb;
			}
			if(ctx->init_param.sck_pin.get_state == NULL) {
				ctx->init_param.sck_pin.get_state = obj_spi_get_state_default_cb;
			}
			
			for(uint8_t i = 0; i < ctx->init_param.ss_pin_num; i++) {
				if(ctx->init_param.ss_pin[i].get_state == NULL) {
					ctx->init_param.ss_pin[i].get_state = obj_spi_get_state_default_cb;
				}
			}
			
			ctx->init_param.miso_pin.set_state(ctx->init_param.handle, false);
			
		}
		break;
	}
	ctx->data_bit_index = 0;
	ctx->data_byte_index = 0;
	ctx->last_tick = false;
	ctx->receiving = false;
	ctx->transmitting = false;
}

//*****************************************************************************/
/*!
* \fn void obj_spi_transmit(obj_spi_ctx *ctx, uint8_t *data, uint16_t len, uint32_t ss_mask);
* \brief object oriented SPI protocol transmit routine
* \param[in] ctx user instance pointer. DO NOT MODIFY
* \param[in] data protocol data to send
* \param[in] len data length
* \param[in] ss_mask slave mask to transmit data
* \return None
*/

void obj_spi_transmit(obj_spi_ctx *ctx, uint8_t *data, uint16_t len, uint32_t ss_mask) {
	//while((ctx->transmitting == true) || (ctx->receiving == true));
	
	ctx->data = data;
	ctx->len = len;
	ctx->ss_mask = ss_mask;
	ctx->data_bit_index = 0;
	ctx->data_byte_index = 0;
	
	ctx->transmitting = true;
	ctx->last_tick = false;
	
	ctx->init_param.timer_start(ctx->init_param.handle, true);
}

//*****************************************************************************/
/*!
* \fn void obj_spi_receive(obj_spi_ctx *ctx, uint8_t *data, uint16_t len, uint32_t ss_mask);
* \brief object oriented SPI protocol transmit routine
* \param[in] ctx user instance pointer. DO NOT MODIFY
* \param[out] data protocol data to receive
* \param[in] len data length
* \param[in] ss_mask slave mask to receive data
* \return None
*/

void obj_spi_receive(obj_spi_ctx *ctx, uint8_t *data, uint16_t len, uint32_t ss_mask) {
	//while((ctx->transmitting == true) || (ctx->receiving == true));
	
	ctx->data = data;
	ctx->len = len;
	ctx->ss_mask = ss_mask;
	ctx->data_bit_index = 0;
	ctx->data_byte_index = 0;
	
	ctx->receiving = true;
	ctx->last_tick = false;
	
	ctx->init_param.timer_start(ctx->init_param.handle, true);
}

//*****************************************************************************/
/*!
* \fn void obj_spi_process(obj_spi_ctx *ctx);
* \brief object oriented SPI protocol processing routine to implement into semaphore or timer interrupt callback
* \param[in] ctx user instance pointer. DO NOT MODIFY
* \return None
*/

void obj_spi_process(obj_spi_ctx *ctx) {	
	//if((ctx->transmitting == false) && (ctx->receiving == false)) return;
	
	if(ctx->transmitting == true) {
		switch(ctx->init_param.side) {
			case OBJ_SPI_MASTER_MODE: {
				obj_spi_master_transmit_routine(ctx);
			}
			break;
			case OBJ_SPI_SLAVE_MODE: {
				obj_spi_slave_transmit_routine(ctx);
			}
			break;
		}
	}
	else if(ctx->receiving == true) {
		
	}
}


bool obj_spi_busy(obj_spi_ctx *ctx) {
	return (ctx->transmitting || ctx->receiving);
}
