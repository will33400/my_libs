/*
 * File:   pic16f628a_hw_interrupt.c
 * Author: Adam
 *
 * Created on September 19, 2022, 9:40 PM
 */

#include "pic16f628a_hw_interrupt.h"

static void hw_it_default_cb(void) {
    
}

static hw_it_data hwItData = {
    .cb = {
        hw_it_default_cb, 
        hw_it_default_cb, 
        hw_it_default_cb, 
        hw_it_default_cb, 
        hw_it_default_cb, 
        hw_it_default_cb, 
        hw_it_default_cb, 
        hw_it_default_cb, 
        hw_it_default_cb, 
        hw_it_default_cb
    }
};
static bool init_done = false;

void hw_it_enable(bool enable) {
    if(enable)ei();
    else di();
}

void hw_it_set_cb(hw_it_vect vect, hw_it_cb cb) {
    hwItData.cb[vect] = cb;
}

void PIC16F628_HW_IT_T0I(void) {
    hwItData.cb[HW_IT_VECT_T0I]();
    INTCONbits.T0IF = 0;
}

void PIC16F628_HW_IT_INT(void) {
    hwItData.cb[HW_IT_VECT_INT]();
    INTCONbits.INTF = 0;
}

void PIC16F628_HW_IT_RBI(void) {
    hwItData.cb[HW_IT_VECT_RBI]();
    INTCONbits.RBIF = 0;
}

void PIC16F628_HW_IT_EEI(void) {
    hwItData.cb[HW_IT_VECT_EEI]();
    PIR1bits.EEIF = 0;
}

void PIC16F628_HW_IT_CMI(void) {
    hwItData.cb[HW_IT_VECT_CMI]();
}

void PIC16F628_HW_IT_RCI(void) {
    hwItData.cb[HW_IT_VECT_RCI]();
}

void PIC16F628_HW_IT_TXI(void) {
    hwItData.cb[HW_IT_VECT_TXI]();
}

void PIC16F628_HW_IT_CCP1I(void) {
    hwItData.cb[HW_IT_VECT_CCP1I]();
    PIR1bits.CCP1IF = 0;
}

void PIC16F628_HW_IT_TMR2I(void) {
    hwItData.cb[HW_IT_VECT_TMR2I]();
    PIR1bits.TMR2IF = 0;
}

void PIC16F628_HW_IT_TMR1I(void) {
    hwItData.cb[HW_IT_VECT_TMR1I]();
    PIR1bits.TMR1IF = 0;
}