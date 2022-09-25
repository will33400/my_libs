/*
 * adc.h
 *
 * Created: 7/19/2020 4:47:50 PM
 *  Author: Adam
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

#define HW_ADC_CONV_XBITS(raw_val, resolution) (int)resolution >= 10 ? (raw_val<<(resolution - 10)) : (raw_val>>(10 - resolution))
#define HW_ADC_CONV_VOLTS(raw_val, vmax) ((double)raw_val * vmax / 1024.0)

#define HW_ADC_INST_COUNT 5

typedef enum {
	//! ADC0, single ended
	HW_ADC_MUX_ADC0 = 0x0,
	//! ADC1, single ended
	HW_ADC_MUX_ADC1 = 0x1,
	//! ADC2, single ended
	HW_ADC_MUX_ADC2 = 0x2,
	//! ADC3, single ended
	HW_ADC_MUX_ADC3 = 0x3,
	//#if MEGA_XX8_A
	//! ADC8, single ended. Temperature sensor on megaxx8A/P/PA devices
	HW_ADC_MUX_TEMPSENSE = 0xF,
	//#endif
	//! 1.1V internal bandgap voltage
	HW_ADC_MUX_1V1 = 0x0C,
	//! GND, single ended
	HW_ADC_MUX_GND = 0x0D,
}hw_adc_mux_setting;

typedef enum {
	//! AREF, internal Vref turned off
	HW_ADC_VREF_AREF = (0x0 << REFS0),
	//! AVCC
	HW_ADC_VREF_AVCC = (0x1  << REFS0),
	//! Internal 1.1V voltage reference
	HW_ADC_VREF_1V1 = (0x2  << REFS0),
	//! Reserved value, not available.
	HW_ADC_VREF_RESERVED = (0x3  << REFS0),
}hw_adc_volt_ref;

typedef enum {
	/*  Divide system clock by 2 */
	HW_ADC_PRESCALER_DIV2 = (0x1 << ADPS0),
	/*  Divide system clock by 4 */
	HW_ADC_PRESCALER_DIV4 = (0x2 << ADPS0),
	/*  Divide system clock by 8 */
	HW_ADC_PRESCALER_DIV8 = (0x3 << ADPS0),
	/*  Divide system clock by 16 */
	HW_ADC_PRESCALER_DIV16 = (0x4 << ADPS0),
	/*  Divide system clock by 32 */
	HW_ADC_PRESCALER_DIV32 = (0x5 << ADPS0),
	/*  Divide system clock by 64 */
	HW_ADC_PRESCALER_DIV64 = (0x6 << ADPS0),
	/*  Divide system clock by 128 */
	HW_ADC_PRESCALER_DIV128 = (0x7 << ADPS0),
}hw_adc_presc;

typedef enum {
	/*  Right adjust result */
	HW_ADC_ADJUSTMENT_RIGHT = 0,
	/*  Left adjust result */
	HW_ADC_ADJUSTMENT_LEFT = (1 << ADLAR),
}hw_adc_adj;

void adc_init(void);
uint16_t get_adc(uint8_t pin);

typedef void(*lo_adc_cb)(void *handle);

typedef struct {
	hw_adc_volt_ref vref;
	hw_adc_presc prescale;
	double vmax;
	bool it_enable;
}hw_adc_peripheral_param;

typedef hw_adc_peripheral_param hw_adc_peripheral_data;

typedef struct hw_adc_inst_{
	hw_adc_mux_setting pin_num;
	uint16_t raw_val_10bits;
	bool enabled;
	bool measure_complete;
	uint8_t resolution;
	lo_adc_cb cb;
	void *handle;
	
	struct hw_adc_inst_ *next_inst;
}hw_adc_inst;

typedef struct {
	hw_adc_mux_setting pin_num;
}hw_adc_param;

void hw_adc_init(hw_adc_peripheral_param *param);
void hw_adc_init_inst(hw_adc_inst *inst, hw_adc_param *param);
void hw_adc_set_cb(hw_adc_inst *inst, lo_adc_cb cb, void *handle);

void hw_adc_enable(hw_adc_inst *inst, bool enable);
uint16_t hw_adc_get_raw(hw_adc_inst *inst);
double hw_adc_get_volts(hw_adc_inst *inst);
bool hw_adc_val_available(hw_adc_inst *inst);

#endif /* ADC_H_ */