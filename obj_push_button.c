/*!
 * \file obj_push_button.c
 * \brief object oriented push button functions
 * \author Adam LUCAS
 * \date 9/4/2021 2:58:29 PM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_push_button.h"

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
* \fn <function declaration>
* \brief <description>
* \param[in] <argument name> <description>
* \param[out] <argument name> <description>
* \return [<anything>] [None]
*/

void obj_push_button_init(
		obj_push_button_ctx *ctx,
		obj_push_button_routine_callback routine_callback,
		obj_push_button_gpio_pin_get_state pin_get_state_callback,
		obj_push_button_mode mode,
		bool spam_enabled,
		uint32_t resolution,
		uint32_t spam_delay,
		void *handle
) {
	ctx->routine_callback = routine_callback;
	ctx->pin_get_state_callback = pin_get_state_callback;
	ctx->mode = mode;
	ctx->spam_enabled = spam_enabled;
	ctx->count_enable = false;
	ctx->resolution = resolution;
	ctx->spam_delay = spam_delay;
	ctx->handle = handle;
	ctx->event = OBJ_PUSH_BUTTON_NOT_PUSHED;
	ctx->spam_wait = false;
	ctx->spam_current_delay = 0;
	ctx->current_count = 0;
	
	ctx->pb_current_state = ctx->pin_get_state_callback(ctx->handle);
}

obj_push_button_event obj_push_button_get_event_flag(obj_push_button_ctx *ctx) {
	return ctx->event;
}

void obj_push_button_gpio_state_event_callback(obj_push_button_ctx *ctx) {
	bool state = ctx->pin_get_state_callback(ctx->handle);
	
	if(
			(ctx->mode == OBJ_PUSH_BUTTON_FALLING_EDGE && state == false) ||
			(ctx->mode == OBJ_PUSH_BUTTON_RISING_EDGE && state == true) ||
			(ctx->mode == OBJ_PUSH_BUTTON_SIMPLE && ctx->event == OBJ_PUSH_BUTTON_NOT_PUSHED)
	) {
		if(ctx->spam_enabled) {
			ctx->event = OBJ_PUSH_BUTTON_WAIT_SPAM;
		}
		else {
			ctx->event = OBJ_PUSH_BUTTON_PUSHED;
		}
	}
	else {
		ctx->event = OBJ_PUSH_BUTTON_NOT_PUSHED;
	}
	
	if(ctx->pb_current_state != state) {
		ctx->pb_current_state = state;
		ctx->count_enable = true;
		ctx->spam_wait = false;
		ctx->spam_current_delay = 0;
		ctx->current_count = ctx->resolution;
	}
	
	
}

void obj_push_button_timer_get_count_callback(obj_push_button_ctx *ctx) {
	
	if(!ctx->count_enable) return;
	
	if(ctx->spam_wait && (ctx->spam_current_delay < ctx->spam_delay)) {
		ctx->spam_current_delay++;
		return;
	}
	else if(ctx->spam_wait && (ctx->spam_current_delay >= ctx->spam_delay)) {
		ctx->event = OBJ_PUSH_BUTTON_SPAMING;
	}
	
	ctx->current_count %= ctx->resolution;
	
	if(ctx->current_count != 0) {
		ctx->current_count++;
		return;
	}
	
	bool state = ctx->pin_get_state_callback(ctx->handle);
	
	if(
			ctx->pb_current_state == state &&
			ctx->mode == OBJ_PUSH_BUTTON_SIMPLE
	) {
		ctx->routine_callback(ctx->handle);
	}
	else if(
			state == false &&
			ctx->mode == OBJ_PUSH_BUTTON_FALLING_EDGE
	) {
		ctx->routine_callback(ctx->handle);
	}
	else if(
			state == true &&
			ctx->mode == OBJ_PUSH_BUTTON_RISING_EDGE
	) {
		ctx->routine_callback(ctx->handle);
	}
	else {
		ctx->pb_current_state = state;
		ctx->count_enable = false;
		ctx->spam_wait = false;
		ctx->spam_current_delay = 0;
		ctx->current_count = ctx->resolution;
		return;
	}
	
	if(!ctx->spam_enabled) {
		ctx->pb_current_state = state;
		ctx->count_enable = false;
		ctx->current_count = ctx->resolution;
	}
	else {
		ctx->spam_wait = true;
	}
}