/*!
 * \file hw_gpio.c
 * \brief low level gpio functions
 * \author Adam LUCAS
 * \date 8/1/2021 9:48:18 PM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "attiny85_hw_gpio.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static hw_gpio_inst *first_inst = (void *)0;
static hw_gpio_inst *last_inst = (void *)0;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn static void hw_gpio_get_state_default_callback(uint8_t pin, bool state);
* \brief low level gpio pin get state default callback. MANDATORY : DO NOT MODIFY
* \param[in] pin low level gpio pin number
* \param[in] state low level gpio pin state
* \return None
*/

static void hw_gpio_get_state_default_cb(void *handle, bool state) {
	(void)handle;
	(void)state;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_init(uint8_t inst, uint8_t pin, bool mode, hw_gpio_port_dir dir);
* \brief low level gpio init pin function
* \param[in] inst low level gpio instance number
* \param[in] pin low level gpio pin number
* \param[in] mode low level gpio pin state
* \param[in] dir low level gpio pin direction
* \return None
*/

void hw_gpio_init(hw_gpio_inst *inst, hw_gpio_parameters *param) {
	
	if(last_inst)
		last_inst->next_inst = inst;
	if(!first_inst)
		first_inst = inst;
	last_inst = inst;
	
	inst->dir = param->dir;
	inst->mode = param->mode;
	inst->pin = param->pin;
	inst->cb = hw_gpio_get_state_default_cb;
	
	if(param->dir == PORT_DIR_OUT) {
		DDRB |= (1<<param->pin);
	}
	else if(param->dir == PORT_DIR_IN) {
		DDRB &= ~(1<<param->pin);
	}
	
	hw_gpio_set_state(inst, param->mode);
	
	if(param->dir == PORT_DIR_IN) {
		PCMSK |= (1<<param->pin);
		GIMSK |= (1<<PCIE);
		sei();
	}
	
}

void hw_gpio_deinit(hw_gpio_inst *inst) {
	
	hw_gpio_inst *inst_temp = first_inst;
	while(inst_temp->next_inst != inst) inst_temp = inst_temp->next_inst;
	inst_temp->next_inst = inst->next_inst;
	
	PORTB &= ~(1<<inst->pin);
	DDRB &= ~(1<<inst->pin);
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_set_state(uint8_t inst, bool state);
* \brief low level gpio pin set state function
* \param[in] inst low level gpio instance number
* \param[in] state low level gpio pin state
* \return None
*/

void hw_gpio_set_state(hw_gpio_inst *inst, bool state) {
	inst->mode = state;
	if(state) {
		PORTB |= (1<<inst->pin);
	}
	else {
		PORTB &= ~(1<<inst->pin);
	}
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_toggle_state(uint8_t inst);
* \brief low level gpio pin toggle state function
* \param[in] inst low level gpio instance number
* \return None
*/

void hw_gpio_toggle_state(hw_gpio_inst *inst) {
	inst->mode ^= true;
	if(inst->mode) {
		PORTB |= (1<<inst->pin);
	}
	else {
		PORTB &= ~(1<<inst->pin);
	}
}

//*****************************************************************************/
/*!
* \fn bool hw_gpio_get_state(uint8_t inst);
* \brief low level gpio pin set state function
* \param[in] inst low level gpio instance number
* \return pin state
*/

bool hw_gpio_get_state(hw_gpio_inst *inst) {
	inst->mode = (PINB>>inst->pin)& 1;
	return inst->mode;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_set_callback(
*		uint8_t inst,
*		hw_gpio_get_state_callback get_state_callback
* );
* \brief low level gpio pin set state function
* \param[in] inst low level gpio instance number
* \param[in] get_state_callback low level gpio user defined get state callback function pointer
* \return None
*/

void hw_gpio_set_cb(hw_gpio_inst *inst, hw_gpio_get_state_cb cb, void *handle) {
	inst->cb = cb;
	inst->handle = handle;
}

ISR(PCINT0_vect) {
	hw_gpio_inst *temp = first_inst;
	
	while(temp) {
		bool state_temp = (PINB>>temp->pin)& 1;
		if(temp->mode != state_temp) {
			temp->mode = state_temp;
			temp->cb(temp->handle, state_temp);
		}
		
		temp = temp->next_inst;
	}
}