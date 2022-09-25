/*!
 * \file lo_gpio.c
 * \brief gpio functions
 * \author Adam LUCAS
 * \date 7/17/2021 4:52:41 PM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "atmega328p_hw_gpio.h"

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

/**
 * \brief Set PORTB pin pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pin       The pin number in PORTB
 * \param[in] pull_mode Pin pull mode
 */
static inline void PORTB_set_pin_pull_mode(const uint8_t pin, const enum port_pull_mode pull_mode)
{

	if (pull_mode == PORT_PULL_UP) {

		DDRB &= ~(1 << pin);

		PORTB |= 1 << pin;
	} else if (pull_mode == PORT_PULL_OFF) {

		PORTB &= ~(1 << pin);
	}
}

/**
 * \brief Set PORTB data direction
 *
 * Select if the port pins selected by mask data direction is input, output
 * or disabled.
 *
 * \param[in] mask      Bit mask where 1 means apply direction setting to the
 *                      corresponding pin
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTB_set_port_dir(const uint8_t mask, const enum port_dir direction)
{
	switch (direction) {
	case PORT_DIR_IN:
		DDRB &= ~mask;
		break;
	case PORT_DIR_OUT:
		DDRB |= mask;
		break;
	case PORT_DIR_OFF:
		DDRB &= ~mask;

		PORTB |= mask;
		break;
	default:
		break;
	}
}

/**
 * \brief Set PORTB single pin data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] pin       The pin number within PORTB (0..7)
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTB_set_pin_dir(const uint8_t pin, const enum port_dir direction)
{
	switch (direction) {
	case PORT_DIR_IN:
		DDRB &= ~(1 << pin);
		break;
	case PORT_DIR_OUT:
		DDRB |= 1 << pin;
		break;
	case PORT_DIR_OFF:
		DDRB &= ~(1 << pin);

		PORTB |= 1 << pin;
		break;
	default:
		break;
	}
}

/**
 * \brief Set PORTB level
 *
 * Sets output level on the pins defined by the bit mask
 *
 * \param[in] mask  Bit mask where 1 means apply port level to the corresponding
 *                  pin
 * \param[in] level true  = Pin levels set to "high" state
 *                  false = Pin levels set to "low" state
 */
static inline void PORTB_set_port_level(const uint8_t mask, const bool level)
{
	if (level) {
		PORTB |= mask;
	} else {
		PORTB &= ~mask;
	}
}

/**
 * \brief Set PORTB level
 *
 * Sets output level on a pin
 *
 * \param[in] pin       The pin number for device
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PORTB_set_pin_level(const uint8_t pin, const bool level)
{
	if (level) {
		PORTB |= 1 << pin;
	} else {
		PORTB &= ~(1 << pin);
	}
}

/**
 * \brief Toggle out level on pins
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] port  Ports are grouped into groups of maximum 32 pins,
 *                  PORT_PORTA = group 0, PORT_PORTB = group 1, etc
 * \param[in] mask  Bit mask where 1 means toggle pin level to the corresponding
 *                  pin
 */
static inline void PORTB_toggle_port_level(const uint8_t mask)
{
	PINB = mask;
}

/**
 * \brief Toggle output level on pin
 *
 * Toggle the pin levels on pin
 *
 * \param[in] pin       The pin number for device
 */
static inline void PORTB_toggle_pin_level(const uint8_t pin)
{
	PINB = 1 << pin;
}

/**
 * \brief Get input level on pins
 *
 * Read the input level on pins connected to a port
 *
 * \param[in] port  Ports are grouped into groups of maximum 32 pins,
 *                  PORT_PORTA = group 0, PORT_PORTB = group 1, etc
 */
static inline uint8_t PORTB_get_port_level(volatile uint8_t *port)
{
	return PINB;
}

/**
 * \brief Get level on pin
 *
 * Reads the level on a pin connected to a port
 *
 * \param[in] pin       The pin number for device
 */
static inline bool PORTB_get_pin_level(const uint8_t pin)
{
	return PINB & (1 << pin);
}

