#include <xc.inc>

; When assembly code is placed in a psect, it can be manipulated as a
; whole by the linker and placed in memory.  
;
; In this example, barfunc is the program section (psect) name, 'local' means
; that the section will not be combined with other sections even if they have
; the same name.  class=CODE means the barfunc must go in the CODE container.
; PIC18's should have a delta (addressible unit size) of 1 (default) since they
; are byte addressible.  PIC10/12/16's have a delta of 2 since they are word
; addressible.  PIC18's should have a reloc (alignment) flag of 2 for any
; psect which contains executable code.  PIC10/12/16's can use the default
; reloc value of 1.  Use one of the psects below for the device you use:

psect   text10,local,class=CODE,delta=2 ; PIC10/12/16
; psect   barfunc,local,class=CODE,reloc=2 ; PIC18

org		0004H
    
global		_PIC16F628_HW_IT_T0I
global		_PIC16F628_HW_IT_INT
global		_PIC16F628_HW_IT_RBI
global		_PIC16F628_HW_IT_EEI
global		_PIC16F628_HW_IT_CMI
global		_PIC16F628_HW_IT_RCI
global		_PIC16F628_HW_IT_TXI
global		_PIC16F628_HW_IT_CCP1I
global		_PIC16F628_HW_IT_TMR2I
global		_PIC16F628_HW_IT_TMR1I
    

    
PIC16F628A_HW_IT_VECT:
    
    btfsc	INTCON, 2
    call	_PIC16F628_HW_IT_T0I
    btfsc	INTCON, 1
    call	_PIC16F628_HW_IT_INT
    btfsc	INTCON, 0
    call	_PIC16F628_HW_IT_RBI
    btfsc	PIR1, 7
    call	_PIC16F628_HW_IT_EEI
    btfsc	PIR1, 6
    call	_PIC16F628_HW_IT_CMI
    btfsc	PIR1, 5
    call	_PIC16F628_HW_IT_RCI
    btfsc	PIR1, 4
    call	_PIC16F628_HW_IT_TXI
    btfsc	PIR1, 2
    call	_PIC16F628_HW_IT_CCP1I
    btfsc	PIR1, 1
    call	_PIC16F628_HW_IT_TMR2I
    btfsc	PIR1, 0
    call	_PIC16F628_HW_IT_TMR1I
    retfie
    
