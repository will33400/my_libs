/*!
 * \file obj_motor_encoder.h
 * \brief motor encoder header
 * \author Adam LUCAS
 * \date Jun 22, 2021
 */

#ifndef INC_OBJ_MOTOR_ENCODER_H_
#define INC_OBJ_MOTOR_ENCODER_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#define OBJ_MOTOR_ENCODER_PWM_PIN_NUM	2

#define OBJ_MOTOR_ENCODER_SIGNAL_A		0
#define OBJ_MOTOR_ENCODER_SIGNAL_B		1

#define OBJ_MOTOR_ENCODER_GPIO_PIN_NUM	2

#define OBJ_MOTOR_ENCODER_CURRENT(resistance, inductance, tension, time) \
	(float)((tension/resistance)* exp(-time/(inductance/resistance)))
#define OBJ_MOTOR_ENCODER_TORQUE(current, k_constant) \
	(float)(k_constant / current * 1000)
		
#ifndef M_PI
#define M_PI 3.14159265
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum obj_motor_encoder_rotation
 * \brief motor encoder direction enum
 */

typedef enum {
	OBJ_MOTOR_RIGHT,
	OBJ_MOTOR_LEFT
}obj_motor_encoder_rotation;

/*!
 * \fn void(*obj_motor_encoder_pwm_set)(void *, uint32_t);
 * \brief object oriented 16bits pwm set value routine
 * \param[in] handle object oriented user defined data
 * \param[in] pwm_val 16bits pwm value
 * \return None
 */

typedef void(*obj_motor_encoder_pwm_set)(void *, uint32_t);

/*!
 * \fn void(*obj_motor_encoder_pwm_set)(void *, uint32_t);
 * \brief object oriented get gpio pin state (true : HIGH) value routine
 * \param[in] handle object oriented user defined data
 * \return pin state
 */

typedef bool(*obj_motor_encoder_gpio_get_state)(void *);

/*!
 * \fn void(*obj_motor_encoder_pwm_set)(void *, uint32_t);
 * \brief object oriented get timer count value routine in ms
 * \param[in] handle object oriented user defined data
 * \return count value
 */

typedef uint32_t(*obj_motor_encoder_counter_get_val)(void *);

/*!
 * \struct obj_motor_enc_ctx
 * \brief object oriented data + functions pointers
 */

typedef struct {
	void *handle;
	obj_motor_encoder_pwm_set pwm_set[OBJ_MOTOR_ENCODER_PWM_PIN_NUM];
	obj_motor_encoder_counter_get_val tim_get_interval_time;
	uint8_t pwm;
	obj_motor_encoder_rotation side;
	uint32_t encoder_resolution; // points/tour
	float motor_alim_max; // V
	float motor_alim_actual;
	float k_constant; // const
	uint32_t rot_freq_max; // tour/s
	uint32_t rot_freq_actual;
	uint32_t encoder_increment;
	float torque_max; // N.mm
	float torque_actual;
	float current_max; //A
	float current_actual;
	uint16_t resistance;
	float inductance;
	bool signal_a;
	bool signal_b;
}obj_motor_enc_ctx;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//!functions descriptions : refer to <code>.c

void obj_motor_init(
		obj_motor_enc_ctx *ctx,
		uint32_t encoder_resolution,
		float motor_alim,
		float k_constant,
		uint32_t rot_freq,
		uint16_t resistance,
		float inductance
);

void obj_motor_set_pwm_16bits(
		obj_motor_enc_ctx *ctx,
		uint16_t pwm_val,
		obj_motor_encoder_rotation side
);

void obj_motor_encoder_signal_a_callback(obj_motor_enc_ctx *ctx, bool state);
void obj_motor_encoder_signal_b_callback(obj_motor_enc_ctx *ctx, bool state);

float obj_motor_get_alim(obj_motor_enc_ctx *ctx);
float obj_motor_get_current(obj_motor_enc_ctx *ctx);
float obj_motor_get_torque(obj_motor_enc_ctx *ctx);
uint32_t obj_motor_get_rot_freq(obj_motor_enc_ctx *ctx);
float obj_motor_get_angle_deg(obj_motor_enc_ctx *ctx);
float obj_motor_get_angle_rad(obj_motor_enc_ctx *ctx);
uint32_t obj_motor_get_encoder_increment_val(obj_motor_enc_ctx *ctx);
obj_motor_encoder_rotation obj_motor_get_side(obj_motor_enc_ctx *ctx);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* INC_OBJ_MOTOR_ENCODER_H_ */
