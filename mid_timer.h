// *****************************************************************************
/*! 
 * \file mid_timer.h
 * \brief Basic timer program functions header
 * \author Adam LUCAS
 * \date 22/05/2021
 */

#ifndef MID_TIMER_H_
#define MID_TIMER_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#include <stdint.h>
#include <stdbool.h>

#include "lo_timer.h"
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

#define dTIMER_TEST							0
#define dTIMER_MID_BUTTONS			1
#define dTIMER_LO_PWM						2
#define dTIMER_LED_BLINK				3
#define dTIMER_WATCHDOG					4
#define dTIMER_IR								5

#define dTIMER_NUM            	6  //Number of managed timer

#define dTIMER_VAL_CALIBRATION	(85) //?S

#define dTIMER_PERIOD(dtimer_delay) (dtimer_delay / 6)

#define dTIMER_6US							dTIMER_PERIOD(6)
#define dTIMER_50US							dTIMER_PERIOD(50)
#define dTIMER_100US            dTIMER_PERIOD(100)
#define dTIMER_150US            dTIMER_PERIOD(150)
#define dTIMER_200US            dTIMER_PERIOD(200)
#define dTIMER_400US            dTIMER_PERIOD(400)
#define dTIMER_500US						dTIMER_PERIOD(500)
#define dTIMER_1MS              dTIMER_PERIOD(1000)
#define dTIMER_2MS              dTIMER_PERIOD(2000)
#define dTIMER_5MS              dTIMER_PERIOD(5000)
#define dTIMER_10MS             dTIMER_PERIOD(10000)
#define dTIMER_50MS             dTIMER_PERIOD(50000)
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

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \fn typedef void(*timer_irq_callback)(void *handle);
 * \brief timer compare match user defined callback for interrupt purpose
 * \param[in] handle user defined callback data pointer
 */

typedef void(*timer_irq_callback)(void *handle);

/*! 
 * \struct LO_TIMER_DATA
 * \brief Data objet of timer
 */

typedef struct
{
    bool init_done;																	/*!<Timer library init done flag */
    bool modeOnOff[dTIMER_NUM];            					/*!<Timer working or not */
    uint32_t numPeriod[dTIMER_NUM];        					/*!<Period of the timers */
    uint32_t numCurrentPeriod[dTIMER_NUM]; 					/*!<Current period of the timers */
    bool endTimer[dTIMER_NUM];             					/*!<timer flag completed */
    timer_irq_callback timer_callback[dTIMER_NUM];	/*!<clock compare match interrupt callback*/
	void *handle[dTIMER_NUM];													/*!<callback user defined handle */
} MID_TIMER_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void MID_TIMER_Initialize(void);
void MID_TIMER_Start(uint32_t uint32NumPeriod, uint32_t uint8Timer);
void MID_TIMER_reStart(uint8_t uint8Timer);
void MID_TIMER_Reset(uint8_t uint8Timer);
bool MID_TIMER_GetEnd(uint8_t uint8Timer);

void MID_TIMER_callback_set(
		uint8_t uint8Timer,
		timer_irq_callback timer_callback,
		void *handle
);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _LO_TIMER_H */

/************ THIS SECTION IS FOR DOCUMENTATION ONLY !**********************//**
* \addtogroup LO_TIMER
* \brief Low level instanciable timer driver for ESP32 devices using ESP-IDF API
* \{

\details
\li \ref lo_timer_intro
\li \ref lo_timer_api_description
\li \ref lo_timer_data_types
\li \ref lo_timer_macros
\li \ref lo_timer_routines
\li \ref lo_timer_example_1
\li \ref lo_timer_example_2

# Introduction {#lo_timer_intro}

This document presents how to use lo_timer.h API to manage "real time" delays and callbacks through interrupts
lo_timer is a low level layer code that uses ESP32 timer code to manage timing

# API_Description {#lo_timer_api_description}

This library contains :

 -# Initialize()
 -# Start()
 -# reStart()
 -# Reset()
 -# GetEnd()
 -# callback_set()

these routines are described further in this document

# Data_Types {#lo_timer_data_types}

enums :

None

function pointer :

 -# timer_irq_callback user defined instance timer callback

structures :

 -# LO_TIMER_DATA library internal instance data structure. DO NOT USE

# Macros {#lo_timer_macros}

 -# dTIMER_PERIOD() insert time value in dTIMER_VAL_CALIBRATION unit

# Routines {#lo_timer_routines}

configuration routines :

 -# LO_TIMER_Initialize()
 -# LO_TIMER_Reset()
 -# LO_TIMER_callback_set()

set timer values routines :

 -# LO_TIMER_Start()
 -# LO_TIMER_reStart()

get timer end status routine :

 -# LO_TIMER_GetEnd()

# Example 1 {#lo_timer_example_1}

\code

#include "lo_timer.h"

void timer_cb(void *handle) {
	(void)handle;
	LO_TIMER_reStart(dTIMER_TURRET);

	//do anything
}

void main(void) {
	LO_TIMER_Initialize();
	LO_TIMER_Start(dTIMER_10MS, dTIMER_TURRET);
	LO_TIMER_callback_set(dTIMER_TURRET, timer_cb, NULL);

	while(1) {

	}
}

\endcode

# Example 2 {#lo_timer_example_2}

\code

#include "lo_timer.h"

void main(void) {
	LO_TIMER_Initialize();
	LO_TIMER_Start(dTIMER_10MS, dTIMER_TURRET);

	while(1) {
		if(LO_TIMER_GetEnd(dTIMER_TURRET)) {
			LO_TIMER_reStart(dTIMER_TURRET);

			//do anything

		}

	}
}

\endcode

* \} end group <lib_name> ****************************************************/


/* *****************************************************************************
 End of File
 */
