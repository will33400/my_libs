
#ifndef LO_DEFINES_H_
#define LO_DEFINES_H_

#include <stdint.h>


#define LO_DEFINES_HW_ATMEGA328P			0
#define LO_DEFINES_HW_ATTINY85				1
#define LO_DEFINES_HW_PIC16F628A            2
#define LO_DEFINES_HW_STM32F103C8			3

#define LO_DEFINES_HW_SEL					LO_DEFINES_HW_STM32F103C8

#ifndef LO_DEFINES_HW_SEL
#error hardware not selected ! define LO_DEFINES_HW_SEL preprocessor
#endif



#if LO_DEFINES_HW_SEL == LO_DEFINES_HW_PIC16F628A
typedef uint32_t uint64_t;
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_STM32F103C8
#define SYSCLK_FREQ_VAL 72000000UL
#define SYSCLK_FREQ_72MHz
#elif LO_DEFINES_HW_SEL == LO_DEFINES_HW_ATMEGA328P
#define LO_DEFINES_ADC_FREQ				8000000UL
#define DEFINES_UART_ENABLE
#define DEFINES_UART_DISABLE

#define DEFINES_UART_SEL				DEFINES_UART_DISABLE

#define DEFINES_DEBUG_LOG_UART
#define DEFINES_DEBUG_LOG_DISABLE

#define DEFINES_DEBUG_LOG_SEL			DEFINES_DEBUG_LOG_DISABLE
#endif



#endif