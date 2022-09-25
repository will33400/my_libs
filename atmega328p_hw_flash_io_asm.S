/*!
 * \file flash_io_asm.S
 * \brief flash functions
 * \author Adam LUCAS
 * \date 4/3/2021 9:55:12 AM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <avr/io.h>

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

.DATA


.TEXT

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn <function declaration>
* \brief <description>
* \param[in] <argument name> <description>
* \param[out] <argument name> <description>
* \return [<anything>] [None]
*/

.GLOBAL	flash_write
;size = R21:R20
;data = R23:R22
;flash_addr = R25:R24
/*
flash_write:
	;SREG stored into STACK and disable interrupts
	LDS		R16, SREG
	PUSH	R16
	CLI

	;init registers
	MOVW	ZL, R24
	MOVW	XL, R22

	MOVW	R24, R20

	;set R1 to 0xFF if size = 1
	CPI		R20, 1
	BRNE	__flash_write_loop_1
	CPI		R21, 0
	BRNE	__flash_write_loop_1
	LDI		R16, 0xFF
	MOV		R1, R16

	;write flash memory considering size and data
	__flash_write_loop_1:
		
		;get data from ram to R1:R0
		LD		R1, X+
		LD		R0, X+

		

		;enable SPM
		LDS		R16, SPMCSR
		ORI		R16, (1<<SPMEN)
		STS		SPMCSR, R16

		;page write enable temp
		LDI		R17, (1<<PGWRT) | (1<<SPMEN)

		;wait for SPMEN to be set
		;LDS		R16, SPMCSR
		;SBRC	R16, SPMEN
		;RJMP	PC - 2

		;page write enable
		STS		SPMCSR, R17

		SPM		Z+

		SBIW	R24, 1
		CPI		R25, 0
		BRNE	__flash_write_loop_1
		CPI		R24, 0
		BRNE	__flash_write_loop_1

	;restore SREG
	POP		R16
	STS		SREG, R16

RET
*/


.GLOBAL flash_read
;size = R21:R20
;data = R23:R22
;flash_addr = R25:R24

flash_read:
	
	MOVW	ZL, R24
	MOVW	XL, R22

	MOVW	R24, R20

	__flash_read_loop_1:

	;LPM		Z+

	ST		X+, R1
	ST		X+, R0

	SBIW	R24, 1
	CPI		R25, 0
	BRNE	__flash_read_loop_1
	CPI		R24, 0
	BRNE	__flash_read_loop_1

RET




/*
flash_write:
	
	MOVW	ZL, R24
	MOVW	R24, R20
	MOVW	YL, R22
	;page erase
	ldi		R18, (1<<PGERS) | (1<<SPMEN)
	call	do_spm

	wrloop:

		ld		r0, Y+
		ld		r1, Y+

		ldi		R18, (1<<SPMEN)
		call	do_spm

		adiw	ZL, 2
		sbiw	R24, 2 ;use subi for PAGESIZEB<=256
		brne	wrloop
		
		ldi		R18, (1<<PGWRT) | (1<<SPMEN)
		call	do_spm

ret


do_spm:

	;input: spmcrval determines SPM action
	;disable interrupts if enabled, store status
	lds		R17, SREG
	cli
	;check for previous SPM complete
	wait: 

		lds R16, SPMCSR
		sbrc R16, SPMEN
		rjmp wait
		;SPM timed sequence
		sts SPMCSR, R18
		spm
		;restore SREG (to enable interrupts if originally enabled)
		sts SREG, R17
ret
*/