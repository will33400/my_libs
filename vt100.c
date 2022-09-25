
/*!
 * \file vt100.c
 * \brief vt100 functions
 * \author Adam LUCAS
 * \date
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "vt100.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn void vt100_move(VT100_CTX *ctx, uint8_t x, uint8_t y);
* \brief vt100 command move cursor position
* \param[in] ctx object oriented data pointer
* \param[in] x position value
* \param[in] y positon value
* \return None
*/

void vt100_move(VT100_CTX *ctx, uint8_t x, uint8_t y)
{
	uint8_t message[VT100_MOVE_CMD_SIZE+5];

	sprintf((char *)message, VT100_MOVE_CMD, VT100_ESC, y + 1, x + 1);
	ctx->uart_write(ctx->handle, message, VT100_MOVE_CMD_SIZE);
}

//*****************************************************************************/
/*!
* \fn void vt100_clear_screen(VT100_CTX *ctx);
* \brief vt100 command clear screen
* \param[in] ctx object oriented data pointer
* \return None
*/

void vt100_clear_screen(VT100_CTX *ctx)
{
	uint8_t message[VT100_ERASE_CMD_SIZE+5];

	sprintf((char *)message, VT100_ERASE_CMD, VT100_ESC, VT100_ESC);
	ctx->uart_write(ctx->handle, message, VT100_ERASE_CMD_SIZE);

}

void vt100_font_set(
		VT100_CTX *ctx, 
		vt100_font_setting setting,
		vt100_font_foreground foreground, 
		vt100_font_background background
) {
	uint8_t message[VT100_FONT_CMD_SIZE + 1];
	sprintf(
			(char *)message, 
			VT100_FONT_CMD, 
			VT100_ESC, 
			setting, 
			foreground, 
			background
	);
	ctx->uart_write(ctx->handle, message, VT100_FONT_CMD_SIZE);
}

