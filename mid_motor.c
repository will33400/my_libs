/*!
 * \file mid_motor.c
 * \brief motor inst functions
 * \author Adam LUCAS
 * \date Jun 22, 2021
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "mid_motor.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static MID_MOTOR_INST_DATA midMotorInstData;
static bool toggle = false;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn static void mid_motor_platform_pwm_set_1(void *handle, uint32_t val);
* \brief object oriented pwm set value function channel 1
* \param[in] handle object oriented handle specific data, refer to obj_motor_encoder.h
* \param[in] val 16bits length pwm value
* \return None
*/


static void mid_motor_platform_pwm_set_1(void *handle, uint32_t val) {
	MID_MOTOR_HANDLE *temp_handle = (MID_MOTOR_HANDLE *)handle;
	lo_pwm_set_effective_val(temp_handle->pwm_pin_1, val);
}

//*****************************************************************************/
/*!
* \fn static void mid_motor_platform_pwm_set_2(void *handle, uint32_t val);
* \brief object oriented pwm set value function channel 2
* \param[in] handle object oriented handle specific data, refer to obj_motor_encoder.h
* \param[in] val 16bits length pwm value
* \return None
*/

static void mid_motor_platform_pwm_set_2(void *handle, uint32_t val) {
	MID_MOTOR_HANDLE *temp_handle = (MID_MOTOR_HANDLE *)handle;
	lo_pwm_set_effective_val(temp_handle->pwm_pin_2, val);
}

//*****************************************************************************/
/*!
* \fn static void mid_motor_platform_time_count(void *handle);
* \brief object oriented timer counting function
* \param[in] handle object oriented handle specific data, refer to obj_motor_encoder.h
* \return None
*/

static void mid_motor_platform_time_count(void *handle) {
	MID_MOTOR_HANDLE *temp_handle = (MID_MOTOR_HANDLE *)handle;
	temp_handle->tim_count++;
}

//*****************************************************************************/
/*!
* \fn static uint32_t mid_motor_platform_count_get(void *handle);
* \brief object oriented timer get counting after reset count function
* \param[in] handle object oriented handle specific data, refer to obj_motor_encoder.h
* \return count value
*/

static uint32_t mid_motor_platform_count_get(void *handle) {
	MID_MOTOR_HANDLE *temp_handle = (MID_MOTOR_HANDLE *)handle;
	uint32_t temp = temp_handle->tim_count;
	temp_handle->tim_count = 0;
	return temp;
}

//*****************************************************************************/
/*!
* \fn static void mid_motor_gpio_callback_signal_a(void *handle, bool state);
* \brief object oriented gpio get state function signal A
* \param[in] state gpio input state, refer to obj_motor_encoder.h
* \return None
*/
static void mid_motor_gpio_callback_signal_a(void *handle, bool state) {
	obj_motor_encoder_signal_a_callback((obj_motor_enc_ctx *)handle, state);
}

//*****************************************************************************/
/*!
* \fn static void mid_motor_gpio_callback_signal_b(GPIO_PinState state);
* \brief object oriented gpio get state function signal B
* \param[in] state gpio input state, refer to obj_motor_encoder.h
* \return None
*/
static void mid_motor_gpio_callback_signal_b(void *handle, bool state) {
	obj_motor_encoder_signal_b_callback((obj_motor_enc_ctx *)handle, state);
}

//*****************************************************************************/
/*!
* \fn void mid_motor_init(
*		uint8_t inst,
*		uint8_t gpio_signal_a,
*		uint8_t gpio_signal_b,
*		uint8_t pwm_tim,
*		uint8_t pwm_pin_1,
*		uint8_t pwm_pin_2,
*		uint8_t tim_inst,
*		TIM_HandleTypeDef *htim
*);
* \brief middleware motor instance init
* \param[in] inst middleware motor instance number
* \param[in] gpio_signal_a gpio encoder input signal A instance number, refer to lo_gpio.h
* \param[in] gpio_signal_b gpio encoder input signal B instance number, refer to lo_gpio.h
* \param[in] pwm_tim pwm timer instance number, refer to lo_pwm.h
* \param[in] pwm_pin_1 pwm channel instance number, refer to lo_pwm.h
* \param[in] pwm_pin_2 pwm channel instance number, refer to lo_pwm_h
* \param[in] tim_inst timer counter instance number, refer to lo_timer.h
* \param[in] htim timer counter instance structure, refer to main.c
* \return None
*/

