/*
 * lo_flash_io.h
 *
 * Created: 8/6/2021 6:18:46 PM
 *  Author: Adam
 */ 


#ifndef LO_FLASH_IO_H_
#define LO_FLASH_IO_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MEMPROG
#define MEMPROG							__attribute__ ((section (".progmem")))
#endif
// ex : uint8_t data MEMPROG;
/*
typedef struct MEMPROG {
	
}my_type;
*/

#define LO_FLASH_FB_SIZE				3
#define LO_FLASH_LB_SIZE				1
#define LO_FLASH_SB_SIZE				42
#define LO_FLASH_PAGE_SIZE				32

#define LO_FLASH_PAGE_LENGTH(var_size)	(var_size / (LO_FLASH_PAGE_SIZE + 1)) + ((var_size % LO_FLASH_PAGE_SIZE != 0) ? 1 : 0)
#define LO_FLASH_VAR_LENGTH(var_size)	(var_size / (LO_FLASH_PAGE_SIZE + 1)* LO_FLASH_PAGE_SIZE) + ((var_size % LO_FLASH_PAGE_SIZE != 0) ? LO_FLASH_PAGE_SIZE : 0)

//fuses bits
#define LO_FLASH_FEB_SELFPRGEN			0

#define LO_FLASH_FEB_SELFPRGEN_ENABLED	(0<<LO_FLASH_FEB_SELFPRGEN)
#define LO_FLASH_FEB_SELFPRGEN_DISABLED	(1<<LO_FLASH_FEB_SELFPRGEN)

#define LO_FLASH_FHB_BODLEVEL0			0
#define LO_FLASH_FHB_BODLEVEL1			1
#define LO_FLASH_FHB_BODLEVEL2			2

#define LO_FLASH_FHB_BODLVL_DISABLED	((1<<LO_FLASH_FHB_BODLEVEL2) | (1<<LO_FLASH_FHB_BODLEVEL1) | (1<<LO_FLASH_FHB_BODLEVEL0))
#define LO_FLASH_FHB_BODLVL_1_8V		((1<<LO_FLASH_FHB_BODLEVEL2) | (1<<LO_FLASH_FHB_BODLEVEL1) | (0<<LO_FLASH_FHB_BODLEVEL0))
#define LO_FLASH_FHB_BODLVL_2_7V		((1<<LO_FLASH_FHB_BODLEVEL2) | (0<<LO_FLASH_FHB_BODLEVEL1) | (1<<LO_FLASH_FHB_BODLEVEL0))
#define LO_FLASH_FHB_BODLVL_4_3V		((1<<LO_FLASH_FHB_BODLEVEL2) | (0<<LO_FLASH_FHB_BODLEVEL1) | (0<<LO_FLASH_FHB_BODLEVEL0))

#define LO_FLASH_FHB_EESAVE				3

#define LO_FLASH_FHB_EESAVE_PRSVRD		(0<<LO_FLASH_FHB_EESAVE)
#define LO_FLASH_FHB_EESAVE_N_PRSVRD	(1<<LO_FLASH_FHB_EESAVE)

#define LO_FLASH_FHB_WDTON				4

#define LO_FLASH_FHB_SPIEN				5

#define LO_FLASH_FHB_SPIEN_ENABLED		(0<<LO_FLASH_FHB_SPIEN)
#define LO_FLASH_FHB_SPIEN_DISABLED		(1<<LO_FLASH_FHB_SPIEN)

#define LO_FLASH_FHB_DWEN				6

#define LO_FLASH_FHB_DWEN_ENABLED		(0<<LO_FLASH_FHB_DWEN)
#define LO_FLASH_FHB_DWEN_DISABLED		(1<<LO_FLASH_FHB_DWEN)

#define LO_FLASH_FHB_RSTDISBL			7

#define LO_FLASH_FHB_RSTDISBL_ENABLED	(0<<LO_FLASH_FHB_RSTDISBL)
#define LO_FLASH_FHB_RSTDISBL_DISABLED	(1<<LO_FLASH_FHB_RSTDISBL)

#define LO_FLASH_FLB_CKSEL0				0
#define LO_FLASH_FLB_CKSEL1				1
#define LO_FLASH_FLB_CKSEL2				2
#define LO_FLASH_FLB_CKSEL3				3

