/*!
 * \file hw_timer.c
 * \brief low level timer functions
 * \author Adam LUCAS
 * \date 20/05/2022
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "stm32f103c8_hw_timer.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static hw_timer_inst *first_inst = (void *)0;
static hw_timer_inst *last_inst = (void *)0;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

static void hw_timer_default_cb(void *handle) {
	(void)handle;
}	

//*****************************************************************************/
/*!
* \fn <function declaration>
* \brief <description>
* \param[in] <argument name> <description>
* \param[out] <argument name> <description>
* \return [<anything>] [None]
*/

static void hw_timer_peripheral_reset(TIM_TypeDef *timer) {
	
	if(timer == TIM1) {
		NVIC_ClearPendingIRQ(TIM1_BRK_IRQn);
		NVIC_DisableIRQ(TIM1_BRK_IRQn);
		NVIC_ClearPendingIRQ(TIM1_UP_IRQn);
		NVIC_DisableIRQ(TIM1_UP_IRQn);
		NVIC_ClearPendingIRQ(TIM1_TRG_COM_IRQn);
		NVIC_DisableIRQ(TIM1_TRG_COM_IRQn);
		NVIC_ClearPendingIRQ(TIM1_CC_IRQn);
		NVIC_DisableIRQ(TIM1_CC_IRQn);
		NVIC_ClearPendingIRQ(TIM1_CC_IRQn);
		NVIC_DisableIRQ(TIM1_CC_IRQn);
			
		RCC->APB2RSTR |= (1u<<11);
		RCC->APB2RSTR &= ~(1u<<11);
	}
	else if(timer == TIM2) {
		NVIC_ClearPendingIRQ(TIM2_IRQn);
		NVIC_DisableIRQ(TIM2_IRQn);
		
		RCC->APB1RSTR |= (1u<<0);
		RCC->APB1RSTR &= ~(1u<<0);
	}
	else if(timer == TIM3) {
		NVIC_ClearPendingIRQ(TIM3_IRQn);
		NVIC_DisableIRQ(TIM3_IRQn);
		
		RCC->APB1RSTR |= (1u<<1);
		RCC->APB1RSTR &= ~(1u<<1);
	}
	else if(timer == TIM4) {
		NVIC_ClearPendingIRQ(TIM4_IRQn);
		NVIC_DisableIRQ(TIM4_IRQn);
		
		RCC->APB1RSTR |= (1u<<2);
		RCC->APB1RSTR &= ~(1u<<2);
	}
	else if(timer == TIM5) {
		RCC->APB1RSTR |= (1u<<3);
		RCC->APB1RSTR &= ~(1u<<3);
	}
	else if(timer == TIM6) {
		RCC->APB1RSTR |= (1u<<4);
		RCC->APB1RSTR &= ~(1u<<4);
	}
	else if(timer == TIM7) {
		RCC->APB1RSTR |= (1u<<5);
		RCC->APB1RSTR &= ~(1u<<5);
	}
}

void hw_timer_init(hw_timer_inst *inst, hw_timer_param *param) {
	
	hw_timer_inst *temp_inst = first_inst;
	
	if(last_inst)
		last_inst->next_inst = inst;
	if(!first_inst)
		first_inst = inst;
	last_inst = inst;
	
	while(temp_inst) {
		if(temp_inst->timer == param->timer) {
			inst->flag = HW_TIMER_PERIPHERAL_ALREADY_USED;
			return;
		}
		temp_inst = temp_inst->next_inst;
	}
	
	hw_timer_peripheral_reset(param->timer);
	
	inst->timer = param->timer;
	inst->irq = param->irq;
	inst->cr1 = param->cr1;
	inst->cc1_cb = hw_timer_default_cb;
	inst->update_cb = hw_timer_default_cb;
	
	if(param->timer == TIM1) 
		RCC->APB2ENR |= (1<<11);
	else if(param->timer == TIM2)
		RCC->APB2ENR |= (1u<<0);
	else if(param->timer == TIM3)
		RCC->APB2ENR |= (1u<<1);
	else if(param->timer == TIM4)
		RCC->APB2ENR |= (1u<<2);
	else if(param->timer == TIM5)
		RCC->APB2ENR |= (1u<<3);
	else if(param->timer == TIM6)
		RCC->APB2ENR |= (1u<<4);
	else if(param->timer == TIM7)
		RCC->APB2ENR |= (1u<<5);
	else {
		inst->flag = HW_TIMER_ERROR;
		return;
	}
	
	//param->timer->CCMR1 |= param->ccmr1;
	
	param->timer->CR1 |= param->cr1;
	param->timer->PSC = 0x0000;
	
	param->timer->DIER |= param->irq;
	
	if(param->timer == TIM1 && param->irq) {
		if(param->irq & HW_TIMER_UIE) {
			NVIC_SetPriority(TIM1_UP_IRQn, 5);
			NVIC_EnableIRQ(TIM1_UP_IRQn);
		}
		if( //complete condition for cc
				(param->irq & (
						HW_TIMER_CC1IE | 
						HW_TIMER_CC2IE | 
						HW_TIMER_CC3IE | 
						HW_TIMER_CC4IE)
			)
		) {
			NVIC_SetPriority(TIM1_CC_IRQn, 5);
			NVIC_EnableIRQ(TIM1_CC_IRQn);
		}
		if( //complete condition for com
				param->irq & (
						HW_TIMER_CC1IE | 
						HW_TIMER_CC2IE | 
						HW_TIMER_CC3IE | 
						HW_TIMER_CC4IE)
		) {
			NVIC_SetPriority(TIM1_BRK_IRQn, 5);
			NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
		}
		if(param->irq & HW_TIMER_TIE) {
			NVIC_SetPriority(TIM1_BRK_IRQn, 5);
			NVIC_EnableIRQ(TIM1_BRK_IRQn);
		}
			
	}
	else if(param->timer == TIM2 && param->irq) {
		NVIC_SetPriority(TIM2_IRQn, 5);
	}
	else if(param->timer == TIM3 && param->irq) {
		NVIC_SetPriority(TIM3_IRQn, 5);
	}
	else if(param->timer == TIM4 && param->irq) {
		NVIC_SetPriority(TIM4_IRQn, 5);
	}
	
	if(param->ccmr1 & HW_TIMER_OC1PE_ENABLE)
		param->timer->CCER |= (1<<0);
	else if(param->ccmr1 & HW_TIMER_OC2PE_ENABLE)
		param->timer->CCER |= (1<<4);
}

