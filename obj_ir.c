/*!
 * \file obj_ir.c
 * \brief object oriented IR functions
 * \author Adam LUCAS
 * \date 07/07/2022
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_ir.h"

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
* \fn <function declaration>
* \brief <description>
* \param[in] <argument name> <description>
* \param[out] <argument name> <description>
* \return [<anything>] [None]
*/

void obj_ir_master_init(obj_ir_master_ctx *ctx, obj_ir_master_param *param) {
	ctx->freq = param->freq;
	ctx->logic = param->logic;
	ctx->protocol = param->protocol;
	ctx->timer_resolution_us = param->timer_resolution_us;
	ctx->timer_enable = param->timer_enable;
	ctx->pin_set_state = param->pin_set_state;
	
	ctx->buf_index = 0;
	ctx->data.uint32 = 0;
	ctx->data_ready = true;
	
	switch(ctx->protocol) {
		case OBJ_IR_MODE_NEC:
			ctx->max_samples = (OBJ_IR_NEC_FRAME_PERIOD_US / ctx->timer_resolution_us) / 2;
		break;
		case OBJ_IR_MODE_RC5:
		break;
		case OBJ_IR_MODE_RC6:
		break;
	}
}

void obj_ir_master_timer_count_cb(obj_ir_master_ctx *ctx) {
	if(ctx->data_ready) return;
	if(ctx->buf_index == ctx->max_samples) {
		ctx->buf_index = 0;
		ctx->data_ready = true;
		ctx->timer_enable((void *)0, false);
		return;
	}
	ctx->pin_set_state(
			(void *)0, 
			OBJ_IR_RETURN_BIT_UINT8_ARR(ctx->buf_data, ctx->buf_index) ? true : false
	);
	ctx->buf_index++;
}

bool obj_ir_master_data_ready(obj_ir_master_ctx *ctx) {
	if(ctx->data_ready) {
		ctx->data_ready = false;
		return true;
	}
	return false;
}

void obj_ir_master_send_data(obj_ir_master_ctx *ctx, uint8_t address, uint8_t data) {
	uint32_t bit_idx = 0;
	obj_ir_data_desc data_temp = {
		.uint8arr = {
			address,
			~address,
			data,
			~data,
		}
	};
	uint32_t cond_temp = OBJ_IR_NEC_AGC_PLS_US / ctx->timer_resolution_us;
	for(;bit_idx < cond_temp; bit_idx++) OBJ_IR_SET_BIT_UINT8_ARR(ctx->buf_data, bit_idx);
	cond_temp += (OBJ_IR_NEC_PAUSE_US / ctx->timer_resolution_us);
	for(;bit_idx < cond_temp; bit_idx++) OBJ_IR_RESET_BIT_UINT8_ARR(ctx->buf_data, bit_idx);
	for(uint32_t data_bit = 0; data_bit < 32; data_bit++) {
		cond_temp += (OBJ_IR_NEC_STRT_BIT_US / ctx->timer_resolution_us);
		for(;bit_idx < cond_temp; bit_idx++) OBJ_IR_SET_BIT_UINT8_ARR(ctx->buf_data, bit_idx);
		if(OBJ_IR_RETURN_BIT_UINT8_ARR(data_temp.uint8arr, data_bit)) 
			cond_temp += (OBJ_IR_NEC_BIT_1_US / ctx->timer_resolution_us);
		else
			cond_temp += (OBJ_IR_NEC_BIT_0_US / ctx->timer_resolution_us);
		for(;bit_idx < cond_temp; bit_idx++) OBJ_IR_RESET_BIT_UINT8_ARR(ctx->buf_data, bit_idx);
	}
	
	cond_temp += (OBJ_IR_NEC_STRT_BIT_US / ctx->timer_resolution_us);
	for(;bit_idx < cond_temp; bit_idx++) OBJ_IR_SET_BIT_UINT8_ARR(ctx->buf_data, bit_idx);
	cond_temp += (OBJ_IR_NEC_PAUSE2_US / ctx->timer_resolution_us);
	for(;bit_idx < cond_temp; bit_idx++) OBJ_IR_RESET_BIT_UINT8_ARR(ctx->buf_data, bit_idx);
	
	ctx->max_samples = bit_idx + 1;
	ctx->buf_index = 0;
	ctx->data_ready = false;
	
	ctx->timer_enable((void *)0, true);
}

