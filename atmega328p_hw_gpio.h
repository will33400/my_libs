/*!
 * \file hw_gpio.h
 * \brief gpio header
 * \author Adam LUCAS
 * \date 7/17/2021 4:52:27 PM
 */

#ifndef HW_GPIO_H_
#define HW_GPIO_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "atmega328p_hw_defines.h"

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

enum port_pull_mode {
	PORT_PULL_OFF,
	PORT_PULL_UP,
};

enum port_dir {
	PORT_DIR_IN,
	PORT_DIR_OUT,
	PORT_DIR_OFF,
};

/*!
 * \enum hw_gpio_port
 * \brief gpio device port enum
 */

typedef enum {
	HW_GPIO_PB,
	HW_GPIO_PC,
	HW_GPIO_PD,
}hw_gpio_port;

/*!
 * \enum hw_gpio_port_dir
 * \brief gpio device enum inspired from port.h file
 */

typedef enum port_dir hw_gpio_port_dir;

/*!
 * \fn typedef void(*hw_gpio_get_state_callback)(void *handle, bool state);
 * \brief gpio get state user defined callback for interrupt purpose
 * \param[in] handle instance pin number
 * \param[in] state instance pion state
 */

typedef void(*hw_gpio_get_state_callback)(void *handle, bool state);

/*!
 * \struct hw_gpio_parameters
 * \brief gpio pin parameters struct
 */

typedef struct {
	hw_gpio_port port;		//!< gpio pin port (B, C, D)
	uint8_t pin;			//!< gpio pin number (0..7)
	bool mode;				//!< gpio pin initial state (true : HIGH, false : LOW)
	hw_gpio_port_dir dir;	//!< gpio pin direction (in, out, off)
}hw_gpio_parameters;

/*!
 * \struct HW_GPIO_DATA
 * \brief gpio pin instance data struct
 */

typedef struct hw_gpio_inst_{
	hw_gpio_port port;
	uint8_t pin;
	bool mode;
	hw_gpio_port_dir dir;
	hw_gpio_get_state_callback get_state_callback;
	
	void *handle;
	
	struct hw_gpio_inst_ *next_inst;
	
}hw_gpio_inst;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void hw_gpio_init_inst(
		hw_gpio_inst *inst, 
		hw_gpio_parameters *param
);
void hw_gpio_deinit_inst(hw_gpio_inst *inst);

void hw_gpio_set_state(hw_gpio_inst *inst, bool state);
void hw_gpio_toggle_state(hw_gpio_inst *inst);
bool hw_gpio_get_state(hw_gpio_inst *inst);

void hw_gpio_set_callback(
		hw_gpio_inst *inst, 
		hw_gpio_get_state_callback get_state_callback,
		void *handle
);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* HW_GPIO_H_ */