/**
 * \brief Write value to PORTB
 *
 * Write directly to the entire port register.
 *
 * \param[in] value   Value to write
 */
static inline void PORTB_write_port(const uint8_t value)
{
	PORTB = value;
}

/**
 * \brief Set PORTC pin pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pin       The pin number in PORTC
 * \param[in] pull_mode Pin pull mode
 */
static inline void PORTC_set_pin_pull_mode(const uint8_t pin, const enum port_pull_mode pull_mode)
{

	if (pull_mode == PORT_PULL_UP) {

		DDRC &= ~(1 << pin);

		PORTC |= 1 << pin;
	} else if (pull_mode == PORT_PULL_OFF) {

		PORTC &= ~(1 << pin);
	}
}

/**
 * \brief Set PORTC data direction
 *
 * Select if the port pins selected by mask data direction is input, output
 * or disabled.
 *
 * \param[in] mask      Bit mask where 1 means apply direction setting to the
 *                      corresponding pin
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTC_set_port_dir(const uint8_t mask, const enum port_dir direction)
{
	switch (direction) {
	case PORT_DIR_IN:
		DDRC &= ~mask;
		break;
	case PORT_DIR_OUT:
		DDRC |= mask;
		break;
	case PORT_DIR_OFF:
		DDRC &= ~mask;

		PORTC |= mask;
		break;
	default:
		break;
	}
}

/**
 * \brief Set PORTC single pin data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] pin       The pin number within PORTC (0..7)
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTC_set_pin_dir(const uint8_t pin, const enum port_dir direction)
{
	switch (direction) {
	case PORT_DIR_IN:
		DDRC &= ~(1 << pin);
		break;
	case PORT_DIR_OUT:
		DDRC |= 1 << pin;
		break;
	case PORT_DIR_OFF:
		DDRC &= ~(1 << pin);

		PORTC |= 1 << pin;
		break;
	default:
		break;
	}
}

/**
 * \brief Set PORTC level
 *
 * Sets output level on the pins defined by the bit mask
 *
 * \param[in] mask  Bit mask where 1 means apply port level to the corresponding
 *                  pin
 * \param[in] level true  = Pin levels set to "high" state
 *                  false = Pin levels set to "low" state
 */
static inline void PORTC_set_port_level(const uint8_t mask, const bool level)
{
	if (level) {
		PORTC |= mask;
	} else {
		PORTC &= ~mask;
	}
}

/**
 * \brief Set PORTC level
 *
 * Sets output level on a pin
 *
 * \param[in] pin       The pin number for device
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PORTC_set_pin_level(const uint8_t pin, const bool level)
{
	if (level) {
		PORTC |= 1 << pin;
	} else {
		PORTC &= ~(1 << pin);
	}
}

/**
 * \brief Toggle out level on pins
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] port  Ports are grouped into groups of maximum 32 pins,
 *                  PORT_PORTA = group 0, PORT_PORTB = group 1, etc
 * \param[in] mask  Bit mask where 1 means toggle pin level to the corresponding
 *                  pin
 */
static inline void PORTC_toggle_port_level(const uint8_t mask)
{
	PINC = mask;
}

/**
 * \brief Toggle output level on pin
 *
 * Toggle the pin levels on pin
 *
 * \param[in] pin       The pin number for device
 */
static inline void PORTC_toggle_pin_level(const uint8_t pin)
{
	PINC = 1 << pin;
}

/**
 * \brief Get input level on pins
 *
 * Read the input level on pins connected to a port
 *
 * \param[in] port  Ports are grouped into groups of maximum 32 pins,
 *                  PORT_PORTA = group 0, PORT_PORTB = group 1, etc
 */
static inline uint8_t PORTC_get_port_level(volatile uint8_t *port)
{
	return PINC;
}

/**
 * \brief Get level on pin
 *
 * Reads the level on a pin connected to a port
 *
 * \param[in] pin       The pin number for device
 */
static inline bool PORTC_get_pin_level(const uint8_t pin)
{
	return PINC & (1 << pin);
}

