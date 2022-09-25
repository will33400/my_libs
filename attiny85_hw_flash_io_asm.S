
/*
 * lo_flash_io_asm.S
 *
 * Created: 8/6/2021 6:22:20 PM
 *  Author: Adam
 */ 

#include <avr/io.h>


.section .text


#define lo_flash_temp_1					r16
#define lo_flash_temp_2					r17

#define lo_flash_temp_3					r28
#define lo_flash_temp_3_l				r28
#define lo_flash_temp_3_h				r29

#define lo_flash_temp_4					r14
#define lo_flash_temp_4_l				r14
#define lo_flash_temp_4_h				r15

#define lo_flash_spm_accumulator		r0
#define lo_flash_spm_accumulator_l		r0
#define lo_flash_spm_accumulator_h		r1


#define lo_flash_ram_vector				X
#define lo_flash_ram_vector_l			XL
#define lo_flash_ram_vector_h			XH

#define lo_flash_flash_vector			Z
#define lo_flash_flash_vector_l			ZL
#define lo_flash_flash_vector_h			ZH

#define LO_FLASH_PCPAGE_MASK_H			0x0F
#define LO_FLASH_PCPAGE_MASK_L			0xE0
#define LO_FLASH_PCBLOCK_MASK_L			0x1F

//--------------------------------------------------------------------------------------------

.global		lo_flash_read

#define lo_flash_read_flash_addr		r24
#define lo_flash_read_flash_addr_l		r24
#define lo_flash_read_flash_addr_h		r25

#define lo_flash_read_data_ptr			r22
#define lo_flash_read_data_ptr_l		r22
#define lo_flash_read_data_ptr_h		r23

#define lo_flash_read_data_length		r20
#define lo_flash_read_data_length_l		r20
#define lo_flash_read_data_length_h		r21

lo_flash_read:

	cli
	
	push	lo_flash_temp_1
	push	lo_flash_temp_2
	
	push	lo_flash_temp_3_l
	push	lo_flash_temp_3_h

	push	lo_flash_flash_vector_l
	push	lo_flash_flash_vector_h

	push	lo_flash_ram_vector_l
	push	lo_flash_ram_vector_h
	
	movw	lo_flash_temp_3, lo_flash_read_data_length

	movw	lo_flash_flash_vector, lo_flash_read_flash_addr

	movw	lo_flash_ram_vector, lo_flash_read_data_ptr

lo_flash_read_loop_1:
	lpm		lo_flash_temp_2, lo_flash_flash_vector+
	st		lo_flash_ram_vector+, lo_flash_temp_2
	sbiw	lo_flash_temp_3, 1
	brne	lo_flash_read_loop_1

	pop		lo_flash_ram_vector_h
	pop		lo_flash_ram_vector_l

	pop		lo_flash_flash_vector_h
	pop		lo_flash_flash_vector_l

	pop		lo_flash_temp_3_h
	pop		lo_flash_temp_3_l

	pop		lo_flash_temp_2
	pop		lo_flash_temp_1

	sei
ret

//--------------------------------------------------------------------------------------------

.global lo_flash_write

#define lo_flash_write_flash_addr		r24
#define lo_flash_write_flash_addr_l		r24
#define lo_flash_write_flash_addr_h		r25

#define lo_flash_write_data_ptr			r22
#define lo_flash_write_data_ptr_l		r22
#define lo_flash_write_data_ptr_h		r23

#define lo_flash_write_data_length		r20
#define lo_flash_write_data_length_l	r20
#define lo_flash_write_data_length_h	r21

lo_flash_write:

	movw	lo_flash_temp_3, lo_flash_write_data_length
	movw	lo_flash_flash_vector, lo_flash_write_flash_addr
	movw	lo_flash_ram_vector, lo_flash_write_data_ptr

	mov		lo_flash_temp_2, lo_flash_write_flash_addr_l

	ldi		lo_flash_temp_1, 4									;convert Z vector for page addressing begin

