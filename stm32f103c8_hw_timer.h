/*!
 * \file hw_timer.h
 * \brief low level timer header
 * \author Adam LUCAS
 * \date 20/05/2022
 */

#ifndef HW_TIMER_H_
#define HW_TIMER_H_

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

#define HW_TIMER_INIT_COUNTER(reload_en, irq_en, dma_en, evt_en, upcount) { \
		.cr1 = 																																	\
				HW_TIMER_CKD_1 | HW_TIMER_ARPE_BUFFERED | 													\
				HW_TIMER_DIR_UP_COUNT | HW_TIMER_OPM_STOPPED,												\
		.irq = HW_TIMER_UIE,																										\
		.timer = TIM1,																													\
	}

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
	HW_TIMER_OK,
	HW_TIMER_ERROR,
	HW_TIMER_PERIPHERAL_ALREADY_USED,
}hw_timer_flag;

typedef enum {
	HW_TIMER_UIE							= (1u<<0),
	HW_TIMER_CC1IE						= (1u<<1),
	HW_TIMER_CC2IE						= (1u<<2),
	HW_TIMER_CC3IE						= (1u<<3),
	HW_TIMER_CC4IE						= (1u<<4),
	HW_TIMER_TIE							= (1u<<6),
	HW_TIMER_UDE							= (1u<<8),
	HW_TIMER_CC1DE						= (1u<<9),
	HW_TIMER_CC2DE						= (1u<<10),
	HW_TIMER_CC3DE						= (1u<<11),
	HW_TIMER_CC4DE						= (1u<<12),
	HW_TIMER_TDE							= (1u<<14),
}hw_timer_dier_mask;

typedef enum {
	HW_TIMER_CEN_DISABLE			= (0u<<0),
	HW_TIMER_CEN_ENABLE				= (1u<<0),
	HW_TIMER_UDIS_ENABLE			= (0u<<1),
	HW_TIMER_UDIS_DIABLE			= (1u<<1),
	HW_TIMER_URS_OTHER				= (0u<<2),
	HW_TIMER_URS_OVF_ONLY			= (1u<<2),
	HW_TIMER_OPM_NOT_STOPPED	= (1u<<3),
	HW_TIMER_OPM_STOPPED			= (0u<<3),
	HW_TIMER_DIR_UP_COUNT			= (0u<<4),
	HW_TIMER_DIR_DOWN_COUNT		= (1u<<4),
	HW_TIMER_CMS_EDGE_ALGN		= (0u<<5),
	HW_TIMER_CMS_MODE1				= (1u<<5),
	HW_TIMER_CMS_MODE2				= (2u<<5),
	HW_TIMER_CMS_MODE3				= (3u<<5),
	HW_TIMER_ARPE_NBUFFERED		= (0u<<7),
	HW_TIMER_ARPE_BUFFERED		= (1u<<7),
	HW_TIMER_CKD_1						= (0u<<8),
	HW_TIMER_CKD_2						= (1u<<8),
	HW_TIMER_CKD_4						= (2u<<8),
}hw_timer_cr1_mask;