/**
 * \brief Write value to PORTC
 *
 * Write directly to the entire port register.
 *
 * \param[in] value   Value to write
 */
static inline void PORTC_write_port(const uint8_t value)
{
	PORTC = value;
}

/**
 * \brief Set PORTD pin pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pin       The pin number in PORTD
 * \param[in] pull_mode Pin pull mode
 */
static inline void PORTD_set_pin_pull_mode(const uint8_t pin, const enum port_pull_mode pull_mode)
{

	if (pull_mode == PORT_PULL_UP) {

		DDRD &= ~(1 << pin);

		PORTD |= 1 << pin;
	} else if (pull_mode == PORT_PULL_OFF) {

		PORTD &= ~(1 << pin);
	}
}

/**
 * \brief Set PORTD data direction
 *
 * Select if the port pins selected by mask data direction is input, output
 * or disabled.
 *
 * \param[in] mask      Bit mask where 1 means apply direction setting to the
 *                      corresponding pin
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTD_set_port_dir(const uint8_t mask, const enum port_dir direction)
{
	switch (direction) {
	case PORT_DIR_IN:
		DDRD &= ~mask;
		break;
	case PORT_DIR_OUT:
		DDRD |= mask;
		break;
	case PORT_DIR_OFF:
		DDRD &= ~mask;

		PORTD |= mask;
		break;
	default:
		break;
	}
}

/**
 * \brief Set PORTD single pin data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] pin       The pin number within PORTD (0..7)
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PORTD_set_pin_dir(const uint8_t pin, const enum port_dir direction)
{
	switch (direction) {
	case PORT_DIR_IN:
		DDRD &= ~(1 << pin);
		break;
	case PORT_DIR_OUT:
		DDRD |= 1 << pin;
		break;
	case PORT_DIR_OFF:
		DDRD &= ~(1 << pin);

		PORTD |= 1 << pin;
		break;
	default:
		break;
	}
}

/**
 * \brief Set PORTD level
 *
 * Sets output level on the pins defined by the bit mask
 *
 * \param[in] mask  Bit mask where 1 means apply port level to the corresponding
 *                  pin
 * \param[in] level true  = Pin levels set to "high" state
 *                  false = Pin levels set to "low" state
 */
static inline void PORTD_set_port_level(const uint8_t mask, const bool level)
{
	if (level) {
		PORTD |= mask;
	} else {
		PORTD &= ~mask;
	}
}

/**
 * \brief Set PORTD level
 *
 * Sets output level on a pin
 *
 * \param[in] pin       The pin number for device
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PORTD_set_pin_level(const uint8_t pin, const bool level)
{
	if (level) {
		PORTD |= 1 << pin;
	} else {
		PORTD &= ~(1 << pin);
	}
}

/**
 * \brief Toggle out level on pins
 *
 * Toggle the pin levels on pins defined by bit mask
 *
 * \param[in] port  Ports are grouped into groups of maximum 32 pins,
 *                  PORT_PORTA = group 0, PORT_PORTB = group 1, etc
 * \param[in] mask  Bit mask where 1 means toggle pin level to the corresponding
 *                  pin
 */
static inline void PORTD_toggle_port_level(const uint8_t mask)
{
	PIND = mask;
}

/**
 * \brief Toggle output level on pin
 *
 * Toggle the pin levels on pin
 *
 * \param[in] pin       The pin number for device
 */
static inline void PORTD_toggle_pin_level(const uint8_t pin)
{
	PIND = 1 << pin;
}

/**
 * \brief Get input level on pins
 *
 * Read the input level on pins connected to a port
 *
 * \param[in] port  Ports are grouped into groups of maximum 32 pins,
 *                  PORT_PORTA = group 0, PORT_PORTB = group 1, etc
 */
static inline uint8_t PORTD_get_port_level(volatile uint8_t *port)
{
	return PIND;
}

/**
 * \brief Get level on pin
 *
 * Reads the level on a pin connected to a port
 *
 * \param[in] pin       The pin number for device
 */
