/*!
 * \file obj_pwm.h
 * \brief object oriented pwm header
 * \author Adam LUCAS
 * \date 9/7/2021 6:46:38 AM
 */

#ifndef OBJ_PWM_H_
#define OBJ_PWM_H_

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

#define OBJ_PWM_CONVERT_EFF_VAL_PERCENT_VAL(float_percent_val, ctx) \
		(uint32_t)((float)ctx->res_max_val * float_percent_val / 100.0F + 0.5F)

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum obj_pwm_resolution
 * \brief object oriented pwm standard resolution (ex : 4bits <=> 4^2 = 16), can be implemented as follows
 */

typedef enum {
	OBJ_PWM_RESOLUTION_4BITS = 16,
	OBJ_PWM_RESOLUTION_5BITS = 32,
	OBJ_PWM_RESOLUTION_6BITS = 64,
	OBJ_PWM_RESOLUTION_7BITS = 128,
	OBJ_PWM_RESOLUTION_8BITS = 256,
	OBJ_PWM_RESOLUTION_10BITS = 1024,
	OBJ_PWM_RESOLUTION_12BITS = 4096,
	OBJ_PWM_RESOLUTION_16BITS = 65536,
	OBJ_PWM_RESOLUTION_24BITS = 16777216,
	OBJ_PWM_RESOLUTION_32BITS = (int)4294967296,
	//OBJ_PWM_RESOLUTION_64BITS = 18446744073709551616,
}obj_pwm_resolution;

/*!
 * \enum obj_pwm_gpio_logic
 * \brief object oriented pwm output logic
 */

typedef enum {
	OBJ_PWM_GPIO_LOGIC_NEG = 1, //for binary operations with XOR
	OBJ_PWM_GPIO_LOGIC_POS = 0,
}obj_pwm_gpio_logic;

/*!
 * \fn void(*obj_pwm_gpio_set_state)(void *, bool)
 * \brief object oriented gpio get state function pointer
 * \param[inout] handle user application data pointer
 * \param[in] state gpio pin state (false : LOW, high : HIGH)
 * \return None
 */

typedef void(*obj_pwm_gpio_set_state)(void *handle, bool state);

/*!
 * \fn void(*obj_pwm_end_pulse_callback)(void *);
 * \brief object oriented pwm user defined end pulse callback
 * \param[inout] handle user defined data pointer
 * \return None
 */

typedef void(*obj_pwm_end_pulse_callback)(void *handle);

/*!
 * \struct ctx_obj_pwm
 * \brief object oriented pwm parameters data
 */

typedef struct {
	uint32_t one_pulse_period;						//! any pulse maximum period considering user specification
	uint32_t pulse_period;							//! any pulse period considering duty cycle high state
	uint32_t timer_period;							//! user application used timer precision
	uint32_t res_max_val;							//! any pulse corresponding to max resolution
	uint32_t res_compare_val;						//! any pulse duty cycle high state from 0 to <= max resolution
	uint32_t count_buffer;							//! timer count buffer
	bool end_pulse_flag;							//! end pulse flag for application specifications
	bool first_pulse_flag;							//! first pulse flag for application specifications
	bool pwm_enabled;								//! indicates if pwm has to run or not
	obj_pwm_gpio_set_state gpio_set_state;			//! user defined gpio set state function pointer
	obj_pwm_end_pulse_callback end_pulse_callback;	//! user defined end pulse callback function pointer
	void *handle;									//! user defined parameters data for function pointers
	obj_pwm_gpio_logic logic;						//! output gpio state logic
}ctx_obj_pwm;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void obj_pwm_init(
		ctx_obj_pwm *ctx,
		obj_pwm_resolution resolution,
		uint32_t pulse_period,
		uint32_t timer_period,
		obj_pwm_gpio_set_state gpio_set_state,
		void *handle,
		obj_pwm_gpio_logic logic
);

void obj_pwm_enable(ctx_obj_pwm *ctx, bool enable);
void obj_pwm_set_effective_val(ctx_obj_pwm *ctx, uint32_t val);
void obj_pwm_set_end_pulse_callback(
		ctx_obj_pwm *ctx, 
		obj_pwm_end_pulse_callback end_pulse_callback
);

void obj_pwm_timer_callback(ctx_obj_pwm *ctx);


/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* OBJ_PWM_H_ */
