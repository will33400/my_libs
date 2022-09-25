// *****************************************************************************
/*! 
 * \file mid_timer.c
 * \brief basic timer program functions
 * \author Adam LUCAS
 * \date 19/12/2021
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "mid_timer.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static volatile MID_TIMER_DATA midTimerData = {
    .init_done = false
};


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

static void MID_TIMER_Manage(void *handle);

//*****************************************************************************/
/*!
* \fn void MID_TIMER_callback_default_callback(void *handler)
* \brief Timer interrupt default callback. MANDATORY do not modify this function
*   use MID_TIMER_callback_set instead.
* \param handler user defined handle data pointer
* \return None
*/

static void MID_TIMER_callback_default_callback(void *handler) {
	(void)handler;
}


//*****************************************************************************/
/*!
* \fn void MID_TIMER_Initialize(void)
* \brief Initialize all timers
* \param None
* \return None
*/

void MID_TIMER_Initialize(void)
{
		lo_timer_param param = {
			.ccmr1 = LO_TIMER_OC1PE_ENABLE,
			.cr1 = 
					(LO_TIMER_CKD_1 | 
					LO_TIMER_DIR_UP_COUNT | LO_TIMER_OPM_STOPPED),
			.irq = LO_TIMER_CC1IE,
			.timer = TIM1,
		};
		
    if(midTimerData.init_done) return;

    midTimerData.init_done = true;
    
    for (uint8_t uint8Loop = 0; uint8Loop < dTIMER_NUM; uint8Loop++)
    {
        MID_TIMER_Reset(uint8Loop);
        midTimerData.timer_callback[uint8Loop] = MID_TIMER_callback_default_callback;
    }
		
		lo_timer_init(LO_TIMER_INST_MID_TIMER, &param);
		lo_timer_set_ccr1(LO_TIMER_INST_MID_TIMER, dTIMER_VAL_CALIBRATION);
		lo_timer_set_cc1_cb(LO_TIMER_INST_MID_TIMER, MID_TIMER_Manage);
		
		lo_timer_enable(LO_TIMER_INST_MID_TIMER, true);
}

//*****************************************************************************/
/*!
* \fn void MID_TIMER_Reset(uint8_t uint8Timer)
* \brief Reset a timer
* \param[in] uint8Timer Timer instance number
* \return None
*/
void MID_TIMER_Reset(uint8_t uint8Timer)
{
    midTimerData.modeOnOff[uint8Timer] = false;
    midTimerData.numPeriod[uint8Timer] = 0;
    midTimerData.numCurrentPeriod[uint8Timer] = 0;
    midTimerData.endTimer[uint8Timer] = false;
}

//*****************************************************************************/
/*!
* \fn void MID_TIMER_Start(uint8_t uint8NumPeriod, uint8_t uint8Timer)
* \brief Starting a timer
* \param[in] uint32NumPeriod time period in ms
* \param[in] uint8Timer timer instance number
* \return None
*/
void MID_TIMER_Start(uint32_t uint32NumPeriod, uint32_t uint8Timer)
{
    midTimerData.modeOnOff[uint8Timer] = false;
    midTimerData.endTimer[uint8Timer] = false;
    midTimerData.numPeriod[uint8Timer] = uint32NumPeriod;
    midTimerData.numCurrentPeriod[uint8Timer] = 0;
    midTimerData.modeOnOff[uint8Timer] = true;

}

//*****************************************************************************/
/*!
* \fn void MID_TIMER_reStart(uint8_t uint8Timer)
* \brief Restarting a timer
* \param[in] uint8Timer timer instance number
* \return None
*/
void MID_TIMER_reStart(uint8_t uint8Timer)
{
    midTimerData.endTimer[uint8Timer] = false;
    midTimerData.numCurrentPeriod[uint8Timer] = 0;
    midTimerData.modeOnOff[uint8Timer] = true;
}

//*****************************************************************************/
/*!
* \fn bool MID_TIMER_GetEnd(uint8_t uint8Timer)
* \brief Get true if timer finished
* \param[in] uint8Timer timer instance number
* \return True if finished, false if not
*/
bool MID_TIMER_GetEnd(uint8_t uint8Timer)
{
    if ((midTimerData.endTimer[uint8Timer]) && 
                        (midTimerData.modeOnOff[uint8Timer]))
    {
        midTimerData.modeOnOff[uint8Timer] = false;
        return true;
    }
    else
        return false;
 }

//*****************************************************************************/
/*!
* \fn static void MID_TIMER_Manage(void *handle)
* \brief Manage all timers
* \param handle application defined handle data pointer (not used)
* \return None
*/

static void MID_TIMER_Manage(void *handle)
{
	(void)handle;

    for (uint8_t uint8Loop = 0; uint8Loop < dTIMER_NUM; uint8Loop++)
    {

        if (midTimerData.modeOnOff[uint8Loop])
        {
            midTimerData.numCurrentPeriod[uint8Loop]++;
            
            if (midTimerData.numCurrentPeriod[uint8Loop] >=
                midTimerData.numPeriod[uint8Loop])
            {
                midTimerData.endTimer[uint8Loop] = true;
				midTimerData.timer_callback[uint8Loop](midTimerData.handle[uint8Loop]);

            }
        }
		
    }
}


//*****************************************************************************/
/*!
* \fn void MID_TIMER_callback_set(
*		uint8_t uint8Timer,
*		timer_irq_callback timer_callback,
*		void *handle
*	};
* \brief User defined callback setting routine
* \param[in] uint8Timer timer instance number
* \param[in] timer_callback user defined callback
* \param[in] handle user defined handle
* \return None
*/

void MID_TIMER_callback_set(
		uint8_t uint8Timer,
		timer_irq_callback timer_callback,
		void *handle
) {
	midTimerData.timer_callback[uint8Timer] = timer_callback;
	midTimerData.handle[uint8Timer] = handle;
}


/* *****************************************************************************
 End of File
 */
