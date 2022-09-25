/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   18/09/2022
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC16F628A_HW_GPIO_H_
#define	PIC16F628A_HW_GPIO_H_

#include <xc.h> // include processor files - each processor file is guarded.  

#include <stdint.h>
#include <stdbool.h>

#include "pic16f628a_hw_interrupt.h"

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#define HW_GPIO_INST_COUNT      16

typedef void(*hw_gpio_cb)(void *handle, bool state);

typedef enum {
    HW_GPIO_PORTA,
    HW_GPIO_PORTB,
}hw_gpio_port;

typedef enum {
    HW_GPIO_DIR_OUT,
    HW_GPIO_DIR_IN,
}hw_gpio_dir;

typedef enum {
    HW_GPIO_IT_NONE,
    HW_GPIO_IT_RB0_INT_RISING,
    HW_GPIO_IT_RB0_INT_FALLING,
    HW_GPIO_IT_RB4,
    HW_GPIO_IT_RB5,
    HW_GPIO_IT_RB6,
    HW_GPIO_IT_RB7,
}hw_gpio_it;

typedef struct hw_gpio_inst_{
    uint8_t pin_num;
    hw_gpio_port port;
    hw_gpio_dir dir;
    bool state;
    hw_gpio_cb cb;
    void *handle;
    hw_gpio_it it;
    
    struct hw_gpio_inst_ *next_inst;
}hw_gpio_inst;

typedef struct {
    uint8_t pin_num;
    hw_gpio_port port;
    hw_gpio_dir dir;
    bool state;
    hw_gpio_it it;
}hw_gpio_param;

void hw_gpio_init_inst(hw_gpio_inst *inst, hw_gpio_param *param);
void hw_gpio_deinit_inst(hw_gpio_inst *inst);
void hw_gpio_set_cb(hw_gpio_inst *inst, hw_gpio_cb cb, void *handle);

void hw_gpio_set_state(hw_gpio_inst *inst, bool state);
void hw_gpio_toggle_state(hw_gpio_inst *inst);
bool hw_gpio_get_state(hw_gpio_inst *inst);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

