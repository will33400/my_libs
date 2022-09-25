/*
 * pwm.h
 *
 * Created: 7/20/2020 4:18:40 PM
 *  Author: Adam
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <stdint.h>

//OC0A   PB0
#define	AN_PB0				  (1<<PB0)
//OC0B / OC1A   PB1
#define	AN_PB1				  (1<<PB1)
//OC1B   PB4
#define	AN_PB4				  (1<<PB4)
//TIMER0
//0   no source, 1   no prescaler, 2   Fosc/8, 3   Fosc/64,
//4   Fosc/256, 5   Fosc/1024, 6   T0 clock falling edge, 7   T0 clock rising edge
#define	TCCR0B_prescaler	  /*(1<<CS02) | (1<<CS01) | */(1<<CS00)

#define	TCCR0A_fast_PWM		  (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00)
#define	TCCR0B_fast_PWM		  TCCR0B_prescaler

#define	TCCR0A_correct_PWM	  (1<<COM0A1) | (1<<COM0B1) | (1<<WGM00)
#define	TCCR0B_correct_PWM	  TCCR0B_prescaler
//TIMER1
// 1   PCK / CK
#define	TCCR1_prescaler		  /*(1<<CS12) | (1<<CS11) | */(1<<CS10)

#define	cpwm_bit			  (1<<LSM)

#define	TCCR1_PWM			  (1<<PWM1A) | (1<<COM1A0) | TCCR1_prescaler
#define	GTCCR_PWM			  (1<<PWM1B) | (1<<COM1B0)
#define	PLLCSR_PWM			  (1<<PCKE) /*| (1<<PLLE)*/
#define	OCR1C_value			  0xff

typedef enum {
	FAST_PWM,
	CORRECT_PWM,
}pwm_t;

typedef enum {
	TIMER0,
	TIMER1,
}timer_t;

void init_pwm(timer_t name, pwm_t type);
void set_pwm(uint8_t pin, uint16_t value);

#endif /* PWM_H_ */