/*
 * obj_ssd1306.c
 *
 * Created: 8/1/2021 10:04:35 PM
 *  Author: Adam
 */ 

/*!
 * \file obj_ssd1306.c
 * \brief object oriented ssd1306 device functions
 * \author Adam LUCAS
 * \date 8/1/2021 10:04:35 PM
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_ssd1306.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

uint8_t obj_ssd1306_disp_font[][OBJ_SSD1306_CHARACTER_LENGTH] 
#if OBJ_SSD1306_DATA_INIT_SEL == OBJ_SSD1306_DATA_FLASH_AVR_INIT
__attribute__ ((section (".progmem")))
#endif
= {
	{0x00, 0x00, 0x00, 0x00, 0x00}, // 32 space
	{0x00, 0x00, 0x5F, 0x00, 0x00}, // 33 !
	{0x00, 0x07, 0x00, 0x07, 0x00}, // 34 "
	{0x14, 0x7f, 0x14, 0x7f, 0x14}, // 35 #
	{0x24, 0x2a, 0x7f, 0x2a, 0x12}, // 36 $
	{0x23, 0x13, 0x08, 0x64, 0x62}, // 37 %
	{0x36, 0x49, 0x55, 0x22, 0x50}, // 38 &
	{0x00, 0x05, 0x03, 0x00, 0x00}, // 39 '
	{0x00, 0x1c, 0x22, 0x41, 0x00}, // 40 (
	{0x00, 0x41, 0x22, 0x1c, 0x00}, // 41 )
	{0x14, 0x08, 0x3e, 0x08, 0x14}, // 42 *
	{0x08, 0x08, 0x3e, 0x08, 0x08}, // 43 +
	{0x00, 0x50, 0x30, 0x00, 0x00}, // 44 ,
	{0x08, 0x08, 0x08, 0x08, 0x08}, // 45 -
	{0x00, 0x60, 0x60, 0x00, 0x00}, // 46 .
	{0x20, 0x10, 0x08, 0x04, 0x02}, // 47 /
	{0x3e, 0x51, 0x49, 0x45, 0x3e}, // 48 0
	{0x00, 0x42, 0x7f, 0x40, 0x00}, // 49 1
	{0x42, 0x61, 0x51, 0x49, 0x46}, // 50 2
	{0x21, 0x41, 0x45, 0x4b, 0x31}, // 51 3
	{0x18, 0x14, 0x12, 0x7f, 0x10}, // 52 4
	{0x27, 0x45, 0x45, 0x45, 0x39}, // 53 5
	{0x3c, 0x4a, 0x49, 0x49, 0x30}, // 54 6
	{0x01, 0x71, 0x09, 0x05, 0x03}, // 55 7
	{0x36, 0x49, 0x49, 0x49, 0x36}, // 56 8
	{0x06, 0x49, 0x49, 0x29, 0x1e}, // 57 9
	{0x00, 0x36, 0x36, 0x00, 0x00}, // 58 :
	{0x00, 0x56, 0x36, 0x00, 0x00}, // 59 ;
	{0x08, 0x14, 0x22, 0x41, 0x00}, // 60 <
	{0x14, 0x14, 0x14, 0x14, 0x14}, // 61 =
	{0x00, 0x41, 0x22, 0x14, 0x08}, // 62 >
	{0x02, 0x01, 0x51, 0x09, 0x06}, // 63 ?
	{0x32, 0x49, 0x79, 0x41, 0x3e}, // 64 @
	{0x7e, 0x11, 0x11, 0x11, 0x7e}, // 65 A
	{0x7f, 0x49, 0x49, 0x49, 0x36}, // 66 B
	{0x3e, 0x41, 0x41, 0x41, 0x22}, // 67 C
	{0x7f, 0x41, 0x41, 0x22, 0x1c}, // 68 D
	{0x7f, 0x49, 0x49, 0x49, 0x41}, // 69 E
	{0x7f, 0x09, 0x09, 0x09, 0x01}, // 70 F
	{0x3e, 0x41, 0x49, 0x49, 0x7a}, // 71 G
	{0x7f, 0x08, 0x08, 0x08, 0x7f}, // 72 H
	{0x00, 0x41, 0x7f, 0x41, 0x00}, // 73 I
	{0x20, 0x40, 0x41, 0x3f, 0x01}, // 74 J
	{0x7f, 0x08, 0x14, 0x22, 0x41}, // 75 K
	{0x7f, 0x40, 0x40, 0x40, 0x40}, // 76 L
	{0x7f, 0x02, 0x0c, 0x02, 0x7f}, // 77 M
	{0x7f, 0x04, 0x08, 0x10, 0x7f}, // 78 N
	{0x3e, 0x41, 0x41, 0x41, 0x3e}, // 79 O
	{0x7f, 0x09, 0x09, 0x09, 0x06}, // 80 P
	{0x3e, 0x41, 0x51, 0x21, 0x5e}, // 81 Q
	{0x7f, 0x09, 0x19, 0x29, 0x46}, // 82 R
	{0x46, 0x49, 0x49, 0x49, 0x31}, // 83 S
	{0x01, 0x01, 0x7f, 0x01, 0x01}, // 84 T
	{0x3f, 0x40, 0x40, 0x40, 0x3f}, // 85 U
	{0x1f, 0x20, 0x40, 0x20, 0x1f}, // 86 V
	{0x3f, 0x40, 0x38, 0x40, 0x3f}, // 87 W
	{0x63, 0x14, 0x08, 0x14, 0x63}, // 88 X
	{0x07, 0x08, 0x70, 0x08, 0x07}, // 89 Y
	{0x61, 0x51, 0x49, 0x45, 0x43}, // 90 Z
#ifndef ASCCI_TABLE_HALF_DEFINED
	{0x00, 0x7f, 0x41, 0x41, 0x00}, // 91 [
	{0x02, 0x04, 0x08, 0x10, 0x20}, // 92 Backslash
	{0x00, 0x41, 0x41, 0x7f, 0x00}, // 93 ]
	{0x04, 0x02, 0x01, 0x02, 0x04}, // 94 ^
	{0x40, 0x40, 0x40, 0x40, 0x40}, // 95 _
	{0x00, 0x01, 0x02, 0x04, 0x00}, // 96 `
	{0x20, 0x54, 0x54, 0x54, 0x78}, // 97 a
	{0x7f, 0x48, 0x44, 0x44, 0x38}, // 98 b
	{0x38, 0x44, 0x44, 0x44, 0x20}, // 99 c
	{0x38, 0x44, 0x44, 0x48, 0x7f}, // 100 d
	{0x38, 0x54, 0x54, 0x54, 0x18}, // 101 e
	{0x08, 0x7e, 0x09, 0x01, 0x02}, // 102 f
	{0x0c, 0x52, 0x52, 0x52, 0x3e}, // 103 g
	{0x7f, 0x08, 0x04, 0x04, 0x78}, // 104 h
	{0x00, 0x44, 0x7d, 0x40, 0x00}, // 105 i
	{0x20, 0x40, 0x44, 0x3d, 0x00}, // 106 j
	{0x7f, 0x10, 0x28, 0x44, 0x00}, // 107 k
	{0x00, 0x41, 0x7f, 0x40, 0x00}, // 108 l
	{0x7c, 0x04, 0x18, 0x04, 0x78}, // 109 m
	{0x7c, 0x08, 0x04, 0x04, 0x78}, // 110 n
	{0x38, 0x44, 0x44, 0x44, 0x38}, // 111 o
	{0x7c, 0x14, 0x14, 0x14, 0x08}, // 112 p
	{0x08, 0x14, 0x14, 0x18, 0x7c}, // 113 q
	{0x7c, 0x08, 0x04, 0x04, 0x08}, // 114 r
	{0x48, 0x54, 0x54, 0x54, 0x20}, // 115 s
	{0x04, 0x3f, 0x44, 0x40, 0x20}, // 116 t
	{0x3c, 0x40, 0x40, 0x20, 0x7c}, // 117 u
	{0x1c, 0x20, 0x40, 0x20, 0x1c}, // 118 v
	{0x3c, 0x40, 0x30, 0x40, 0x3c}, // 119 w
	{0x44, 0x28, 0x10, 0x28, 0x44}, // 120 x
	{0x0c, 0x50, 0x50, 0x50, 0x3c}, // 121 y
	{0x44, 0x64, 0x54, 0x4c, 0x44}, // 122 z
	{0x00, 0x08, 0x36, 0x41, 0x00}, // 123 {
	{0x00, 0x00, 0x7f, 0x00, 0x00}, // 124 |
	{0x00, 0x41, 0x36, 0x08, 0x00}, // 125 }
	{0x10, 0x08, 0x08, 0x10, 0x08}, // 126 ~
	{0x78, 0x46, 0x41, 0x46, 0x78}  // 127 square*/
