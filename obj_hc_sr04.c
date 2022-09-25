/*!
 * \file obj_hc_sr04.c
 * \brief object oriented hc_sr04 device functions
 * \author Adam LUCAS
 * \date 7/16/2021 5:39:34 PM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_hc_sr04.h"

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
* \fn void obj_hc_sr04_init(
*		ctx_hc_sr04 *ctx,
*		void *handle,
*		hc_sr04_delay_us delay_us,
*		hc_sr04_gpio_set_state trigger_set_state,
*		hc_sr04_mode mode
* );
* \brief object oriented hc_sr04 device init routine
* \param[in] ctx object struct pointer
* \param[in] handle object oriented user defined struct handle pointer
* \param[in] delay_us delay in us user defined function pointer
* \param[in] trigger_set_state gpio trigger set state user defined function pointer
* \param[in] mode device working mode
* \return None
*/

void obj_hc_sr04_init(
		ctx_hc_sr04 *ctx,
		void *handle,
		hc_sr04_delay_us delay_us,
		hc_sr04_gpio_set_state trigger_set_state,
		hc_sr04_mode mode
) {
	ctx->handle = handle;
	ctx->delay_us = delay_us;
	ctx->trigger_set_state = trigger_set_state;
	
	ctx->measuring = false;
	ctx->echo_previous_state = false;
	
	ctx->mode = mode;
}

//*****************************************************************************/
/*!
* \fn void obj_hc_sr04_echo_get_state_callback(ctx_hc_sr04 *ctx, bool state);
* \brief object oriented hc_sr04 device gpio echo pin get state callback user defined interrupt routine
* \param[in] ctx object struct pointer
* \param[in] state gpio echo pin get state (true : HIGH, false : LOW)
* \return None
*/

void obj_hc_sr04_echo_get_state_callback(ctx_hc_sr04 *ctx, bool state) {
	if(ctx->echo_previous_state != state) {
		ctx->echo_previous_state = state;
		if(state) {
			ctx->counting = true;
		}
		else {
			ctx->counting = false;
			ctx->sensor_value.value_us = ctx->timer_count;
			ctx->timer_count = 0;
			
			ctx->sensor_value.value_mm =
			ctx->sensor_value.value_us * OBJ_HC_SR04_SOUND_VELOCITY / 2000;
			
			ctx->measuring = false;
			
			if(ctx->mode == HC_SR04_MODE_MULTIPLE) {
				obj_hc_sr04_start_measure(ctx);
			}
		}
	}
	
}

//*****************************************************************************/
/*!
* \fn void obj_hc_sr04_timer_get_count_1us_callback(ctx_hc_sr04 *ctx);
* \brief object oriented hc_sr04 device timer 1us callback user defined interrupt routine
* \param[in] ctx object struct pointer
* \return None
*/

void obj_hc_sr04_timer_get_count_1us_callback(ctx_hc_sr04 *ctx) {
	if(ctx->counting) {
		ctx->timer_count++;
	}
}

//*****************************************************************************/
/*!
* \fn void obj_hc_sr04_start_measure(ctx_hc_sr04 *ctx);
* \brief object oriented hc_sr04 device start measure routine
* \param[in] ctx object struct pointer
* \return None
*/

void obj_hc_sr04_start_measure(ctx_hc_sr04 *ctx) {
	if(ctx->measuring || ctx->mode == HC_SR04_MODE_DISABLED) return;
	
	ctx->measuring = true;
	ctx->trigger_set_state(ctx->handle, true);
	ctx->delay_us(ctx->handle, 10);
	ctx->trigger_set_state(ctx->handle, false);
}

//*****************************************************************************/
/*!
* \fn void obj_hc_sr_04_set_mode(ctx_hc_sr04 *ctx, hc_sr04_mode mode);
* \brief object oriented hc_sr04 device edit working mode routine
* \param[in] ctx object struct pointer
* \param[in] mode device working mode
* \return None
*/

void obj_hc_sr_04_set_mode(ctx_hc_sr04 *ctx, hc_sr04_mode mode) {
	ctx->mode = mode;
}

//*****************************************************************************/
/*!
* \fn uint32_t obj_hc_sr04_get_value_us(ctx_hc_sr04 *ctx);
* \brief object oriented hc_sr04 device get value routine
* \param[in] ctx object struct pointer
* \return value in us
*/

uint32_t obj_hc_sr04_get_value_us(ctx_hc_sr04 *ctx) {
	return ctx->sensor_value.value_us;
}

//*****************************************************************************/
/*!
* \fn uint32_t obj_hc_sr04_get_value_mm(ctx_hc_sr04 *ctx);
* \brief object oriented hc_sr04 device get value routine
* \param[in] ctx object struct pointer
* \return value in mm
*/

uint32_t obj_hc_sr04_get_value_mm(ctx_hc_sr04 *ctx) {
	return ctx->sensor_value.value_mm;
}