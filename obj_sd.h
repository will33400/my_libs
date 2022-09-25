/*
 * obj_sd.h
 *
 * Created: 7/18/2022 7:43:05 AM
 *  Author: Adam
 */ 


#ifndef OBJ_SD_H_
#define OBJ_SD_H_

#include <stdint.h>
#include <stdbool.h>

#include "app_serial.h"

#define LO_SD_ENABLE_CRC						1

//CMD list

#define LO_SD_CMD0_GO_IDLE_STATE				0b000000
#define LO_SD_CMD1_SEND_OP_COND					0b000001

#define LO_SD_CMD6_SWITCH_FUNC					0b000110

#define LO_SD_CMD8_SEND_IF_COND					0b001000
#define LO_SD_CMD9_SEND_CSD						0b001001
#define LO_SD_CMD10_SEND_CID					0b001010

#define LO_SD_CMD12_STOP_TRANSMIT				0b001100
#define LO_SD_CMD13_SEND_STATUS					0b001101

#define LO_SD_CMD16_SET_BLOCKLEN				0b010000
#define LO_SD_CMD17_READ_SINGLE_BLOCK			0b010001
#define LO_SD_CMD18_READ_MULTIPLE_BLOCK			0b010010

#define LO_SD_CMD24_WRITE_BLOCK					0b011000
#define LO_SD_CMD25_WRITE_MULTIPLE_BLOCK		0b011001

#define LO_SD_CMD27_PROGRAM_CSD					0b011011
#define LO_SD_CMD28_SET_WRITE_PROT				0b011100
#define LO_SD_CMD29_CLR_WRITE_PROT				0b011101
#define LO_SD_CMD30_SEND_WRITE_PROT				0b011110

#define LO_SD_CMD32_ERASE_WR_BL_K_START_ADDR	0b100000	//set the address of the first write block to be erased
#define LO_SD_CMD33_ERASE_WR_BL_K_END_ADDR		0b100001	//set the address of the last write block to be erased

#define LO_SD_CMD38_ERASE						0b100110

#define LO_SD_CMD42_LOCK_UNLOCK					0b101010
#define LO_SD_CMD55_APP_CMD						0b110111
#define LO_SD_CMD56_GEN_CMD						0b111000

#define LO_SD_CMD58_READ_OCR					0b111010
#define LO_SD_CMD59_CRC_ON_OFF					0b111011

//CMD bit fields

#define LO_SD_CMD0_STUFF_BITS_SHIFT				0
#define LO_SD_CMD0_STUFF_BITS_LEN				32
#define LO_SD_CMD0_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD1_RESERVED_B_SHIFT				0
#define LO_SD_CMD1_RESERVED_B_LEN				32
#define LO_SD_CMD1_RESERVED_B_MASK				\
	(0xFFFFFFFF & ~LO_SD_CMD1_HCS_MASK)
#define LO_SD_CMD1_HCS_SHIFT					30
#define LO_SD_CMD1_HCS_LEN						1
#define LO_SD_CMD1_HCS_MASK						\
	(0x1<<LO_SD_CMD0_HCS_SHIFT)

#define LO_SD_CMD6_MODE_SHIFT					31
#define LO_SD_CMD6_MODE_LEN						1
#define LO_SD_CMD6_MODE_MASK					\
	(0x1<<LO_SD_CMD6_MODE_SHIFT)
#define LO_SD_CMD6_MODE_CHECK_FUNC				~LO_SD_CMD6_MODE_MASK
#define LO_SD_CMD6_MODE_SWITCH_FUNC				LO_SD_CMD6_MODE_MASK
#define LO_SD_CMD6_RESERVED_B_SHIFT				0
#define LO_SD_CMD6_RESERVED_B_LEN				31
#define LO_SD_CMD6_RESERVED_B_MASK				\
	(0xFFFFFFFF & ~LO_SD_CMD6_MODE_MASK)

#define LO_SD_CMD8_RESERVED_B_SHIFT				12
#define LO_SD_CMD8_RESERVED_B_LEN				\
	(32 - LO_SD_CMD8_RESERVED_B_SHIFT)
#define LO_SD_CMD8_RESERVED_B_MASK				\
	(0xFFFFFFFF)
#define LO_SD_CMD8_SUPPLY_VOLT_SHIFT			8
#define LO_SD_CMD8_SUPPLY_VOLT_LEN				4
#define LO_SD_CMD8_SUPPLY_VOLT_MASK				\
	(0x7<<LO_SD_CMD8_SUPPLY_VOLT_SHIFT)
