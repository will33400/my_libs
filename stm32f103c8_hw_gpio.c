/*!
 * \file hw_gpio.c
 * \brief low level gpio functions
 * \author Adam LUCAS
 * \date 14/05/2022
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "stm32f103c8_hw_gpio.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static hw_gpio_inst *first_inst = (void *)0;
static hw_gpio_inst *last_inst = (void *)0;

static bool lib_init;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn static void hw_gpio_default_cb(void *handle, bool state);
* \brief low level gpio instance default callback. DO NOT MODIFY
* \param[inout] handle user defined data pointer (not used)
* \param[in] state pin current state (not used)
* \return None
*/

static void hw_gpio_default_cb(void *handle, bool state) {
	(void)handle;
	(void)state;
}

//*****************************************************************************/
/*!
* \fn static void hw_gpio_reset_peripherals(void);
* \brief low level gpio peripherals complete reset routine
* \param None
* \return None
*/

static void hw_gpio_reset_peripherals(void) {
	
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	NVIC_ClearPendingIRQ(EXTI4_IRQn);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	NVIC_DisableIRQ(EXTI0_IRQn);
	NVIC_DisableIRQ(EXTI1_IRQn);
	NVIC_DisableIRQ(EXTI2_IRQn);
	NVIC_DisableIRQ(EXTI3_IRQn);
	NVIC_DisableIRQ(EXTI4_IRQn);
	NVIC_DisableIRQ(EXTI9_5_IRQn);
	NVIC_DisableIRQ(EXTI15_10_IRQn);
	
	RCC->APB2ENR &= ~0x00000001u; //enable EXTI clock
	
	EXTI->RTSR = 0x00000000; //reset EXTI peripheral
	EXTI->FTSR = 0x00000000;
	EXTI->IMR = 0x00000000;
	EXTI->EMR = 0x00000000;
	EXTI->PR = 0x00000000;
	EXTI->SWIER = 0x00000000;
	
	RCC->APB2RSTR |= 0x000000FDu; //reset all GPIO and AFIO peripherals
	RCC->APB2ENR &= ~0x000000FCu; //disable all GPIO clocks
	RCC->APB2RSTR &= ~0x000000FDu; //set all GPIO and AFIO peripherals
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_reset(hw_gpio_inst inst);
* \brief low level gpio instance reset routine
* \param[in] inst low level gpio instance number
* \return None
*/

void hw_gpio_reset(hw_gpio_inst *inst) {
	(void)inst;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_init(hw_gpio_inst inst, hw_gpio_init_param *param);
* \brief low level gpio instance init routine
* \param[in] inst low level gpio instance number
* \param[in] param instance init parameters
* \return None
*/

void hw_gpio_init(hw_gpio_inst *inst, hw_gpio_init_param *param) {
	
	
	if(!lib_init) hw_gpio_reset_peripherals(); // reset peripherals once
	lib_init = true;
	/*
	if(inst->is_init) { // returns if instance already init
		inst->flag = HW_GPIO_ALREADY_INIT;
		return;
	}*/
	
	if(last_inst)
		last_inst->next_inst = inst;
	if(!first_inst)
		first_inst = inst;
	last_inst = inst;
	
	inst->pin_num = param->pin_num;
	inst->port = param->port;
	inst->port_config = param->port_config;
	inst->irq = param->irq;
	inst->alternate_function = param->alternate_function;
	inst->cb_rising = hw_gpio_default_cb;
	inst->cb_falling = hw_gpio_default_cb;
	inst->cb_both = hw_gpio_default_cb;
	
	RCC->APB2ENR |= (1<<0);
	if(param->port == GPIOA) // enable peripheral clock
		RCC->APB2ENR |= (1<<2);
	else if(param->port == GPIOB)
		RCC->APB2ENR |= (1<<3);
	else if(param->port == GPIOC)
		RCC->APB2ENR |= (1<<4);
	else if(param->port == GPIOD)
		RCC->APB2ENR |= (1<<5);
	else if(param->port == GPIOE)
		RCC->APB2ENR |= (1<<6);
	else if(param->port == GPIOF)
		RCC->APB2ENR |= (1<<7);
	else if(param->port == GPIOG)
		RCC->APB2ENR |= (1<<8);
	else {
		inst->flag = HW_GPIO_WRONG_PERIPHERAL;
		return;
	}
	
	if(inst->pin_num < 8) { //set port config
		param->port->CRL &= ~(0xFu<<(param->pin_num * 4));
		param->port->CRL |= ((uint32_t)param->port_config<<(param->pin_num * 4));
	}
	else {
		param->port->CRH &= ~(0xFu<<((param->pin_num - 8) * 4));
		param->port->CRH |= ((uint32_t)param->port_config<<((param->pin_num - 8) * 4));
	}
	
	if(param->state) // set default state
		param->port->ODR |= ((uint32_t)param->state<<param->pin_num);
	else 
		param->port->ODR &= ~((uint32_t)param->state<<param->pin_num);
	
	inst->previous_state = 
			((1u<<(uint32_t)inst->pin_num) & inst->port->IDR) ? true : false;
	
	switch(param->irq) { // set interrupts
		case HW_GPIO_EXTI_NONE:
		break;
		case HW_GPIO_EXTI_FALLING:
			EXTI->IMR |= (1<<param->pin_num);
			EXTI->FTSR |= (1<<param->pin_num);
		break;
		case HW_GPIO_EXTI_RISING:
			EXTI->IMR |= (1<<param->pin_num);
			EXTI->RTSR |= (1<<param->pin_num);
		break;
		case HW_GPIO_EXTI_BOTH:
			EXTI->IMR |= (1<<param->pin_num);
			EXTI->FTSR |= (1<<param->pin_num);
			EXTI->RTSR |= (1<<param->pin_num);
		break;
	}
	
	if(param->irq) {
		
		AFIO->EXTICR[param->pin_num / 4] |= (
			(
					param->port == GPIOA ? 0u :
					param->port == GPIOB ? 1u :
					param->port == GPIOC ? 2u :
					param->port == GPIOD ? 3u :
					param->port == GPIOE ? 4u :
					param->port == GPIOF ? 5u :
					param->port == GPIOG ? 6u : 0u
			)<<((param->pin_num % 4)* 4));
		
		if(param->pin_num == 0) {
			NVIC_SetPriority(EXTI0_IRQn, 5);
			NVIC_EnableIRQ(EXTI0_IRQn);
		}
		else if(param->pin_num == 1) {
			NVIC_SetPriority(EXTI1_IRQn, 5);
			NVIC_EnableIRQ(EXTI1_IRQn);	
		}
		else if(param->pin_num == 2) {
			NVIC_SetPriority(EXTI2_IRQn, 5);
			NVIC_EnableIRQ(EXTI2_IRQn);
		}
		else if(param->pin_num == 3) {
			NVIC_SetPriority(EXTI3_IRQn, 5);
			NVIC_EnableIRQ(EXTI3_IRQn);
		}
		else if(param->pin_num == 4) {
			NVIC_SetPriority(EXTI4_IRQn, 5);
			NVIC_EnableIRQ(EXTI4_IRQn);
		}
		else if(param->pin_num >= 5 && param->pin_num <= 9) {
			NVIC_SetPriority(EXTI9_5_IRQn, 5);
			NVIC_EnableIRQ(EXTI9_5_IRQn);
		}
		else if(param->pin_num >= 10 && param->pin_num <= 15) {
			NVIC_SetPriority(EXTI15_10_IRQn, 5);
			NVIC_EnableIRQ(EXTI15_10_IRQn);
		}
	}
	
	inst->is_init = true;
	inst->flag = HW_GPIO_OK;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_set_cb_rising(hw_gpio_inst inst, hw_gpio_cb cb);
* \brief low level gpio rising edge interrupt callback set routine
* \param[in] inst low level gpio instance number
* \param[in] cb user defined callback
* \return None
*/

void hw_gpio_set_cb_rising(hw_gpio_inst *inst, hw_gpio_cb cb) {
	if(!inst->is_init) {
		inst->flag = HW_GPIO_NOT_INIT;
		return;
	}
	inst->cb_rising = cb;
	inst->flag = HW_GPIO_OK;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_set_cb_falling(hw_gpio_inst inst, hw_gpio_cb cb);
* \brief low level gpio falling edge interrupt callback set routine
* \param[in] inst low level gpio instance number
* \param[in] cb user defined callback
* \return None
*/

void hw_gpio_set_cb_falling(hw_gpio_inst *inst, hw_gpio_cb cb) {
	if(!inst->is_init) {
		inst->flag = HW_GPIO_NOT_INIT;
		return;
	}
	inst->cb_falling = cb;
	inst->flag = HW_GPIO_OK;
}

void hw_gpio_set_cb_both(hw_gpio_inst *inst, hw_gpio_cb cb) {
	if(!inst->is_init) {
		inst->flag = HW_GPIO_NOT_INIT;
		return;
	}
	inst->cb_both = cb;
	inst->flag = HW_GPIO_OK;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_set_handle(hw_gpio_inst inst, void *handle);
* \brief low level gpio set data pointer for falling and rising edge custom argument routine
* \param[in] inst low level gpio instance number
* \param[in] handle user defined data pointer
* \return None
*/

void hw_gpio_set_handle(hw_gpio_inst *inst, void *handle) {
	if(!inst->is_init) {
		inst->flag = HW_GPIO_NOT_INIT;
		return;
	}
	inst->handle = handle;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_set_state(hw_gpio_inst inst, bool state);
* \brief low level gpio set current state routine
* \param[in] inst low level gpio instance number
* \param[in] state pin state (true : HIGH, false : LOW)
* \return None
*/

void hw_gpio_set_state(hw_gpio_inst *inst, bool state) {
	if(!inst->is_init) {
		inst->flag = HW_GPIO_NOT_INIT;
		return;
	}
	
	if(state)
		inst->port->ODR |= ((uint32_t)1<<inst->pin_num);
	else
		inst->port->ODR &= ~((uint32_t)1<<inst->pin_num);
	
	inst->previous_state = state;
	inst->flag = HW_GPIO_OK;
}

//*****************************************************************************/
/*!
* \fn void hw_gpio_toggle_state(hw_gpio_inst inst);
* \brief low level gpio toggle current state routine
* \param[in] inst low level gpio instance number
* \return None
*/

void hw_gpio_toggle_state(hw_gpio_inst *inst) {
	if(!inst->is_init) {
		inst->flag = HW_GPIO_NOT_INIT;
		return;
	}
	
	if(inst->port->ODR & (1<<inst->pin_num)) {
		inst->port->ODR &= ~(1<<inst->pin_num);
		inst->previous_state = false;
	}
	else {
		inst->port->ODR |= (1<<inst->pin_num);
		inst->previous_state = true;
	}
	
	inst->flag = HW_GPIO_OK;
}

//*****************************************************************************/
/*!
* \fn bool hw_gpio_get_state(hw_gpio_inst inst);
* \brief low level gpio get current state routine
* \param[in] inst low level gpio instance number
* \return true (HIGH) else false (LOW)
*/

bool hw_gpio_get_state(hw_gpio_inst *inst) {
	if(!inst->is_init) {
		inst->flag = HW_GPIO_NOT_INIT;
		return false;
	}
	inst->flag = HW_GPIO_OK;
	
	inst->previous_state = 
			((1u<<inst->pin_num) & inst->port->IDR) ? true : false;
	
	return inst->previous_state;
}

//*****************************************************************************/
/*!
* \fn hw_gpio_flag hw_gpio_get_flag(hw_gpio_inst inst);
* \brief low level gpio get routines state flag routine
* \param[in] inst low level gpio instance number
* \return flag state
*/

hw_gpio_flag hw_gpio_get_flag(hw_gpio_inst *inst) {
	return inst->flag;
}

//*****************************************************************************/
/*!
* \fn static void EXTIx_IRQHandler(void);
* \brief low level gpio generic interrupt routine
* \param None
* \return None
*/

static void EXTIx_IRQHandler(void) {
	hw_gpio_inst *temp_inst = first_inst;
	uint32_t temp = EXTI->PR;
	EXTI->PR = 0x000FFFFF;
	
	while(temp_inst) {
		if(
				temp_inst->is_init &&
				((1<<temp_inst->pin_num) & temp) &&
				temp_inst->irq &&
				temp_inst->previous_state != (
						temp_inst->port->IDR & 
						(1<<temp_inst->pin_num) ? true : false
				)
		) {
			temp &= ~(1<<temp_inst->pin_num);
			temp_inst->previous_state = (
					temp_inst->port->IDR & 
					(1<<temp_inst->pin_num) ? true : false
			);
					
			switch(temp_inst->irq) {
				case HW_GPIO_EXTI_NONE:
				break;
				case HW_GPIO_EXTI_RISING: {
					if(temp_inst->previous_state == true)
						temp_inst->cb_rising(temp_inst->handle, true);
				}
				break;
				case HW_GPIO_EXTI_FALLING: {
					if(temp_inst->previous_state == false)
						temp_inst->cb_falling(temp_inst->handle, false);
				}
				break;
				case HW_GPIO_EXTI_BOTH: {
					if(temp_inst->previous_state == true)
						temp_inst->cb_rising(temp_inst->handle, true);
					else
						temp_inst->cb_falling(temp_inst->handle, false);
					
					temp_inst->cb_both(temp_inst->handle, hw_gpio_get_state(temp_inst));
				}
				break;
			}
		}
		temp_inst = temp_inst->next_inst;
	}
} 


void EXTI0_IRQHandler(void) {
	EXTIx_IRQHandler();
}

void EXTI1_IRQHandler(void) {
	EXTIx_IRQHandler();
}

void EXTI2_IRQHandler(void) {
	EXTIx_IRQHandler();
}

void EXTI3_IRQHandler(void) {
	EXTIx_IRQHandler();
}

void EXTI4_IRQHandler(void) {
	EXTIx_IRQHandler();
}

void EXTI9_5_IRQHandler(void) {
	EXTIx_IRQHandler();
}

void EXTI15_10_IRQHandler(void) { //doesn't work correctly !!
	EXTIx_IRQHandler();
}





