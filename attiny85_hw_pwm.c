/*
 * pwm.c
 *
 * Created: 7/20/2020 4:18:28 PM
 *  Author: Adam
 */ 


#include "attiny85_hw_pwm.h"

static timer_t pwm_timer;

void init_pwm(timer_t name, pwm_t type) {
	pwm_timer = name;
	switch(name) {
		case TIMER0:
		switch(type) {
			case CORRECT_PWM:
				TCCR0A = TCCR0A_correct_PWM;
				TCCR0B = TCCR0B_correct_PWM;
			break;
			case FAST_PWM:
				TCCR0A = TCCR0A_fast_PWM;
				TCCR0B = TCCR0B_fast_PWM;
			break;
			default:
			DDRB |= (1<<PB1) | (1<<PB0);
		}
		break;
		case TIMER1:
			TCCR1 = TCCR1_prescaler;
			GTCCR = GTCCR_PWM;
			PLLCSR = PLLCSR_PWM;
			OCR1C = OCR1C_value;
			DDRB |= (1<<PB4) | (1<<PB1);
		break;
	}
}

void set_pwm(uint8_t pin, uint16_t value) {
	switch(pin) {
		case PB0:
			OCR0A = value;
		break;
		case PB1:
			switch(pwm_timer) {
				case TIMER0:
					
					OCR0B = value;
				break;
				case TIMER1:
					TCCR1 |= TCCR1_PWM;
					OCR1A = value;
					TCCR1 &= ~TCCR1_PWM;
				break;
			}
		break;
		case PB4:
			//GTCCR |= GTCCR_PWM;
			while(!(OCR1B&(0x00ff&value)))
				OCR1B = value;
			while(!(TIFR&(1<<OCF1B)));
			TIFR |= (1<<OCF1B);
			//GTCCR &= ~GTCCR_PWM;
		break;
	}
}