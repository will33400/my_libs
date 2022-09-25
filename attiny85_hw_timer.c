#if 0
// *****************************************************************************
/*! 
 * \file lo_timer.c
 * \brief basic timer program functions
 * \author ATEK
 * \version 1.0
 * \date 2018 June
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "attiny85_hw_timer.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

LO_TIMER_DATA LoTimerData = {
    .init_done = false
};

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

static void LO_TIMER_callback_default_callback(void *handler) {
	(void)handler;
}


//*****************************************************************************/
/*!
* \fn void LO_TIMER_Initialize(void)
* \brief Initialize all timers
* \param None
* \return None
*/

void LO_TIMER_Initialize(void)
{

    if(LoTimerData.init_done) return;

    LoTimerData.init_done = true;

    uint8_t uint8Loop;
    
    for (uint8Loop = 0; uint8Loop < dTIMER_NUM; uint8Loop++)
    {
        LO_TIMER_Reset(uint8Loop);
        LoTimerData.timer_callback[uint8Loop] = LO_TIMER_callback_default_callback;
    }
	
	OCR0A = 160; //60us considering algorithm delay
	TCCR0A = (1<<WGM00); //CTC
	TCCR0B = (1<<CS00);	//no prescale
	
	TIMSK = (1<<OCIE0A); //enable compare A interrupt
	
	sei();
}

//*****************************************************************************/
/*!
* \fn void LO_TIMER_Reset(uint8_t uint8Timer)
* \brief Reset a timer
* \param[in] uint8Timer Timer instance number
* \return None
*/
void LO_TIMER_Reset(uint8_t uint8Timer)
{
    LoTimerData.modeOnOff[uint8Timer] = false;
    LoTimerData.numPeriod[uint8Timer] = 0;
    LoTimerData.numCurrentPeriod[uint8Timer] = 0;
    LoTimerData.endTimer[uint8Timer] = false;
}

//*****************************************************************************/
/*!
* \fn void LO_TIMER_Start(uint8_t uint8NumPeriod, uint8_t uint8Timer)
* \brief Starting a timer
* \param[in] uint32NumPeriod time period in ms
* \param[in] uint8Timer timer instance number
* \return None
*/
void LO_TIMER_Start(uint32_t uint32NumPeriod, uint8_t uint8Timer)
{
    LoTimerData.modeOnOff[uint8Timer] = false;
    LoTimerData.endTimer[uint8Timer] = false;
    LoTimerData.numPeriod[uint8Timer] = uint32NumPeriod;
    LoTimerData.numCurrentPeriod[uint8Timer] = 0;
    LoTimerData.modeOnOff[uint8Timer] = true;

}

//*****************************************************************************/
/*!
* \fn void LO_TIMER_reStart(uint8_t uint8Timer)
* \brief Restarting a timer
* \param[in] uint8Timer timer instance number
* \return None
*/
void LO_TIMER_reStart(uint8_t uint8Timer)
{
    LoTimerData.endTimer[uint8Timer] = false;
    LoTimerData.numCurrentPeriod[uint8Timer] = 0;
    LoTimerData.modeOnOff[uint8Timer] = true;
}

//*****************************************************************************/
/*!
* \fn bool LO_TIMER_GetEnd(uint8_t uint8Timer)
* \brief Get true if timer finished
* \param[in] uint8Timer timer instance number
* \return True if finished, false if not
*/
bool LO_TIMER_GetEnd(uint8_t uint8Timer)
{
    if ((LoTimerData.endTimer[uint8Timer]) && 
                        (LoTimerData.modeOnOff[uint8Timer]))
    {
        LoTimerData.modeOnOff[uint8Timer] = false;
        return true;
    }
    else
        return false;
 }

//*****************************************************************************/
/*!
* \fn static void LO_TIMER_Manage(void)
* \brief Manage all timers
* \param None
* \return None
*/
static void LO_TIMER_Manage(void)
{
    uint8_t uint8Loop;
    
    for (uint8Loop = 0; uint8Loop < dTIMER_NUM; uint8Loop++)
    {
        if (LoTimerData.modeOnOff[uint8Loop])
        {
            LoTimerData.numCurrentPeriod[uint8Loop]++;
            
            if (LoTimerData.numCurrentPeriod[uint8Loop] >=
                LoTimerData.numPeriod[uint8Loop])
            {
                LoTimerData.endTimer[uint8Loop] = true;
				LoTimerData.timer_callback[uint8Loop](LoTimerData.handle[uint8Loop]);
            }
        }
		
    }
}

void LO_TIMER_callback_set(
		uint8_t uint8Timer,
		timer_riq_callback timer_callback,
		void *handle
) {
	LoTimerData.timer_callback[uint8Timer] = timer_callback;
	LoTimerData.handle[uint8Timer] = handle;
}

