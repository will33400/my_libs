/*!
 * \file lo_push_button.h
 * \brief low level push button header
 * \author Adam LUCAS
 * \date 9/4/2021 2:53:42 PM
 */

#ifndef LO_PUSH_BUTTON_H_
#define LO_PUSH_BUTTON_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>

#include "lo_timer.h"
#include "lo_gpio.h"
#include "obj_push_button.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#define LO_PUSH_BUTTON_SW1			0
#define LO_PUSH_BUTTON_SW2_SPAM		1
#define LO_PUSH_BUTTON_SW2_S_P		2

#define LO_PUSH_BUTTON_INST_NUM		3

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

typedef struct {
	uint8_t lo_gpio_inst;
	uint8_t lo_gpio_pin;
}lo_push_button_handle;

typedef struct {
	obj_push_button_ctx ctx[LO_PUSH_BUTTON_INST_NUM];
	lo_push_button_handle handle[LO_PUSH_BUTTON_INST_NUM];
	uint32_t resolution[LO_PUSH_BUTTON_INST_NUM];
	obj_push_button_mode mode[LO_PUSH_BUTTON_INST_NUM];
	bool spam_enabled[LO_PUSH_BUTTON_INST_NUM];
}LO_PUSH_BUTTON_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void lo_push_button_init(
		uint8_t inst,
		obj_push_button_routine_callback routine_callback,
		lo_gpio_parameters *gpio_pin, 
		uint32_t resolution,
		obj_push_button_mode mode,
		bool spam_enabled,
		uint32_t spam_delay
);

obj_push_button_event lo_push_button_get_event_flag(uint8_t inst);


/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* LO_PUSH_BUTTON_H_ */