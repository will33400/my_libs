/*!
 * \file obj_hmi.h
 * \brief object oriented human machine interface header
 * \author Adam LUCAS
 * \date 23/03/2022
 */

#ifndef OBJ_HMI_H_
#define OBJ_HMI_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum obj_hmi_state
 * \brief object oriented state machine steps
 */

typedef enum {
    OBJ_HMI_STATE_INIT,     //!< HMI init step. Recommanded to be used once
    OBJ_HMI_STATE_COMPUTE,  //!< HMI computing step. Required to ensure multiple processes
    OBJ_HMI_STATE_IDLE,     //!< HMI idle step. Waits for any event to ensure
    OBJ_HMI_STATE_PAUSE,    //!< HMI pause step. State machine is in sleep state, no callback required
}obj_hmi_state;

/*!
 * \fn typedef void(*obj_hmi_cb)(void *handle);
 * \brief <description>
 * \param[inout] handle user defined data pointer
 * \return None
 */

typedef void(*obj_hmi_cb)(void *handle);

/*!
 * \struct obj_hmi_ctx
 * \brief object oriented data structure
 */

typedef struct _obj_hmi_ctx {
    bool active;                //!< state machine is enabled (true) or not (false)
    obj_hmi_state state;        //!< current machine step
    obj_hmi_cb init;            //!< user defined routine for init step
    obj_hmi_cb compute;         //!< user defined routine for compute step
    obj_hmi_cb idle;            //!< user defined routine for idle step
    obj_hmi_cb pause;           //!< user defined routine for pause step
    void *handle;               //!< user define data pointer for all callbacks defined above
    struct _obj_hmi_ctx *next;  //!< queue next object oriented element pointer for multiple machine state applications
}obj_hmi_ctx;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void obj_hmi_init(obj_hmi_ctx *ctx);
void obj_hmi_set_active(obj_hmi_ctx *ctx_list, uint16_t index);
void obj_hmi_set_state(obj_hmi_ctx *ctx, obj_hmi_state state);
void obj_hmi_process(obj_hmi_ctx *ctx_list);

char *obj_hmi_get_state_str(obj_hmi_ctx *ctx);
bool obj_hmi_is_in_pause(obj_hmi_ctx *ctx);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif /* OBJ_HMI_H_ */

/************ THIS SECTION IS FOR DOCUMENTATION ONLY !**********************//**
* \addtogroup OBJ_HMI
* \brief Object oriented HMI structural code driver
* \{

\details
\li \ref obj_hmi_intro
\li \ref obj_hmi_api_description
\li \ref obj_hmi_data_types
\li \ref obj_hmi_macros
\li \ref obj_hmi_routines
\li \ref obj_hmi_example

# Introduction {#obj_hmi_intro}

This document presents how to use obj_hmi.h API to manage any interface method
obj_hmi is a library that uses user callbacks to structure interfacing processes

# API_Description {#obj_hmi_api_description}

This library contains :

 -# init()
 -# set_active()
 -# set_state()
 -# process()
 -# get_state_str()
 -# is_in_pause()

these routines are described further in this document

# Data_Types {#obj_hmi_data_types}

enums :

 -# obj_hmi_state HMI interfacing machine state steps

function pointer :

 -# obj_hmi_cb user defined callbacks for each steps

structures :

 -# obj_hmi_ctx HMI object data structure

# Macros {#obj_hmi_macros}

None

# Routines {#obj_hmi_routines}

configuration routines :

 -# obj_hmi_init()
 -# obj_hmi_set_active()
 -# obj_hmi_set_state()

main process routine :

 -# obj_hmi_process()

get status routines :

 -# obj_hmi_get_state_str()
 -# obj_hmi_is_in_pause()

# Example {#obj_hmi_example}

\code

#include "obj_hmi.h"

typedef enum {
    MAIN_HMI_WIN_MENU,
    MAIN_HMI_WIN_0,
    MAIN_HMI_WIN_1,
    MAIN_HMI_WIN_NUM
}main_hmi_win;

static void menu_init(void *handle) {
    //put code
    
    obj_hmi_set_state(
            &hmi[MAIN_HMI_WIN_MENU], 
            OBJ_HMI_STATE_IDLE
    );
}

static void menu_idle(void *handle) {
    
    if(0) { //put any event
        
        obj_hmi_set_state(
                &hmi[MAIN_HMI_WIN_0], 
                OBJ_HMI_STATE_INIT
        );
        obj_hmi_set_state(
                &hmi[MAIN_HMI_WIN_MENU], 
                OBJ_HMI_STATE_INIT
        );
        obj_hmi_set_active(
                hmi, 
                MAIN_HMI_WIN_0
        );
    }
}

static void win_0_init(void *handle) {
    //put code
    obj_hmi_set_state(
            &hmi[MAIN_HMI_WIN_0], 
            OBJ_HMI_STATE_IDLE
    );
}

static void win_0_idle(void *handle) {
    
    if(0) { //put any event
        obj_hmi_set_state(
                &hmi[MAIN_HMI_WIN_0], 
                OBJ_HMI_STATE_INIT
        );
        obj_hmi_set_state(
                &hmi[MAIN_HMI_WIN_MENU], 
                OBJ_HMI_STATE_INIT
        );
        obj_hmi_set_active(
                hmi, 
                MAIN_HMI_WIN_MENU
        );
    }
}

obj_hmi_ctx hmi[MAIN_HMI_WIN_NUM] = {
    {
        .state = OBJ_HMI_STATE_PAUSE,
        .init = menu_init,
        .idle = menu_idle,
    },
    {
        .state = OBJ_HMI_STATE_PAUSE,
        .init = win_0_init,
        .idle = win_0_idle,
    },
};

void main(void) {
    for(uint8_t i = 0; i < MAIN_HMI_WIN_NUM; i++) {
        obj_hmi_init(&hmi[i]);

        if(i + 1 < MAIN_HMI_WIN_NUM) 
                hmi[i].next = &hmi[i + 1];
        else hmi[i].next = NULL;
    }
    obj_hmi_set_state(
            &hmi[MAIN_HMI_WIN_MENU], 
            OBJ_HMI_STATE_INIT
    );
    obj_hmi_set_active(
            hmi, 
            MAIN_HMI_WIN_MENU
    );

    while(1) {
        obj_hmi_process(hmi);
    }
}

\endcode

* \} end group obj_hmi ****************************************************/
