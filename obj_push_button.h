/*!
 * \file obj_push_button.h
 * \brief object oriented push button header
 * \author Adam LUCAS
 * \date 9/4/2021 2:58:10 PM
 */

#ifndef OBJ_PUSH_BUTTON_H_
#define OBJ_PUSH_BUTTON_H_

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

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum <name>
 * \union <name>
 * \struct <name>
 * \brief <description>
 */

typedef enum {
	OBJ_PUSH_BUTTON_SIMPLE,
	OBJ_PUSH_BUTTON_RISING_EDGE,
	OBJ_PUSH_BUTTON_FALLING_EDGE,
}obj_push_button_mode;

typedef enum {
	OBJ_PUSH_BUTTON_WAIT_SPAM,
	OBJ_PUSH_BUTTON_SPAMING,
	OBJ_PUSH_BUTTON_PUSHED,
	OBJ_PUSH_BUTTON_NOT_PUSHED,
}obj_push_button_event;

typedef void(*obj_push_button_routine_callback)(void *);
typedef bool(*obj_push_button_gpio_pin_get_state)(void *);

typedef struct {
	obj_push_button_routine_callback routine_callback;
	obj_push_button_gpio_pin_get_state pin_get_state_callback;
	obj_push_button_mode mode;
	bool spam_enabled;
	bool count_enable;
	bool pb_current_state;
	uint32_t resolution;
	uint32_t current_count;
	uint32_t spam_delay;
	bool spam_wait;
	uint32_t spam_current_delay;
	void *handle;
	obj_push_button_event event;
}obj_push_button_ctx;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void obj_push_button_init(
		obj_push_button_ctx *ctx, 
		obj_push_button_routine_callback routine_callback,
		obj_push_button_gpio_pin_get_state pin_get_state_callback,
		obj_push_button_mode mode,
		bool spam_enabled,
		uint32_t resolution,
		uint32_t spam_delay,
		void *handle
);

obj_push_button_event obj_push_button_get_event_flag(obj_push_button_ctx *ctx);

void obj_push_button_gpio_state_event_callback(obj_push_button_ctx *ctx);
void obj_push_button_timer_get_count_callback(obj_push_button_ctx *ctx);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* OBJ_PUSH_BUTTON_H_ */