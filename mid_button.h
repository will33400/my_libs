/*!
 * \file mid_button.h
 * \brief middleware push button header
 * \author Adam LUCAS
 * \date 23/03/2022
 */

#ifndef MID_BUTTON_H_
#define MID_BUTTON_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>

#include "lo_gpio.h"
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

#define MID_BUTTON_DEBONCE_TIME_MS      30

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef enum {
    MID_BUTTON_INST_SCROLL,
    MID_BUTTON_INST_SEL,

    MID_BUTTON_INST_NUM,
}mid_button_inst;

/*!
 * \enum <name>
 * \union <name>
 * \struct <name>
 * \brief <description>
 */

typedef void(*mid_button_cb)(void *handle);

typedef struct {
    lo_gpio_inst gpio_inst;
    uint8_t pin;
		GPIO_TypeDef *port;
}mid_button_param;

typedef struct {
    lo_gpio_init_param gpio[MID_BUTTON_INST_NUM];
    bool prev_state[MID_BUTTON_INST_NUM];
    uint8_t debounce_count[MID_BUTTON_INST_NUM];
    mid_button_cb rising_cb[MID_BUTTON_INST_NUM];
    mid_button_cb falling_cb[MID_BUTTON_INST_NUM];
    mid_button_cb edge_cb[MID_BUTTON_INST_NUM];
    void *handle[MID_BUTTON_INST_NUM];
    bool lib_init;
}mid_button_data;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void mid_button_init(mid_button_inst inst, mid_button_param *param);
void mid_button_rising_edge_cb_set(mid_button_inst inst, mid_button_cb cb);
void mid_button_falling_edge_cb_set(mid_button_inst inst, mid_button_cb cb);
void mid_button_edge_cb_set(mid_button_inst inst, mid_button_cb cb);
void mid_button_handle_set(mid_button_inst inst, void *handle);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif /* MID_BUTTON_H_ */