#define LO_SD_CMD8_CHECK_PATTERN_SHIFT			0
#define LO_SD_CMD8_CHECK_PATTERN_LEN			8
#define LO_SD_CMD8_CHECK_PATTERN_MASK			\
	(0xF<<LO_SD_CMD8_CHECK_PATTERN_SHIFT)

#define LO_SD_CMD9_STUFF_BITS_SHIFT				0
#define LO_SD_CMD9_STUFF_BITS_LEN				32
#define LO_SD_CMD9_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD10_STUFF_BITS_SHIFT			0
#define LO_SD_CMD10_STUFF_BITS_LEN				32
#define LO_SD_CMD10_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD12_STUFF_BITS_SHIFT			0
#define LO_SD_CMD12_STUFF_BITS_LEN				32
#define LO_SD_CMD12_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD13_STUFF_BITS_SHIFT			0
#define LO_SD_CMD13_STUFF_BITS_LEN				32
#define LO_SD_CMD13_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD16_BLOCK_LEN_SHIFT				0
#define LO_SD_CMD16_BLOCK_LEN_LEN				32
#define LO_SD_CMD16_BLOCK_LEN_MASK				0xFFFFFFFF

#define LO_SD_CMD17_DATA_ADDR_SHIFT				0
#define LO_SD_CMD17_DATA_ADDR_LEN				32
#define LO_SD_CMD17_DATA_ADDR_MASK				0xFFFFFFFF

#define LO_SD_CMD18_DATA_ADDR_SHIFT				0
#define LO_SD_CMD18_DATA_ADDR_LEN				32
#define LO_SD_CMD18_DATA_ADDR_MASK				0xFFFFFFFF

#define LO_SD_CMD24_DATA_ADDR_SHIFT				0
#define LO_SD_CMD24_DATA_ADDR_LEN				32
#define LO_SD_CMD24_DATA_ADDR_MASK				0xFFFFFFFF

#define LO_SD_CMD25_DATA_ADDR_SHIFT				0
#define LO_SD_CMD25_DATA_ADDR_LEN				32
#define LO_SD_CMD25_DATA_ADDR_MASK				0xFFFFFFFF

#define LO_SD_CMD27_STUFF_BITS_SHIFT			0
#define LO_SD_CMD27_STUFF_BITS_LEN				32
#define LO_SD_CMD27_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD28_DATA_ADDR_SHIFT				0
#define LO_SD_CMD28_DATA_ADDR_LEN				32
#define LO_SD_CMD28_DATA_ADDR_MASK				0xFFFFFFFF

#define LO_SD_CMD29_DATA_ADDR_SHIFT				0
#define LO_SD_CMD29_DATA_ADDR_LEN				32
#define LO_SD_CMD29_DATA_ADDR_MASK				0xFFFFFFFF

#define LO_SD_CMD30_W_PROT_ADDR_SHIFT			0
#define LO_SD_CMD30_W_PROT_ADDR_LEN				32
#define LO_SD_CMD30_W_PROT_ADDR_MASK			0xFFFFFFFF

#define LO_SD_CMD32_DATA_ADDR_SHIFT				0
#define LO_SD_CMD32_DATA_ADDR_LEN				32
#define LO_SD_CMD32_DATA_ADDR_MASK				0xFFFFFFFF

#define LO_SD_CMD33_DATA_ADDR_SHIFT				0
#define LO_SD_CMD33_DATA_ADDR_LEN				32
#define LO_SD_CMD33_DATA_ADDR_MASK				0xFFFFFFFF

#define LO_SD_CMD38_STUFF_BITS_SHIFT			0
#define LO_SD_CMD38_STUFF_BITS_LEN				32
#define LO_SD_CMD38_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD42_RESERVED_B_SHIFT			0
#define LO_SD_CMD42_RESERVED_B_LEN				32
#define LO_SD_CMD42_RESERVED_B_MASK				0xFFFFFFFF

#define LO_SD_CMD55_STUFF_BITS_SHIFT			0
#define LO_SD_CMD55_STUFF_BITS_LEN				32
#define LO_SD_CMD55_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD56_STUFF_BITS_SHIFT			1
#define LO_SD_CMD56_STUFF_BITS_LEN				31
#define LO_SD_CMD56_STUFF_BITS_MASK				\
	(0xFFFFFFFF & ~LO_SD_CMD56_RD_WR_MASK)