void hw_timer_enable(hw_timer_inst *inst, bool enable) {
	if(enable)
		inst->timer->CR1 |= HW_TIMER_CEN_ENABLE;
	else
		inst->timer->CR1 &= ~HW_TIMER_CEN_ENABLE;
}

void hw_timer_set_update_cb(hw_timer_inst *inst, hw_timer_cb cb) {
	inst->update_cb = cb;
}

void hw_timer_set_cc1_cb(hw_timer_inst *inst, hw_timer_cb cb) {
	inst->cc1_cb = cb;
}

void hw_timer_set_cc2_cb(hw_timer_inst *inst, hw_timer_cb cb) {
	inst->cc2_cb = cb;
}

void hw_timer_set_cc3_cb(hw_timer_inst *inst, hw_timer_cb cb) {
	inst->cc3_cb = cb;
}

void hw_timer_set_cc4_cb(hw_timer_inst *inst, hw_timer_cb cb) {
	inst->cc4_cb = cb;
}

void hw_timer_set_handle(hw_timer_inst *inst, void *handle) {
	inst->handle = handle;
}

void hw_timer_set_ccr1(hw_timer_inst *inst, uint16_t val) {
	inst->timer->CCR1 = val;
}

void hw_timer_set_ccr2(hw_timer_inst *inst, uint16_t val) {
	inst->timer->CCR2 = val;
}

void hw_timer_set_ccr3(hw_timer_inst *inst, uint16_t val) {
	inst->timer->CCR3 = val;
}

void hw_timer_set_ccr4(hw_timer_inst *inst, uint16_t val) {
	inst->timer->CCR4 = val;
}

uint16_t hw_timer_get_ccr1(hw_timer_inst *inst) {
	return inst->timer->CCR1;
}

uint16_t hw_timer_get_ccr2(hw_timer_inst *inst) {
	return inst->timer->CCR2;
}

uint16_t hw_timer_get_ccr3(hw_timer_inst *inst) {
	return inst->timer->CCR3;
}

uint16_t hw_timer_get_ccr4(hw_timer_inst *inst) {
	return inst->timer->CCR4;
}


void TIM1_BRK_IRQHandler(void) {
}

void TIM1_UP_IRQHandler(void) {
	hw_timer_inst *temp_inst = first_inst;
	
	TIM1->SR &= ~(1<<0);
	
	while(temp_inst) {
		if(temp_inst->timer == TIM1)
			temp_inst->update_cb(temp_inst->handle);
		temp_inst = temp_inst->next_inst;
	}
}

void TIM1_TRG_COM_IRQHandler(void) {
}

void TIM1_CC_IRQHandler(void) {
	hw_timer_inst *temp_inst = first_inst;
	
	TIM1->SR &= ~(1<<1);
	
	while(temp_inst) {
		if(temp_inst->timer == TIM1)
			temp_inst->cc1_cb(temp_inst->handle);
		temp_inst = temp_inst->next_inst;
	}
	
	TIM1->CNT = 0;
}

void TIM2_IRQHandler(void) {
	
}

void TIM3_IRQHandler(void) {
	
}

void TIM4_IRQHandler(void) {
	
}
