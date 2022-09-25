/*!
 * \file obj_ir.h
 * \brief object oriented IR header
 * \author Adam LUCAS
 * \date 07/07/2022
 */

#ifndef OBJ_IR_H_
#define OBJ_IR_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

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

#define OBJ_IR_NEC_FRAME_PERIOD_US		66720
#define OBJ_IR_NEC_AGC_PLS_US			9000
#define OBJ_IR_NEC_PAUSE_US				4500
#define OBJ_IR_NEC_BIT_PLS_US			26
#define OBJ_IR_NEC_STRT_BIT_US			600
#define OBJ_IR_NEC_BIT_0_US				600
#define OBJ_IR_NEC_BIT_1_US				1671
#define OBJ_IR_NEC_PAUSE2_US			40000

#define OBJ_IR_SLAVE_STATE_BUF_SIZE	(128 + 64)
#define OBJ_IR_SLAVE_STATE_BUF_SIZE_BIT	(OBJ_IR_SLAVE_STATE_BUF_SIZE * 8)
#define OBJ_IR_SET_BIT_UINT8_ARR(uint8_array, index) uint8_array[(index) / 8] |= (1<<((index) % 8))
#define OBJ_IR_RESET_BIT_UINT8_ARR(uint8_array, index) uint8_array[(index) / 8] &= ~(1<<((index) % 8))
#define OBJ_IR_RETURN_BIT_UINT8_ARR(uint8_array, index) (uint8_array[(index) / 8] & (1<<((index) % 8)))

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
	OBJ_IR_MODE_NEC,
	OBJ_IR_MODE_RC5,
	OBJ_IR_MODE_RC6,
	//OBJ_IR_MODE_SONY,
}obj_ir_protocol;

typedef enum {
	OBJ_IR_LOGIC_POS,
	OBJ_IR_LOGIC_NEG,
}obj_ir_logic;

typedef enum {
	OBJ_IR_FREQ_30KHZ = 30000,
	OBJ_IR_FREQ_33KHZ = 33000,
	OBJ_IR_FREQ_36KHZ = 36000,
	OBJ_IR_FREQ_38KHZ = 38000,
	OBJ_IR_FREQ_40KHZ = 40000,
	OBJ_IR_FREQ_56KHZ = 56000,
}obj_ir_freq;

typedef enum {
	OBJ_IR_NEC_AGC_PLS,
	OBJ_IR_NEC_PAUSE,
	OBJ_IR_NEC_ADDR,
	OBJ_IR_NEC_NADDR,
	OBJ_IR_NEC_CMD,
	OBj_IR_NEC_NCMD,
}obj_ir_nec;

typedef union {
	uint32_t uint32;
	uint8_t uint8arr[4];	
}obj_ir_data_desc;

typedef void(*obj_ir_timer_enable)(void *handle, bool enable);
typedef void(*obj_ir_gpio_set_state)(void *handle, bool state);

typedef struct {
	obj_ir_protocol protocol;
	
	bool data_ready;
	bool sampling_data;
	bool pin_prev_state;
	
	uint8_t pos_edge_cnt;
	uint32_t max_samples;
	uint32_t timer_resolution_us;
	obj_ir_freq freq;
	obj_ir_logic logic;
	
	uint32_t data;
	uint32_t buf_index;
	uint8_t buf_data[OBJ_IR_SLAVE_STATE_BUF_SIZE];
	
	obj_ir_timer_enable timer_enable;
}obj_ir_slave_ctx;

typedef struct {
	obj_ir_protocol protocol;
	uint32_t timer_resolution_us;
	obj_ir_freq freq;
	obj_ir_logic logic;
	obj_ir_timer_enable timer_enable;
}obj_ir_slave_param;

typedef struct {
	obj_ir_protocol protocol;
	
	bool data_ready;
	
	uint32_t max_samples;
	uint32_t timer_resolution_us;
	obj_ir_freq freq;
	obj_ir_logic logic; //TODO handle input logic
	
	obj_ir_data_desc data;
	uint32_t buf_index;
	uint8_t buf_data[OBJ_IR_SLAVE_STATE_BUF_SIZE];
	
	obj_ir_timer_enable timer_enable;
	obj_ir_gpio_set_state pin_set_state;
}obj_ir_master_ctx;

typedef struct {
	obj_ir_protocol protocol;
	uint32_t timer_resolution_us;
	obj_ir_freq freq;
	obj_ir_logic logic;
	obj_ir_timer_enable timer_enable;
	obj_ir_gpio_set_state pin_set_state;
}obj_ir_master_param;


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void obj_ir_master_init(obj_ir_master_ctx *ctx, obj_ir_master_param *param);
void obj_ir_master_timer_count_cb(obj_ir_master_ctx *ctx);
bool obj_ir_master_data_ready(obj_ir_master_ctx *ctx);
void obj_ir_master_send_data(obj_ir_master_ctx *ctx, uint8_t address, uint8_t data);

void obj_ir_slave_init(obj_ir_slave_ctx *ctx, obj_ir_slave_param *param);
void obj_ir_slave_get_pin_state_cb(obj_ir_slave_ctx *ctx, bool state);
void obj_ir_slave_timer_count_cb(obj_ir_slave_ctx *ctx);
uint32_t obj_ir_slave_get_data(obj_ir_slave_ctx *ctx);
bool obj_ir_slave_data_ready(obj_ir_slave_ctx *ctx);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif /* OBJ_IR_H_ */
