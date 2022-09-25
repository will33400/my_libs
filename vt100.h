/*!
 * \file vt100.h
 * \brief vt100 header
 * \author Adam LUCAS
 * \date
 */

#ifndef VT100_H_
#define VT100_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#define VT100_SCREEN_HEIGHT (24)
#define VT100_SCREEN_WIDTH  (80)

#define VT100_SCREEN_XMIN (1)
#define VT100_SCREEN_YMIN (1)
#define VT100_SCREEN_XMAX (VT100_SCREEN_WIDTH)
#define VT100_SCREEN_YMAX (VT100_SCREEN_HEIGHT)

#define VT100_ESC       (27)
#define VT100_FORM_FEED (12)

#define VT100_MOVE_CMD		"%c[%03d;%03dH"
#define VT100_MOVE_CMD_SIZE	(sizeof("c[000;000H") - 1)

#define VT100_ERASE_CMD		"%c[2J%c[?25l"
#define VT100_ERASE_CMD_SIZE (sizeof("c[2Jc[?25l") - 1)

#define VT100_FONT_CMD		"%c[%01d;%02d;%02dm"
#define VT100_FONT_CMD_SIZE (sizeof("c[d;dd;ddm") - 1)
	
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef enum {
	VT100_FONT_SETTING_BOLD = 1,
	VT100_FONT_SETTING_UNDERLINE = 4,
	VT100_FONT_SETTING_BLINKING = 5,
	VT100_FONT_SETTING_INVERSE = 7,
}vt100_font_setting;

typedef enum {
	VT100_FONT_COLOR_BLACK_FRGND = 30,
	VT100_FONT_COLOR_RED_FRGND = 31,
	VT100_FONT_COLOR_GREEN_FRGND = 32,
	VT100_FONT_COLOR_YELLOW_FRGND = 33,
	VT100_FONT_COLOR_BLUE_FRGND = 34,
	VT100_FONT_COLOR_MAGENTA_FRGND = 35,
	VT100_FONT_COLOR_CYAN_FRGND = 36,
	VT100_FONT_COLOR_WHITE_FRGND = 37,
}vt100_font_foreground;

typedef enum {
	VT100_FONT_COLOR_BLACK_BKGND = 40,
	VT100_FONT_COLOR_RED_BKGND = 41,
	VT100_FONT_COLOR_GREEN_BKGND = 42,
	VT100_FONT_COLOR_YELLOW_BKGND = 43,
	VT100_FONT_COLOR_BLUE_BKGND = 44,
	VT100_FONT_COLOR_MAGENTA_BKGND = 45,
	VT100_FONT_COLOR_CYAN_BKGND = 46,
	VT100_FONT_COLOR_WHITE_BKGND = 47,
}vt100_font_background;

/*!
 * \fn void(*vt100_uart_write)(void *, uint8_t *, uint16_t);
 * \brief uart write function pointer
 * \param[in] handle data
 * \param[in] data pointer
 * \param[in] data length
 */

typedef void(*vt100_uart_write)(void *, uint8_t *, uint16_t);

/*!
 * \struct VT100_CTX
 * \brief object oriented vt100 required parameters struct
 */

typedef struct {
	vt100_uart_write uart_write;
	void *handle;
}VT100_CTX;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void vt100_move(VT100_CTX *ctx, uint8_t x, uint8_t y);
void vt100_clear_screen(VT100_CTX *ctx);
void vt100_font_set(
		VT100_CTX *ctx, 
		vt100_font_setting setting,
		vt100_font_foreground foreground, 
		vt100_font_background background
);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* VT100_H_ */
