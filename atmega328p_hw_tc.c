/*
 * hw_tc.c
 *
 * Created: 11/13/2021 9:42:15 AM
 *  Author: Adam
 */ 

#include "atmega328p_hw_tc.h"

static hw_tc_data *tc0;
static hw_tc_data *tc1;
static hw_tc_data *tc2;

static void hw_tc_default_cb(void *handle) {
	(void)handle;
}


static void hw_tc0_init(void) {
	TCCR0A = 0;
	TCCR0B = 0;
	TIMSK0 = 0;
	
	tc0->prescale_val = 0;
	tc0->enabled = false;
	tc0->compa_cb = hw_tc_default_cb;
	tc0->compa_handle = (void*)0;
	tc0->compb_cb = hw_tc_default_cb;
	tc0->compb_handle = (void*)0;
	tc0->tov_cb = hw_tc_default_cb;
	tc0->tov_handle = (void*)0;
	
#if HW_TC0_MODE_SEL != HW_TC_DISABLED
	PRR &= ~(1<<PRTIM0);
#endif
	
#if HW_TC0_MODE_SEL == HW_TC_TIM_COUNT
#elif HW_TC0_MODE_SEL == HW_TC_CTC
	TCCR0A |= (1<<WGM01);
#elif HW_TC0_MODE_SEL == HW_TC_F_PWM
	TCCR0A |= (1<<WGM01) | (1<<WGM00);
	TCCR0B |= (1<<WGM02);
#elif HW_TC0_MODE_SEL == HW_TC_N_PWM
	TCCR0A |= (1<<WGM00);
	TCCR0B |= (1<<WGM02);
#endif

#if HW_TC0_PSCALE_SEL == HW_TC_NO_PRESCALE
	tc0->prescale_val = (1<<CS00);
#elif HW_TC0_PSCALE_SEL == HW_TC_PSCALE_8
	tc0->prescale_val = (1<<CS01);
#elif HW_TC0_PSCALE_SEL == HW_TC_PSCALE_64
	tc0->prescale_val = (1<<CS01) | (1<<CS00);
#elif HW_TC0_PSCALE_SEL == HW_TC_PSCALE_256
	tc0->prescale_val = (1<<CS02);
#elif HW_TC0_PSCALE_SEL == HW_TC_PSCALE_1024
	tc0->prescale_val = (1<<CS02) | (1<<CS00);
#elif HW_TC0_PSCALE_SEL == HW_TC_PSCALE_T0_FE
	tc0->prescale_val = (1<<CS02) | (1<<CS01)
#elif HW_TC0_PSCALE_SEL == HW_TC_PSCALE_T0_RE
	tc0->prescale_val = (1<<CS02) | (1<<CS01) | (1<<CS00);
	DDRD &= ~(1<<DDD4);
	PORTD &= ~(1<<PORTD4);
#endif

#if HW_TC0_MODE_SEL != HW_TC_DISABLED
	TIMSK0 |=/* (1<<OCIE0B) | */(1<<OCIE0A)/* | (1<<TOIE0)*/;
	sei();
#endif
	
}

//TODO setup tc1

static void hw_tc1_init(void) {
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
	
	tc1->prescale_val = 0;
	tc1->enabled = false;
	tc1->compa_cb = hw_tc_default_cb;
	tc1->compa_handle = (void*)0;
	tc1->compb_cb = hw_tc_default_cb;
	tc1->compb_handle = (void*)0;
	tc1->tov_cb = hw_tc_default_cb;
	tc1->tov_handle = (void*)0;
	tc1->capt_cb = hw_tc_default_cb;
	tc1->capt_handle = (void *)0;
	
#if HW_TC1_MODE_SEL != HW_TC_DISABLED
	PRR &= ~(1<<PRTIM1);
#endif
	
#if HW_TC1_MODE_SEL == HW_TC1_TIM_COUNT
#elif HW_TC1_MODE_SEL == HW_TC1_N_PWM_8B
	TCCR1A |= (1<<WGM10);
#elif HW_TC1_MODE_SEL == HW_TC1_N_PWM_9B
	TCCR1A |= (1<<WGM11);
#elif HW_TC1_MODE_SEL == HW_TC1_N_PWM_10B
	TCCR1A |= (1<<WGM11) | (1<<WGM10);
#elif HW_TC1_MODE_SEL == HW_TC1_CTC
	TCCR1B |= (1<<WGM12);
#elif HW_TC1_MODE_SEL == HW_TC1_F_PWM_8B
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);
#elif HW_TC1_MODE_SEL == HW_TC1_F_PWM_9B
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12);
#elif HW_TC1_MODE_SEL == HW_TC1_F_PWM_10B
	TCCR1A |= (1<<WGM11) | (1<<WGM10);
	TCCR1B |= (1<<WGM12);
