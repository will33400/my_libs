/*!
 * \file obj_hc_sr04.h
 * \brief object oriented hc_sr04 device header
 * \author Adam LUCAS
 * \date 7/16/2021 5:38:40 PM
 */

#ifndef OBJ_HC_SR04_H_
#define OBJ_HC_SR04_H_

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

#define OBJ_HC_SR04_SOUND_VELOCITY		340 // m/s

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum hc_sr04_mode
 * \brief hc_sr04 device working mode enum
 */

typedef enum {
	HC_SR04_MODE_DISABLED,
	HC_SR04_MODE_SINGLE,
	HC_SR04_MODE_MULTIPLE,
}hc_sr04_mode;

/*!
 * \fn typedef void(*hc_sr04_delay_us)(void *, uint16_t);
 * \brief hc_sr04 device delay in us function to define by user in ctx
 * \param[in] handle user defined handle struct pointer
 * \param[in] value delay value in us
 * \return None
 */

typedef void(*hc_sr04_delay_us)(void *, uint16_t);

/*!
 * \fn typedef void(*hc_sr04_gpio_set_state)(void *, bool);
 * \brief hc_sr04 device gpio in set state routine to define by user in ctx
 * \param[in] handle user defined handle struct pointer
 * \param[in] state gpio pin state (true : HIGH, low : LOW)
 * \return None
 */

typedef void(*hc_sr04_gpio_set_state)(void *, bool);

/*!
 * \struct hc_sr04_value
 * \brief hc_sr04 device values struct
 */

typedef struct {
	volatile uint32_t value_us;
	volatile uint32_t value_mm;
}hc_sr04_value;

/*!
 * \struct ctx_hc_sr04
 * \brief object oriented hc_sr04 device struct
 */

typedef struct {
	void *handle;
	hc_sr04_delay_us delay_us;
	hc_sr04_gpio_set_state trigger_set_state;
	
	hc_sr04_value sensor_value;
	hc_sr04_value previous_value;
	
	volatile uint32_t timer_count;
	
	bool echo_previous_state;
	bool counting;
	bool measuring;
	
	hc_sr04_mode mode;
}ctx_hc_sr04;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void obj_hc_sr04_init(
		ctx_hc_sr04 *ctx,
		void *handle,
		hc_sr04_delay_us delay_us, 
		hc_sr04_gpio_set_state trigger_set_state,
		hc_sr04_mode mode
);

void obj_hc_sr04_echo_get_state_callback(ctx_hc_sr04 *ctx, bool state);
void obj_hc_sr04_timer_get_count_1us_callback(ctx_hc_sr04 *ctx);

void obj_hc_sr04_start_measure(ctx_hc_sr04 *ctx);
void obj_hc_sr_04_set_mode(ctx_hc_sr04 *ctx, hc_sr04_mode mode);
uint32_t obj_hc_sr04_get_value_us(ctx_hc_sr04 *ctx);
uint32_t obj_hc_sr04_get_value_mm(ctx_hc_sr04 *ctx);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* OBJ_HC_SR04_H_ */