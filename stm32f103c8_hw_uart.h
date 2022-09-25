/*!
 * \file hw_uart.h
 * \brief low level uart header
 * \author Adam LUCAS
 * \date 20/05/2022
 */

#ifndef HW_UART_H_
#define HW_UART_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>

#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "lo_defines.h"
#include "stm32f103c8_hw_gpio.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

#ifdef SYSCLK_FREQ_HSE
#elif defined SYSCLK_FREQ_24MHz
#elif defined SYSCLK_FREQ_36MHz
#define HW_UART_BRR_2400_VAL				((938u<<4) | (1u<<0))
#define HW_UART_BRR_4800_VAL				((469u<<4) | (1u<<0))
#define HW_UART_BRR_9600_VAL				((234u<<4) | (1u<<0))
#define HW_UART_BRR_19200_VAL				((117u<<4) | (1u<<0))
#define HW_UART_BRR_57600_VAL				((39u<<4) | (1u<<0))
#define HW_UART_BRR_115200_VAL			((20u<<4) | (1u<<0))
#define HW_UART_BRR_230769_VAL			((10u<<4) | (1u<<0))
#define HW_UART_BRR_461538_VAL			((5u<<4) | (1u<<0))
#define HW_UART_BRR_923076_VAL			((2u<<4) | (1u<<0))
#define HW_UART_BRR_2250000_VAL			((1u<<4) | (1u<<0))
#elif defined SYSCLK_FREQ_48MHz
#elif defined SYSCLK_FREQ_56MHz
#elif defined SYSCLK_FREQ_72MHz
#define HW_UART_BRR_2400_VAL				((1875u<<4) | (1u<<0))
#define HW_UART_BRR_4800_VAL				((938u<<4) | (1u<<0))
#define HW_UART_BRR_9600_VAL				((469u<<4) | (1u<<0))
#define HW_UART_BRR_19200_VAL				((234u<<4) | (1u<<0))
#define HW_UART_BRR_57600_VAL				((78u<<4) | (1u<<0))
#define HW_UART_BRR_115200_VAL			((39u<<4) | (1u<<0))
#define HW_UART_BRR_230769_VAL			((20u<<4) | (1u<<0))
#define HW_UART_BRR_461538_VAL			((10u<<4) | (1u<<0))
#define HW_UART_BRR_923076_VAL			((5u<<4) | (1u<<0))
#define HW_UART_BRR_2250000_VAL			((2u<<4) | (1u<<0))
#define HW_UART_BRR_4500000_VAL			((1u<<4) | (1u<<0))
#endif

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
 

typedef enum {
	HW_UART_OK,
	HW_UART_ERROR,
}hw_uart_flag;

typedef enum {
	HW_UART_BAUD_2400				= (uint32_t)HW_UART_BRR_2400_VAL,
	HW_UART_BAUD_4800				= (uint32_t)HW_UART_BRR_4800_VAL,
	HW_UART_BAUD_9600				= (uint32_t)HW_UART_BRR_9600_VAL,
	HW_UART_BAUD_19200			= (uint32_t)HW_UART_BRR_19200_VAL,
	HW_UART_BAUD_57600			= (uint32_t)HW_UART_BRR_57600_VAL,
	HW_UART_BAUD_115200			= (uint32_t)HW_UART_BRR_115200_VAL,
	HW_UART_BAUD_230769			= (uint32_t)HW_UART_BRR_230769_VAL,
	HW_UART_BAUD_461538			= (uint32_t)HW_UART_BRR_461538_VAL,
	HW_UART_BAUD_923076			= (uint32_t)HW_UART_BRR_923076_VAL,
	HW_UART_BAUD_2250000		= (uint32_t)HW_UART_BRR_2250000_VAL,
#ifdef SYSCLK_FREQ_72MHz
	HW_UART_BAUD_4500000		= (uint32_t)HW_UART_BRR_4500000_VAL,
#endif
}hw_uart_brr;
 
typedef void(*hw_uart_rx_cb)(void *handle);

typedef struct {
	hw_uart_brr baud;
	USART_TypeDef *uart;
}hw_uart_param;

typedef struct hw_uart_inst_{
	int8_t last_char;
	hw_uart_rx_cb rx_cb;
	void *rx_handle;
	
	uint16_t buffer_size;

	bool transmit_complete;
	bool is_init;
	hw_uart_flag flag;
	
	hw_gpio_inst rx_inst;
	hw_gpio_inst tx_inst;
	
	USART_TypeDef *uart;
	
	struct hw_uart_inst_ *next_inst;
}hw_uart_inst;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void hw_uart_init(hw_uart_inst *inst, hw_uart_param *param);
void hw_uart_set_rx_cb(hw_uart_inst *inst, hw_uart_rx_cb rx_cb, void *rx_handle);
void hw_uart_putchar(hw_uart_inst *inst, const int8_t data, uint32_t timeout);
void hw_uart_puts(hw_uart_inst *inst, const int8_t *data, uint16_t size, uint32_t timeout);
char hw_uart_getchar(hw_uart_inst *inst);
hw_uart_flag hw_uart_get_flag(hw_uart_inst *inst);


void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif /* HW_UART_H_ */