#define LO_FLASH_FLB_CKSEL_16MHZ		(0<<LO_FLASH_FLB_CKSEL3) | (0<<LO_FLASH_FLB_CKSEL2) | (0<<LO_FLASH_FLB_CKSEL1) | (1<<LO_FLASH_FLB_CKSEL0)
#define LO_FLASH_FLB_CKSEL_8MHZ			(0<<LO_FLASH_FLB_CKSEL3) | (0<<LO_FLASH_FLB_CKSEL2) | (1<<LO_FLASH_FLB_CKSEL1) | (0<<LO_FLASH_FLB_CKSEL0)
#define LO_FLASH_FLB_CKSEL_6_4MHZ		(0<<LO_FLASH_FLB_CKSEL3) | (0<<LO_FLASH_FLB_CKSEL2) | (1<<LO_FLASH_FLB_CKSEL1) | (1<<LO_FLASH_FLB_CKSEL0)
#define LO_FLASH_FLB_CKSEL_128KHZ		(0<<LO_FLASH_FLB_CKSEL3) | (1<<LO_FLASH_FLB_CKSEL2) | (0<<LO_FLASH_FLB_CKSEL1) | (0<<LO_FLASH_FLB_CKSEL0)
#define LO_FLASH_FLB_CKSEL_32_768KHZ	(0<<LO_FLASH_FLB_CKSEL3) | (1<<LO_FLASH_FLB_CKSEL2) | (1<<LO_FLASH_FLB_CKSEL1) | (0<<LO_FLASH_FLB_CKSEL0)

#define LO_FLASH_FLB_CKSEL_0_4_0_9MHZ	(1<<LO_FLASH_FLB_CKSEL3) | (0<<LO_FLASH_FLB_CKSEL2) | (0<<LO_FLASH_FLB_CKSEL1)
#define LO_FLASH_FLB_CKSEL_0_9_3_0MHZ	(1<<LO_FLASH_FLB_CKSEL3) | (0<<LO_FLASH_FLB_CKSEL2) | (1<<LO_FLASH_FLB_CKSEL1)
#define LO_FLASH_FLB_CKSEL_3_0_8_0MHZ	(1<<LO_FLASH_FLB_CKSEL3) | (1<<LO_FLASH_FLB_CKSEL2) | (0<<LO_FLASH_FLB_CKSEL1)
#define LO_FLASH_FLB_CKSEL_8_0_MHZ		(1<<LO_FLASH_FLB_CKSEL3) | (1<<LO_FLASH_FLB_CKSEL2) | (1<<LO_FLASH_FLB_CKSEL1)

#define LO_FLASH_FLB_SUT0				4
#define LO_FLASH_FLB_SUT1				5

#define LO_FLASH_FLB_CKOUT				6

#define LO_FLASH_FLB_CKOUT_ENABLED		(0<<LO_FLASH_FLB_CKOUT)
#define LO_FLASH_FLB_CKOUT_DISABLED		(1<<LO_FLASH_FLB_CKOUT)

#define LO_FLASH_FLB_CKDIV8				7

#define LO_FLASH_FLB_CKDIV8_ENABLED		(0<<LO_FLASH_FLB_CKDIV8)
#define LO_FLASH_FLB_CKDIV8_DISABLED	(1<<LO_FLASH_FLB_CKDIV8)

//lock bits
#define LO_FLASH_LB1					0
#define LO_FLASH_LB2					1

#define LO_FLASH_LB_NO_MEM_LOCK			((1<<LO_FLASH_LB2) | (1<<LO_FLASH_LB1))
#define LO_FLASH_LB_ICSP_HV_DISABLED	((1<<LO_FLASH_LB2) | (0<<LO_FLASH_LB1))
#define LO_FLAHS_LB_ICSP_HV_VER_DIABLED ((0<<LO_FLASH_LB2) | (0<<LO_FLASH_LB1))

typedef void(*lo_flash_data_pointer)(void);

typedef struct {
	uint8_t LB;
}lo_flash_io_lock_bits;

typedef struct {
	uint8_t FLB;
	uint8_t FHB;
	uint8_t FEB;
}lo_flash_io_fuses;

typedef struct {
	uint8_t SB[5];
	uint8_t SB_RESERVED[37];
}lo_flash_io_device_signature;

typedef struct {
	lo_flash_io_lock_bits lock_bits;
	lo_flash_io_fuses fuse_bits;
	lo_flash_io_device_signature device_signature;
}LO_FLASH_IO_DATA;

void lo_flash_read(lo_flash_data_pointer flash_addr, uint8_t *data, uint16_t length);
void lo_flash_write(lo_flash_data_pointer flash_addr, uint8_t *data, uint16_t length);
void lo_flash_erase_page(lo_flash_data_pointer flash_addr, uint16_t length);
void lo_flash_read_lock_bits(void);
void lo_flash_read_fuse_bits(void);
void lo_flash_read_device_signature(void);


#endif /* LO_FLASH_IO_H_ */