#define LO_SD_CMD56_RD_WR_SHIFT					0
#define LO_SD_CMD56_RD_WR_LEN					1
#define LO_SD_CMD56_RD_WR_MASK					\
	(0x1<<LO_SD_CMD56_RD_WR_SHIFT)
#define LO_SD_CMD56_READ						LO_SD_CMD56_RD_WR_MASK
#define LO_SD_CMD56_WRITE						~LO_SD_CMD56_RD_WR_MASK

#define LO_SD_CMD58_STUFF_BITS_SHIFT			0
#define LO_SD_CMD58_STUFF_BITS_LEN				32
#define LO_SD_CMD58_STUFF_BITS_MASK				0xFFFFFFFF

#define LO_SD_CMD59_STUFF_BITS_SHIFT			1
#define LO_SD_CMD59_STUFF_BITS_LEN				31
#define LO_SD_CMD59_STUFF_BITS_MASK				\
	(0xFFFFFFFF & ~LO_SD_CMD55_CRC_OPTION_MASK)
#define LO_SD_CMD59_CRC_OPTION_SHIFT			0
#define LO_SD_CMD55_CRC_OPTION_LEN				1
#define LO_SD_CMD55_CRC_OPTION_MASK				\
	(0x1<<LO_SD_CMD59_CRC_OPTION_SHIFT)
#define LO_SD_CMD55_CRC_ON						LO_SD_CMD55_CRC_OPTION_MASK
#define LO_SD_CMD55_CRC_OFF						~LO_SD_CMD55_CRC_OPTION_MASK

//ACMD list for CMD55 command

#define LO_SD_ACMD13_SD_STATUS					0x0000000D

#define LO_SD_ACMD22_SEND_NUM_WR_BLOCKS			0x00000016
#define LO_SD_ACMD23_SET_WR_BLK_ERASE_COUNT		0x00000017

#define LO_SD_ACMD41_SD_SEND_OP_COND			0x00000029
#define LO_SD_ACMD42_SD_SET_CLR_CART_DETECT		0x0000002A

#define LO_SD_ACMD51_SEND_SCR					0x00000033

//ACMD bit fields

#define LO_SD_ACMD13_STUFF_BITS_SHIFT			0
#define LO_SD_ACMD13_STUFF_BITS_LEN				32
#define LO_SD_ACMD13_STUFF_BITS_MASK			0xFFFFFFFF

#define LO_SD_ACMD22_STUFF_BITS_SHIFT			0
#define LO_SD_ACMD22_STUFF_BITS_LEN				32
#define LO_SD_ACMD22_STUFF_BITS_MASK			0xFFFFFFFF

#define LO_SD_ACMD23_STUFF_BITS_SHIFT			23
#define LO_SD_ACMD23_STUFF_BITS_LEN				\
	(32 - LO_SD_ACMD23_STUFF_BITS_SHIFT)
#define LO_SD_ACMD23_STUFF_BITS_MASK			\
	(0xFFFFFFFF & ~LO_SD_ACMD23_NUMBER_OF_BLOCKS_MASK)
#define LO_SD_ACMD23_NUMBER_OF_BLOCKS_SHIFT		0
#define LO_SD_ACMD23_NUMBER_OF_BLOCKS_LEN		23
#define LO_SD_ACMD23_NUMBER_OF_BLOCKS_MASK		\
	(0x7FFFFF<<LO_SD_ACMD23_NUMBER_OF_BLOCKS_SHIFT)

#define LO_SD_ACMD41_STUFF_BITS_SHIFT			0
#define LO_SD_ACMD41_STUFF_BITS_LEN				32
#define LO_SD_ACMD41_STUFF_BITS_MASK			\
	(0xFFFFFFFF & ~LO_SD_ACMD41_STUFF_BITS_MASK)
#define LO_SD_ACMD42_STUFF_BITS_SHIFT			30
#define LO_SD_ACMD42_STUFF_BITS_LEN				1
#define LO_SD_ACMD42_STUFF_BITS_MASK			\
	(0x1<<LO_SD_ACMD42_STUFF_BITS_SHIFT)

#define LO_SD_ACMD51_STUFF_BITS_SHIFT			0
#define LO_SD_ACMD51_STUFF_BITS_LEN				32
#define LO_SD_ACMD51_STUFF_BITS_MASK			0xFFFFFFFF

//responses

/*
 * R1 response format for
 * CMD0, 1, 6, 9, 10, 16, 17, 18, 24, 25, 27, 30, 32, 33, 42, 55, 56, 59
 * ACMD22, 23, 41, 42, 51
 */

