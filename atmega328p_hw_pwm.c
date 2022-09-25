/*!
 * \file lo_pwm.c
 * \brief low level pwm functions
 * \author Adam LUCAS
 * \date 9/7/2021 7:01:42 AM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "atmega328p_hw_pwm.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static LO_PWM_DATA loPwmData;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn static void lo_pwm_gpio_set_state_callback(void *handle, bool state);
* \brief low level pwm gpio set state callback routine
* \param[in] handle low level pwm defined data pointer
* \param[in] state gpio pin state (false : LOW, true : HIGH)
* \return None
*/

static void lo_pwm_gpio_set_state_callback(void *handle, bool state) {
	lo_pwm_handle *handle_temp = (lo_pwm_handle *)handle;
	
	lo_gpio_set_state(handle_temp->gpio_inst, state);
}

//*****************************************************************************/
/*!
* \fn static void lo_pwm_timer_callback(void *handle);
* \brief low level pwm timer callback routine
* \param[in] handle low level pwm defined data pointer
* \return None
*/

static void lo_pwm_timer_callback(void *handle) {
	(void)handle;
	MID_TIMER_reStart(dTIMER_LO_PWM);
	
	//lo_gpio_toggle_state(LO_GPIO_INST_TEST1);
	
	for(uint8_t i = 0; i < LO_PWM_INST_NUM; i++) {
		obj_pwm_timer_callback(&loPwmData.ctx[i]);
	}
}

//*****************************************************************************/
/*!
* \fn void lo_pwm_init(
*		uint8_t inst,
*		lo_gpio_parameters *gpio_pin,
*		obj_pwm_resolution resolution,
*		uint32_t pulse_frequency,
*		obj_pwm_gpio_logic logic
* );
* \brief low level pwm init instance
* \param[in] inst low level pwm instance number
* \param[in] gpio_pin gpio pin parameters to init pwm. Refer to lo_gpio.h lo_gpio_parameters structure
* \param[in] resolution low level pwm resolution (ex : OBJ_PWM_RESOLUTION_4BITS <=> 4^2 = 16)
* \param[in] pulse_frequency low level pwm frequency to generate pwm output in Hz
* \param[in] logic gpio output state logic
* \return None
*/

void lo_pwm_init(
		uint8_t inst,
		lo_gpio_parameters *gpio_pin,
		obj_pwm_resolution resolution,
		uint32_t pulse_frequency,
		obj_pwm_gpio_logic logic
) {
	loPwmData.handle[inst].pwm_inst = inst;
	loPwmData.handle[inst].gpio_inst = gpio_pin->inst;
	
	gpio_pin->mode = (logic == OBJ_PWM_GPIO_LOGIC_NEG) ? true : false;
	
	lo_gpio_init_inst(gpio_pin->inst, gpio_pin);

	MID_TIMER_Initialize();
	MID_TIMER_Start(dTIMER_50US, dTIMER_LO_PWM);
	MID_TIMER_callback_set(dTIMER_LO_PWM, lo_pwm_timer_callback, (void *)0);
	
	obj_pwm_init(
			&loPwmData.ctx[inst],
			resolution,
			1000000 / pulse_frequency,
			LO_PWM_FREQ_ADJUST,
			lo_pwm_gpio_set_state_callback, 
			&loPwmData.handle[inst], 
			logic
	);
}

//*****************************************************************************/
/*!
* \fn void lo_pwm_set_end_pulse_callback(
*		uint8_t inst,
*		obj_pwm_end_pulse_callback end_pulse_callback,
*		void *lo_pwm_user_handle
* );
* \brief low level pwm timer callback routine
* \param[in] inst low level pwm instance number
* \param[in] end_pulse_callback user defined end pulse callback function pointer
* \param[in] obj_pwm_user_handle function pointer data pointer
* \return None
*/

void lo_pwm_set_end_pulse_callback(
		uint8_t inst, 
		obj_pwm_end_pulse_callback end_pulse_callback, 
		void *lo_pwm_user_handle
) {
	(void)lo_pwm_user_handle;
	obj_pwm_set_end_pulse_callback(&loPwmData.ctx[inst], end_pulse_callback);
}

//*****************************************************************************/
/*!
* \fn uint32_t lo_pwm_convert_percent_pwm_val(uint8_t inst, float percent);
* \brief low level pwm convert percent value into pwm val to use with lo_pwm_set_effective_val() function
* \param[in] inst low level pwm instance number
* \param[in] percent pwm value in percent
* \return pwm value from 0 to <= pwm resolution
*/

uint32_t lo_pwm_convert_percent_pwm_val(uint8_t inst, float percent) {
	ctx_obj_pwm *obj_pwm_temp = &loPwmData.ctx[inst];
	return OBJ_PWM_CONVERT_EFF_VAL_PERCENT_VAL(percent, obj_pwm_temp);
}

//*****************************************************************************/
/*!
* \fn void lo_pwm_enable(uint8_t inst, bool enable);
* \brief low level pwm enable or disable pwm output
* \param[in] inst low level pwm instance number
* \param[in] enable (false : disable, true : enable)
* \return None
*/

void lo_pwm_enable(uint8_t inst, bool enable) {
	obj_pwm_enable(&loPwmData.ctx[inst], enable);
}

//*****************************************************************************/
/*!
* \fn void lo_pwm_set_effective_val(uint8_t inst, uint32_t val);
* \brief low level pwm enable or disable pwm output
* \param[in] inst low level pwm instance number
* \param[in] val pwm value from 0 to <= pwm resolution
* \return None
*/

void lo_pwm_set_effective_val(uint8_t inst, uint32_t val) {
	if(val == 0) val = 1;
	obj_pwm_set_effective_val(&loPwmData.ctx[inst], val);
}
