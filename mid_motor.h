
#ifndef MID_MOTOR_H_
#define MID_MOTOR_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include "obj_motor_encoder.h"
#include "lo_gpio.h"
#include "lo_pwm.h"
#include "lo_timer.h"
#include "lo_uart.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#define MID_MOTOR_INST_1		0

#define MID_MOTOR_NUM			1

#define MID_MOTOR_PWM_RES			OBJ_PWM_RESOLUTION_4BITS
#define MID_MOTOR_PWM_FREQ		50

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \struct MID_MOTOR_HANDLE
 * \brief object oriented handle data for obj_motor_encoder.h ctx struct
 */

typedef struct {
	uint8_t mid_motor_inst;
	uint8_t pwm_tim;
	uint8_t pwm_pin_1;
	uint8_t pwm_pin_2;
	uint32_t tim_count;
}MID_MOTOR_HANDLE;

/*!
 * \struct MID_MOTOR_GPIO
 * \brief motor gpio pin configuration for encoder struct
 */

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	uint32_t mode;
}MID_MOTOR_GPIO;

/*!
 * \struct MID_MOTOR_CHARACTERISTICS
 * \brief motor miscellaneous data struct
 */

typedef struct {
	uint32_t encoder_resolution;
	float motor_alim;
	float k_constant;
	uint32_t rot_freq;
	uint16_t resistance;
	float inductance;
}MID_MOTOR_CHARACTERISTICS;

typedef struct {
	MID_MOTOR_CHARACTERISTICS settings;
	lo_gpio_init_param signal_a;
	lo_gpio_init_param signal_b;
	lo_gpio_init_param pwm1;
	lo_gpio_init_param pwm2;
	uint8_t pwm1_inst;
	uint8_t pwm2_inst;
	uint8_t timer_inst;
}mid_motor_param;

/*!
 * \struct MID_MOTOR_INST_DATA
 * \brief middleware motor instance data
 */

typedef struct {
	obj_motor_enc_ctx ctx[MID_MOTOR_NUM];
	MID_MOTOR_HANDLE handle[MID_MOTOR_NUM];

	uint16_t current_pwm[MID_MOTOR_NUM];

}MID_MOTOR_INST_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void mid_motor_init(
		uint8_t inst,
		mid_motor_param *param
);

void mid_motor_set_pwm(
		uint8_t inst,
		uint16_t pwm_val,
		obj_motor_encoder_rotation side
);

float mid_motor_get_alim(uint8_t inst);

float mid_motor_get_current(uint8_t inst);

float mid_motor_get_torque(uint8_t inst);

uint32_t mid_motor_get_rot_freq(uint8_t inst);

float mid_motor_get_angle_deg(uint8_t inst);
float mid_motor_get_angle_rad(uint8_t inst);

uint32_t mid_motor_get_encoder_increment_val(uint8_t inst);

obj_motor_encoder_rotation mid_motor_get_side(uint8_t inst);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif
