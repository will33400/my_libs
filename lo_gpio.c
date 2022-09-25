/*
 * lo_gpio.c
 *
 * Created: 9/15/2022 6:54:37 AM
 *  Author: Adam
 */ 

#include "lo_gpio.h"

void lo_gpio_init(lo_gpio_inst *inst, lo_gpio_param *param) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_gpio_parameters temp_param = {
		.dir = param->dir,
		.mode = param->state,
	};
	
	if(param->pin_num < 8) {
		temp_param.port = HW_GPIO_PB;
		temp_param.pin = param->pin_num;
	}
	else if(param->pin_num < 15) {
		temp_param.port = HW_GPIO_PC;
		temp_param.pin = param->pin_num - 8;
	}
	else {
		temp_param.port = HW_GPIO_PD;
		temp_param.pin = param->pin_num - 15;
	}
	
	hw_gpio_init_inst(inst, &temp_param);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	hw_gpio_parameters temp_param = {
		.dir = param->dir,
		.mode = param->state,
		.pin = param->pin_num,
	};
	hw_gpio_init(inst, &temp_param);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    hw_gpio_param temp_param = {
        .state = param->state
    };
    
    if(param->dir == LO_GPIO_DIR_IN) temp_param.dir = HW_GPIO_DIR_IN;
    else temp_param.dir = HW_GPIO_DIR_OUT;
    
    if(param->pin_num < 8) temp_param.port = HW_GPIO_PORTA;
    else temp_param.port = HW_GPIO_PORTB;
    temp_param.pin_num = param->pin_num % 8;
    
    if(param->it) {
        switch(param->pin_num) {
            case 0:
                param->it = HW_GPIO_IT_RB0_INT_RISING;
            break;
            case 1:
            case 2:
            case 3:
            break;
                param->it = HW_GPIO_IT_NONE;
            break;
            case 4:
                param->it = HW_GPIO_IT_RB4;
            break;
            case 5:
                param->it = HW_GPIO_IT_RB5;
            break;
            case 6:
                param->it = HW_GPIO_IT_RB6;
            break;
            case 7:
                param->it = HW_GPIO_IT_RB7;
            break;
        }
    }
    
    hw_gpio_init_inst(inst, &temp_param);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8

	hw_gpio_init_param temp_param = {
		.port_config = param->dir == LO_GPIO_DIR_IN ? HW_GPIO_CR_INPUT_PUPD :
				param->dir == LO_GPIO_DIR_OUT ? HW_GPIO_CR_OUT2MHZ_PSPL :
				param->dir == LO_GPIO_DIR_OPEN_DRAIN ? HW_GPIO_CR_OUT2MHZ_OPDR : 
				HW_GPIO_CR_INPUT_PUPD,
		.irq = param->it ? HW_GPIO_EXTI_BOTH : HW_GPIO_EXTI_NONE,
		.state = param->state,
		.pin_num = param->pin_num > 31 ? (param->pin_num - 19) % 16 : param->pin_num % 16,
		.port = param->pin_num < 16 ? GPIOA :
				param->pin_num < 32 ? GPIOB : GPIOC,
	};
	
	hw_gpio_init(inst, &temp_param);
#endif
}

void lo_gpio_deinit(lo_gpio_inst *inst) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_gpio_deinit_inst(inst);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    hw_gpio_deinit_inst(inst);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_gpio_reset(inst);
#endif
}

void lo_gpio_set_state(lo_gpio_inst *inst, bool state) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_gpio_set_state(inst, state);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	hw_gpio_set_state(inst, state);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    hw_gpio_set_state(inst, state);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_gpio_set_state(inst, state);
#endif
}

void lo_gpio_toggle_state(lo_gpio_inst *inst) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_gpio_toggle_state(inst);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	hw_gpio_toggle_state(inst);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    hw_gpio_toggle_state(inst);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_gpio_toggle_state(inst);
#endif
}

bool lo_gpio_get_state(lo_gpio_inst *inst) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	return hw_gpio_get_state(inst);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	return hw_gpio_get_state(inst);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    return hw_gpio_get_state(inst);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	return hw_gpio_get_state(inst);
#endif
}

void lo_gpio_set_cb(lo_gpio_inst *inst, lo_gpio_cb cb, void *handle) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	hw_gpio_set_callback(inst, cb, handle);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	hw_gpio_set_cb(inst, cb, handle);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    hw_gpio_set_cb(inst, cb, handle);
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	hw_gpio_set_cb_both(inst, cb);
	hw_gpio_set_handle(inst, handle);
#endif
}

lo_gpio_flag lo_gpio_get_flag(lo_gpio_inst *inst) {
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
	return 1;
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
	return 1;
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
    return 1;
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
	return (hw_gpio_get_flag(inst) == HW_GPIO_OK ? LO_GPIO_FLAG_OK : LO_GPIO_FLAG_ERROR);
#endif
}
