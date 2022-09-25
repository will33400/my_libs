/*
 * hw_tc.h
 *
 * Created: 11/13/2021 9:42:01 AM
 *  Author: Adam
 */ 


#ifndef HW_TC_H_
#define HW_TC_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "atmega328p_hw_defines.h"

#define HW_TC_NUM			3
#define HW_TC0_PSCL_MASK	((1<<CS02) | (1<<CS01) | (1<<CS00))
#define HW_TC1_PSCL_MASK	((1<<CS12) | (1<<CS11) | (1<<CS10))
#define HW_TC2_PSCL_MASK	((1<<CS22) | (1<<CS21) | (1<<CS20))

#define HW_TC_DISABLED		0
#define HW_TC_TIM_COUNT		1
#define HW_TC_CTC			2
#define HW_TC_F_PWM			3
#define HW_TC_N_PWM			4

#define HW_TC1_DISABLED		0
#define HW_TC1_TIM_COUNT	1
#define HW_TC1_N_PWM_8B		2
#define HW_TC1_N_PWM_9B		3
#define HW_TC1_N_PWM_10B	4
#define HW_TC1_CTC			5
#define HW_TC1_F_PWM_8B		6
#define HW_TC1_F_PWM_9B		7
#define HW_TC1_F_PWM_10B	8
#define HW_TC1_N_PWM		9
#define HW_TC1_F_PWM		10

#ifndef HW_TC0_MODE_SEL
#warning HW_TC0_MODE_SEL undefined
#define HW_TC0_MODE_SEL		HW_TC_DISABLED
#endif
#ifndef HW_TC1_MODE_SEL
#warning HW_TC1_MODE_SEL undefined
#define HW_TC1_MODE_SEL		HW_TC_DISABLED
#endif
#ifndef HW_TC2_MODE_SEL
#warning HW_TC2_MODE_SEL undefined
#define HW_TC2_MODE_SEL		HW_TC1_DISABLED
#endif

#define HW_TC_NO_PRESCALE	0
#define HW_TC_PSCALE_8		1
#define HW_TC_PSCALE_64		2
#define HW_TC_PSCALE_256	3
#define HW_TC_PSCALE_1024	4

#define HW_TC_PSCALE_T0_FE	5
#define HW_TC_PSCALE_T0_RE	6

#define HW_TC_PSCALE_T1_FE	7
#define HW_TC_PSCALE_T1_RE	8

#if HW_TC0_MODE_SEL != HW_TC_DISABLED

#ifndef HW_TC0_PSCALE_SEL
#warning HW_TC0_PSCALE_SEL undefined
#define HW_TC0_PSCALE_SEL	HW_TC_NO_PRESCALE
#endif

#if HW_TC0_PSCALE_SEL == HW_TC_PSCALE_T1_FE || HW_TC0_PSCALE_SEL == HW_TC_PSCALE_T1_RE
#warning wrong setting for HW_TC2_PSCALE_SEL, select another clock source
#endif

#endif

#if HW_TC1_MODE_SEL != HW_TC1_DISABLED

#ifndef HW_TC1_PSCALE_SEL
#warning HW_TC1_PSCALE_SEL undefined
#define HW_TC1_PSCALE_SEL	HW_TC_NO_PRESCALE
#endif

#if HW_TC1_PSCALE_SEL == HW_TC_PSCALE_T0_FE || HW_TC1_PSCALE_SEL == HW_TC_PSCALE_T0_RE
#warning wrong setting for HW_TC1_PSCALE_SEL, select another clock source
#endif

#endif

#if HW_TC2_MODE_SEL != HW_TC_DISABLED

#ifndef HW_TC2_PSCALE_SEL
#warning HW_TC2_PSCALE_SEL undefined
#define HW_TC2_PSCALE_SEL	HW_TC_NO_PRESCALE
#endif

#if HW_TC2_PSCALE_SEL == HW_TC_PSCALE_T0_FE || HW_TC2_PSCALE_SEL == HW_TC_PSCALE_T0_RE ||\
	HW_TC2_PSCALE_SEL == HW_TC_PSCALE_T1_FE || HW_TC2_PSCALE_SEL == HW_TC_PSCALE_T1_RE
#warning wrong setting for HW_TC2_PSCALE_SEL, select another clock source
#endif

#endif

typedef void(*hw_tc_cb)(void *handle);

typedef enum {
	LO_TC_PERIPH_0,
	LO_TC_PERIPH_1,
	LO_TC_PERIPH_2,
}hw_tc_timer_periph;

typedef enum {
	LO_TC_CHAN_COMPA,
	LO_TC_CHAN_COMPB,
	LO_TC_CHAN_TOV,
	LO_TC_CHAN_CAPT,
}hw_tc_timer_chan;

typedef struct {
	hw_tc_timer_periph peripheral;
}hw_tc_data_param;

typedef struct hw_tc_data_{
	hw_tc_timer_periph peripheral;
	uint8_t prescale_val;
	bool enabled;
	hw_tc_cb compa_cb;
	void *compa_handle;
	hw_tc_cb compb_cb;
	void *compb_handle;
	hw_tc_cb tov_cb;
	void *tov_handle;
	hw_tc_cb capt_cb;
	void *capt_handle;
	
	struct hw_tc_data_ *next_inst;
}hw_tc_data;

typedef struct {
	hw_tc_cb cb;
	void *handle;
	hw_tc_timer_chan channel;
}hw_tc_chan_param;

void hw_tc_periph_init(hw_tc_data *inst, hw_tc_data_param *param);
void hw_tc_enable(hw_tc_data *inst, bool enable);
void hw_tc_init_chan(hw_tc_data *inst, hw_tc_chan_param *param);
void hw_tc_set_val(hw_tc_data *inst, hw_tc_timer_chan channel, uint16_t val);
uint16_t hw_tc_get_val(hw_tc_data *inst, hw_tc_timer_chan channel);

#endif /* HW_TC_H_ */