typedef enum {
	
	//output compare mode
	
	HW_TIMER_OC2CE_DISABLE		= (0<<15),
	HW_TIMER_OC2CE_ENABLE			= (1<<15),
	HW_TIMER_OC2PE_DISABLE		= (0<<11),
	HW_TIMER_OC2PE_ENABLE			= (1<<11),
	HW_TIMER_OC2FE_DISABLE		= (0<<10),
	HW_TIMER_OC2FE_ENABLE			= (1<<10),
	HW_TIMER_CC2S_OUTPUT			= (0<<8),
	HW_TIMER_CC2S_IN_TI2			= (1<<8),
	HW_TIMER_CC2S_IN_TI1			= (2<<8),
	HW_TIMER_CC2S_IN_TRC			= (3<<8),
	HW_TIMER_OC1CE_DISABLE		= (0<<7),
	HW_TIMER_OC1CE_ENABLE			= (1<<7),
	HW_TIMER_OC1M_FROZEN			= (0<<4),
	HW_TIMER_OC1M_CH1_EN			= (1<<4),
	HW_TIMER_OC1M_CH1_DIS			= (2<<4),
	HW_TIMER_OC1M_TOGGLE_MTCH	= (3<<4),
	HW_TIMER_OC1M_OC1REF_LOW	= (4<<4),
	HW_TIMER_OC1M_OC1REF_HIGH	= (5<<4),
	HW_TIMER_OC1M_PWM1				= (6<<4),
	HW_TIMER_OC1M_PWM2				= (7<<4),
	HW_TIMER_OC1PE_DISABLE		= (0<<3),
	HW_TIMER_OC1PE_ENABLE			= (1<<3),
	HW_TIMER_OC1FE_NORMAL			= (0<<2),
	HW_TIMER_OC1FE_EDGE				= (1<<2),
	HW_TIMER_CC1S_OUTPUT			= (0<<0),
	HW_TIMER_CC1S_IN_TI1			= (1<<0),
	HW_TIMER_CC1S_IN_TI2			= (2<<0),
	HW_TIMER_CC1S_IN_TRC			= (3<<0),
	
	//input capture mode
	
}hw_timer_ccmr1_mask;
/*
typedef enum {
	
}hw_timer_ccmr2_mask;
*/
typedef void(*hw_timer_cb)(void *handle);

typedef struct {
	TIM_TypeDef *timer;
	hw_timer_dier_mask irq;
	hw_timer_cr1_mask cr1;
	hw_timer_ccmr1_mask ccmr1;
}hw_timer_param;
 
typedef struct hw_timer_inst_{
	hw_timer_flag flag;
	TIM_TypeDef *timer;
	hw_timer_dier_mask irq;
	hw_timer_cr1_mask cr1;
	hw_timer_cb update_cb;
	hw_timer_cb cc1_cb;
	hw_timer_cb cc2_cb;
	hw_timer_cb cc3_cb;
	hw_timer_cb cc4_cb;
	void *handle;
	
	struct hw_timer_inst_ *next_inst;
}hw_timer_inst;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void hw_timer_init(hw_timer_inst *inst, hw_timer_param *param);
void hw_timer_enable(hw_timer_inst *inst, bool enable);
void hw_timer_set_update_cb(hw_timer_inst *inst, hw_timer_cb cb);

void hw_timer_set_cc1_cb(hw_timer_inst *inst, hw_timer_cb cb);
void hw_timer_set_cc2_cb(hw_timer_inst *inst, hw_timer_cb cb);
void hw_timer_set_cc3_cb(hw_timer_inst *inst, hw_timer_cb cb);
void hw_timer_set_cc4_cb(hw_timer_inst *inst, hw_timer_cb cb);
void hw_timer_set_handle(hw_timer_inst *inst, void *handle);

void hw_timer_set_ccr1(hw_timer_inst *inst, uint16_t val);
void hw_timer_set_ccr2(hw_timer_inst *inst, uint16_t val);
void hw_timer_set_ccr3(hw_timer_inst *inst, uint16_t val);
void hw_timer_set_ccr4(hw_timer_inst *inst, uint16_t val);

uint16_t hw_timer_get_ccr1(hw_timer_inst *inst);
uint16_t hw_timer_get_ccr2(hw_timer_inst *inst);
uint16_t hw_timer_get_ccr3(hw_timer_inst *inst);
uint16_t hw_timer_get_ccr4(hw_timer_inst *inst);

void TIM1_BRK_IRQHandler(void);
void TIM1_UP_IRQHandler(void);
void TIM1_TRG_COM_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif /* HW_TIMER_H_ */
/*

#include "stm32f10x.h"
#include "hw_timer.h"

static void test_cb(void *handle) {
	(void)handle;
}


int main(void) {
	
	hw_timer_param param = {
		.cr1 = 
				HW_TIMER_CKD_1 | HW_TIMER_ARPE_BUFFERED | 
				HW_TIMER_DIR_UP_COUNT | HW_TIMER_OPM_STOPPED,
		.irq = HW_TIMER_UIE,
		.timer = TIM1,
	};
	
	hw_timer_init(HW_TIMER_INST_1, &param);
	hw_timer_enable(HW_TIMER_INST_1, true);
	hw_timer_set_update_cb(HW_TIMER_INST_1, test_cb);
	
	while(1) {
		
		
	}
}

*/