#define LO_SD_R1_LEN							8
#define LO_SD_R1_MASK							0x7F
#define LO_SD_R1_PARAM_ERROR_SHIFT				6
#define LO_SD_R1_PARAM_ERROR_MASK				\
	(1<<LO_SD_R1_PARAM_ERROR_SHIFT)
#define LO_SD_R1_ADDRESS_ERROR_SHIFT			5
#define LO_SD_R1_ADDRESS_ERROR_MASK				\
	(1<<LO_SD_R1_ADDRESS_ERROR_SHIFT)
#define LO_SD_R1_ERASE_SEQ_ERROR_SHIFT			4
#define LO_SD_R1_ERASE_SEQ_ERROR_MASK			\
	(1<<LO_SD_R1_ERASE_SEQ_ERROR_SHIFT)
#define LO_SD_R1_COM_CRC_ERROR_SHIFT			3
#define LO_SD_R1_COM_CRC_ERROR_MASK				\
	(1<<LO_SD_R1_COM_CRC_ERROR_SHIFT)
#define LO_SD_R1_ILLEGAL_COMMAND_SHIFT			2
#define LO_SD_R1_ILLEGAL_COMMAND_MASK			\
	(1<<LO_SD_R1_ILLEGAL_COMMAND_SHIFT)
#define LO_SD_R1_ERASE_RESET_SHIFT				1
#define LO_SD_R1_ERASE_RESET_MASK				\
	(1<<LO_SD_R1_ERASE_RESET_SHIFT)
#define LO_SD_R1_IDLE_STATE_SHIFT				0
#define LO_SD_R1_IDLE_STATE_MASK				\
	(1<<LO_SD_R1_IDLE_STATE_SHIFT)

/*
 * R1b response format for
 * CMD12, 28, 29, 38
 */

#define LO_SD_R1B_LEN							8
#define LO_SD_R1B_MASK							0xFF
#define LO_SD_R1B_BUSY_SHIFT					7
#define LO_SD_R1B_BUSY_MASK						\
	(1<<LO_SD_R1B_BUSY_SHIFT)
#define LO_SD_R1B_PARAM_ERROR_SHIFT				6
#define LO_SD_R1B_PARAM_ERROR_MASK				\
	(1<<LO_SD_R1B_PARAM_ERROR_SHIFT)
#define LO_SD_R1B_ADDRESS_ERROR_SHIFT			5
#define LO_SD_R1B_ADDRESS_ERROR_MASK			\
	(1<<LO_SD_R1B_ADDRESS_ERROR_SHIFT)
#define LO_SD_R1B_ERASE_SEQ_ERROR_SHIFT			4
#define LO_SD_R1B_ERASE_SEQ_ERROR_MASK			\
	(1<<LO_SD_R1B_ERASE_SEQ_ERROR_SHIFT)
#define LO_SD_R1B_COM_CRC_ERROR_SHIFT			3
#define LO_SD_R1B_COM_CRC_ERROR_MASK			\
	(1<<LO_SD_R1B_COM_CRC_ERROR_SHIFT)
#define LO_SD_R1B_ILLEGAL_COMMAND_SHIFT			2
#define LO_SD_R1B_ILLEGAL_COMMAND_MASK			\
	(1<<LO_SD_R1B_ILLEGAL_COMMAND_SHIFT)
#define LO_SD_R1B_ERASE_RESET_SHIFT				1
#define LO_SD_R1B_ERASE_RESET_MASK				\
	(1<<LO_SD_R1B_ERASE_RESET_SHIFT)
#define LO_SD_R1B_IDLE_STATE_SHIFT				0
#define LO_SD_R1B_IDLE_STATE_MASK				\
	(1<<LO_SD_R1B_IDLE_STATE_SHIFT)

/*
 * R2 response format for
 * CMD13
 * ACMD13
 */

#define LO_SD_R2_LEN							16
#define LO_SD_R2_MASK							0x7FFF
#define LO_SD_R2_PARAM_ERROR_SHIFT				14
#define LO_SD_R2_PARAM_ERROR_MASK				\
	(1<<LO_SD_R2_PARAM_ERROR_SHIFT)
#define LO_SD_R2_ADDRESS_ERROR_SHIFT			13
#define LO_SD_R2_ADDRESS_ERROR_MASK				\
	(1<<LO_SD_R2_ADDRESS_ERROR_SHIFT)
#define LO_SD_R2_ERASE_SEQ_ERROR_SHIFT			12
#define LO_SD_R2_ERASE_SEQ_ERROR_MASK			\
	(1<<LO_SD_R2_ERASE_SEQ_ERROR_SHIFT)