void mid_motor_init(uint8_t inst, mid_motor_param *param) {
	midMotorInstData.ctx[inst].pwm_set[0] = mid_motor_platform_pwm_set_1;
	midMotorInstData.ctx[inst].pwm_set[1] = mid_motor_platform_pwm_set_2;
	midMotorInstData.ctx[inst].tim_get_interval_time = mid_motor_platform_count_get;

	midMotorInstData.handle[inst].mid_motor_inst = inst;
	midMotorInstData.handle[inst].pwm_tim = param->timer_inst;
	midMotorInstData.handle[inst].pwm_pin_1 = param->pwm1_inst;
	midMotorInstData.handle[inst].pwm_pin_2 = param->pwm2_inst;
	midMotorInstData.handle[inst].tim_count = 0;
	
	midMotorInstData.ctx[inst].handle = &midMotorInstData.handle[inst];
	
	lo_gpio_init_param gpio_param = {
			.port_config = LO_GPIO_CR_INPUT_PUPD,
			.irq = LO_GPIO_EXTI_BOTH,
	};
	
	MID_TIMER_Initialize();

	MID_TIMER_callback_set(
			param->timer_inst,
			mid_motor_platform_time_count,
			&midMotorInstData.handle[inst]
	);
	
	gpio_param.inst = param->signal_a.inst;
	gpio_param.pin_num = param->signal_a.pin_num;
	gpio_param.port = param->signal_a.port;

	lo_gpio_init(param->signal_a.inst, &gpio_param);

	lo_gpio_set_cb_both(
			param->signal_a.inst,
			mid_motor_gpio_callback_signal_a
	);
	
	lo_gpio_set_handle(
			param->signal_a.inst,
			&midMotorInstData.ctx[inst]
	);

	gpio_param.inst = param->signal_b.inst;
	gpio_param.pin_num = param->signal_b.pin_num;
	gpio_param.port = param->signal_b.port;

	lo_gpio_init(param->signal_b.inst, &gpio_param);

	lo_gpio_set_cb_both(
			param->signal_b.inst,
			mid_motor_gpio_callback_signal_b
	);
	
	lo_gpio_set_handle(
			param->signal_b.inst,
			&midMotorInstData.ctx[inst]
	);
	
	param->pwm1.port_config = LO_GPIO_CR_OUT2MHZ_PSPL;
	param->pwm1.irq = LO_GPIO_EXTI_NONE;
	param->pwm1.state = false;

	lo_pwm_init(
			param->pwm1_inst, 
			&param->pwm1, 
			MID_MOTOR_PWM_RES, 
			MID_MOTOR_PWM_FREQ, 
			OBJ_PWM_GPIO_LOGIC_POS
	);
	
	param->pwm2.port_config = LO_GPIO_CR_OUT2MHZ_PSPL;
	param->pwm2.irq = LO_GPIO_EXTI_NONE;
	param->pwm2.state = false;
	
	lo_pwm_init(
			param->pwm2_inst, 
			&param->pwm2, 
			MID_MOTOR_PWM_RES, 
			MID_MOTOR_PWM_FREQ, 
			OBJ_PWM_GPIO_LOGIC_POS
	);
	
	lo_pwm_enable(param->pwm1_inst, true);
	lo_pwm_enable(param->pwm2_inst, true);

	obj_motor_init(
			&midMotorInstData.ctx[inst],
			param->settings.encoder_resolution,
			param->settings.motor_alim,
			param->settings.k_constant,
			param->settings.rot_freq,
			param->settings.resistance,
			param->settings.inductance
	);
}

//*****************************************************************************/
/*!
* \fn void mid_motor_set_pwm(
*		uint8_t inst,
*		uint16_t pwm_val,
*		obj_motor_encoder_rotation side
*);
* \brief middleware motor set motor pwm value
* \param[in] inst middleware motor instance number
* \param[in] pwm_val 16bits pwm value
* \param[in] side rotation direction
* \return None
*/

void mid_motor_set_pwm(
		uint8_t inst,
		uint16_t pwm_val,
		obj_motor_encoder_rotation side
) {
	obj_motor_set_pwm_16bits(
			&midMotorInstData.ctx[inst],
			pwm_val,
			side
	);
}

//*****************************************************************************/
/*!
* \fn float mid_motor_get_alim(uint8_t inst);
* \brief middleware motor get current alimentation in volts
* \param[in] inst middleware motor instance number
* \return alim value
*/

float mid_motor_get_alim(uint8_t inst) {
	return obj_motor_get_alim(&midMotorInstData.ctx[inst]);
}

//*****************************************************************************/
/*!
* \fn float mid_motor_get_current(uint8_t inst);
* \brief middleware motor get current in amperes
* \param[in] inst middleware motor instance number
* \return current value
*/

float mid_motor_get_current(uint8_t inst) {
	return obj_motor_get_current(&midMotorInstData.ctx[inst]);
}

//*****************************************************************************/
/*!
* \fn float mid_motor_get_torque(uint8_t inst);
* \brief middleware motor get torque in N.mm
* \param[in] inst middleware motor instance number
* \return torque value
*/

float mid_motor_get_torque(uint8_t inst) {
	return obj_motor_get_torque(&midMotorInstData.ctx[inst]);
}

//*****************************************************************************/
/*!
* \fn uint32_t mid_motor_get_rot_freq(uint8_t inst);
* \brief middleware motor get rotation frequency in revolution per minute
* \param[in] inst middleware motor instance number
* \return rotation frequency value
*/

uint32_t mid_motor_get_rot_freq(uint8_t inst) {
	return obj_motor_get_rot_freq(&midMotorInstData.ctx[inst]);
}

float mid_motor_get_angle_deg(uint8_t inst) {
	return obj_motor_get_angle_deg(&midMotorInstData.ctx[inst]);
}

float mid_motor_get_angle_rad(uint8_t inst) {
	return obj_motor_get_angle_rad(&midMotorInstData.ctx[inst]);
}

uint32_t mid_motor_get_encoder_increment_val(uint8_t inst) {
	return obj_motor_get_encoder_increment_val(&midMotorInstData.ctx[inst]);
}

obj_motor_encoder_rotation mid_motor_get_side(uint8_t inst) {
	return obj_motor_get_side(&midMotorInstData.ctx[inst]);
}

