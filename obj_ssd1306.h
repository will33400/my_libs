/*!
 * \file obj_ssd1306.h
 * \brief object oriented ssd1306 device header
 * \author Adam LUCAS
 * \date 8/1/2021 10:04:19 PM
 */

#ifndef OBJ_SSD1306_H_
#define OBJ_SSD1306_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include "lo_flash_io.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

//ASCII table definition parameters
//#define ASCCI_TABLE_HALF_DEFINED

#define OBJ_SSD1306_DATA_RAM_INIT							0
#define OBJ_SSD1306_DATA_FLASH_AVR_INIT						1

#define OBJ_SSD1306_DATA_INIT_SEL							OBJ_SSD1306_DATA_FLASH_AVR_INIT

//device default address
#define OBJ_SSD1306_DEFAULT_ADDR							0x78

//device default configs
#define OBJ_SSD1306_HEIGHT									32
#define OBJ_SSD1306_WIDTH									128
#define OBJ_SSD1306_AREA									(OBJ_SSD1306_WIDTH * OBJ_SSD1306_HEIGHT)

#define OBJ_SSD1306_CHARACTER_INDEX(ascii)					(ascii - ' ')
#define OBJ_SSD1306_CHARACTER_LENGTH						5

//fundamental command table
#define OBJ_SSD1306_SET_CONTR_REG							0x81

#define OBJ_SSD1306_SET_CONTR_VAL_MIN_REG_A					0x01
#define OBJ_SSD1306_SET_CONTR_VAL_MAX_REG_A					0x7F

#define OBJ_SSD1306_ENT_DISP_ON_REG							0xA4
#define OBJ_SSD1306_ENT_DISP_OFF_REG						0xA5

#define OBJ_SSD1306_SET_DISP_NORMAL_REG						0xA6
#define OBJ_SSD1306_SET_DISP_REVERSE_REG					0xA7

#define OBJ_SSD1306_SET_DISP_OFF_REG						0xAE
#define OBJ_SSD1306_SET_DISP_ON_REG							0xAF

//scrolling command table
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_RIGHT_REG				0x26
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_LEFT_REG				0x27

#define OBJ_SSD1306_CNTNS_HRZT_SCRL_DUMMY_REG_A				0x00

#define OBJ_SSD1306_CNTNS_HRZT_SCRL_STRT_PAGE_0_ADDR_REG_B	0x00
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_STRT_PAGE_1_ADDR_REG_B	0x01
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_STRT_PAGE_2_ADDR_REG_B	0x02
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_STRT_PAGE_3_ADDR_REG_B	0x03
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_STRT_PAGE_4_ADDR_REG_B	0x04
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_STRT_PAGE_5_ADDR_REG_B	0x05
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_STRT_PAGE_6_ADDR_REG_B	0x06
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_STRT_PAGE_7_ADDR_REG_B	0x07

#define OBJ_SSD1306_CNTNS_HRZT_SCRL_5_FRAMES_REG_C			0x00
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_64_FRAMES_REG_C			0x01
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_128_FRAMES_REG_C		0x02
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_256_FRAMES_REG_C		0x03
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_3_FRAMES_REG_C			0x04
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_4_FRAMES_REG_C			0x05
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_25_FRAMES_REG_C			0x06
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_2_FRAMES_REG_C			0x07

#define OBJ_SSD1306_CNTNS_HRZT_SCRL_END_PAGE_0_REG_D		0x00
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_END_PAGE_1_REG_D		0x01
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_END_PAGE_2_REG_D		0x02
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_END_PAGE_3_REG_D		0x03
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_END_PAGE_4_REG_D		0x04
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_END_PAGE_5_REG_D		0x05
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_END_PAGE_6_REG_D		0x06
#define OBJ_SSD1306_CNTNS_HRZT_SCRL_END_PAGE_7_REG_D		0x07

#define OBJ_SSD1306_CNTNS_HRZT_SCRL_DUMMY_REG_E				0x00

#define OBJ_SSD1306_CNTNS_HRZT_SCRL_DUMMY_REG_F				0xFF

#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_VRT_RIGHT_REG			0x29
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_VRT_LEFT_REG			0x2A

#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_DUMMY_REG_A			0x00

#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_STRT_PAGE_0_REG_B		0x00
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_STRT_PAGE_1_REG_B		0x01
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_STRT_PAGE_2_REG_B		0x02
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_STRT_PAGE_3_REG_B		0x03
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_STRT_PAGE_4_REG_B		0x04
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_STRT_PAGE_5_REG_B		0x05
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_STRT_PAGE_6_REG_B		0x06
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_STRT_PAGE_7_REG_B		0x07

#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_5_FRAMES_REG_C			0x00
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_64_FRAMES_REG_C		0x01
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_128_FRAMES_REG_C		0x02
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_256_FRAMES_REG_C		0x03
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_3_FRAMES_REG_C			0x04
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_4_FRAMES_REG_C			0x05
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_25_FRAMES_REG_C		0x06
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_2_FRAMES_REG_C			0x07

