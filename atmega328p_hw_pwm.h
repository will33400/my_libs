/*!
 * \file lo_pwm.h
 * \brief low level pwm header
 * \author Adam LUCAS
 * \date 9/7/2021 7:01:13 AM
 */

#ifndef LO_PWM_H_
#define LO_PWM_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>

#include "obj_pwm.h"
#include "atmega328p_hw_gpio.h"
#include "mid_timer.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#define LO_PWM_1									0
#define LO_PWM_2									1

#define LO_PWM_INST_NUM						2

#define LO_PWM_TIMER_LO_TIMER			0
#define LO_PWM_TIMER_INTERNAL			1

#define LO_PWM_TIMER_SEL				LO_PWM_TIMER_LO_TIMER
	
#define LO_PWM_FREQ_ADJUST				58

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \struct lo_pwm_handle
 * \brief low level pwm local defined data for any object oriented callback function pointers
 */

typedef struct {
	uint8_t pwm_inst;
	uint8_t gpio_inst;
}lo_pwm_handle;

/*!
 * \struct LO_PWM_DATA
 * \brief low level pwm instances data structure
 */

typedef struct {
	ctx_obj_pwm ctx[LO_PWM_INST_NUM];				//!< object oriented pwm structure
	lo_pwm_handle handle[LO_PWM_INST_NUM];			//!< object oriented pwm data
}LO_PWM_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void lo_pwm_init(
		uint8_t inst,
		lo_gpio_parameters *gpio_pin,
		obj_pwm_resolution resolution,
		uint32_t pulse_frequency,
		obj_pwm_gpio_logic logic
);

void lo_pwm_set_end_pulse_callback(
		uint8_t inst, 
		obj_pwm_end_pulse_callback end_pulse_callback,
		void *lo_pwm_user_handle
);

uint32_t lo_pwm_convert_percent_pwm_val(uint8_t inst, float percent);
void lo_pwm_enable(uint8_t inst, bool enable);
void lo_pwm_set_effective_val(uint8_t inst, uint32_t val);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* LO_PWM_H_ */
		
/*
lo_gpio_init_param param = {
	.inst = LO_GPIO_INST_M1_L,
	.pin_num = APP_ROBOT_M1_L_PIN,
	.port = APP_ROBOT_M1_L_PORT,
	.port_config = LO_GPIO_CR_OUT2MHZ_PSPL,
	.state = false,
};

lo_gpio_init_param param1 = {
	.inst = LO_GPIO_INST_TEST1,
	.pin_num = 0,
	.port = GPIOB,
	.port_config = LO_GPIO_CR_OUT2MHZ_PSPL,
	.state = false,
};

lo_gpio_init(LO_GPIO_INST_TEST1, &param1);

lo_pwm_init(LO_PWM_1, &param, OBJ_PWM_RESOLUTION_4BITS, 50, OBJ_PWM_GPIO_LOGIC_POS);
lo_pwm_set_effective_val(LO_PWM_1, 1);

param.inst = LO_GPIO_INST_M1_R;
param.pin_num = APP_ROBOT_M1_R_PIN;
param.port = APP_ROBOT_M1_R_PORT;

lo_pwm_init(LO_PWM_2, &param, OBJ_PWM_RESOLUTION_4BITS, 50, OBJ_PWM_GPIO_LOGIC_POS);
lo_pwm_set_effective_val(LO_PWM_2, 1);

lo_pwm_enable(LO_PWM_1, true);
lo_pwm_enable(LO_PWM_2, true);*/
