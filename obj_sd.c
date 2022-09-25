/*
 * obj_sd.c
 *
 * Created: 7/18/2022 7:43:19 AM
 *  Author: Adam
 */ 

#include "obj_sd.h"

#if LO_SD_ENABLE_CRC == 1
static uint8_t CRC7Table[256] = {
	0x00, 0x09, 0x12, 0x1B, 0x24, 0x2D, 0x36, 0x3F,
	0x48, 0x41, 0x5A, 0x53, 0x6C, 0x65, 0x7E, 0x77,
	0x19, 0x10, 0x0B, 0x02, 0x3D, 0x34, 0x2F, 0x26,
	0x51, 0x58, 0x43, 0x4A, 0x75, 0x7C, 0x67, 0x6E,
	0x32, 0x3B, 0x20, 0x29, 0x16, 0x1F, 0x04, 0x0D,
	0x7A, 0x73, 0x68, 0x61, 0x5E, 0x57, 0x4C, 0x45,
	0x2B, 0x22, 0x39, 0x30, 0x0F, 0x06, 0x1D, 0x14,
	0x63, 0x6A, 0x71, 0x78, 0x47, 0x4E, 0x55, 0x5C,
	0x64, 0x6D, 0x76, 0x7F, 0x40, 0x49, 0x52, 0x5B,
	0x2C, 0x25, 0x3E, 0x37, 0x08, 0x01, 0x1A, 0x13,
	0x7D, 0x74, 0x6F, 0x66, 0x59, 0x50, 0x4B, 0x42,
	0x35, 0x3C, 0x27, 0x2E, 0x11, 0x18, 0x03, 0x0A,
	0x56, 0x5F, 0x44, 0x4D, 0x72, 0x7B, 0x60, 0x69,
	0x1E, 0x17, 0x0C, 0x05, 0x3A, 0x33, 0x28, 0x21,
	0x4F, 0x46, 0x5D, 0x54, 0x6B, 0x62, 0x79, 0x70,
	0x07, 0x0E, 0x15, 0x1C, 0x23, 0x2A, 0x31, 0x38,
	0x41, 0x48, 0x53, 0x5A, 0x65, 0x6C, 0x77, 0x7E,
	0x09, 0x00, 0x1B, 0x12, 0x2D, 0x24, 0x3F, 0x36,
	0x58, 0x51, 0x4A, 0x43, 0x7C, 0x75, 0x6E, 0x67,
	0x10, 0x19, 0x02, 0x0B, 0x34, 0x3D, 0x26, 0x2F,
	0x73, 0x7A, 0x61, 0x68, 0x57, 0x5E, 0x45, 0x4C,
	0x3B, 0x32, 0x29, 0x20, 0x1F, 0x16, 0x0D, 0x04,
	0x6A, 0x63, 0x78, 0x71, 0x4E, 0x47, 0x5C, 0x55,
	0x22, 0x2B, 0x30, 0x39, 0x06, 0x0F, 0x14, 0x1D,
	0x25, 0x2C, 0x37, 0x3E, 0x01, 0x08, 0x13, 0x1A,
	0x6D, 0x64, 0x7F, 0x76, 0x49, 0x40, 0x5B, 0x52,
	0x3C, 0x35, 0x2E, 0x27, 0x18, 0x11, 0x0A, 0x03,
	0x74, 0x7D, 0x66, 0x6F, 0x50, 0x59, 0x42, 0x4B,
	0x17, 0x1E, 0x05, 0x0C, 0x33, 0x3A, 0x21, 0x28,
	0x5F, 0x56, 0x4D, 0x44, 0x7B, 0x72, 0x69, 0x60,
	0x0E, 0x07, 0x1C, 0x15, 0x2A, 0x23, 0x38, 0x31,
	0x46, 0x4F, 0x54, 0x5D, 0x62, 0x6B, 0x70, 0x79,
};
#endif
uint8_t obj_sd_crc7(uint8_t *data, uint16_t len) {
#if LO_SD_ENABLE_CRC == 1
	uint8_t CRC = 0;
	
	for(uint16_t i = 0; i < len; ++i) {
		CRC = CRC7Table[(CRC << 1) ^ data[i]];
	}
	
	return CRC;
#else
	(void)data;
	(void)len;
	return 0xff;
#endif
}

static inline obj_sd_r1 obj_sd_cmd_r1_send(obj_sd_ctx *ctx, obj_sd_cmd_48b_desc *cmd) {
	uint8_t temp[2];
	obj_sd_cmd_48b_desc cmd_temp = *cmd;
	cmd_temp.desc.cmd |= 0x40;
	cmd_temp.desc.crc7 = (obj_sd_crc7(cmd_temp.uint8arr, 5)<<1) | 1;
	
	ctx->flag = ctx->spi_transmit(ctx->handle, cmd_temp.uint8arr, sizeof(obj_sd_cmd_48b_desc));
	ctx->flag = ctx->spi_receive(ctx->handle, temp, 2);
	
	return temp[1];
}