#define LO_SD_R2_COM_CRC_ERROR_SHIFT			11
#define LO_SD_R2_COM_CRC_ERROR_MASK				\
	(1<<LO_SD_R2_COM_CRC_ERROR_SHIFT)
#define LO_SD_R2_ILLEGAL_COMMAND_SHIFT			10
#define LO_SD_R2_ILLEGAL_COMMAND_MASK			\
	(1<<LO_SD_R2_ILLEGAL_COMMAND_SHIFT)
#define LO_SD_R2_ERASE_RESET_SHIFT				9
#define LO_SD_R2_ERASE_RESET_MASK				\
	(1<<LO_SD_R2_ERASE_RESET_SHIFT)
#define LO_SD_R2_IDLE_STATE_SHIFT				8
#define LO_SD_R2_IDLE_STATE_MASK				\
	(1<<LO_SD_R2_IDLE_STATE_SHIFT)
#define LO_SD_R2_OUT_OF_RANGE_CSD_OVERW_SHIFT	7
#define LO_SD_R2_OUT_OF_RANGE_CSD_OVERW_MASK	\
	(1<<LO_SD_R2_OUT_OF_RANGE_CSD_OVERW_SHIFT)
#define LO_SD_R2_ERASE_PARAM_SHIFT				6
#define LO_SD_R2_ERASE_PARAM_MASK				\
	(1<<LO_SD_R2_ERASE_PARAM_SHIFT)
#define LO_SD_R2_WP_VIOLATION_SHIFT				5
#define LO_SD_R2_WP_VIOLATION_MASK				\
	(1<<LO_SD_R2_WP_VIOLATION_SHIFT)
#define LO_SD_R2_CARD_CC_FAILED_SHIFT			4
#define LO_SD_R2_CARD_CC_FAILED_MASK			\
	(1<<LO_SD_R2_CARD_CC_FAILED_SHIFT)
#define LO_SD_R2_CC_ERROR_SHIFT					3
#define LO_SD_R2_CC_ERROR_MASK					\
	(1<<LO_SD_R2_CC_ERROR_SHIFT)
#define LO_SD_R2_ERROR_SHIFT					2
#define LO_SD_R2_ERROR_MASK						\
	(1<<LO_SD_R2_ERROR_SHIFT)
#define LO_SD_R2_WP_ERA_SKP_LK_ULK_CMD_F_SHIFT	1
#define LO_SD_R2_WP_ERA_SKP_LK_ULK_CMD_F_MASK	\
	(1<<LO_SD_R2_WP_ERA_SKP_LK_ULK_CMD_F_SHIFT)
#define LO_SD_R2_CARD_LOCKED_SHIFT				0
#define LO_SD_R2_CARD_LOCKED_MASK				\
	(1<<LO_SD_R2_CARD_LOCKED_SHIFT)

/*
 * R3 response format for
 * CMD58
 */

#define LO_SD_R3_LEN							40
#define LO_SD_R3_MASK							0x7FFFFFFFFF
#define LO_SD_R3_PARAM_ERROR_SHIFT				38
#define LO_SD_R3_PARAM_ERROR_MASK				\
	(1<<LO_SD_R3_PARAM_ERROR_SHIFT)
#define LO_SD_R3_ADDRESS_ERROR_SHIFT			37
#define LO_SD_R3_ADDRESS_ERROR_MASK				\
	(1<<LO_SD_R3_ADDRESS_ERROR_SHIFT)
#define LO_SD_R3_ERASE_SEQ_ERROR_SHIFT			36
#define LO_SD_R3_ERASE_SEQ_ERROR_MASK			\
	(1<<LO_SD_R3_ERASE_SEQ_ERROR_SHIFT)
#define LO_SD_R3_COM_CRC_ERROR_SHIFT			35
#define LO_SD_R3_COM_CRC_ERROR_MASK				\
	(1<<LO_SD_R3_COM_CRC_ERROR_SHIFT)
#define LO_SD_R3_ILLEGAL_COMMAND_SHIFT			34
#define LO_SD_R3_ILLEGAL_COMMAND_MASK			\
	(1<<LO_SD_R3_ILLEGAL_COMMAND_SHIFT)
#define LO_SD_R3_ERASE_RESET_SHIFT				33
#define LO_SD_R3_ERASE_RESET_MASK				\
	(1<<LO_SD_R3_ERASE_RESET_SHIFT)
