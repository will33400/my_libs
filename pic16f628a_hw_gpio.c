/*
 * File:   pic16f628a_hw_gpio.c
 * Author: Adam
 *
 * Created on September 18, 2022, 10:09 AM
 */

#include "pic16f628a_hw_gpio.h"


static hw_gpio_inst *first_inst = (void *)0;
static hw_gpio_inst *last_inst = (void *)0;

static uint8_t inst_count = 0;

static void hw_gpio_it_rb4_7_routine(void);
static void hw_gpio_it_rb0_routine(void);

void hw_gpio_init_inst(
		hw_gpio_inst *inst, 
		hw_gpio_param *param
) {
    inst_count++;
    if(inst_count > HW_GPIO_INST_COUNT) return;
    
	if(last_inst)
		last_inst->next_inst = inst;
	if(!first_inst)
		first_inst = inst;
	last_inst = inst;
    
    inst->dir = param->dir;
    inst->pin_num = param->pin_num;
    inst->port = param->port;
    inst->state = param->state;
    
    if(
            !(
                    param->dir == HW_GPIO_DIR_IN &&
                    param->port == HW_GPIO_PORTB &&
                    (
                            param->pin_num == 0 ||
                            param->pin_num >= 4
                    )
            )
    ) {
        param->it = HW_GPIO_IT_NONE;
    }
    
    inst->it = param->it;
    
    switch(param->port) {
        case HW_GPIO_PORTA:
            STATUSbits.RP = 1;
            if(param->dir) TRISA |= (1<<param->pin_num);
            else TRISA &= ~(1<<param->pin_num);
            STATUSbits.RP = 0;
            if(param->state) PORTA |= (1<<param->pin_num);
            else PORTA &= ~(1<<param->pin_num);
        break;
        case HW_GPIO_PORTB:
            STATUSbits.RP = 1;
            if(param->dir) TRISB |= (1<<param->pin_num);
            else TRISB &= ~(1<<param->pin_num);
            STATUSbits.RP = 0;
            if(param->state) PORTB |= (1<<param->pin_num);
            else PORTB &= ~(1<<param->pin_num);
            switch(param->it) {
                case HW_GPIO_IT_NONE:
                break;
                case HW_GPIO_IT_RB0_INT_RISING:
                case HW_GPIO_IT_RB0_INT_FALLING:
                    if(HW_GPIO_IT_RB0_INT_RISING) OPTION_REGbits.INTEDG = 1;
                    else OPTION_REGbits.INTEDG = 0;
                    INTCONbits.INTE = 1;
                    hw_it_set_cb(HW_IT_VECT_INT, hw_gpio_it_rb0_routine);
                break;
                case HW_GPIO_IT_RB4:
                case HW_GPIO_IT_RB5:
                case HW_GPIO_IT_RB6:
                case HW_GPIO_IT_RB7:
                    INTCONbits.RBIE = 1;
                    hw_it_set_cb(HW_IT_VECT_RBI, hw_gpio_it_rb4_7_routine);
                break;
            }
        break;    
    }
}
void hw_gpio_deinit_inst(hw_gpio_inst *inst) {
    
    hw_gpio_inst *inst_temp = first_inst;
	while(inst_temp->next_inst != inst) inst_temp = inst_temp->next_inst;
	inst_temp->next_inst = inst->next_inst;
    
    switch(inst->port) {
        case HW_GPIO_PORTA:
            STATUSbits.RP = 1;
            TRISA &= ~(1<<inst->pin_num);
            STATUSbits.RP = 0;
            PORTA &= ~(1<<inst->pin_num);
        break;
        case HW_GPIO_PORTB:
            STATUSbits.RP = 1;
            TRISB &= ~(1<<inst->pin_num);
            STATUSbits.RP = 0;
            PORTB &= ~(1<<inst->pin_num);
        break;    
    }
}

void hw_gpio_set_cb(hw_gpio_inst *inst, hw_gpio_cb cb, void *handle) {
    inst->cb = cb;
    inst->handle = handle;
}

void hw_gpio_set_state(hw_gpio_inst *inst, bool state) {
    inst->state = state;
    switch(inst->port) {
        case HW_GPIO_PORTA:
            STATUSbits.RP = 0;
            if(inst->state) PORTA |= (1<<inst->pin_num);
            else PORTA &= ~(1<<inst->pin_num);
        break;
        case HW_GPIO_PORTB:
            STATUSbits.RP = 0;
            if(inst->state) PORTB |= (1<<inst->pin_num);
            else PORTB &= ~(1<<inst->pin_num);
        break;    
    }
}

void hw_gpio_toggle_state(hw_gpio_inst *inst) {
    inst->state ^= true;
    
    switch(inst->port) {
        case HW_GPIO_PORTA:
            STATUSbits.RP = 0;
            if(inst->state) PORTA |= (1<<inst->pin_num);
            else PORTA &= ~(1<<inst->pin_num);
        break;
        case HW_GPIO_PORTB:
            STATUSbits.RP = 0;
            if(inst->state) PORTB |= (1<<inst->pin_num);
            else PORTB &= ~(1<<inst->pin_num);
        break;    
    }
}

bool hw_gpio_get_state(hw_gpio_inst *inst) {
    switch(inst->port) {
        case HW_GPIO_PORTA:
            STATUSbits.RP = 0;
            if(PORTA &(1<<inst->pin_num))inst->state = true;
            else inst->state = false;
        break;
        case HW_GPIO_PORTB:
            STATUSbits.RP = 0;
            if(PORTB &(1<<inst->pin_num)) inst->state = true;
            else inst->state = false;
        break;    
    }
    return inst->state;
}

static void hw_gpio_it_rb4_7_routine(void) {
    hw_gpio_inst *temp_inst = first_inst;
    uint8_t temp_state = PORTB;
    while(temp_inst) {
        if(
                temp_inst->it > HW_GPIO_IT_RB0_INT_FALLING && 
                temp_inst->state != ((temp_state>>temp_inst->pin_num)&1)
        ) {
            //temp_inst->state = true;
            temp_inst->cb(temp_inst->handle, temp_inst->state);
        }
        temp_inst = temp_inst->next_inst;
    }
}

static void hw_gpio_it_rb0_routine(void) {
    hw_gpio_inst *temp_inst = first_inst;
    bool temp_state = PORTB&1;
    while(temp_inst) {
        if(temp_inst->pin_num == 0) {
            temp_inst->state = temp_state;
            temp_inst->cb(temp_inst->handle, temp_inst->state);
        }
        temp_inst = temp_inst->next_inst;
    }
}