static inline obj_sd_r1b obj_sd_cmd_r1b_send(obj_sd_ctx *ctx, obj_sd_cmd_48b_desc *cmd) {
	return (obj_sd_r1b)obj_sd_cmd_r1_send(ctx, cmd);
}

static inline obj_sd_r2 obj_sd_cmd_r2_send(obj_sd_ctx *ctx, obj_sd_cmd_48b_desc *cmd) {
	uint8_t temp[3];
	obj_sd_cmd_48b_desc cmd_temp = *cmd;
	cmd_temp.desc.cmd |= 0x40;
	cmd_temp.desc.crc7 = (obj_sd_crc7(cmd_temp.uint8arr, 5)<<1) | 1;
	
	ctx->flag = ctx->spi_transmit(ctx->handle, cmd_temp.uint8arr, sizeof(obj_sd_cmd_48b_desc));
	ctx->flag = ctx->spi_receive(ctx->handle, temp, 3);
	
	return (temp[2]<<8) | temp[1];
}

static inline obj_sd_r3 obj_sd_cmd_r3_send(obj_sd_ctx *ctx, obj_sd_cmd_48b_desc *cmd) {
	obj_sd_r3_desc temp;
	obj_sd_cmd_48b_desc cmd_temp = *cmd;
	cmd_temp.desc.cmd |= 0x40;
	cmd_temp.desc.crc7 = (obj_sd_crc7(cmd_temp.uint8arr, 5)<<1) | 1;
	
	ctx->flag = ctx->spi_transmit(ctx->handle, cmd_temp.uint8arr, sizeof(obj_sd_cmd_48b_desc));
	ctx->flag = ctx->spi_receive(ctx->handle, temp.uint8arr, 6);
	
	return temp.desc;
}

static inline obj_sd_r7 obj_sd_cmd_r7_send(obj_sd_ctx *ctx, obj_sd_cmd_48b_desc *cmd) {
	obj_sd_r7_desc temp;
	obj_sd_cmd_48b_desc cmd_temp = *cmd;
	cmd_temp.desc.cmd |= 0x40;
	cmd_temp.desc.crc7 = (obj_sd_crc7(cmd_temp.uint8arr, 5)<<1) | 1;
	
	ctx->flag = ctx->spi_transmit(ctx->handle, cmd_temp.uint8arr, sizeof(obj_sd_cmd_48b_desc));
	ctx->flag = ctx->spi_receive(ctx->handle, temp.uint8arr, 6);
	
	return temp.desc;
}



static void obj_sd_assert_r1(obj_sd_r1 r1) {
	if(
			r1 & (
				LO_SD_R1_PARAM_ERROR_MASK |
				LO_SD_R1_ADDRESS_ERROR_MASK |
				LO_SD_R1_ERASE_SEQ_ERROR_MASK |
				LO_SD_R1_COM_CRC_ERROR_MASK |
				LO_SD_R1_ILLEGAL_COMMAND_MASK
			)
	) while(1);
}

static bool obj_sd_busy_r1b(obj_sd_r1b r1b) {
	if(r1b & LO_SD_R1B_BUSY_MASK) return true;
	return false;
}

static bool obj_sd_idle_r1(obj_sd_r1 r1) {
	if(r1 & LO_SD_R1_IDLE_STATE_MASK) return true;
	return false;
}

//CMD0
static obj_sd_r1 obj_sd_go_idle_state(obj_sd_ctx *ctx) { 
	
	obj_sd_cmd_48b_desc data = {
		.desc.cmd = LO_SD_CMD0_GO_IDLE_STATE,
	};
	
	return obj_sd_cmd_r1_send(ctx, &data);
}

//CMD8
static obj_sd_r7 obj_sd_send_if_cond(obj_sd_ctx *ctx, obj_sd_r7_voltage vhs, uint8_t check_pattern) {
	obj_sd_r7 temp;
	
	obj_sd_cmd_48b_desc data = {
		.desc.cmd = LO_SD_CMD8_SEND_IF_COND,
		.desc.args = {
			0x00, 0x00, vhs, check_pattern
		},
	};
	
	do temp = obj_sd_cmd_r7_send(ctx, &data);
	while(temp.check_pattern != check_pattern);
	
	if(temp.voltage_accepted == 0x1)
		ctx->voltage_range = OBJ_SD_R7_VOLT_RANGE_2_7_3_6V;
	else if(temp.voltage_accepted == 0x2)
		ctx->voltage_range = OBJ_SD_R7_VOLT_RANGE_RESERVED_LV;
	else if(temp.voltage_accepted == 0x4 || temp.voltage_accepted == 0x8)
		ctx->voltage_range = OBJ_SD_R7_VOLT_RANGE_RESERVED;
	else
		ctx->voltage_range = OBJ_SD_R7_VOLT_RANGE_UNDEF;
	
	return temp;
}