#endif
};

uint8_t obj_ssd1306_buffer[][OBJ_SSD1306_CHARACTER_LENGTH]
#if OBJ_SSD1306_DATA_INIT_SEL == OBJ_SSD1306_DATA_FLASH_AVR_INIT
__attribute__ ((section (".progmem")))
#endif
= {
	{OBJ_SSD1306_SET_DISP_OFF_REG},
	{OBJ_SSD1306_SET_MEM_ADDR_MODE_REG},
	{OBJ_SSD1306_SET_LOWER_CLMN_ADDR_PAGE_MODE_MIN_REG},
	{OBJ_SSD1306_SET_PAGE_START_MIN_REG},
	{OBJ_SSD1306_SET_COM_OUT_SCAN_DIR_REMAP_REG},
	{OBJ_SSD1306_SET_LOWER_CLMN_ADDR_PAGE_MODE_MIN_REG},
	{OBJ_SSD1306_SET_HIGHER_CLMN_ADDR_PAGE_MODE_MIN_REG},
	{OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_SCRL_MAX_REG_B},
	{OBJ_SSD1306_SET_CONTR_REG, OBJ_SSD1306_SET_CONTR_VAL_MAX_REG_A},
	{OBJ_SSD1306_SET_SEGMENT_REMAP_ADDR_127_REG},
	{OBJ_SSD1306_SET_DISP_NORMAL_REG},
	{OBJ_SSD1306_SET_MUX_RATIO_REG, OBJ_SSD1306_SET_MUX_RATIO_MAX_REG_A},
	{OBJ_SSD1306_ENT_DISP_ON_REG},
	{OBJ_SSD1306_SET_DISPLAY_OFFSET_REG, OBJ_SSD1306_SET_DISPLAY_OFFSET_MIN_REG_A},
	{OBJ_SSD1306_SET_DISP_PRSCL_REG, 0x80},
	{OBJ_SSD1306_SET_PRE_CHRG_PRD_REG, 0x22},
	{OBJ_SSD1306_SET_COM_PINS_REG, OBJ_SSD1306_SET_COM_PINS_ALTERN_COM_DIS_COM_REG_A},
	{OBJ_SSD1306_SET_VCOM_DSLCT_LVL_REG, OBJ_SSD1306_SET_VCOM_DSLCT_LVL_0_77_VCC_REG_A},
	{OBJ_SSD1306_CHARGE_PUMP_SETTING_REG, OBJ_SSD1306_CHARGE_PUMP_SETTING_ENABLE_REG_A},
	{OBJ_SSD1306_SET_DISP_ON_REG}
};

