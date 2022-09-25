/*!
 * \file obj_pwm.c
 * \brief object oriented pwm functions
 * \author Adam LUCAS
 * \date 9/7/2021 6:46:23 AM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_pwm.h"

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
* \fn static void boj_pwm_pulse_end_default_callback(void *lo_pwm_user_handle);
* \brief object oriented pwm default user defined end pulse callback routine. MANDATORY DO NOT MODIFY
* \param[inout] handle user defined data pointer
* \return None
*/

static void obj_pwm_pulse_end_default_callback(void *handle) {
	(void)handle;
}

//*****************************************************************************/
/*!
* \fn uint32_t obj_pwm_pow(uint32_t val, uint32_t _pow);
* \brief local math function power
* \param[in] val any value to power
* \param[in] _pow power value
* \return power result
*/
/*
static uint32_t obj_pwm_pow(uint32_t val, uint32_t _pow) {
	uint32_t pow_temp = val;
	
	if(_pow == 0) return 1;
	
	for(uint32_t i = 0; i < _pow - 1; i++) {
		pow_temp *= val;
	}
	
	return pow_temp;
}
*/
//*****************************************************************************/
/*!
* \fn void obj_pwm_init(
*		ctx_obj_pwm *ctx,
*		obj_pwm_resolution resolution,
*		uint32_t pulse_period,
*		uint32_t timer_period,
*		obj_pwm_gpio_set_state gpio_set_state,
*		void *handle,
*		obj_pwm_gpio_logic logic
* );
* \brief object oriented pwm init ctx data structure
* \param[in] ctx object oriented pwm data structure pointer to init
* \param[in] resolution pwm resolution
* \param[in] pulse_period pwm any pulse period in time unit (has same unit that timer_period)
* \param[in] timer_period pwm user timer used in time unit
* \param[in] gpio_set_state user defined gpio set state function pointer
* \param[in] handle user defined data to adapt other user functions pointers
* \param[in] logic gpio output state
* \return None
*/

void obj_pwm_init(
		ctx_obj_pwm *ctx,
		obj_pwm_resolution resolution,
		uint32_t pulse_period,
		uint32_t timer_period,
		obj_pwm_gpio_set_state gpio_set_state,
		void *handle,
		obj_pwm_gpio_logic logic
) {
	ctx->pulse_period = pulse_period;
	ctx->timer_period = timer_period;
	ctx->gpio_set_state = gpio_set_state;
	ctx->handle = handle;
	ctx->logic = logic;
	
	ctx->one_pulse_period = pulse_period / (resolution * timer_period);
	ctx->res_max_val = (pulse_period / timer_period) - ctx->one_pulse_period;
	//ctx->res_max_val = obj_pwm_pow(2, (uint32_t)resolution) - 1;
	ctx->count_buffer = 0;
	ctx->pwm_enabled = 0;
	
	ctx->end_pulse_flag = false;
	ctx->first_pulse_flag = false;
	ctx->end_pulse_callback = obj_pwm_pulse_end_default_callback;
}

//*****************************************************************************/
/*!
* \fn void obj_pwm_enable(ctx_obj_pwm *ctx, bool enable);
* \brief object oriented pwm enable or disable state
* \param[in] ctx object oriented pwm data structure pointer to init
* \param[in] enable pwm select state (false : disabled, true : enabled)
* \return None
*/

void obj_pwm_enable(ctx_obj_pwm *ctx, bool enable) {
	
	if(enable) {
		if(ctx->res_compare_val == 0) ctx->res_compare_val = 1;
		
		ctx->end_pulse_flag = false;
		ctx->first_pulse_flag = true;
	}
	
	ctx->pwm_enabled = enable;
	ctx->count_buffer = 0;

}

//*****************************************************************************/
/*!
* \fn void obj_pwm_set_effective_val(ctx_obj_pwm *ctx, uint32_t val);
* \brief object oriented pwm set duty cycle for the next obj_pwm_enable() usage
* \param[in] ctx object oriented pwm data structure pointer to init
* \param[in] val pwm value from 0 to <= pwm resolution
* \return None
*/

void obj_pwm_set_effective_val(ctx_obj_pwm *ctx, uint32_t val) {
	val = (val * ctx->one_pulse_period) % (ctx->res_max_val + 1);
	ctx->res_compare_val = val;
}

//*****************************************************************************/
/*!
* \fn void obj_pwm_set_end_pulse_callback(
*		ctx_obj_pwm *ctx,
*		obj_pwm_end_pulse_callback end_pulse_callback
* );
* \brief object oriented pwm set user defined end pulse callback routine
* \param[in] ctx object oriented pwm data structure pointer to init
* \param[in] end_pulse_callback pwm end callback function pointer. The handle corresponds to previous obj_pwm_init() argument routine
* \return None
*/

void obj_pwm_set_end_pulse_callback(
		ctx_obj_pwm *ctx,
		obj_pwm_end_pulse_callback end_pulse_callback
) {
	ctx->end_pulse_callback = end_pulse_callback;
}

//*****************************************************************************/
/*!
* \fn void obj_pwm_timer_callback(ctx_obj_pwm *ctx);
* \brief object oriented pwm timer callback to be used into user timer interrupt routine
* \param[in] ctx object oriented pwm data structure pointer to init
* \return None
*/

void obj_pwm_timer_callback(ctx_obj_pwm *ctx) {
	if(!ctx->pwm_enabled) return;
	
	if(ctx->first_pulse_flag) {
		ctx->gpio_set_state(ctx->handle, true ^ ctx->logic);
		ctx->first_pulse_flag = false;
		return;
	}
	
	ctx->count_buffer++;

	if(ctx->count_buffer == ctx->res_compare_val) {
		ctx->gpio_set_state(ctx->handle, false ^ ctx->logic);
	}
	else if(ctx->count_buffer > ctx->res_max_val) {
		
		if(!ctx->first_pulse_flag) {
			ctx->end_pulse_callback(ctx->handle);
		}
		
		ctx->gpio_set_state(ctx->handle, ctx->pwm_enabled ^ ctx->logic);
		
		ctx->count_buffer = 0;
	}
}