#define LO_SD_R3_IDLE_STATE_SHIFT				32
#define LO_SD_R3_IDLE_STATE_MASK				\
	(1<<LO_SD_R3_IDLE_STATE_SHIFT)
#define LO_SD_R3_OCR_SHIFT						0
#define LO_SD_R3_OCR_MASK						\
	(0xFFFFFFFF<<LO_SD_R3_OCR_SHIFT)

/*
 * R7 response format for
 * CMD8
 */

#define LO_SD_R7_LEN							40
#define LO_SD_R7_MASK							0x7FFFFFFFFF
#define LO_SD_R7_PARAM_ERROR_SHIFT				38
#define LO_SD_R7_PARAM_ERROR_MASK				\
	(1<<LO_SD_R7_PARAM_ERROR_SHIFT)
#define LO_SD_R7_ADDRESS_ERROR_SHIFT			37
#define LO_SD_R7_ADDRESS_ERROR_MASK				\
	(1<<LO_SD_R7_ADDRESS_ERROR_SHIFT)
#define LO_SD_R7_ERASE_SEQ_ERROR_SHIFT			36
#define LO_SD_R7_ERASE_SEQ_ERROR_MASK			\
	(1<<LO_SD_R7_ERASE_SEQ_ERROR_SHIFT)
#define LO_SD_R7_COM_CRC_ERROR_SHIFT			35
#define LO_SD_R7_COM_CRC_ERROR_MASK				\
	(1<<LO_SD_R7_COM_CRC_ERROR_SHIFT)
#define LO_SD_R7_ILLEGAL_COMMAND_SHIFT			34
#define LO_SD_R7_ILLEGAL_COMMAND_MASK			\
	(1<<LO_SD_R7_ILLEGAL_COMMAND_SHIFT)
#define LO_SD_R7_ERASE_RESET_SHIFT				33
#define LO_SD_R7_ERASE_RESET_MASK				\
	(1<<LO_SD_R7_ERASE_RESET_SHIFT)
#define LO_SD_R7_IDLE_STATE_SHIFT				32
#define LO_SD_R7_IDLE_STATE_MASK				\
	(1<<LO_SD_R7_IDLE_STATE_SHIFT)
#define LO_SD_R7_COMMAND_VERSION_SHIFT			28
#define LO_SD_R7_COMMAND_VERSION_LEN			4
#define LO_SD_R7_COMMAND_VERSION_MASK			\
	(0xF<<LO_SD_R7_COMMAND_VERSION_SHIFT)
#define LO_SD_R7_RESERVED_SHIFT					12
#define LO_SD_R7_RESERVED_LEN					16
#define LO_SD_R7_RESERVED_MASK					\
	(0xFFFF<<LO_SD_R7_COMMAND_VERSION_SHIFT)
#define LO_SD_R7_VOLTAGE_ACCEPTED_SHIFT			8
#define LO_SD_R7_VOLTAGE_ACCEPTED_LEN			4
#define LO_SD_R7_VOLTAGE_ACCEPTED_MASK			\
	(0xF<<LO_SD_R7_VOLTAGE_ACCEPTED_SHIFT)
#define LO_SD_R7_CHECK_PATTERN_SHIFT			0
#define LO_SD_R7_CHECK_APTTERN_LEN				8
#define LO_SD_R7_CHECK_PATTERN_MASK			\
	(0xFF<<LO_SD_R7_CHECK_PATTERN_SHIFT)

/*
 * Response token
 */

#define LO_SD_RESP_TOKEN_LEN					8
#define LO_SD_RESP_TOKEN_DEFAULT				0b11100001
#define LO_SD_RESP_TOKEN_STATUS_SHIFT			1
#define LO_SD_RESP_TOKEN_STATUS_MASK			\
	(0x7<<LO_SD_RESP_TOKEN_STATUS_SHIFT)
#define LO_SD_RESP_TOKEN_STATUS_DATA_ACCEPTED	\
	(0b010<<LO_SD_RESP_TOKEN_STATUS_SHIFT)
#define LO_SD_RESP_TOKEN_STATUS_DATA_REJ_CRC_ERR \
	(0b101<<LO_SD_RESP_TOKEN_STATUS_SHIFT)
#define LO_SD_RESP_TOKEN_STATUS_DATA_REJ_WR_ERR \
	(0b110<<LO_SD_RESP_TOKEN_STATUS_SHIFT)

/*
 * Start / stop write block token
 */

