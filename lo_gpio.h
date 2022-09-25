/*
 * lo_gpio.h
 *
 * Created: 9/15/2022 6:54:24 AM
 *  Author: Adam
 */ 


#ifndef LO_GPIO_H_
#define LO_GPIO_H_

#include "lo_defines.h"
#include <stdint.h>
#include <stdbool.h>


#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
#include "atmega328p/atmega328p_hw_gpio.h"
#define LO_GPIO_PIN_MAX			23
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
#include "attiny85/attiny85_hw_gpio.h"
#define LO_GPIO_PIN_MAX			6
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
#include "pic16f628a_hw_gpio.h"
#define LO_GPIO_PIN_MAX         16
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
#include "stm32f103c8_hw_gpio.h"
#define LO_GPIO_PIN_MAX         35
#endif


typedef void(*lo_gpio_cb)(void *handle, bool state);

typedef enum {
	LO_GPIO_DIR_IN,
	LO_GPIO_DIR_OUT,	
	LO_GPIO_DIR_OPEN_DRAIN,
}lo_gpio_dir;

typedef enum {
	LO_GPIO_FLAG_OK,
	LO_GPIO_FLAG_ERROR,
}lo_gpio_flag;

typedef
#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
hw_gpio_inst
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATTINY85
hw_gpio_inst
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
hw_gpio_inst
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
hw_gpio_inst
#endif
lo_gpio_inst;

typedef struct {
	uint16_t pin_num;
	bool state;
	lo_gpio_dir dir;
	bool it;
}lo_gpio_param;


void lo_gpio_init(lo_gpio_inst *inst, lo_gpio_param *param);
void lo_gpio_deinit(lo_gpio_inst *inst);
void lo_gpio_set_state(lo_gpio_inst *inst, bool state);
void lo_gpio_toggle_state(lo_gpio_inst *inst);
bool lo_gpio_get_state(lo_gpio_inst *inst);
void lo_gpio_set_cb(lo_gpio_inst *inst, lo_gpio_cb cb, void *handle);
lo_gpio_flag lo_gpio_get_flag(lo_gpio_inst *inst);


#endif /* LO_GPIO_H_ */
