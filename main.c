
#include <stm32f10x.h>
#include "lo_gpio.h"
#include "lo_timer.h"
#include "mid_serial.h"

#include "stm32f103c8_hw_gpio.h"

static hw_gpio_inst pin_in;
static hw_gpio_inst pin_out;

static mid_serial_inst serial;

static lo_timer_periph timer;

static void main_gpio_cb(void *handle, bool state) {
	(void)handle;
	//hw_gpio_set_state(&pin_out, state);
	lo_gpio_set_state(&pin_out, state);
}

static void main_timer_cb(void *handle) {
	(void)handle;
	lo_gpio_toggle_state(&pin_out);
}

static void main_hw_gpio_init(void);
static void main_lo_gpio_init(void);

static void main_mid_serial_init(void);
static void main_mid_serial_process(void);

static void main_lo_timer_init(void);

int main(void) {
	
	main_mid_serial_init();
	main_lo_gpio_init();
	main_lo_timer_init();
	
	while(1) {
		
		main_mid_serial_process();
		
	}
}

static void main_hw_gpio_init(void) {
	hw_gpio_init_param param_out = {
		.pin_num = 11,
		.port = GPIOB,
		.port_config = HW_GPIO_CR_OUT2MHZ_PSPL,
		.state = false,
	};
	
	hw_gpio_init_param param_in = {
		.pin_num = 10,
		.port = GPIOB,
		.port_config = HW_GPIO_CR_INPUT_PUPD,
		.state = false,
		.irq = HW_GPIO_EXTI_BOTH,
	};
	
	hw_gpio_init(&pin_out, &param_out);
	hw_gpio_init(&pin_in, &param_in);
	hw_gpio_set_cb_both(&pin_in, main_gpio_cb);
}

static void main_lo_gpio_init(void) {
	/*lo_gpio_param param_in = {
		.dir = LO_GPIO_DIR_IN,
		.it = true,
		.pin_num = 16, //(B0)
		.state = false,
	};*/
	
	lo_gpio_param param_out = {
		.dir = LO_GPIO_DIR_OUT,
		.it = false,
		.pin_num = 27, //(B11)
		.state = false,
	};
	
	//lo_gpio_init(&pin_in, &param_in);
	//lo_gpio_set_cb(&pin_in, main_gpio_cb, (void *)0);
	lo_gpio_init(&pin_out, &param_out);
}

static void main_mid_serial_init(void) {
	lo_uart_param param = {
		.baudrate = 115200,
		.periph_sel = 0, //(USART1)
	};
	mid_serial_init(&serial, &param);
	mid_serial_reset_screen(&serial);
	mid_serial_font_set(
			&serial, 
			VT100_FONT_SETTING_BOLD, 
			VT100_FONT_COLOR_GREEN_FRGND, 
			VT100_FONT_COLOR_BLACK_BKGND
	);
	mid_serial_echo_enable(&serial, true);
}

static void main_mid_serial_process(void) {
	mid_serial_echo_process(&serial);
}

static void main_lo_timer_init(void) {
	lo_timer_periph_param param = {
		.frequency = 72000000,
		.mode = LO_TIMER_MODE_COUNT_INCR,
		.periph_sel = 0,
		.resolution = 16,
	};
	
	lo_timer_chan_param chan_param = {
		.cb = main_timer_cb,
		.handle = (void *)0,
		.channel = 0, // update channel
	};
	
	lo_timer_periph_init(&timer, &param);
	lo_timer_init_chan(&timer, &chan_param);
	lo_timer_enable(&timer, true);
}