//CMD17
static obj_sd_r1 obj_sd_read_single_block(obj_sd_ctx *ctx, uint32_t address) {
	
	obj_sd_addr_desc temp = {.desc = address};
	
	obj_sd_cmd_48b_desc data = {
		.desc.cmd = LO_SD_CMD17_READ_SINGLE_BLOCK,
		.desc.args = {
			temp.uint8arr[3], 
			temp.uint8arr[2],
			temp.uint8arr[1],
			temp.uint8arr[0]
		},
	};
	
	return obj_sd_cmd_r1_send(ctx, &data);
}

//CMD55
static obj_sd_r1 obj_sd_app_cmd(obj_sd_ctx *ctx, uint16_t RCA) {
	
	obj_sd_cmd_48b_desc data = {
		.desc.cmd = LO_SD_CMD55_APP_CMD,
		.desc.args = {
			(RCA>>8), RCA, 0x00, 0x00
		},
	};
	
	return obj_sd_cmd_r1_send(ctx, &data);
}

//CMD58
static obj_sd_r7 obj_sd_read_extr_multi(
		obj_sd_ctx *ctx, 
		uint8_t MIO, 
		uint8_t FNO, 
		uint8_t BUS, 
		uint16_t ADDR, 
		uint16_t BUC
) {
	obj_sd_cmd_48b_desc data = {
		.desc.cmd = LO_SD_CMD58_READ_OCR,
		.desc.args = {
			(MIO<<7) | (FNO<<2) | (BUS<<1) | (ADDR>>15),
			(ADDR>>7),
			(ADDR<<1) | (BUC>>8),
			BUC,
		}
	};
	
	return obj_sd_cmd_r7_send(ctx, &data);
}

//ACMD41

static obj_sd_r1 obj_sd_send_op_cond(
		obj_sd_ctx *ctx, 
		uint8_t BUSY, 
		uint8_t HCS, 
		uint8_t FB, 
		uint8_t XPC, 
		uint8_t S18R, 
		uint16_t OCR
) {
	obj_sd_cmd_48b_desc data = {
		.desc.cmd = LO_SD_ACMD41_SD_SEND_OP_COND,
		.desc.args = {
			(BUSY<<7) | (HCS<<6) | (FB<<5) | (XPC<<4) | S18R,
			(OCR>>8),
			OCR,
			0x00,
		}
	};
	
	return obj_sd_cmd_r1_send(ctx, &data);
}


void obj_sd_init(obj_sd_ctx *ctx, obj_sd_param *param) {
	ctx->spi_receive = param->spi_receive;
	ctx->spi_transmit = param->spi_transmit;
	ctx->fifo_write = param->fifo_write;
	ctx->handle = param->handle;
	/*
	uint8_t data_rst[] = {
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0x40, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 
		0xff, 0x48, 0x00, 0x00, 0x01, 0xaa, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0xff, 0x77, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
		0xff, 0x69, 0x40, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
		0xff, 0x77, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
		0xff, 0x69, 0x40, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
		0xff, 0x7a, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	};
	ctx->spi_transmit(ctx->handle, data_rst, sizeof(data_rst));*/
	
	uint8_t data_rst[] = {
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
	};
	ctx->spi_transmit(ctx->handle, data_rst, sizeof(data_rst));
	
	obj_sd_assert_r1(obj_sd_go_idle_state(ctx));
	obj_sd_assert_r1(obj_sd_send_if_cond(ctx, OBJ_SD_R7_VOLT_RANGE_2_7_3_6V, 0xaa).r1);
	
	obj_sd_r1 r1;
	
	do {
		obj_sd_assert_r1(obj_sd_app_cmd(ctx, 0));
		r1 = obj_sd_send_op_cond(ctx, 0, 1, 0, 0, 0, 0);
		obj_sd_assert_r1(r1);
	}
	while(obj_sd_idle_r1(r1));
	
	obj_sd_assert_r1(obj_sd_read_extr_multi(ctx, 0, 0, 0, 0, 0).r1);
}

void obj_sd_block_read(obj_sd_ctx *ctx, uint32_t address) {
	uint8_t data = 0xff;
	
	obj_sd_assert_r1(obj_sd_read_single_block(ctx, address));
	
	while(data == 0xff) ctx->flag = ctx->spi_receive(ctx->handle, &data, 1);
	
	for(uint16_t i = 0; i < 512; i++) {
		ctx->flag = ctx->spi_receive(ctx->handle, &data, 1);
		ctx->flag = ctx->fifo_write(ctx->handle, data);
	}
	
	data = 0xff;
	ctx->flag = ctx->spi_transmit(ctx->handle, &data, 1);
	ctx->flag = ctx->spi_transmit(ctx->handle, &data, 1);
}