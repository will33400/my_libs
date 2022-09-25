/*!
 * \file hw_gpio.h
 * \brief low level gpio header
 * \author Adam LUCAS
 * \date 14/05/2022
 */

#ifndef HW_GPIO_H_
#define HW_GPIO_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>

#include "stm32f10x.h"
#include "lo_defines.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum <name>
 * \union <name>
 * \struct <name>
 * \brief <description>
 */
 
typedef enum {
	HW_GPIO_CR_ANALOG					= 0x0,
	HW_GPIO_CR_RESET					= 0x4,
	HW_GPIO_CR_INPUT_PUPD			= 0x8,
	HW_GPIO_CR_OUT2MHZ_PSPL 	= 0x2,
	HW_GPIO_CR_OUT2MHZ_OPDR 	= 0x6,
	HW_GPIO_CR_OUT10MHZ_PSPL 	= 0x1,
	HW_GPIO_CR_OUT10MHZ_OPDR 	= 0x5,
	HW_GPIO_CR_OUT50MHZ_PSPL 	= 0x3,
	HW_GPIO_CR_OUT50MHZ_OPDR 	= 0x7,
	HW_GPIO_CR_ALT2MHZ_PSPL 	= 0xA,
	HW_GPIO_CR_ALT2MHZ_OPDR 	= 0xE,
	HW_GPIO_CR_ALT10MHZ_PSPL 	= 0x9,
	HW_GPIO_CR_ALT10MHZ_OPDR 	= 0xD,
	HW_GPIO_CR_ALT50MHZ_PSPL 	= 0xB,
	HW_GPIO_CR_ALT50MHZ_OPDR 	= 0xF,
}hw_gpio_cr;

typedef enum {
	HW_GPIO_AF_TIMX,
	HW_GPIO_AF_USARTX,
	HW_GPIO_AF_SPIX,
	HW_GPIO_AF_I2S,
	HW_GPIO_AF_I2C,
	HW_GPIO_AF_CAN,
	HW_GPIO_AF_USB,
	HW_GPIO_AF_SDIO,
	HW_GPIO_AF_ADC_DAC,
	HW_GPIO_AF_FSMC,
}hw_gpio_af;

typedef enum {
	HW_GPIO_EXTI_NONE,
	HW_GPIO_EXTI_RISING,
	HW_GPIO_EXTI_FALLING,
	HW_GPIO_EXTI_BOTH,
}hw_gpio_exti_mode;

typedef enum {
	HW_GPIO_NOT_INIT,
	HW_GPIO_OK,
	HW_GPIO_WRONG_PERIPHERAL,
	HW_GPIO_ALREADY_INIT,
	HW_GPIO_ERROR,
}hw_gpio_flag;

typedef void(*hw_gpio_cb)(void *handle, bool state);

typedef struct {
	uint8_t pin_num;
	GPIO_TypeDef *port;
	hw_gpio_cr port_config;
	bool state;
	hw_gpio_exti_mode irq;
	hw_gpio_af alternate_function;
}hw_gpio_init_param;
 
typedef struct hw_gpio_inst_{
	bool is_init;
	bool previous_state;
	uint8_t pin_num;
	GPIO_TypeDef *port;
	hw_gpio_cr port_config;
	hw_gpio_flag flag;
	hw_gpio_exti_mode irq;
	hw_gpio_af alternate_function;
	hw_gpio_cb cb_rising;
	hw_gpio_cb cb_falling;
	hw_gpio_cb cb_both;
	void *handle;
	
	struct hw_gpio_inst_ *next_inst;
}hw_gpio_inst;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void hw_gpio_reset(hw_gpio_inst *inst);
void hw_gpio_init(hw_gpio_inst *inst, hw_gpio_init_param *param);
void hw_gpio_set_cb_rising(hw_gpio_inst *inst, hw_gpio_cb cb);
void hw_gpio_set_cb_falling(hw_gpio_inst *inst, hw_gpio_cb cb);
void hw_gpio_set_cb_both(hw_gpio_inst *inst, hw_gpio_cb cb);
void hw_gpio_set_handle(hw_gpio_inst *inst, void *handle);
void hw_gpio_set_state(hw_gpio_inst *inst, bool state);
void hw_gpio_toggle_state(hw_gpio_inst *inst);
bool hw_gpio_get_state(hw_gpio_inst *inst);

hw_gpio_flag hw_gpio_get_flag(hw_gpio_inst *inst);


void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif /* HW_GPIO_H_ */

/*

#include "stm32f10x.h"
#include "hw_gpio.h"

int main(void) {
	
	hw_gpio_init_param param = {
		.pin_num = 13,
		.port = GPIOC,
		.port_config = HW_GPIO_CR_OUT2MHZ_PSPL,
		.state = true,
	};
	hw_gpio_init_param param_1 = {
		.pin_num = 6,
		.port = GPIOA,
		.port_config = HW_GPIO_CR_INPUT_PUPD,
		.state = false,
		.irq = HW_GPIO_EXTI_BOTH,
	};
	
	hw_gpio_init(HW_GPIO_INST_1, &param);
	hw_gpio_init(HW_GPIO_INST_2, &param_1);
	
	while(1) {
		
		if(hw_gpio_get_state(HW_GPIO_INST_2)) {
			hw_gpio_toggle_state(HW_GPIO_INST_1);
			for(uint32_t i = 0; i < 100000; i++);
		}
		
	}
}



*/