void obj_ir_slave_init(obj_ir_slave_ctx *ctx, obj_ir_slave_param *param) {
	ctx->freq = param->freq;
	ctx->logic = param->logic;
	ctx->protocol = param->protocol;
	ctx->timer_resolution_us = param->timer_resolution_us;
	ctx->timer_enable = param->timer_enable;
	
	for(uint16_t i = 0; i < 128; i++) ctx->buf_data[i] = 0xff;
	
	ctx->buf_index = 0;
	ctx->data = 0;
	ctx->pos_edge_cnt = 0;
	ctx->data_ready = true;
	ctx->sampling_data = false;
	ctx->pin_prev_state = false;
	
	switch(ctx->protocol) {
		case OBJ_IR_MODE_NEC:
			ctx->max_samples = (
					OBJ_IR_NEC_FRAME_PERIOD_US / ctx->timer_resolution_us +
					(OBJ_IR_NEC_FRAME_PERIOD_US / ctx->timer_resolution_us / 10)
			) / 2;
		break;
		case OBJ_IR_MODE_RC5:
		break;
		case OBJ_IR_MODE_RC6:
		break;
	}
}

void obj_ir_slave_get_pin_state_cb(obj_ir_slave_ctx *ctx, bool state) {
	switch(ctx->protocol) {
		case OBJ_IR_MODE_NEC:
			ctx->pin_prev_state = state;
			if(state) ctx->pos_edge_cnt++;
			if(ctx->sampling_data) return;
			ctx->sampling_data = true;
			ctx->timer_enable((void *)0, true);
		break;
		case OBJ_IR_MODE_RC5:
		break;
		case OBJ_IR_MODE_RC6:
		break;
	}
}

void obj_ir_slave_timer_count_cb(obj_ir_slave_ctx *ctx) {
	switch(ctx->protocol) {
		case OBJ_IR_MODE_NEC:
			if(!ctx->sampling_data) return;
			if(ctx->buf_index == ctx->max_samples || ctx->pos_edge_cnt == 34) {
				ctx->max_samples = ctx->buf_index;
				ctx->pos_edge_cnt = 0;
				ctx->buf_index = 0;
				ctx->sampling_data = false;
				ctx->data_ready = true;
				ctx->timer_enable((void *)0, false);
				return;
			}
			if(ctx->pin_prev_state) OBJ_IR_SET_BIT_UINT8_ARR(ctx->buf_data, ctx->buf_index);
			else OBJ_IR_RESET_BIT_UINT8_ARR(ctx->buf_data, ctx->buf_index);
			ctx->buf_index++;
		break;
		case OBJ_IR_MODE_RC5:
		break;
		case OBJ_IR_MODE_RC6:
		break;
	}
}

uint32_t obj_ir_slave_get_data(obj_ir_slave_ctx *ctx) {
	
	obj_ir_data_desc data = {.uint32 = 0};
	uint32_t bit_idx = 0;
	uint8_t idx = 0;
	
	switch(ctx->protocol) {
		case OBJ_IR_MODE_NEC:
		
			while(!OBJ_IR_RETURN_BIT_UINT8_ARR(ctx->buf_data, bit_idx)) bit_idx++;
			while(OBJ_IR_RETURN_BIT_UINT8_ARR(ctx->buf_data, bit_idx)) bit_idx++;
			for(; idx < 32; idx++) {
				while(
						!OBJ_IR_RETURN_BIT_UINT8_ARR(ctx->buf_data, bit_idx) && 
						bit_idx < ctx->max_samples
				) bit_idx++;
				bit_idx += 8;
				if(OBJ_IR_RETURN_BIT_UINT8_ARR(ctx->buf_data, bit_idx)) {
					OBJ_IR_SET_BIT_UINT8_ARR(data.uint8arr, idx);
				}
				else OBJ_IR_RESET_BIT_UINT8_ARR(data.uint8arr, idx);
				while(
						OBJ_IR_RETURN_BIT_UINT8_ARR(ctx->buf_data, bit_idx) &&
						bit_idx < ctx->max_samples
				) bit_idx++;
			}
	
			ctx->max_samples = (
					OBJ_IR_NEC_FRAME_PERIOD_US / ctx->timer_resolution_us + 
					(OBJ_IR_NEC_FRAME_PERIOD_US / ctx->timer_resolution_us / 10)
			) / 2;
	
		break;
		case OBJ_IR_MODE_RC5:
		break;
		case OBJ_IR_MODE_RC6:
		break;
	}
	ctx->data = data.uint32;
	return ctx->data;
}

bool obj_ir_slave_data_ready(obj_ir_slave_ctx *ctx) {
	if(ctx->data_ready) {
		ctx->data_ready = false;
		return true;
	}
	return false;
}