uint8_t obj_ssd1306_buffer_sizes[] 
#if OBJ_SSD1306_DATA_INIT_SEL == OBJ_SSD1306_DATA_FLASH_AVR_INIT
__attribute__ ((section (".progmem")))
#endif
= {
	1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

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

void obj_ssd1306_init(ctx_obj_ssd1306 *ctx, obj_ssd1306_i2c_transmit i2c_transmit, void *handle) {
	
	uint8_t init_data_temp[OBJ_SSD1306_CHARACTER_LENGTH];
	uint8_t init_size_temp;
	
	ctx->handle = handle;
	ctx->i2c_transmit = i2c_transmit;
	ctx->cursor_x = 0;
	ctx->cursor_y = 0;
	
	for(uint16_t i = 0; i < sizeof(obj_ssd1306_buffer_sizes); i++) {
		
		lo_flash_read(
				(lo_flash_data_pointer)&obj_ssd1306_buffer_sizes[i],
				&init_size_temp,
				1
		);
		
		lo_flash_read(
				(lo_flash_data_pointer)obj_ssd1306_buffer[i], 
				init_data_temp, 
				init_size_temp
		);
		
		ctx->i2c_transmit(
				ctx->handle, 
				0x00, 
				init_data_temp, 
				init_size_temp
		);
	}
}

void obj_ssd1306_clear(ctx_obj_ssd1306 *ctx) {
	uint8_t data[8] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	for(uint16_t i = 0; i < OBJ_SSD1306_AREA / 16; i++) {
		ctx->i2c_transmit(
				ctx->handle, 
				OBJ_SSD1306_STRT_L_MIN_REG, 
				data, 
				sizeof(data)
		);
	}
}

void obj_ssd1306_move_cursor(ctx_obj_ssd1306 *ctx) {
	uint8_t data[] = {
		0x00
	};
	
	ctx->i2c_transmit(
			ctx->handle,
			OBJ_SSD1306_STRT_L_MIN_REG,
			data,
			1
	);
}

void obj_ssd1306_transmit_sprites(ctx_obj_ssd1306 *ctx, uint8_t *data, uint16_t length) {
	ctx->i2c_transmit(ctx->handle, OBJ_SSD1306_STRT_L_MIN_REG, data, length);
}

void obj_ssd1306_transmit_text(ctx_obj_ssd1306 *ctx, uint8_t *data, uint16_t length) {
	for(uint16_t i = 0; i < length; i++) {
		
		uint8_t character_buffer[OBJ_SSD1306_CHARACTER_LENGTH];
		
		if(
				(data[i] == '\r' && data[i + 1] == '\n') ||
				(data[i] == '\n' && data[i + 1] == '\r')
		) {
			while(ctx->cursor_x < 125) {
				
				uint8_t dummy[] = {
					0x00, 0x00, 0x00, 0x00, 0x00
				};
				
				ctx->i2c_transmit(
						ctx->handle,
						OBJ_SSD1306_STRT_L_MIN_REG,
						dummy,
						OBJ_SSD1306_CHARACTER_LENGTH
				);
				ctx->cursor_x += 5;
			}
			i++;
		}
		else {
			lo_flash_read(
					(lo_flash_data_pointer)obj_ssd1306_disp_font[OBJ_SSD1306_CHARACTER_INDEX(data[i])],
					character_buffer,
					OBJ_SSD1306_CHARACTER_LENGTH
			);
					
			ctx->i2c_transmit(
					ctx->handle,
					OBJ_SSD1306_STRT_L_MIN_REG,
					character_buffer,
					OBJ_SSD1306_CHARACTER_LENGTH
			);
					
			ctx->cursor_x += 5;
		}
		
		if(ctx->cursor_x == 125) {
			ctx->cursor_x = 0;
			ctx->cursor_y++;
			ctx->cursor_y %= OBJ_SSD1306_HEIGHT;
			
			for(uint16_t j = 0; j < 1; j++) {
				
				uint8_t character_buffer[3] = {
					0x00, 0x00, 0x00
				};
				
				ctx->i2c_transmit(
						ctx->handle,
						OBJ_SSD1306_STRT_L_MIN_REG,
						character_buffer,
						sizeof(character_buffer)
				);
			}
		}
	}
}