#elif HW_TC1_MODE_SEL == HW_TC1_N_PWM
	TCCR1A |= (1<<WGM11) | (1<<WGM10);
	TCCR1B |= (1<<WGM13);
#elif HW_TC1_MODE_SEL == HW_TC1_F_PWM
	TCCR1A |= (1<<WGM11) | (1<<WGM10);
	TCCR1B |= (1<<WGM13) | (1<<WGM12);
#endif

#if HW_TC1_PSCALE_SEL == HW_TC_NO_PRESCALE
	tc1->prescale_val = (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC_PSCALE_8
	tc1->prescale_val = (1<<CS11);
#elif HW_TC1_PSCALE_SEL == HW_TC_PSCALE_64
	tc1->prescale_val = (1<<CS11) | (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC_PSCALE_256
	tc1->prescale_val = (1<<CS12);
#elif HW_TC1_PSCALE_SEL == HW_TC_PSCALE_1024
	tc1->prescale_val = (1<<CS12) | (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC_PSCALE_T1_FE
	tc1->prescale_val = (1<<CS12) | (1<<CS11)
#elif HW_TC1_PSCALE_SEL == HW_TC_PSCALE_T1_RE
	tc1->prescale_val = (1<<CS12) | (1<<CS11) | (1<<CS10);
	DDRD &= ~(1<<DDD5);
	PORTD &= ~(1<<PORTD5);
#endif
	
#if HW_TC1_MODE_SEL != HW_TC1_DISABLED
	TIMSK1 |=/* (1<<TOIE1) | */(1<<OCIE1A)/* | (1<<OCIE1B) | (1<<ICIE1)*/;
	sei();
#endif
}

static void hw_tc2_init(void) {
	TCCR2A = 0;
	TCCR2B = 0;
	TIMSK2 = 0;
	
	tc2->prescale_val = 0;
	tc2->enabled = false;
	tc2->compa_cb = hw_tc_default_cb;
	tc2->compa_handle = (void*)0;
	tc2->compb_cb = hw_tc_default_cb;
	tc2->compb_handle = (void*)0;
	tc2->tov_cb = hw_tc_default_cb;
	tc2->tov_handle = (void*)0;
	
	#if HW_TC2_MODE_SEL != HW_TC_DISABLED
	PRR &= ~(1<<PRTIM2);
	#endif
	
	#if HW_TC2_MODE_SEL == HW_TC_TIM_COUNT
	#elif HW_TC2_MODE_SEL == HW_TC_CTC
	TCCR2A |= (1<<WGM21);
	#elif HW_TC2_MODE_SEL == HW_TC_F_PWM
	TCCR2A |= (1<<WGM21) | (1<<WGM20);
	TCCR2B |= (1<<WGM22);
	#elif HW_TC2_MODE_SEL == HW_TC_N_PWM
	TCCR2A |= (1<<WGM20);
	TCCR2B |= (1<<WGM22);
	#endif

	#if HW_TC2_PSCALE_SEL == HW_TC_NO_PRESCALE
	tc2->prescale_val = (1<<CS20);
	#elif HW_TC2_PSCALE_SEL == HW_TC_PSCALE_8
	tc2->prescale_val = (1<<CS21);
	#elif HW_TC2_PSCALE_SEL == HW_TC_PSCALE_64
	tc2->prescale_val = (1<<CS21) | (1<<CS20);
	#elif HW_TC2_PSCALE_SEL == HW_TC_PSCALE_256
	tc2->prescale_val = (1<<CS22);
	#elif HW_TC2_PSCALE_SEL == HW_TC_PSCALE_1024
	tc2->prescale_val = (1<<CS22) | (1<<CS20);
	#endif

	#if HW_TC2_MODE_SEL != HW_TC_DISABLED
	TIMSK2 |= (1<<OCIE2B) | (1<<OCIE2A) | (1<<TOIE2);
	sei();
	#endif
	
}

static void hw_tc0_enable(bool enable) {
#if HW_TC0_MODE_SEL != HW_TC_DISABLED
	if(enable) {
		if(tc0->enabled) return;
		tc0->enabled = true;
		TCCR0B &= ~HW_TC0_PSCL_MASK;
		TCCR0B |= tc0->prescale_val;
	}
	else {
		tc0->enabled = false;
		TCCR0B &= ~HW_TC0_PSCL_MASK;
	}
#endif
}

static void hw_tc1_enable(bool enable) {
	#if HW_TC1_MODE_SEL != HW_TC_DISABLED
	if(enable) {
		if(tc1->enabled) return;
		tc1->enabled = true;
		TCCR1B &= ~HW_TC1_PSCL_MASK;
		TCCR1B |= tc1->prescale_val;
	}
	else {
		tc1->enabled = false;
		TCCR1B &= ~HW_TC1_PSCL_MASK;
	}
	#endif
}

static void hw_tc2_enable(bool enable) {
	#if HW_TC2_MODE_SEL != HW_TC_DISABLED
	if(enable) {
		if(tc2->enabled) return;
		tc2->enabled = true;
		TCCR2B &= ~HW_TC2_PSCL_MASK;
		TCCR2B |= tc2->prescale_val;
	}
	else {
		tc2->enabled = false;
		TCCR2B &= ~HW_TC2_PSCL_MASK;
	}
	#endif
}

void hw_tc_periph_init(hw_tc_data *inst, hw_tc_data_param *param) {
	
	inst->peripheral = param->peripheral;
	
	switch(param->peripheral) {
		case LO_TC_PERIPH_0:
		tc0 = inst;
		hw_tc0_init();
		break;
		case LO_TC_PERIPH_1:
		tc1 = inst;
		hw_tc1_init();
		break;
		case LO_TC_PERIPH_2:
		tc2 = inst;
		hw_tc2_init();
		break;
	}
}

void hw_tc_enable(hw_tc_data *inst, bool enable) {
	switch(inst->peripheral) {
		case LO_TC_PERIPH_0:
		hw_tc0_enable(enable);
		break;
		case LO_TC_PERIPH_1:
		hw_tc1_enable(enable);
		break;
		case LO_TC_PERIPH_2:
		hw_tc2_enable(enable);
		break;
	}
}

void hw_tc_init_chan(hw_tc_data *inst, hw_tc_chan_param *param) {
	
	switch(param->channel) {
		case LO_TC_CHAN_COMPA:
		inst->compa_cb = param->cb;
		inst->compa_handle = param->handle;
		break;
		case LO_TC_CHAN_COMPB:
		inst->compb_cb = param->cb;
		inst->compb_handle = param->handle;
		break;
		case LO_TC_CHAN_TOV:
		inst->tov_cb = param->cb;
		inst->tov_handle = param->handle;
		break;
		case LO_TC_CHAN_CAPT:
		inst->capt_cb = param->cb;
		inst->capt_handle = param->handle;
		break;
	}
}

void hw_tc_set_val(hw_tc_data *inst, hw_tc_timer_chan channel, uint16_t val) {
	switch(inst->peripheral) {
		case LO_TC_PERIPH_0:
		switch(channel) {
			case LO_TC_CHAN_COMPA:
			OCR0A = (uint8_t)val;
			break;
			case LO_TC_CHAN_COMPB:
			OCR0B = (uint8_t)val;
			break;
			case LO_TC_CHAN_TOV:
			TCNT0 = (uint8_t)val;
			break;
			case LO_TC_CHAN_CAPT:
			break;
		}
		break;
		case LO_TC_PERIPH_1:
		switch(channel) {
			case LO_TC_CHAN_COMPA:
			OCR1A = val;
			break;
			case LO_TC_CHAN_COMPB:
			OCR1B = val;
			break;
			case LO_TC_CHAN_TOV:
			TCNT1 = val;
			break;
			case LO_TC_CHAN_CAPT:
			ICR1 = val;
			break;
		}
		break;
		case LO_TC_PERIPH_2:
		switch(channel) {
			case LO_TC_CHAN_COMPA:
			OCR2A = (uint8_t)val;
			break;
			case LO_TC_CHAN_COMPB:
			OCR2B = (uint8_t)val;
			break;
			case LO_TC_CHAN_TOV:
			TCNT2 = (uint8_t)val;
			break;
			case LO_TC_CHAN_CAPT:
			break;
		}
		break;
	}
}

uint16_t hw_tc_get_val(hw_tc_data *inst, hw_tc_timer_chan channel) {
	switch(inst->peripheral) {
		case LO_TC_PERIPH_0:
		switch(channel) {
			case LO_TC_CHAN_COMPA:
			return OCR0A;
			break;
			case LO_TC_CHAN_COMPB:
			return OCR0B;
			break;
			case LO_TC_CHAN_TOV:
			return TCNT0;
			break;
			case LO_TC_CHAN_CAPT:
			break;
		}
		break;
		case LO_TC_PERIPH_1:
		switch(channel) {
			case LO_TC_CHAN_COMPA:
			return OCR1A;
			break;
			case LO_TC_CHAN_COMPB:
			return OCR1B;
			break;
			case LO_TC_CHAN_TOV:
			return TCNT1;
			break;
			case LO_TC_CHAN_CAPT:
			return ICR1;
			break;
		}
		break;
		case LO_TC_PERIPH_2:
		switch(channel) {
			case LO_TC_CHAN_COMPA:
			return OCR2A;
			break;
			case LO_TC_CHAN_COMPB:
			return OCR2B;
			break;
			case LO_TC_CHAN_TOV:
			return TCNT2;
			break;
			case LO_TC_CHAN_CAPT:
			break;
		}
		break;
	}
	return 0;
}

ISR(TIMER0_COMPA_vect) {
	tc0->compa_cb(tc0->compa_handle);
	TIFR0 |= (1<<OCF0A);
}

ISR(TIMER0_COMPB_vect) {
	tc0->compb_cb(tc0->compb_handle);
	TIFR0 |= (1<<OCF0B);
}

ISR(TIMER0_OVF_vect) {
	tc0->tov_cb(tc0->tov_handle);
	TIFR0 |= (1<<TOV0);
}

ISR(TIMER1_COMPA_vect) {
	tc1->compa_cb(tc1->compa_handle);
	TIFR1 |= (1<<OCF1A);
}

ISR(TIMER1_COMPB_vect) {
	tc1->compb_cb(tc1->compb_handle);
	TIFR1 |= (1<<OCF1B);
}

ISR(TIMER1_OVF_vect) {
	tc1->tov_cb(tc1->tov_handle);
	TIFR1 |= (1<<TOV1);
}

ISR(TIMER1_CAPT_vect) {
	tc1->capt_cb(tc1->capt_handle);
	TIFR1 |= (1<<ICF1);
}

ISR(TIMER2_COMPA_vect) {
	tc2->compa_cb(tc2->compa_handle);
	TIFR2 |= (1<<OCF2A);
}

ISR(TIMER2_COMPB_vect) {
	tc2->compb_cb(tc2->compb_handle);
	TIFR2 |= (1<<OCF2B);
}

ISR(TIMER2_OVF_vect) {
	tc2->tov_cb(tc2->tov_handle);
	TIFR2 |= (1<<TOV2);
}