static inline bool PORTD_get_pin_level(const uint8_t pin)
{
	return PIND & (1 << pin);
}

/**
 * \brief Write value to PORTD
 *
 * Write directly to the entire port register.
 *
 * \param[in] value   Value to write
 */
static inline void PORTD_write_port(const uint8_t value)
{
	PORTD = value;
}


//*****************************************************************************/
/*!
* \fn static void hw_gpio_default_callback(void *handle, bool state);
* \brief gpio get state deault callback function. DO NOT MODIFY
* \param[in] handle instance pin number
* \param[in] state instance pin state
* \return None
*/

static void hw_gpio_default_callback(void *handle, bool state) {
	(void)handle;
	(void)state;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_init_inst(
*		uint8_t inst,
*		hw_gpio_parameters *param
* );
* \brief gpio instance init function
* \param[in] gpio instance number. Refer to hw_gpio.h HW_GPIO_ defines
* \param[in] gpio init paremeters. Refer to hw_gpio.h hw_gpio_parameters typedef
* \return None
*/

void hw_gpio_init_inst(
		hw_gpio_inst *inst,
		hw_gpio_parameters *param
) {
	
	if(last_inst)
		last_inst->next_inst = inst;
	if(!first_inst)
		first_inst = inst;
	last_inst = inst;
	
	inst->dir = param->dir;
	inst->mode = param->mode;
	inst->pin = param->pin;
	inst->port = param->port;
	
	inst->get_state_callback = hw_gpio_default_callback;
	
	if(param->port == HW_GPIO_PB) {
		PORTB_set_pin_level(param->pin, param->mode);
		PORTB_set_pin_dir(param->pin, param->dir);
		if(param->dir == PORT_DIR_IN) {
			inst->mode = PORTB_get_pin_level(param->pin);
			PCMSK0 |= (1<<param->pin);
		}
	}
	else if(param->port == HW_GPIO_PC) {
		PORTC_set_pin_level(param->pin, param->mode);
		PORTC_set_pin_dir(param->pin, param->dir);
		if(param->dir == PORT_DIR_IN) {
			inst->mode = PORTC_get_pin_level(param->pin);
			PCMSK1 |= (1<<param->pin);
		}
	}
	else if(param->port == HW_GPIO_PD) {
		PORTD_set_pin_level(param->pin, param->mode);
		PORTD_set_pin_dir(param->pin, param->dir);
		if(param->dir == PORT_DIR_IN) {
			inst->mode = PORTD_get_pin_level(param->pin);
			PCMSK2 |= (1<<param->pin);
		}
	}
	
	PCICR = (1<<PCIE0) | (1<<PCIE1) | (1<<PCIE2);
	
	sei();
	
}

void hw_gpio_deinit_inst(hw_gpio_inst *inst) {
	
	hw_gpio_inst *inst_temp = first_inst;
	while(inst_temp->next_inst != inst) inst_temp = inst_temp->next_inst;
	inst_temp->next_inst = inst->next_inst;
	
	if(inst->port == HW_GPIO_PB) {
		PORTB_set_pin_level(inst->pin, false);
		PORTB_set_pin_dir(inst->pin, PORT_DIR_IN);
	}
	else if(inst->port == HW_GPIO_PC) {
		PORTC_set_pin_level(inst->pin, false);
		PORTC_set_pin_dir(inst->pin, PORT_DIR_IN);
	}
	else if(inst->port == HW_GPIO_PD) {
		PORTD_set_pin_level(inst->pin, false);
		PORTD_set_pin_dir(inst->pin, PORT_DIR_IN);
	}
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_set_state(uint8_t inst, bool state);
* \brief gpio instance pin set state function
* \param[in] inst gpio instance number. Refer to hw_gpio.h HW_GPIO_ defines
* \param[in] state gpio pin initial state (true : HIGH, false : LOW)
* \return None
*/

void hw_gpio_set_state(hw_gpio_inst *inst, bool state) {
	
	inst->mode = state;
	
	if(inst->port == HW_GPIO_PB) {
		PORTB_set_pin_level(inst->pin, inst->mode);
	}
	else if(inst->port == HW_GPIO_PC) {
		PORTC_set_pin_level(inst->pin, inst->mode);
	}
	else if(inst->port == HW_GPIO_PD) {
		PORTD_set_pin_level(inst->pin, inst->mode);
	}
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_toggle_state(uint8_t inst);
* \brief gpio instance pin toggle state function
* \param[in] inst gpio instance number. Refer to hw_gpio.h HW_GPIO_ defines
* \return None
*/

void hw_gpio_toggle_state(hw_gpio_inst *inst) {
	
	if(inst->port == HW_GPIO_PB) {
		PORTB_toggle_pin_level(inst->pin);
	}
	else if(inst->port == HW_GPIO_PC) {
		PORTC_toggle_pin_level(inst->pin);
	}
	else if(inst->port == HW_GPIO_PD) {
		PORTD_toggle_pin_level(inst->pin);
	}
}

//*****************************************************************************/
/*!
* \fn bool hw_gpio_get_state(uint8_t inst);
* \brief gpio instance pin get state function
* \param[in] inst gpio instance number. Refer to hw_gpio.h HW_GPIO_ defines
* \return state level (true : HIGH, false : LOW)
*/

bool hw_gpio_get_state(hw_gpio_inst *inst) {
	if(inst->port == HW_GPIO_PB) {
		inst->mode = PORTB_get_pin_level(inst->pin);
		return inst->mode;
	}
	else if(inst->port == HW_GPIO_PC) {
		inst->mode = PORTC_get_pin_level(inst->pin);
		return inst->mode;
	}
	else if(inst->port == HW_GPIO_PD) {
		inst->mode = PORTD_get_pin_level(inst->pin);
		return inst->mode;
	}
	return false;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_set_callback(
*		uint8_t inst,
*		hw_gpio_get_state_callback get_state_callback,
*		void *handle
* );
* \brief gpio instance pin set callback function
* \param[in] inst gpio instance number. Refer to hw_gpio.h HW_GPIO_ defines
* \param[in] get_state_callback user defined pin get state callback function pointer
* \param[in] handle user defined argument data for cb
* \return None
*/

void hw_gpio_set_callback(
		hw_gpio_inst *inst,
		hw_gpio_get_state_callback get_state_callback,
		void *handle
) {
	inst->get_state_callback = get_state_callback;
	inst->handle = handle;
}

//*****************************************************************************/
/*!
* \fn ISR(vector _vector);
* \brief gpio interrupts routines
* \param[in] _vector interrupt vector
* \return None
*/

ISR(PCINT0_vect) {
	hw_gpio_inst *temp = first_inst;
	while(temp) {
		if(temp->port == HW_GPIO_PB && temp->dir == PORT_DIR_IN) {
			bool state = PORTB_get_pin_level(temp->pin);
			if(temp->mode != state) {
				temp->mode ^= true;
				temp->get_state_callback(
				temp->handle,
				state
				);
			}
		}
		temp = temp->next_inst;
	}
}

ISR(PCINT1_vect) {
	hw_gpio_inst *temp = first_inst;
	while(temp) {
		if(temp->port == HW_GPIO_PC && temp->dir == PORT_DIR_IN) {
			bool state = PORTC_get_pin_level(temp->pin);
			if(temp->mode != state) {
				temp->mode ^= true;
				temp->get_state_callback(
				temp->handle,
				state
				);
			}
		}
		temp = temp->next_inst;
	}
}

ISR(PCINT2_vect) {
	hw_gpio_inst *temp = first_inst;
	while(temp) {
		if(temp->port == HW_GPIO_PD && temp->dir == PORT_DIR_IN) {
			bool state = PORTD_get_pin_level(temp->pin);
			if(temp->mode != state) {
				temp->mode ^= true;
				temp->get_state_callback(
				temp->handle,
				state
				);
			}
		}
		temp = temp->next_inst;
	}
}