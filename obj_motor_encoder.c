/*!
 * \file obj_motor_encoder.c
 * \brief motor encoder functions
 * \author Adam LUCAS
 * \date Jun 22, 2021
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_motor_encoder.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn void obj_motor_init(
*		obj_motor_enc_ctx *ctx,
*		uint32_t encoder_resolution,
*		float motor_alim,
*		float k_constant,
*		uint32_t rot_freq,
*		uint16_t resistance,
*		float inductance
*);
* \brief object oriented ctx struct init routine
*\param[in] ctx instance struct pointer
*\param[in] encoder_resolution encoder resolution value
*\param[in] motor_alim motor alim in volts
*\param[in] k_constant DC motor k constant value
*\param[in]	rot_freq rotation frequency value in revolution per minute
*\param[in]	resistance in ohm
*\param[in] inductance in Henri
* \return None
*/

void obj_motor_init(
		obj_motor_enc_ctx *ctx,
		uint32_t encoder_resolution,
		float motor_alim,
		float k_constant,
		uint32_t rot_freq,
		uint16_t resistance,
		float inductance
) {
	ctx->pwm_set[0](ctx->handle, 0);
	ctx->pwm_set[1](ctx->handle, 0);

	ctx->encoder_resolution = encoder_resolution;
	ctx->motor_alim_max = motor_alim;
	ctx->motor_alim_actual = 0;
	ctx->k_constant = k_constant;
	ctx->rot_freq_max = rot_freq;
	ctx->rot_freq_actual = 0;
	ctx->resistance = resistance;
	ctx->inductance = inductance;

	ctx->current_max =
			OBJ_MOTOR_ENCODER_CURRENT(resistance, inductance, motor_alim, 1.0/rot_freq);
	ctx->current_actual = 0;
	ctx->torque_max = OBJ_MOTOR_ENCODER_TORQUE(ctx->current_max, k_constant);
	ctx->torque_actual = 0;
}

//*****************************************************************************/
/*!
* \fn void obj_motor_set_pwm_16bits(
*		obj_motor_enc_ctx *ctx,
*		uint16_t pwm_val,
*		obj_motor_encoder_rotation side
*);
* \brief object oriented set motor pwm value considering direction routine
*\param[in] ctx instance struct pointer
*\param[in] pwm_val 16bits pwm value
*\param[in] side direction
* \return None
*/

void obj_motor_set_pwm_16bits(
		obj_motor_enc_ctx *ctx,
		uint16_t pwm_val,
		obj_motor_encoder_rotation side
) {
	ctx->side = side;
	ctx->pwm = (uint8_t)pwm_val;
	ctx->motor_alim_actual = ctx->pwm * ctx->motor_alim_max / 0xFFFF;
	if(side == OBJ_MOTOR_LEFT) {
		ctx->pwm_set[1](ctx->handle, 0);
		ctx->pwm_set[0](ctx->handle, pwm_val);
	}
	else {
		ctx->pwm_set[0](ctx->handle, 0);
		ctx->pwm_set[1](ctx->handle, pwm_val);
	}
}

//*****************************************************************************/
/*!
* \fn void obj_motor_encoder_signal_a_callback(obj_motor_enc_ctx *ctx, bool state);
* \brief object oriented encoder signal A callback routine
*\param[in] ctx instance struct pointer
*\param[in] state true corresponds to true
* \return None
*/

void obj_motor_encoder_signal_a_callback(obj_motor_enc_ctx *ctx, bool state) {
	ctx->signal_a = state;
	if(ctx->signal_a && !ctx->signal_b)
		ctx->side = OBJ_MOTOR_LEFT;
	
	//ctx->rot_freq_actual = 1 / ctx->tim_get_interval_time(ctx->handle) * 1000;
	
	if(ctx->side == OBJ_MOTOR_RIGHT)
		ctx->encoder_increment++;
	else if(ctx->side == OBJ_MOTOR_LEFT)
		ctx->encoder_increment--;
}

//*****************************************************************************/
/*!
* \fn void obj_motor_encoder_signal_b_callback(obj_motor_enc_ctx *ctx, bool state);
* \brief object oriented encoder signal B callback routine
*\param[in] ctx instance struct pointer
*\param[in] state true corresponds to true
* \return None
*/

void obj_motor_encoder_signal_b_callback(obj_motor_enc_ctx *ctx, bool state) {
	ctx->signal_b = state;
	if(!ctx->signal_a && ctx->signal_b)
		ctx->side = OBJ_MOTOR_RIGHT;
	
	if(ctx->side == OBJ_MOTOR_RIGHT)
		ctx->encoder_increment++;
	else if(ctx->side == OBJ_MOTOR_LEFT)
		ctx->encoder_increment--;
	//ctx->rot_freq_actual = 1 / ctx->tim_get_interval_time(ctx->handle) * 1000;
	
}

//*****************************************************************************/
/*!
* \fn float obj_motor_get_alim(obj_motor_enc_ctx *ctx);
* \brief object oriented get motor alim in volts considering pwm input
*\param ctx instance struct pointer
* \return alim value
*/

float obj_motor_get_alim(obj_motor_enc_ctx *ctx) {
	return ctx->motor_alim_actual;
}

//*****************************************************************************/
/*!
* \fn float obj_motor_get_current(obj_motor_enc_ctx *ctx);
* \brief object oriented get motor alim current in amperes considering pwm input
*\param ctx instance struct pointer
* \return current value
*/

float obj_motor_get_current(obj_motor_enc_ctx *ctx) {
	return
			OBJ_MOTOR_ENCODER_CURRENT(
					ctx->resistance,
					ctx->inductance,
					ctx->motor_alim_actual,
					1.0F / (float)ctx->rot_freq_actual
			);
}

//*****************************************************************************/
/*!
* \fn float obj_motor_get_torque(obj_motor_enc_ctx *ctx);
* \brief object oriented get motor torque in N.mm considering pwm input
*\param ctx instance struct pointer
* \return torque value
*/

float obj_motor_get_torque(obj_motor_enc_ctx *ctx) {
	return OBJ_MOTOR_ENCODER_TORQUE(obj_motor_get_current(ctx), ctx->k_constant);
}

//*****************************************************************************/
/*!
* \fn uint32_t obj_motor_get_rot_freq(obj_motor_enc_ctx *ctx);
* \brief object oriented get motor rotation frequency in revolution per minute
*  considering pwm input
*\param ctx instance struct pointer
* \return rotation frequency value
*/

uint32_t obj_motor_get_rot_freq(obj_motor_enc_ctx *ctx) {
	return ctx->rot_freq_actual;
}

float obj_motor_get_angle_deg(obj_motor_enc_ctx *ctx) {
	return (float)ctx->encoder_increment * 360 / (float)ctx->encoder_resolution;
}

float obj_motor_get_angle_rad(obj_motor_enc_ctx *ctx) {
	return (float)ctx->encoder_increment * 2.0F * (float)M_PI / (float)ctx->encoder_resolution;
}

uint32_t obj_motor_get_encoder_increment_val(obj_motor_enc_ctx *ctx) {
	return ctx->encoder_increment;
}

obj_motor_encoder_rotation obj_motor_get_side(obj_motor_enc_ctx *ctx) {
	return ctx->side;
}

