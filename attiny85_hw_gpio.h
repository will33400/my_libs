/*!
 * \file hw_gpio.h
 * \brief low level gpio header
 * \author Adam LUCAS
 * \date 8/1/2021 9:48:04 PM
 */

#ifndef HW_GPIO_H_
#define HW_GPIO_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
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

#define HW_GPIO_COUNT			6

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \struct hw_gpio_port_dir
 * \brief gpio pin direction enum
 */

typedef enum {
	PORT_DIR_IN,
	PORT_DIR_OUT,
	PORT_DIR_OFF,
}hw_gpio_port_dir;

/*!
 * \fn typedef void(*hw_gpio_get_state_cb)(void *handle, bool state);
 * \brief gpio get state user defined callback for interrupt purpose
 * \param[in] handle user defined instance data
 * \param[in] state instance pin state
 */

typedef void(*hw_gpio_get_state_cb)(void *handle, bool state);

/*!
 * \struct hw_gpio_parameters
 * \brief gpio pin parameters struct
 */

typedef struct {
	uint8_t pin;
	bool mode;
	hw_gpio_port_dir dir;
}hw_gpio_parameters;

/*!
 * \struct HW_GPIO_DATA
 * \brief gpio pin instance data struct
 */

typedef struct hw_gpio_inst_{
	uint8_t inst;
	uint8_t pin;
	bool mode;
	hw_gpio_port_dir dir;
	hw_gpio_get_state_cb cb;
	void *handle;
	
	struct hw_gpio_inst_ *next_inst;
}hw_gpio_inst;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void hw_gpio_init(hw_gpio_inst *inst, hw_gpio_parameters *param);
void hw_gpio_deinit(hw_gpio_inst *inst);
void hw_gpio_set_state(hw_gpio_inst *inst, bool state);
void hw_gpio_toggle_state(hw_gpio_inst *inst);
bool hw_gpio_get_state(hw_gpio_inst *inst);

void hw_gpio_set_cb(hw_gpio_inst *inst, hw_gpio_get_state_cb cb, void *handle);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* HW_GPIO_H_ */