#define LO_SD_SB_TOKEN_LEN						8
#define LO_SD_SB_TOKEN_ONE_BLOCK				0b11111110 //for start
#define LO_SD_SB_TOKEN_MULTIPLE_BLOCK			0b11111100 //for start
#define LO_SD_PB_TOKEN_MULTIPLE_BLOCK			0b11111101 //for stop

/*
 * Data error token
 */

#define LO_SD_DE_TOKEN_LEN						8
#define LO_SD_DE_TOKEN_DEFAULT					0b00000000
#define LO_SD_DE_TOKEN_OUT_OF_RANGE_SHIFT		3
#define LO_SD_SB_TOKEN_OUT_OF_RANGE_MASK		\
	(1<<LO_SD_DE_TOKEN_OUT_OF_RANGE_SHIFT)
#define LO_SD_DE_TOKEN_CARD_ECC_FAILED_SHIFT	2
#define LO_SD_DE_TOKEN_CARD_ECC_FAILED_MASK		\
	(1<<LO_SD_DE_TOKEN_CARD_ECC_FAILED_SHIFT)
#define LO_SD_DE_TOKEN_CC_ERROR_SHIFT			1
#define LO_SD_DE_TOKEN_CC_ERROR_MASK			\
	(1<<LO_SD_DE_TOKEN_CC_ERROR_SHIFT)
#define LO_SD_DE_TOKEN_ERROR_SHIFT				0
#define LO_SD_DE_TOKEN_ERROR_MASK				\
	(1<<LO_SD_DE_TOKEN_ERROR_SHIFT)

//card command configs

#define LO_SD_COMMAND_R1_CONDITION(cmd)		( 								\
		cmd == LO_SD_CMD0_GO_IDLE_STATE || 									\
		cmd == LO_SD_CMD1_SEND_OP_COND || 									\
		cmd == LO_SD_CMD6_SWITCH_FUNC || 									\
		cmd == LO_SD_CMD9_SEND_CSD || 										\
		cmd == LO_SD_CMD10_SEND_CID || 										\
		cmd == LO_SD_CMD16_SET_BLOCKLEN || 									\
		cmd == LO_SD_CMD17_READ_SINGLE_BLOCK || 							\
		cmd == LO_SD_CMD18_READ_MULTIPLE_BLOCK || 							\
		cmd == LO_SD_CMD24_WRITE_BLOCK || 									\
		cmd == LO_SD_CMD25_WRITE_MULTIPLE_BLOCK || 							\
		cmd == LO_SD_CMD27_PROGRAM_CSD || 									\
		cmd == LO_SD_CMD30_SEND_WRITE_PROT || 								\
		cmd == LO_SD_CMD32_ERASE_WR_BL_K_START_ADDR || 						\
		cmd == LO_SD_CMD33_ERASE_WR_BL_K_END_ADDR || 						\
		cmd == LO_SD_CMD42_LOCK_UNLOCK || 									\
		cmd == LO_SD_CMD55_APP_CMD || 										\
		cmd == LO_SD_CMD56_GEN_CMD || 										\
		cmd == LO_SD_CMD59_CRC_ON_OFF 										\
)

#define LO_SD_R1B_CONDITION(cmd)		(									\
		cmd == LO_SD_CMD12_STOP_TRANSMIT || 								\
		cmd == LO_SD_CMD28_SET_WRITE_PROT || 								\
		cmd == LO_SD_CMD29_CLR_WRITE_PROT	 								\
)

#define LO_SD_R2_CONDITION(cmd)		(										\
		cmd == LO_SD_CMD13_SEND_STATUS										\
)

#define LO_SD_R3_CONDITION(cmd)		(										\
		cmd == LO_SD_CMD58_READ_OCR											\
)

#define LO_SD_R7_CONDITION(cmd)		(										\
		cmd == LO_SD_CMD8_SEND_IF_COND										\
)

#define LO_SD_CMD_START_B_SHIFT			47
#define LO_SD_CMD_START_B_LEN			1
#define LO_SD_CMD_START_B_MASK			\
	~(1<<LO_SD_CMD_START_B_SHIFT)
#define LO_SD_CMD_TRANSMIT_B_SHIFT		46
#define LO_SD_CMD_TRANSMIT_B_LEN		1
#define LO_SD_CMD_TRANSMIT_B_MASK		\
	(1<<LO_SD_CMD_TRANSMIT_B_SHIFT)
#define LO_SD_CMD_CMD_INDEX_SHIFT		40
#define LO_SD_CMD_CMD_INDEX_LEN			6
#define LO_SD_CMD_CMD_INDEX_MASK		\
	(0b111111<<LO_SD_CMD_CMD_INDEX_SHIFT)
