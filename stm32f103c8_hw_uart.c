/*!
 * \file hw_uart.c
 * \brief low level uart functions
 * \author Adam LUCAS
 * \date 20/05/2022
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "stm32f103c8_hw_uart.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static hw_uart_inst *first_inst = (void *)0;
static hw_uart_inst *last_inst = (void *)0;
static bool lib_init;

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

static void hw_uart_default_cb(void *handle) {
	(void)handle;
}

static void hw_uart_reset_peripherals(void) {
	NVIC_ClearPendingIRQ(USART1_IRQn);
	NVIC_ClearPendingIRQ(USART2_IRQn);
	NVIC_ClearPendingIRQ(USART3_IRQn);
	NVIC_DisableIRQ(USART1_IRQn);
	NVIC_DisableIRQ(USART2_IRQn);
	NVIC_DisableIRQ(USART3_IRQn);
	
	RCC->APB1RSTR |= (1u<<17) | (1u<<18);
	RCC->APB2RSTR |= (1u<<14);
	RCC->APB1ENR &= ~((1u<<17) | (1u<<18));
	RCC->APB2ENR &= ~(1u<<14);
	RCC->APB1RSTR &= ~((1u<<17) | (1u<<18));
	RCC->APB2RSTR &= ~(1u<<14);
}

void hw_uart_init(hw_uart_inst *inst, hw_uart_param *param) {
	
	hw_gpio_init_param param_rx;
	hw_gpio_init_param param_tx;
	hw_uart_inst *temp_inst = first_inst;
	
	if(last_inst)
		last_inst->next_inst = inst;
	if(!first_inst)
		first_inst = inst;
	last_inst = inst;
	
	if(param->uart == USART1) {
		param_rx.pin_num = 10;
		param_rx.port = GPIOA;
		param_rx.port_config = HW_GPIO_CR_INPUT_PUPD;
		param_rx.state = true;
		
		param_tx.pin_num = 9;
		param_tx.port = GPIOA;
		param_tx.port_config = HW_GPIO_CR_ALT2MHZ_PSPL;
	}
	else if(param->uart == USART2) {
		param_rx.pin_num = 3;
		param_rx.port = GPIOA;
		param_rx.port_config = HW_GPIO_CR_INPUT_PUPD;
		param_rx.state = true;
		
		param_tx.pin_num = 2;
		param_tx.port = GPIOA;
		param_tx.port_config = HW_GPIO_CR_ALT2MHZ_PSPL;
	}
	else if(param->uart == USART3) {
		param_rx.pin_num = 11;
		param_rx.port = GPIOB;
		param_rx.port_config = HW_GPIO_CR_INPUT_PUPD;
		param_rx.state = true;
		
		param_tx.pin_num = 10;
		param_tx.port = GPIOB;
		param_tx.port_config = HW_GPIO_CR_ALT2MHZ_PSPL;
	}
	
	if(!lib_init) {
		hw_uart_reset_peripherals();
		lib_init = true;
	}
	
	while(temp_inst) {
		if(temp_inst->uart == param->uart) {
			temp_inst->flag = HW_UART_ERROR;
			return;
		}
		
		temp_inst = temp_inst->next_inst;
	}
	
	if(inst->is_init) {
		inst->flag = HW_UART_ERROR;
		return;
	}
	
	if(param->uart == USART1) // enable peripheral clock
		RCC->APB2ENR |= (1u<<14);
	else if(param->uart == USART2)
		RCC->APB1ENR |= (1u<<17);
	else if(param->uart == USART3)
		RCC->APB1ENR |= (1u<<18);
	
	inst->uart = param->uart;
	inst->rx_cb = hw_uart_default_cb;
	
	hw_gpio_init(&inst->rx_inst, &param_rx);
	hw_gpio_init(&inst->tx_inst, &param_tx);
	
	param->uart->BRR |= param->baud;
	param->uart->CR1 |= (1u<<6) | (1<<5) | (1<<3) | (1<<2);
	//param->uart->GTPR |= 1;
	
	if(param->uart == USART1) {
		NVIC_SetPriority(USART1_IRQn, 5);
		NVIC_EnableIRQ(USART1_IRQn);
	}
	else if(param->uart == USART2) {
		NVIC_SetPriority(USART2_IRQn, 5);
		NVIC_EnableIRQ(USART2_IRQn);
	}
	else if(param->uart == USART3) {
		NVIC_SetPriority(USART3_IRQn, 5);
		NVIC_EnableIRQ(USART3_IRQn);
	}
	
	param->uart->CR1 |= (1u<<13);
}

void hw_uart_set_rx_cb(hw_uart_inst *inst, hw_uart_rx_cb rx_cb, void *rx_handle) {
	inst->rx_cb = rx_cb;
	inst->rx_handle = rx_handle;
}

void hw_uart_putchar(hw_uart_inst *inst, const int8_t data, uint32_t timeout) {
	(void)timeout;
	inst->uart->DR = (uint8_t)data;
	while(!inst->transmit_complete);
	inst->transmit_complete = false;
}

void hw_uart_puts(hw_uart_inst *inst, const int8_t *data, uint16_t size, uint32_t timeout) {
	(void)timeout;
	for(uint16_t i = 0; i < size; i++) {
		while(!inst->transmit_complete);
		inst->uart->DR = (uint8_t)data[i];
		inst->transmit_complete = false;
	}
}

char hw_uart_getchar(hw_uart_inst *inst) {
	return (char)inst->last_char;
}

hw_uart_flag hw_uart_get_flag(hw_uart_inst *inst) {
	(void)inst;
	return HW_UART_OK;
}

inline static void USARTx_IRQHandler(USART_TypeDef *uart) {
	
	hw_uart_inst *temp_inst = first_inst;
	
	while(temp_inst) {
		if(temp_inst->uart == uart) break;
		temp_inst = temp_inst->next_inst;
	}
	
	if(uart->SR & (1<<6)) {
		uart->SR &= ~(1<<6);
		temp_inst->transmit_complete = true;
	}
	else if(uart->SR & (1<<5)) {
		uart->SR &= ~(1<<5);
		if(!temp_inst->transmit_complete) return;
		temp_inst->last_char = (int8_t)uart->DR;
		temp_inst->rx_cb(temp_inst->rx_handle);
	}
}

void USART1_IRQHandler(void) {
	USARTx_IRQHandler(USART1);
}

void USART2_IRQHandler(void) {
	USARTx_IRQHandler(USART2);
}

void USART3_IRQHandler(void) {
	USARTx_IRQHandler(USART3);
}