#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_END_PAGE_0_REG_D		0x00
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_END_PAGE_1_REG_D		0x01
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_END_PAGE_2_REG_D		0x02
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_END_PAGE_3_REG_D		0x03
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_END_PAGE_4_REG_D		0x04
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_END_PAGE_5_REG_D		0x05
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_END_PAGE_6_REG_D		0x06
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_END_PAGE_7_REG_D		0x07

#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_SCRL_OFFSET_MIN_REG_E	0x01
#define OBJ_SSD1306_CNTNS_VRTCL_SCRL_SCRL_OFFSET_MAX_REG_E	0x3F

#define OBJ_SSD1306_DEACTIVATE_SCROLL_REG					0x2E
#define OBJ_SSD1306_ACTIVATE_SCROLL_REG						0x2F

#define OBJ_SSD1306_SET_VRTCL_SCRL_AREA_REG					0xA3

#define OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_FIX_MIN_REG_A	0x00
#define OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_FIX_MAX_REG_A	0x1F
#define OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_DEFAULT_REG_A	OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_FIX_MIN_REG_A

#define OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_SCRL_MIN_REG_B	0x00
#define OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_SCRL_MAX_REG_B	0x40
#define OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_DEFAULT_REG_B	OBJ_SSD1306_SET_VRTCL_SCRL_AREA_ROWS_SCRL_MAX_REG_B

//addressing setting command table
#define OBJ_SSD1306_SET_LOWER_CLMN_ADDR_PAGE_MODE_MIN_REG	0x00
#define OBJ_SSD1306_SET_LOWER_CLMN_ADDR_PAGE_MODE_MAX_REG	0x0F

#define OBJ_SSD1306_SET_HIGHER_CLMN_ADDR_PAGE_MODE_MIN_REG	0x10
#define OBJ_SSD1306_SET_HIGHER_CLMN_ADDR_PAGE_MODE_MAX_REG	0x1F

#define OBJ_SSD1306_SET_MEM_ADDR_MODE_REG					0x20

#define OBJ_SSD1306_SET_MEM_ADDR_MODE_HRZT_ADDR_MODE_REG_A	0x00
#define OBJ_SSD1306_SET_MEM_ADDR_MODE_VRTCL_ADDR_MODE_REG_A	0x01
#define OBJ_SSD1306_SET_MEM_ADDR_MODE_PAGE_ADDR_MODE_REG_A	0x02

#define OBJ_SSD1306_SET_CLMN_ADDR_REG						0x21

#define OBJ_SSD1306_SET_CLMN_ADDR_START_ADDR_MIN_REG_A		0x00
#define OBJ_SSD1306_SET_CLMN_ADDR_START_ADDR_MAX_REG_A		0x8F

#define OBJ_SSD1306_SET_CLMN_ADDR_END_ADDR_MIN_REG_B		0x00
#define OBJ_SSD1306_SET_CLMN_ADDR_END_ADDR_MAX_REG_B		0x8F

#define OBJ_SSD1306_SET_PAGE_ADDR_REG						0x22

#define OBJ_SSD1306_SET_PAGE_ADDR_START_ADDR_MIN_REG_A		0x00
#define OBJ_SSD1306_SET_PAGE_ADDR_START_ADDR_MAX_REG_A		0x07

#define OBJ_SSD1306_SET_PAGE_ADDR_END_ADDR_MIN_REG_B		0x00
#define OBJ_SSD1306_SET_PAGE_ADDR_END_ADDR_MAX_REG_B		0x07

#define OBJ_SSD1306_SET_PAGE_START_MIN_REG					0xB0
#define OBJ_SSD1306_SET_PAGE_START_MAX_REG					0xB7

//hardware configuration (panel resolution & layout related) command table
#define OBJ_SSD1306_STRT_L_MIN_REG							0x40
#define OBJ_SSD1306_STRT_L_MAX_REG							0x7F

#define OBJ_SSD1306_SET_SEGMENT_REMAP_ADDR_0_REG			0xA0
#define OBJ_SSD1306_SET_SEGMENT_REMAP_ADDR_127_REG			0xA1

#define OBJ_SSD1306_SET_MUX_RATIO_REG						0xA8

#define OBJ_SSD1306_SET_MUX_RATIO_MIN_REG_A					0x00
#define OBJ_SSD1306_SET_MUX_RATIO_MAX_REG_A					0x3F

#define OBJ_SSD1306_SET_COM_OUT_SCAN_DIR_NORMAL_REG			0xC0
#define OBJ_SSD1306_SET_COM_OUT_SCAN_DIR_REMAP_REG			0xC8

#define OBJ_SSD1306_SET_DISPLAY_OFFSET_REG					0xD3

