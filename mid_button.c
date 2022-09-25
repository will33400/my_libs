/*!
 * \file mid_button.c
 * \brief middleware push button functions
 * \author Adam LUCAS
 * \date 23/03/2022
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "mid_button.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static mid_button_data midButtonData = {
    .lib_init = false,
};

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

static void mid_buttons_debounce(void *handle) {
    (void)handle;
    MID_TIMER_reStart(dTIMER_MID_BUTTONS);
    for(uint8_t i = 0; i < MID_BUTTON_INST_NUM; i++) {
        if(midButtonData.debounce_count[i] > 0)
                midButtonData.debounce_count[i]--;
    }
}

//*****************************************************************************/
/*!
* \fn <function declaration>
* \brief <description>
* \param[in] <argument name> <description>
* \param[out] <argument name> <description>
* \return [<anything>] [None]
*/

static void mid_button_default_cb(void *handle) {
    (void)handle;
}

static void mid_button_gpio_cb(void *handle, bool state) {
    lo_gpio_init_param *temp = handle;
    mid_button_inst inst = 0; 
    uint8_t i = 0;
    for(; i < MID_BUTTON_INST_NUM; i++) {
        if(temp->inst == midButtonData.gpio[i].inst) {
            inst = i;
            break;
        }
    }

    if(
            midButtonData.debounce_count[inst] > 0 || 
            state == midButtonData.prev_state[inst] ||
            i == MID_BUTTON_INST_NUM
    ) return;

    midButtonData.prev_state[inst] = state;
    midButtonData.debounce_count[inst] = MID_BUTTON_DEBONCE_TIME_MS;

    if(state) {
        midButtonData.rising_cb[inst](midButtonData.handle[inst]);
    }
    else {
        midButtonData.falling_cb[inst](midButtonData.handle[inst]);
    }
    
    midButtonData.edge_cb[inst](midButtonData.handle[inst]);
}

void mid_button_init(mid_button_inst inst, mid_button_param *param) {
		
		lo_gpio_init_param gpio = {
        .pin_num = param->pin,
        .inst = param->gpio_inst,
				.irq = LO_GPIO_EXTI_BOTH,
				.port = param->port,
    };
		
    if(!midButtonData.lib_init) {
        MID_TIMER_Initialize();
				MID_TIMER_Start(dTIMER_1MS, dTIMER_MID_BUTTONS);
				MID_TIMER_callback_set(dTIMER_MID_BUTTONS, mid_buttons_debounce, (void *)0);
        midButtonData.lib_init = true;
    }

    midButtonData.gpio[inst] = gpio;
    lo_gpio_init(param->gpio_inst, &gpio);
    midButtonData.prev_state[inst] = lo_gpio_get_state(param->gpio_inst);
    lo_gpio_set_cb_both(
            param->gpio_inst, 
            mid_button_gpio_cb
    );
		lo_gpio_set_handle(
						param->gpio_inst,
            &midButtonData.gpio
		);

    midButtonData.falling_cb[inst] = mid_button_default_cb;
    midButtonData.rising_cb[inst] = mid_button_default_cb;
    midButtonData.edge_cb[inst] = mid_button_default_cb;
}

void mid_button_rising_edge_cb_set(mid_button_inst inst, mid_button_cb cb) {
    midButtonData.rising_cb[inst] = cb;
}

void mid_button_falling_edge_cb_set(mid_button_inst inst, mid_button_cb cb) {
    midButtonData.falling_cb[inst] = cb;
}

void mid_button_edge_cb_set(mid_button_inst inst, mid_button_cb cb) {
    midButtonData.edge_cb[inst] = cb;
}

void mid_button_handle_set(mid_button_inst inst, void *handle) {
    midButtonData.handle[inst] = handle;
}