#if 0
/*!
 * \file lo_push_button.c
 * \brief low level push button functions
 * \author Adam LUCAS
 * \date 9/4/2021 2:54:07 PM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "attiny85_hw_push_button.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static LO_PUSH_BUTTON_DATA loPushButtonData;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

bool lo_push_button_get_state_callback(void *handle) {
	lo_push_button_handle *handle_temp = (lo_push_button_handle *)handle;
	
	return lo_gpio_get_state(handle_temp->lo_gpio_inst);
}

void lo_push_button_gpio_interrupt_callback(uint8_t pin, bool state) {
	for(uint8_t i = 0; i < LO_PUSH_BUTTON_INST_NUM; i++) {
		if(pin == loPushButtonData.handle[i].lo_gpio_pin) {
			obj_push_button_gpio_state_event_callback(&loPushButtonData.ctx[i]);
		}
	}
}

void lo_push_button_timer_get_count_callback(void *handle) {
	(void)handle;
	for(uint8_t i = 0; i < LO_PUSH_BUTTON_INST_NUM; i++) {
		obj_push_button_timer_get_count_callback(&loPushButtonData.ctx[i]);
	}
	LO_TIMER_reStart(dTIMER_SW);
}

//*****************************************************************************/
/*!
* \fn <function declaration>
* \brief <description>
* \param[in] <argument name> <description>
* \param[out] <argument name> <description>
* \return [<anything>] [None]
*/

void lo_push_button_init(
		uint8_t inst,
		obj_push_button_routine_callback routine_callback,
		lo_gpio_parameters *gpio_pin,
		uint32_t resolution,
		obj_push_button_mode mode,
		bool spam_enabled,
		uint32_t spam_delay
) {
	loPushButtonData.handle[inst].lo_gpio_inst = gpio_pin->inst;
	loPushButtonData.handle[inst].lo_gpio_pin = gpio_pin->pin;
	loPushButtonData.mode[inst] = mode;
	loPushButtonData.resolution[inst] = resolution;
	loPushButtonData.spam_enabled[inst] = spam_enabled;
	
	gpio_pin->dir = PORT_DIR_IN;
	lo_gpio_init(gpio_pin->inst, gpio_pin->pin, gpio_pin->mode, gpio_pin->dir);
	lo_gpio_set_callback(gpio_pin->inst, lo_push_button_gpio_interrupt_callback);
	
	LO_TIMER_Initialize();
	LO_TIMER_Start(dTIMER_1MS, dTIMER_SW);
	LO_TIMER_callback_set(dTIMER_SW, lo_push_button_timer_get_count_callback, (void *)0);
	
	obj_push_button_init(
			&loPushButtonData.ctx[inst], 
			routine_callback, 
			lo_push_button_get_state_callback, 
			mode, 
			spam_enabled,
			resolution,
			spam_delay,
			&loPushButtonData.handle[inst]
	);
}

obj_push_button_event lo_push_button_get_event_flag(uint8_t inst) {
	return obj_push_button_get_event_flag(&loPushButtonData.ctx[inst]);
}
#endif