ISR(TIM0_COMPA_vect) {
	LO_TIMER_Manage();
	TIFR |= (1<<OCF0A);
}

/* *****************************************************************************
 End of File
 */
#endif

/*
 * hw_tc.c
 *
 * Created: 11/13/2021 9:42:15 AM
 *  Author: Adam
 */ 

#include "attiny85_hw_timer.h"

static hw_tc_data *tc0;
static hw_tc_data *tc1;

static void hw_tc_default_cb(void *handle) {
	(void)handle;
}


static void hw_tc0_init(void) {
	TCCR0A = 0;
	TCCR0B = 0;
	
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
	DDRB &= ~(1<<DDB0);
	PORTB &= ~(1<<PB0);
#endif

#if HW_TC0_MODE_SEL != HW_TC_DISABLED
	TIMSK |=/* (1<<OCIE0B) | */(1<<OCIE0A)/* | (1<<TOIE0)*/;
	sei();
#endif
	
}

static void hw_tc1_init(void) {
	TCCR1 = 0;
	GTCCR = 0;
	
	tc1->prescale_val = 0;
	tc1->enabled = false;
	tc1->compa_cb = hw_tc_default_cb;
	tc1->compa_handle = (void*)0;
	tc1->compb_cb = hw_tc_default_cb;
	tc1->compb_handle = (void*)0;
	tc1->tov_cb = hw_tc_default_cb;
	tc1->tov_handle = (void*)0;
	
#if HW_TC1_MODE_SEL != HW_TC_DISABLED
	PRR &= ~(1<<PRTIM1);
#endif
	
#if HW_TC1_MODE_SEL == HW_TC1_TIM_COUNT
#elif HW_TC1_MODE_SEL == HW_TC1_CTC
	TCCR1 |= (1<<CTC1);
#elif HW_TC1_MODE_SEL == HW_TC1_PWMA_B
	TCCR1 |= (1<<PWM1A);
	GTCCR |= (1<<PWM1B);
#endif

#if HW_TC1_PSCALE_SEL == HW_TC_NO_PRESCALE
	tc1->prescale_val = (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_2
	tc1->prescale_val = (1<<CS11);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_4
	tc1->prescale_val = (1<<CS11) | (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_8
	tc1->prescale_val = (1<<CS12);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_16
	tc1->prescale_val = (1<<CS12) | (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_32
	tc1->prescale_val = (1<<CS12) | (1<<CS11)
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_64
	tc1->prescale_val = (1<<CS12) | (1<<CS11) | (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_128
	tc1->prescale_val = (1<<CS13);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_256
	tc1->prescale_val = (1<<CS13) | (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_512
	tc1->prescale_val = (1<<CS13) | (1<<CS11);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_1024
	tc1->prescale_val = (1<<CS13) | (1<<CS11) | (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_2048
	tc1->prescale_val = (1<<CS13) | (1<<CS12);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_4096
	tc1->prescale_val = (1<<CS13) | (1<<CS12) | (1<<CS10);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_8192
	tc1->prescale_val = (1<<CS13) | (1<<CS12) | (1<<CS11);
#elif HW_TC1_PSCALE_SEL == HW_TC1_PSCALE_16384
	tc1->prescale_val = (1<<CS13) | (1<<CS12) | (1<<CS11) | (1<<CS10);
#endif
	
#if HW_TC1_MODE_SEL != HW_TC1_DISABLED
	TIMSK |=/* (1<<TOIE1) | */(1<<OCIE1A) | (1<<OCIE1B) /*| (1<<ICIE1)*/;
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
		TCCR1 &= ~HW_TC1_PSCL_MASK;
		TCCR1 |= tc1->prescale_val;
	}
	else {
		tc1->enabled = false;
		TCCR1 &= ~HW_TC1_PSCL_MASK;
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
		}
		break;
	}
	return 0;
}

ISR(TIMER0_COMPA_vect) {
	tc0->compa_cb(tc0->compa_handle);
	TIFR |= (1<<OCF0A);
}

ISR(TIMER0_COMPB_vect) {
	tc0->compb_cb(tc0->compb_handle);
	TIFR |= (1<<OCF0B);
}

ISR(TIMER0_OVF_vect) {
	tc0->tov_cb(tc0->tov_handle);
	TIFR |= (1<<TOV0);
}

ISR(TIMER1_COMPA_vect) {
	tc1->compa_cb(tc1->compa_handle);
	TIFR |= (1<<OCF1A);
}

ISR(TIMER1_COMPB_vect) {
	tc1->compb_cb(tc1->compb_handle);
	TIFR |= (1<<OCF1B);
}

ISR(TIMER1_OVF_vect) {
	tc1->tov_cb(tc1->tov_handle);
	TIFR |= (1<<TOV1);
}