#define LO_SD_CMD_ARGUMENT_SHIFT		8
#define LO_SD_CMD_ARGUMENT_LEN			32
#define LO_SD_CMD_ARGUMENT_MASK			\
	(0xFFFFFFFF<<LO_SD_CMD_ARGUMENT_SHIFT)
#define LO_SD_CMD_CRC7_SHIFT			1
#define LO_SD_CMD_CRC7_LEN				1
#define LO_SD_CMD_CRC7_MASK				\
	(0b1111111<<LO_SD_CMD_CRC7_SHIFT)
#define LO_SD_CMD_END_B_SHIFT			0
#define LO_SD_CMD_END_B_LEN				1
#define LO_SD_CMD_END_B_MASK			\
	(1<<LO_SD_CMD_END_B_SHIFT)

#define LO_SD_CMD(cmd, argument) {												\
		cmd | (1<<LO_SD_CMD_CMD_INDEX_LEN),										\
		(uint8_t)(argument>>24)& 0xFF, 											\
		(uint8_t)(argument>>16)& 0xFF, 											\
		(uint8_t)(argument>>8)& 0xFF, 											\
		(uint8_t)(argument)& 0xFF, 												\
		(uint8_t)LO_SD_CMD_END_B_MASK |											\
			(uint8_t)~LO_SD_CMD_CRC7_MASK	 									\
}

#define SD_SPI_SET_CRC7(cmd, ctx_ptr)											\
		cmd[5] |= (																\
				ctx_ptr->crc7_get(ctx_ptr->handle, cmd, sizeof(cmd))<< 			\
				LO_SD_CMD_CRC7_SHIFT											\
		);

typedef enum {
	OBJ_SD_R7_VOLT_RANGE_UNDEF,
	OBJ_SD_R7_VOLT_RANGE_2_7_3_6V,
	OBJ_SD_R7_VOLT_RANGE_RESERVED_LV,
	OBJ_SD_R7_VOLT_RANGE_RESERVED,
}obj_sd_r7_voltage;

typedef enum {
	OBJ_SD_OK,
	OBJ_SD_ERROR,
}obj_sd_flag;

typedef struct {
	uint8_t cmd;
	uint8_t args[4];
	uint8_t crc7;
}obj_sd_cmd_48_bits;

typedef union {
	obj_sd_cmd_48_bits desc;
	uint8_t uint8arr[sizeof(obj_sd_cmd_48_bits)];
}obj_sd_cmd_48b_desc;

typedef uint8_t obj_sd_r1;
typedef uint8_t obj_sd_r1b;
typedef uint16_t obj_sd_r2;

typedef struct {
	uint8_t reserved;
	obj_sd_r1 r1;
	uint8_t ocr[4];
}obj_sd_r3;

typedef union {
	obj_sd_r3 desc;
	uint8_t uint8arr[6];
}obj_sd_r3_desc;

typedef struct {
	uint8_t reserved;
	obj_sd_r1 r1;
	uint8_t command_version;
	uint8_t reserved1;
	uint8_t voltage_accepted;
	uint8_t check_pattern;
}obj_sd_r7;

typedef union {
	obj_sd_r7 desc;
	uint8_t uint8arr[6];
}obj_sd_r7_desc;

typedef union {
	uint32_t desc;
	uint8_t uint8arr[4];
}obj_sd_addr_desc;

typedef obj_sd_flag(*obj_sd_spi_transmit)(void *handle, uint8_t *data, uint16_t len);
typedef obj_sd_flag(*obj_sd_spi_receive)(void *handle, uint8_t *data, uint16_t len);
typedef obj_sd_flag(*obj_sd_fifo_write)(void *handle, uint8_t data);

typedef struct {
	uint8_t buf[128];
	uint8_t buf_idx;
	obj_sd_spi_transmit spi_transmit;
	obj_sd_spi_receive spi_receive;
	obj_sd_fifo_write fifo_write;
	void *handle;
	obj_sd_flag flag;
	
	obj_sd_r7_voltage voltage_range;
}obj_sd_ctx;

typedef struct {
	obj_sd_spi_transmit spi_transmit;
	obj_sd_spi_receive spi_receive;
	obj_sd_fifo_write fifo_write;
	void *handle;
}obj_sd_param;

void obj_sd_init(obj_sd_ctx *ctx, obj_sd_param *param);
void obj_sd_block_read(obj_sd_ctx *ctx, uint32_t address);






#endif /* OBJ_SD_H_ */