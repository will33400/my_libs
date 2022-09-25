// *****************************************************************************
/*! 
 * \file lo_timer.h
 * \brief Basic timer program functions header
 * \author ATEK
 * \version 1.0
 * \date 2018 June
 */

// This is a guard condition so that contents of this file are not included
// more than once. 
//#ifndef _LO_TIMER_H    /* Guard against multiple inclusion */
//#define _LO_TIMER_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

//#include <stdint.h>
//#include <stdbool.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>

/* Provide C++ Compatibility */
//#ifdef __cplusplus
//extern "C" {
//#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************
/*
#define dTIMER_TEST				0
#define dTIMER_I2C				1
#define dTIMER_DELAY			2
#define dTIMER_SW				3

#define dTIMER_NUM              4  //Number of managed timer

#define dTIMER_PERIOD(dtimer_delay) (dtimer_delay / 60) //Period of timer is 1us

#define dTIMER_60US				dTIMER_PERIOD(60)
#define dTIMER_120US            dTIMER_PERIOD(100)
#define dTIMER_240US            dTIMER_PERIOD(240)
#define dTIMER_400US            dTIMER_PERIOD(400)
#define dTIMER_500US            dTIMER_PERIOD(500)
#define dTIMER_740US            dTIMER_PERIOD(740)
#define dTIMER_1MS              dTIMER_PERIOD(1000)
#define dTIMER_2MS              dTIMER_PERIOD(2000)
#define dTIMER_5MS              dTIMER_PERIOD(5000)
#define dTIMER_10MS             dTIMER_PERIOD(10000)
#define dTIMER_100MS            dTIMER_PERIOD(100000)
#define dTIMER_200MS            dTIMER_PERIOD(200000)
#define dTIMER_400MS            dTIMER_PERIOD(400000)
#define dTIMER_500MS            dTIMER_PERIOD(500000)
#define dTIMER_750MS            dTIMER_PERIOD(750000)
#define dTIMER_1S               dTIMER_PERIOD(1000000)
#define dTIMER_2S               dTIMER_PERIOD(2000000)
#define dTIMER_3S               dTIMER_PERIOD(3000000)
#define dTIMER_5S               dTIMER_PERIOD(5000000)
#define dTIMER_10S              dTIMER_PERIOD(10000000)
#define dTIMER_12S              dTIMER_PERIOD(12000000)
#define dTIMER_15S              dTIMER_PERIOD(15000000)
#define dTIMER_1MN              dTIMER_PERIOD(60000000)
#define dTIMER_10MN             dTIMER_PERIOD(600000000)
#define dTIMER_1H               dTIMER_PERIOD(3600000000)
    
#define dTIMER_PERIOD_SLEEP     dTIMER_1H
    
#define dTIMER_TIMER1_CC1_VAL   20090
*/
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/*! 
 * \struct LO_TIMER_DATA
 * \brief Data objet of timer
 */
/*
typedef void(*timer_riq_callback)(void *);

typedef struct
{
    bool init_done;
    bool modeOnOff[dTIMER_NUM];            //!<Timer working or not 
    uint32_t numPeriod[dTIMER_NUM];        //!<Period of the timers 
    uint32_t numCurrentPeriod[dTIMER_NUM]; //!<Current period of the timers 
    bool endTimer[dTIMER_NUM];             //!<timer flag completed 
    timer_riq_callback timer_callback[dTIMER_NUM];
	void *handle[dTIMER_NUM];
} LO_TIMER_DATA;
*/
// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
/*
void LO_TIMER_Initialize(void);
void LO_TIMER_Start(uint32_t uint32NumPeriod, uint8_t uint8Timer);
void LO_TIMER_reStart(uint8_t uint8Timer);
void LO_TIMER_Reset(uint8_t uint8Timer);
bool LO_TIMER_GetEnd(uint8_t uint8Timer);

void LO_TIMER_callback_set(
		uint8_t uint8Timer,
		timer_riq_callback timer_callback,
		void *handle
);
*/
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

#include "attiny85_hw_defines.h"

#define HW_TC_NUM			3
#define HW_TC0_PSCL_MASK	((1<<CS02) | (1<<CS01) | (1<<CS00))
#define HW_TC1_PSCL_MASK	((1<<CS12) | (1<<CS11) | (1<<CS10))

#define HW_TC_DISABLED		0
#define HW_TC_TIM_COUNT		1
#define HW_TC_CTC			2
#define HW_TC_F_PWM			3
#define HW_TC_N_PWM			4

#define HW_TC1_DISABLED		0
#define HW_TC1_TIM_COUNT	1
#define HW_TC1_CTC			2
#define HW_TC1_PWMA_B		3

#ifndef HW_TC0_MODE_SEL
#warning HW_TC0_MODE_SEL undefined
#define HW_TC0_MODE_SEL		HW_TC_DISABLED
#endif
#ifndef HW_TC1_MODE_SEL
#warning HW_TC1_MODE_SEL undefined
#define HW_TC1_MODE_SEL		HW_TC_DISABLED
#endif

#define HW_TC_NO_PRESCALE	0
#define HW_TC_PSCALE_8		1
#define HW_TC_PSCALE_64		2
#define HW_TC_PSCALE_256	3
#define HW_TC_PSCALE_1024	4

#define HW_TC1_NO_PRESCALE	0
#define HW_TC1_PSCALE_2		1
#define HW_TC1_PSCALE_4		2
#define HW_TC1_PSCALE_8		3
#define HW_TC1_PSCALE_16	4
#define HW_TC1_PSCALE_32	5
#define HW_TC1_PSCALE_64	6	
#define HW_TC1_PSCALE_128	7
#define HW_TC1_PSCALE_256	8
#define HW_TC1_PSCALE_512	9
#define HW_TC1_PSCALE_1024	10
#define HW_TC1_PSCALE_2048	11
#define HW_TC1_PSCALE_4096	12
#define HW_TC1_PSCALE_8192	13
#define HW_TC1_PSCALE_16384 14

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

typedef void(*hw_tc_cb)(void *handle);

typedef enum {
	LO_TC_PERIPH_0,
	LO_TC_PERIPH_1,
}hw_tc_timer_periph;

typedef enum {
	LO_TC_CHAN_COMPA,
	LO_TC_CHAN_COMPB,
	LO_TC_CHAN_TOV,
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

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _LO_TIMER_H */

/* *****************************************************************************
 End of File
 */