#define OBJ_SSD1306_SET_DISPLAY_OFFSET_MIN_REG_A			0x00
#define OBJ_SSD1306_SET_DISPLAY_OFFSET_MAX_REG_A			0x3F

#define OBJ_SSD1306_SET_COM_PINS_REG						0xDA

#define OBJ_SSD1306_SET_COM_PINS_SEQ_COM_DIS_COM_REG_A		0x02
#define OBJ_SSD1306_SET_COM_PINS_ALTERN_COM_DIS_COM_REG_A	0x12
#define OBJ_SSD1306_SET_COM_PINS_SEQ_COM_LR_COM_REG_A		0x22
#define OBJ_SSD1306_SET_COM_PINS_ALTERN_COM_LR_COM_REG_A	0x32

//timing & driving scheme setting command table
#define OBJ_SSD1306_SET_DISP_PRSCL_REG						0xD5

#define OBJ_SSD1306_SET_DISP_PRSCL_PRSCL_MIN_MASK_A			0x00
#define OBJ_SSD1306_SET_DISP_PRSCL_PRSCL_MAX_MASK_A			0x0F

#define OBJ_SSD1306_SET_DISP_PRSCL_OSC_MIN_MASK_A			0x00
#define OBJ_SSD1306_SET_DISP_PRSCL_OSC_MAX_MASK_A			0xF0

#define OBJ_SSD1306_SET_DISP_PRSCL_DEFAULT_REG_A			(OBJ_SSD1306_SET_DISP_PRSCL_PRSCL_MIN_MASK_A | OBJ_SSD1306_SET_DISP_PRSCL_OSC_MIN_MASK_A)

#define OBJ_SSD1306_SET_PRE_CHRG_PRD_REG					0xD9

#define OBJ_SSD1306_SET_PRE_CHRG_PRD_PHASE_1_MIN_MASK_A		0x02
#define OBJ_SSD1306_SET_PRE_CHRG_PRD_PHASE_1_MAX_MASK_A		0x0F

#define OBJ_SSD1306_SET_PRE_CHRG_PRD_PHASE_2_MIN_MASK_A		0x02
#define OBJ_SSD1306_SET_PRE_CHRG_PRD_PHASE_2_MAX_MASK_A		0x0F

#define OBJ_SSD1306_SET_PRE_CHRG_PRD_DEFAULT_REG_A			(OBJ_SSD1306_SET_PRE_CHRG_PRD_PHASE_1_MIN_MASK_A | OBJ_SSD1306_SET_PRE_CHRG_PRD_PHASE_2_MIN_MASK_A)

#define OBJ_SSD1306_SET_VCOM_DSLCT_LVL_REG					0xDB

#define OBJ_SSD1306_SET_VCOM_DSLCT_LVL_0_65_VCC_REG_A		0x00
#define OBJ_SSD1306_SET_VCOM_DSLCT_LVL_0_77_VCC_REG_A		0x20
#define OBJ_SSD1306_SET_VCOM_DSLCT_LVL_0_83_VCC_REG_A		0x30

#define OBJ_SSD1306_SET_VCOM_DSLCT_LVL_DEFAULT_REG_A		OBJ_SSD1306_SET_VCOM_DSLCT_LVL_0_77_VCC_REG_A

#define OBJ_SSD1306_NOP_REG									0xE3

//command for charge pump setting
#define OBJ_SSD1306_CHARGE_PUMP_SETTING_REG					0x8D //set regs as follows : 0x8D, 0x14, 0xAF(set screen ON)

#define OBJ_SSD1306_CHARGE_PUMP_SETTING_DISABLE_REG_A		0x10
#define OBJ_SSD1306_CHARGE_PUMP_SETTING_ENABLE_REG_A		0x14

#define OBJ_SSD1306_CHARGE_PUMP_DEFAULT_REG_A				OBJ_SSD1306_CHARGE_PUMP_SETTING_ENABLE_REG_A

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum <name>
 * \union <name>
 * \struct <name>
 * \brief <description>
 */

typedef void(*obj_ssd1306_i2c_transmit)(void *, uint8_t, uint8_t *, uint16_t);

typedef struct {
	obj_ssd1306_i2c_transmit i2c_transmit;
	void *handle;
	uint8_t cursor_x;
	uint8_t cursor_y;
}ctx_obj_ssd1306;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void obj_ssd1306_init(ctx_obj_ssd1306 *ctx, obj_ssd1306_i2c_transmit i2c_transmit, void *handle);

void obj_ssd1306_clear(ctx_obj_ssd1306 *ctx);
void obj_ssd1306_move_cursor(ctx_obj_ssd1306 *ctx);
void obj_ssd1306_transmit_sprites(ctx_obj_ssd1306 *ctx, uint8_t *data, uint16_t length);
void obj_ssd1306_transmit_text(ctx_obj_ssd1306 *ctx, uint8_t *data, uint16_t length);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* OBJ_SSD1306_H_ */