lo_flash_write_loop_2:
	lsr		lo_flash_flash_vector_l
	sbrc	lo_flash_flash_vector_h, 0
	ori		lo_flash_flash_vector_l, 0x80
	lsr		lo_flash_flash_vector_h
	dec		lo_flash_temp_1
	brne	lo_flash_write_loop_2

	andi	lo_flash_flash_vector_h, LO_FLASH_PCPAGE_MASK_H
	andi	lo_flash_flash_vector_l, LO_FLASH_PCPAGE_MASK_L
	andi	lo_flash_temp_2, LO_FLASH_PCBLOCK_MASK_L

	or		lo_flash_flash_vector_l, lo_flash_temp_2
	
	//ldi		lo_flash_flash_vector_l, (3<<5)
	//ldi		lo_flash_flash_vector_h, 1
				;convert Z vector for page addressing end

lo_flash_write_loop_1:
	ld		lo_flash_spm_accumulator_l, lo_flash_ram_vector+	;get into r0 byte data from RAM

	sbiw	lo_flash_temp_3, 1									;decrement length
	breq	lo_flash_write_loop_continue_1						;branch if length == 0

	ld		lo_flash_spm_accumulator_h, lo_flash_ram_vector+	;get into r1 byte data from RAM

	rjmp	lo_flash_write_loop_continue_2						;jump to lo_flash_write_loop_continue_2

lo_flash_write_loop_continue_1:
	clr		lo_flash_spm_accumulator_h							;clear r1 if length reached 0

	ldi		lo_flash_temp_1, (1<<PGWRT) | (1<<SPMEN)			;init register for spm command
	out		_SFR_IO_ADDR(SPMCSR), lo_flash_temp_1

	spm															;generate flash write considering r0, r1
ret
lo_flash_write_loop_continue_2:

	ldi		lo_flash_temp_1, (1<<PGWRT) | (1<<SPMEN)			;init register for spm command
	out		_SFR_IO_ADDR(SPMCSR), lo_flash_temp_1

	spm															;generate flash write considering r0, r1
	
	sbiw	lo_flash_temp_3, 1									;decrement length

	inc		lo_flash_flash_vector_l								;increment block address
	mov		lo_flash_temp_1, lo_flash_flash_vector_l
	andi	lo_flash_temp_1, LO_FLASH_PCBLOCK_MASK_L
	cpi		lo_flash_temp_1, LO_FLASH_PCBLOCK_MASK_L
	brne	lo_flash_write_loop_continue_3						;branch if block address full

	adiw	lo_flash_flash_vector, 0x20							;increment page address
	andi	lo_flash_flash_vector_h, 0x0F						;mask to prevent page address overflow

lo_flash_write_loop_continue_3:
	cpi		lo_flash_temp_3_h, 0
	brne	lo_flash_write_loop_1								;branch if length_h != 0
	cpi		lo_flash_temp_3_l, 0
	brne	lo_flash_write_loop_1								;branch if length_l != 0

ret

//--------------------------------------------------------------------------------------------

.global lo_flash_erase_page

lo_flash_erase_page:
	


ret

//--------------------------------------------------------------------------------------------

.global lo_flash_read_lock_bits

.extern loFlashIoData

lo_flash_read_lock_bits:
	
	movw	lo_flash_temp_3, lo_flash_read_data_length

	//ldi		lo_flash_flash_vector_l, lo_flash_lock_bits
	//ldi		lo_flash_flash_vector_h, (lo_flash_lock_bits >> 8)

	ldi		lo_flash_temp_1, (1<<RFLB)|(1<<SPMEN)
	out		_SFR_IO_ADDR(SPMCSR), lo_flash_temp_1

	lpm		lo_flash_temp_2, lo_flash_flash_vector+
	st		lo_flash_ram_vector+, lo_flash_temp_2

ret

//--------------------------------------------------------------------------------------------

.global lo_flash_read_fuse_bits

lo_flash_read_fuse_bits:

ret

//--------------------------------------------------------------------------------------------

.global lo_flash_read_device_signature

lo_flash_read_device_signature:

ret