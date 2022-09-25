/*!
 * \file obj_ssd1675a.h
 * \brief ssd1675a device header
 * \author Adam LUCAS
 * \date 06/12/2021
 */

#ifndef OBJ_SSD1675A_H_
#define OBJ_SSD1675A_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdio.h>
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

/*! RAM data convert macro */

#define OBJ_SSD1675A_RAM_DATA_CONV(data)                (0xFF ^ data)

/*! device registers + bit mapping */

#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL                   0x01
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A0_MASK           0xFF
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A1_MASK           0x01     
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_MASK            0x07
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_16_PX           0x0010
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_296_PX          0x0127
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_FRML(pix)               \
        (                                                       \
            pix > OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_296_PX ?      \
            OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_296_PX :            \
            (                                                   \
                pix < OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_16_PX ?   \
                OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_16_PX :         \
                pix                                             \
            )                                                   \
        )
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_DEFAULT         (OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_296_PX)
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_GD              2
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_GD_LIL_ENDIAN   (0<<OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_GD)
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_GD_BIG_ENDIAN   (1<<OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_GD)
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_SM              1
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_SM_NORMAL       (0<<OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_SM)
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_SM_INTERLACED   (1<<OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_SM)
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_TB              0
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_TB_0_TO_295     (0<<OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_TB)
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_TB_295_TO_0     (1<<OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_TB)
#define OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_DEFAULT             \
        (                                                   \
            OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_GD_LIL_ENDIAN | \
            OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_SM_NORMAL |     \
            OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_TB_295_TO_0     \
        )

#define OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL              0x03
#define OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_MASK       0x1F
#define OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_VGH_10V    0x03
#define OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_VGH_20V    0x17
#define OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_VGH_DEFAULT 0x19
#define OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_CONV1(vlt) ((uint8_t)(vlt * 2)- 12)/ 2)
#define OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_FRML(vlt)              \
        (                                                           \
            OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_CONV1(vlt) >       \
            OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_VGH_20V            \
        ) ? OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_VGH_20V : (        \
            (                                                       \
                OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_CONV1(vlt) <   \
                OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_VGH_10V        \
            ) ? OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_VGH_10V :      \
            OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_CONV1(vlt)         \
        )

#define OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL               0x04
#define OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_A_MASK        0xFF
#define OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_A_VSH1_P15V   0x41    //default
#define OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_B_MASK        0xFF
#define OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_B_VSH2_P5V    0xA8    //default
#define OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_C_MASK        0xFF
#define OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_C_VSL_N15V    0x32    //default
//see documentation for more details

#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL            0x0C
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_A_MASK     0xFF
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_A_PHASE1   0x8B    //default
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_B_MASK     0xFF
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_B_PHASE2   0x9C    //default
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_C_MASK     0xFF
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_C_PHASE3   0x96    //default
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_MASK     0x7F
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTN_STT 0x0F    //default
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_STGTH    0x07
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_STGTH_SHFT 4
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_STGTH_MASK       \
        (                                                       \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_STGTH<<      \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_STGTH_SHFT   \
        )
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_WEAK     (0<<OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_STGTH_SHFT)
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_STRNG    (7<<OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_STGTH_SHFT)
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR  0x07
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR_SHFT 0
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR_MASK     \
        (                                                       \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR<<    \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR_SHFT \
        )
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR_MIN  (3<<OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR_SHFT)
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR_MAX  (15<<OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_X_MOFTGDR_SHFT)
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH    2
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH3_SHFT 4
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH2_SHFT 2
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH1_SHFT 0
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH3_MASK       \
        (                                                           \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH<<       \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH3_SHFT   \
        )
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH2_MASK       \
        (                                                           \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH<<       \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH2_SHFT   \
        )
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH1_MASK       \
        (                                                           \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH<<       \
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH1_SHFT   \
        )
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH_MIN(shift) (0<<shift)
#define OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH_MAX(shift) (3<<shift)

#define OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS              0x0F
#define OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS_A0_MASK      0xFF
#define OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS_A1_MASK      0x01
#define OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS_A_MIN        0x0000
#define OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS_A_MAX        0x0127
#define OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS_A_DEFAULT    OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS_A_MIN

#define OBJ_SSD1675A_CMD_DEEP_SLP_MODE                  0x10
#define OBJ_SSD1675A_CMD_DEEP_SLP_MODE_A_MASK           0x03
#define OBJ_SSD1675A_CMD_DEEP_SLP_MODE_A_NORMAL         0x00
#define OBJ_SSD1675A_CMD_DEEP_SLP_MODE_A_MODE1          0x01
#define OBJ_SSD1675A_CMD_DEEP_SLP_MODE_A_MODE2          0x03
#define OBJ_SSD1675A_CMD_DEEP_SLP_MODE_A_DEFAULT        OBJ_SSD1675A_CMD_DEEP_SLP_MODE_A_NORMAL

#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG            0x11
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_MASK     0x07
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_AM_SHFT  2
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_AM_UPDX  (0<<OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_AM_SHFT)
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_AM_UPDY  (1<<OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_AM_SHFT)
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_SHFT  0
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_YDXD  (0<<OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_SHFT)
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_YDXI  (1<<OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_SHFT)
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_YIXD  (2<<OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_SHFT)
#define OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_YIXI  (3<<OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_SHFT)

#define OBJ_SSD1675A_CMD_SW_RESET                       0x12

#define OBJ_SSD1675A_CMD_HV_RDY_DTCT                    0x14

#define OBJ_SSD1675A_CMD_VCI_DTCT                       0x15
#define OBJ_SSD1675A_CMD_VCI_DTCT_A_MASK                0x07
#define OBJ_SSD1675A_CMD_VCI_DTCT_A_MIN                 0x03
#define OBJ_SSD1675A_CMD_VCI_DTCT_A_MAX                 0x07
#define OBJ_SSD1675A_CMD_VCI_DTCT_A_CONV1(vlt)          ((uint8_t)(vlt*10)-19)
#define OBJ_SSD1675A_CMD_VCI_DTCT_A_FRML(vlt)           \
        (                                               \
            OBJ_SSD1675A_CMD_VCI_DTCT_A_CONV1(vlt) >    \
            OBJ_SSD1675A_CMD_VCI_DTCT_A_MAX             \
        ) ? OBJ_SSD1675A_CMD_VCI_DTCT_A_MAX : (         \
            (                                           \
                OBJ_SSD1675A_CMD_VCI_DTCT_A_CONV1(vlt) <\
                OBJ_SSD1675A_CMD_VCI_DTCT_A_MIN         \
            ) ? OBJ_SSD1675A_CMD_VCI_DTCT_A_MIN :       \
            OBJ_SSD1675A_CMD_VCI_DTCT_A_CONV1(vlt)      \
        )

#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL                  0x18
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_A_MASK           0xFF
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_A_EXT_SNSR       0x48
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_A_INT_SNSR       0x80

#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W                0x1A
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_A0_MASK        0xFF
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_A1_MASK        0xF0
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_A_DEFAULT      0x70FF

#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_R                0x1B
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_R_A0_MASK        0xFF
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_R_A1_MASK        0xF0

#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT            0x1C
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_MASK     0xFF
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_DEFAULT  0x00
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM     2
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_SHFT 6
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_MASK        \
        (                                                       \
            OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM<<       \
            OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_SHFT    \
        )
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_AP  (0<<OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_SHFT)
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_AP1P (1<<OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_SHFT)
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_AP1P2P (2<<OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_SHFT)
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_ADDR (3<<OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_BNUM_SHFT)
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_A_PTR_STNG 0x1F
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_B_MASK     0xFF
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_B_DEFAULT  0x00
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_B_PARAM1   OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_B_MASK
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_C_MASK     0xFF
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_C_DEFAULT  0x00
#define OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_C_PARAM2   OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_EXT_B_MASK

#define OBJ_SSD1675A_CMD_MSTR_ACTIVATE                  0x20

#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1              0x21
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_MASK       0xFF
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_DEFAULT    0x00
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_REDRAM_SHFT 4
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_BWRAM_SHFT 0
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_OPTN_NORMAL(shift) (0<<shift)
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_OPTN_BPC0(shift) (4<<shift)
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_OPTN_IPC(shift) (8<<shift)

#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_2              0x22
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_2_A_MASK       0xFF
#define OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_2_A_DEFAULT    OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_2_A_MASK
//see documentation for more details

#define OBJ_SSD1675A_CMD_WRT_RAM_BW                     0x24

#define OBJ_SSD1675A_CMD_WRT_RAM_RED                    0x26

#define OBJ_SSD1675A_CMD_RD_RAM                         0x27

#define OBJ_SSD1675A_CMD_VCOM_SENSE                     0x28

#define OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN                0x29
#define OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_MASK         0xFF
#define OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_MODE_SHFT    6
#define OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_MODE_NORMAL  (1<<OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_MODE_SHFT)
#define OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_MODE_REVERSE (0<<OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_MODE_SHFT)
#define OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_DRTN         0x09 // defaut : 10s
#define OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_DRTN_FRML(sec) ((sec + 1)& 0xF)

#define OBJ_SSD1675A_CMD_PRG_VCOM_OTP                   0x2A

#define OBJ_SSD1675A_CMD_WRT_VCOM_REG                   0x2C
#define OBJ_SSD1675A_CMD_WRT_VCOM_REG_A_MASK            0xFF
#define OBJ_SSD1675A_CMD_WRT_VCOM_REG_A_DEFAULT         0x00

#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN           0x2D
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_A_MASK    0xFF
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_A_OTP_SEL OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_A_MASK
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_B_MASK    0xFF
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_B_VCOM_REG OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_B_MASK
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_C_MASK    0xFF
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_C_DSPLY_MODE OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_C_MASK
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_D_MASK    0xFF
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_D_DSPLY_MODE OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_D_MASK
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_E_MASK    0xFF
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_E_DSPLY_MODE OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_E_MASK
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_F_MASK    0xFF
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_F_DSPLY_MODE OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_F_MASK
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_G_MASK    0xFF
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_G_WVFRM_VER OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_G_MASK
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_H_MASK    0xFF
#define OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_H_WVFRM_VER OBJ_SSD1675A_CMD_OTP_REG_RD_DPLY_OPTN_H_MASK

#define OBJ_SSD1675A_CMD_USER_ID_RD                     0x2E
#define OBJ_SSD1675A_CMD_USER_ID_RD_A_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_B_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_C_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_D_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_E_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_F_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_G_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_H_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_I_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_J_MASK              0xFF
#define OBJ_SSD1675A_CMD_USER_ID_RD_X_USER_ID           0xFF

#define OBJ_SSD1675A_CMD_STATUS_BIT_RD                  0x2F
#define OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_MASK           0x33
#define OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_HV_DTCT_F_SHFT 5
#define OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_VCI_DTCT_F_SHFT 4
#define OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_CHIP_ID         0
#define OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_HV_DTCT_RDY   (0<<OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_HV_DTCT_F_SHFT)
#define OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_HV_DTCT_NRDY  (1<<OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_HV_DTCT_F_SHFT)
#define OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_VCI_DTCT_NORMAL (0<<OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_VCI_DTCT_F_SHFT)
#define OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_VCI_DTCT_BUSY (1<<OBJ_SSD1675A_CMD_STATUS_BIT_RD_A_VCI_DTCT_F_SHFT)

#define OBJ_SSD1675A_CMD_PRG_WS_STOP                    0x30

#define OBJ_SSD1675A_CMD_LD_WS_STOP                     0x31

#define OBJ_SSD1675A_CMD_WRT_LUT_REG                    0x32
#define OBJ_SSD1675A_CMD_WRT_LUT_REG_0_TO_69_MASK       0xFF

#define OBJ_SSD1675A_CMD_WRT_LUT_MACRO_1(_vs_lut0, _vs_lut1, _vs_lut2_3, _vs_lut4, _tp, _rp)\
        (_vs_lut0 ),\
        (_vs_lut0 ),\
        (_vs_lut0 ),\
        (_vs_lut0 ),\
        (_vs_lut0 ),\
        (_vs_lut0 ),\
        (_vs_lut0 ),\
        (_vs_lut1 ),\
        (_vs_lut1 ),\
        (_vs_lut1 ),\
        (_vs_lut1 ),\
        (_vs_lut1 ),\
        (_vs_lut1 ),\
        (_vs_lut1 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut2_3 ),\
        (_vs_lut4 ),\
        (_vs_lut4 ),\
        (_vs_lut4 ),\
        (_vs_lut4 ),\
        (_vs_lut4 ),\
        (_vs_lut4 ),\
        (_vs_lut4 ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_rp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_rp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_rp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_rp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_rp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_rp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_tp ),\
        (_rp )

#define OBJ_SSD1675A_CMD_CRC_CALC                       0x34

#define OBJ_SSD1675A_CMD_CRC_STATUS_RD                  0x35
#define OBJ_SSD1675A_CMD_CRC_STATUS_RD_A0_MASK          0xFF
#define OBJ_SSD1675A_CMD_CRC_STATUS_RD_A1_MASK          0xFF

#define OBJ_SSD1675A_CMD_PRG_OTP_SEL                    0x36

#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP         0x37
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_A_MASK  0x80
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_A_SPRVOS OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_A_MASK
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_B_MASK  0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_B_MODWS0 OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_B_MASK
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_C_MASK  0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_C_MODWS1 OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_C_MASK
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_D_MASK  0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_D_MODWS2 OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_D_MASK
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_E_MASK  0xC1
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_E_DMWS_SHFT 0
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_E_PPMDM2_SHFT 6
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_E_PPMDM1_SHFT 7
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_F_MASK  0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_F_MIDWFV1 OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_F_MASK
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_G_MASK  0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_G_MIDWFV1 OBJ_SSD1675A_CMD_WRT_REG_DSPLY_OPTN_OTP_D_MASK

#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID                0x38
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_A_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_B_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_C_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_D_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_E_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_F_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_G_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_H_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_I_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_J_MASK         0xFF
#define OBJ_SSD1675A_CMD_WRT_REG_USER_ID_J_USER_ID      0xFF

#define OBJ_SSD1675A_CMD_OTP_PRG_MODE                   0x39
#define OBJ_SSD1675A_CMD_OTP_PRG_MODE_A_MASK            0x03
#define OBJ_SSD1675A_CMD_OTP_PRG_MODE_A_SHFT            0
#define OBJ_SSD1675A_CMD_OTP_PRG_MODE_A_NORMAL          (0<<OBJ_SSD1675A_CMD_OTP_PRG_MODE_A_SHFT)
#define OBJ_SSD1675A_CMD_OTP_PRG_MODE_A_INTERNAL        (3<<OBJ_SSD1675A_CMD_OTP_PRG_MODE_A_SHFT)
#define OBJ_SSD1675A_CMD_OTP_PRG_MODE_A_DEFAULT         OBJ_SSD1675A_CMD_OTP_PRG_MODE_A_NORMAL

#define OBJ_SSD1675A_CMD_SET_DMMY_LINE_PRD              0x3A
#define OBJ_SSD1675A_CMD_SET_DMMY_LINE_PRD_A_MASK       0x7F
#define OBJ_SSD1675A_CMD_SET_DMMY_LINE_PRD_A_DEFAULT    0x30
//see documentation for more details

#define OBJ_SSD1675A_CMD_SET_GATE_LN_WDTH               0x3B
#define OBJ_SSD1675A_CMD_SET_GATE_LN_WDTH_A_MASK        0x0F
#define OBJ_SSD1675A_CMD_SET_GATE_LN_WDTH_A_DEFAULT     0x0A
//see documentation for more details

#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL              0x3C
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_MASK       0xF3
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_VBD_OPTN_SHFT 6
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_GSTR       (0<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_VBD_OPTN_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_FXLVL      (1<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_VBD_OPTN_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_VCOM       (2<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_VBD_OPTN_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_HIZ        (3<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_VBD_OPTN_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_STNG_SHFT 4
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_VSS    (0<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_STNG_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_VSH1   (1<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_STNG_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_VSL    (2<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_STNG_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_VSH2   (3<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_STNG_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_STNG_SHFT  0
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_LUT0   (0<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_STNG_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_LUT1   (1<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_STNG_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_LUT2   (2<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_STNG_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_LUT3   (3<<OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_STNG_SHFT)
#define OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_DEFAULT    (   \
        OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_FXLVL |         \
        OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_VSL |      \
        OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_LUT1       \
)

#define OBJ_SSD1675A_CMD_RD_RAM_OPTN                    0x41
#define OBJ_SSD1675A_CMD_RD_RAM_OPTN_A_MASK             0x01
#define OBJ_SSD1675A_CMD_RD_RAM_OPTN_A_SHFT             0
#define OBJ_SSD1675A_CMD_RD_RAM_OPTN_A_RDRAM24H         (0<<OBJ_SSD1675A_CMD_RD_RAM_OPTN_A_MASK)
#define OBJ_SSD1675A_CMD_RD_RAM_OPTN_A_RDRAM26H         (1<<OBJ_SSD1675A_CMD_RD_RAM_OPTN_A_MASK)

#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN        0x44
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_A_MASK 0x3F
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_A_MIN  0x00
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_A_MAX  0x13
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_A_DEFAULT OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_A_MIN
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_B_MASK 0x3F
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_B_MIN  0x00
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_B_MAX  0x13
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_B_DEFAULT OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_B_MAX

#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN        0x45
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_A0_MASK 0xFF
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_A1_MASK 0x01
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_A_MIN   0x0000
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_A_MAX   0x0127
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_A_DEFAULT OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_A_MIN
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_B0_MASK 0xFF
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_B1_MASK 0x01
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_B_MIN   0x0000
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_B_MAX   0x0127
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_B_DEFAULT OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN_B_MAX

#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN     0x46
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_MASK 0xF7
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_FSV_SHFT 7
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_FSV_DEFAULT (0<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_FSV_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_SHFT 4
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH8 (0<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH16 (1<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH32 (2<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH64 (3<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH128 (4<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH256 (5<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH296 (6<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH_DEFAULT OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_YDRH8
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH_SHFT 0
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH8 (0<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH16 (1<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH32 (2<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH64 (3<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH128 (4<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH160 (5<<OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH_DEFAULT OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_XDRH8

#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN      0x47
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_MASK 0xF7
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_FSV_SHFT 7
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_FSV_DEFAULT (0<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_FSV_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_SHFT 4
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH8 (0<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH16 (1<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH32 (2<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH64 (3<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH128 (4<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH256 (5<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH296 (6<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH_DEFAULT OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_YDRH8
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH_SHFT 0
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH8 (0<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH16 (1<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH32 (2<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH64 (3<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH128 (4<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH160 (5<<OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH_SHFT)
#define OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH_DEFAULT OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_XDRH8

#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_CNTR            0x4E
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_CNTR_A_MASK     0x3F
#define OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_CNTR_A_DEFAULT  0x00

#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_CNTR            0x4F
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_CNTR_A0_MASK    0xFF
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_CNTR_A1_MASK    0x01
#define OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_CNTR_A_DEFAULT  0x0000

#define OBJ_SSD1675A_CMD_SET_ANA_BLCK_CTRL              0x74
#define OBJ_SSD1675A_CMD_SET_ANA_BLCK_CTRL_A_MASK       0xFF
#define OBJ_SSD1675A_CMD_SET_ANA_BLCK_CTRL_A_DEFAULT    0x54

#define OBJ_SSD1675A_CMD_SET_DGTL_BLCK_CTRL             0x7E
#define OBJ_SSD1675A_CMD_SET_DGTL_BLCK_CTRL_A_MASK      0xFF
#define OBJ_SSD1675A_CMD_SET_DGTL_BLCK_CTRL_A_DEFAULT   0x3B

#define OBJ_SSD1675A_CMD_NOP                            0x7F

/*! ssd1675a_ctx struct init default macro */

#define OBJ_SSD1675A_INIT_DEFAULT(x_dim, y_dim, spi_w, spi_r, busy_get, nres_ss, d_nc_ss, delay_us_, handle_) \
        {                                           \
            .interface = SSD1675A_SPI_4WIRE,        \
            .ram_sel = SSD1675A_BW_RAM,             \
            .vcom_sensing = SSD1675A_VCOM_NORMAL,   \
            .vci_detection = SSD1675A_VCI_NORMAL,   \
            .hv_ready = SSD1675A_READY,             \
            .init_seq = SSD1675A_NORMAL_DISPLAY,    \
            .x_dimension = x_dim,                   \
            .y_dimension = y_dim,                   \
            .drv_out_contr = SSD1675A_Y_INC_X_INC,  \
            .spi_write = spi_w,                     \
            .spi_read = spi_r,                      \
            .busy_get_state = busy_get,             \
            .nres_set_state = nres_ss,              \
            .d_nc_set_state = d_nc_ss,              \
            .delay_us = delay_us_,                  \
            .handle = handle_                       \
        }

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum ssd1675a_refresh_freq
 * \brief 0x3a + 0x3b commands frequency setting
 */

typedef enum {
    SSD1675A_REFRESH_15HZ = 0x0E7D,
    SSD1675A_REFRESH_20HZ = 0x0E14,
    SSD1675A_REFRESH_25HZ = 0x0D2A,
    SSD1675A_REFRESH_30HZ = 0x0C52,
    SSD1675A_REFRESH_35HZ = 0x0C01,
    SSD1675A_REFRESH_40HZ = 0x0B47,
    SSD1675A_REFRESH_45HZ = 0x0B1E,
    SSD1675A_REFRESH_50HZ = 0x0A30,
    SSD1675A_REFRESH_55HZ = 0x0A11,
    SSD1675A_REFRESH_60HZ = 0x0925,
    SSD1675A_REFRESH_65HZ = 0x090B,
    SSD1675A_REFRESH_70HZ = 0x082C,
    SSD1675A_REFRESH_75HZ = 0x0815,
    SSD1675A_REFRESH_80HZ = 0x0801,
    SSD1675A_REFRESH_85HZ = 0x071E,
    SSD1675A_REFRESH_90HZ = 0x070C,
    SSD1675A_REFRESH_95HZ = 0x0636,
    SSD1675A_REFRESH_100HZ = 0x0625,
    SSD1675A_REFRESH_105HZ = 0x0615,
    SSD1675A_REFRESH_110HZ = 0x0607,
    SSD1675A_REFRESH_115HZ = 0x0526,
    SSD1675A_REFRESH_120HZ = 0x0518,
    SSD1675A_REFRESH_125HZ = 0x050B,
    SSD1675A_REFRESH_135HZ = 0x0428,
    SSD1675A_REFRESH_140HZ = 0x041C,
    SSD1675A_REFRESH_145HZ = 0x0411,
    SSD1675A_REFRESH_150HZ = 0x0407,
    SSD1675A_REFRESH_155HZ = 0x032B,
    SSD1675A_REFRESH_160HZ = 0x0320,
    SSD1675A_REFRESH_165HZ = 0x0316,
    SSD1675A_REFRESH_170HZ = 0x030D,
    SSD1675A_REFRESH_175HZ = 0x0304,
    SSD1675A_REFRESH_180HZ = 0x0233,
    SSD1675A_REFRESH_185HZ = 0x0229,
    SSD1675A_REFRESH_190HZ = 0x0220,
    SSD1675A_REFRESH_195HZ = 0x0218,
    SSD1675A_REFRESH_200HZ = 0x0210,
}ssd1675a_refresh_freq;

/*!
 * \enum ssd1675a_cmd_3c_vs
 * \brief 0x32 command voltages settings
 */

typedef enum {
    SSD1675A_VS_VSS = 0x00,        //!< 0V
    SSD1675A_VS_VSH1 = 0x55,       //!< +15V
    SSD1675A_VS_VSL = 0xAA,        //!< -15V
    SSD1675A_VS_VSH2 = 0xFF,       //!< +5V
}ssd1675a_cmd_3c_vs;

/*!
 * \enum ssd1675a_mcu_interface
 * \brief device interface selection
 */

typedef enum {
    SSD1675A_SPI_4WIRE,
    SSD1675A_SPI_3WIRE,
}ssd1675a_mcu_interface;

/*!
 * \enum ssd1675a_ram_sel
 * \brief device ram color selection (black,white/red)
 */

typedef enum {
    SSD1675A_BW_RAM,
    SSD1675A_RED_RAM,
}ssd1675a_ram_sel;

/*!
 * \enum ssd1675a_vcom_sensing
 * \brief device VCOM pin sensing mode selection
 */

typedef enum {
    SSD1675A_VCOM_NORMAL,
    SSD1675A_VCOM_REVERSED,
}ssd1675a_vcom_sensing;

/*!
 * \enum ssd1675a_vci_detection
 * \brief device VCI pin detection mode selection
 */

typedef enum {
    SSD1675A_VCI_NORMAL,
    SSD1675A_VCI_INFERIOR,
}ssd1675a_vci_detection;

/*!
 * \enum ssd1675a_hv_ready_detection
 * \brief device HV pin detection mode selection
 */

typedef enum {
    SSD1675A_HV_IDLE,
    SSD1675A_READY,
}ssd1675a_hv_ready_detection;

/*!
 * \enum ssd1675a_init_operating_seq
 * \brief device init operating sequence
 */

typedef enum {
    SSD1675A_NORMAL_DISPLAY,
    SSD1675A_VCOM_OTP_PROGRAM,
    SSD1675A_WS_OTP_PROGRAM,
}ssd1675a_init_operating_seq;

/*!
 * \enum ssd1675a_state
 * \brief device function return flag
 */

typedef enum {
    SSD1675A_SUCCESS,
    SSD1675A_ERROR,
}ssd1675a_state;

/*!
 * \enum ssd1675a_x_dimension
 * \brief device width selection (in pixels)
 */

typedef enum {
    SSD1675A_X_8PX = 8,
    SSD1675A_X_16PX = 16,
    SSD1675A_X_32PX = 32,
    SSD1675A_X_64PX = 64,
    SSD1675A_X_128PX = 128,
    SSD1675A_X_160PX = 160,
}ssd1675a_x_dimension;

/*!
 * \enum ssd1675a_y_dimension
 * \brief device height selection (in pixels)
 */

typedef enum {
    SSD1675A_Y_8PX = 8,
    SSD1675A_Y_16PX = 16,
    SSD1675A_Y_32PX = 32,
    SSD1675A_Y_64PX = 64,
    SSD1675A_Y_128PX = 128,
    SSD1675A_Y_256PX = 256,
    SSD1675A_Y_296PX = 296,
}ssd1675a_y_dimension;

/*!
 * \enum ssd1675a_driver_output_control
 * \brief device output display direction
 */

typedef enum {
    SSD1675A_Y_DEC_X_DEC = OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_YDXD,
    SSD1675A_Y_DEC_X_INC = OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_YDXI,
    SSD1675A_Y_INC_X_DEC = OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_YIXD,
    SSD1675A_Y_INC_X_INC = OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_ID_YIXI,
}ssd1675a_driver_output_control;

/*!
 * \enum ssd1675a_frame_type
 * \brief device frame type
 */

typedef enum {
    SSD1675A_COMMAND = false,
    SSD1675A_DATA = true,
}ssd1675a_frame_type;


typedef void(*obj_ssd1675a_delay_us)(void *handle, uint32_t val);


/*!
 * \fn typedef void(*obj_ssd1675a_spi_write)(void *handle, uint8_t *data, uint16_t len);
 * \brief user defined spi protocol write routine
 * \param[in] handle user defined argument
 * \param[in] data protocol data to send pointer
 * \param[in] len data length
 * \return error flag
 */

typedef ssd1675a_state(*obj_ssd1675a_spi_write)(void *handle, uint8_t *data, uint16_t len);

/*!
 * \fn typedef void(*obj_ssd1675a_spi_read)(void *handle, uint8_t *data, uint16_t len);
 * \brief user defined spi protocol read routine
 * \param[in] handle user defined argument
 * \param[out] data protocol data to read pointer
 * \param[in] len data length
 * \return error flag
 */

typedef ssd1675a_state(*obj_ssd1675a_spi_read)(void *handle, uint8_t *data, uint16_t len);

/*!
 * \fn typedef void(*obj_ssd1675a_gpio_set_state)(void *handle, bool state);
 * \brief user defined gpio pin set state routine
 * \param[in] handle user defined argument
 * \param[in] state pin state (false : LOW, true: HIGH)
 * \return None
 */

typedef void(*obj_ssd1675a_gpio_set_state)(void *handle, bool state);

typedef bool(*obj_ssd1675a_gpio_get_state)(void *handle);

/*!
 * \struct ssd1675a_data_format
 * \brief internal screen specific data structure to handle d_nc pin + spi function pointers
 */

typedef struct {
    uint8_t data;
    ssd1675a_frame_type type;
}ssd1675a_data_format;

/*!
 * \struct ssd1675a_ctx
 * \brief object oriented device data. Use OBJ_SSD1675A_INIT_DEFAULT() macro to setup dafault parameters
 */

typedef struct {
    ssd1675a_mcu_interface interface;           //!< Refer to ssd1675a_mcu_interface typedef
    ssd1675a_ram_sel ram_sel;                   //!< Refer to ssd1675a_ram_sel typedef
    ssd1675a_vcom_sensing vcom_sensing;         //!< Refer to ssd1675a_vcom_sensing typedef
    ssd1675a_vci_detection vci_detection;       //!< Refer to ssd1675a_vci_detection typedef
    ssd1675a_hv_ready_detection hv_ready;       //!< Refer to ssd1675a_hv_ready_detection typedef
    ssd1675a_init_operating_seq init_seq;       //!< Refer to ssd1675a_init_operating_seq typedef
    ssd1675a_x_dimension x_dimension;           //!< Refer to ssd1675a_x_dimension typedef
    ssd1675a_y_dimension y_dimension;           //!< Refer to ssd1675a_y_dimension typedef
    ssd1675a_driver_output_control drv_out_contr;//!< Refer to ssd1675a_driver_output_control typedef
    ssd1675a_state flag;

    obj_ssd1675a_spi_write spi_write;           //!< spi write function pointer
    obj_ssd1675a_spi_read spi_read;             //!< spi read function pointer
    obj_ssd1675a_gpio_get_state busy_get_state; //!< busy pin get state function pointer
    obj_ssd1675a_gpio_set_state nres_set_state; //!< hardware reset pin set state function pointer
    obj_ssd1675a_gpio_set_state d_nc_set_state; //!< data_not_command pin set state function pointer
    obj_ssd1675a_delay_us delay_us;             //!< blocking delay function pointer
    void *handle;                               //!< user defined argument for all object function pointers
}ssd1675a_ctx;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

ssd1675a_state obj_ssd1675a_reset(ssd1675a_ctx *ctx);
ssd1675a_state obj_ssd1675a_init(ssd1675a_ctx *ctx);
ssd1675a_state obj_ssd1675a_set_display_bw_ram(
        ssd1675a_ctx *ctx, 
        uint16_t x_offset, 
        uint16_t y_offset, 
        uint8_t *data,
        uint16_t data_len
);
ssd1675a_state obj_ssd1675a_set_display_red_ram(
        ssd1675a_ctx *ctx, 
        uint16_t x_offset, 
        uint16_t y_offset,
        uint8_t *data, 
        uint16_t data_len
);
ssd1675a_state obj_ssd1675a_set_display_refresh(ssd1675a_ctx *ctx);

void obj_ssd1675a_cmd_driver_out_control(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_gate_driver_voltage_control(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_source_driver_voltage_control(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_boost_soft_start_control(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_gate_scan_start_position(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_deep_sleep_mode(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_data_entry_mode_setting(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_software_reset(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_hardware_ready_detection(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_VCI_detection(ssd1675a_ctx *ctx, double vlt);
void obj_ssd1675a_cmd_temperature_sensor_control(ssd1675a_ctx *ctx, bool internal);
void obj_ssd1675a_cmd_temperature_sensor_control_write(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_temperature_sensor_control_read(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_master_activate(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_display_update_control_1(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_display_update_control_2(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_write_ram_black_white(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_write_ram_red(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_read_ram(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_VCOM_sense(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_VCOM_sense_duration(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_program_VCOM_into_OTP(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_write_VCOM_register(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_write_LUT_register(
        ssd1675a_ctx *ctx,
        ssd1675a_cmd_3c_vs black_vs, 
        ssd1675a_cmd_3c_vs white_vs, 
        ssd1675a_cmd_3c_vs red_vs, 
        ssd1675a_cmd_3c_vs vcom_vs, 
        uint8_t tp, 
        uint8_t rp
);
void obj_ssd1675a_cmd_set_dummy_line_period(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_set_gate_line_width(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_border_waveform_control(ssd1675a_ctx *ctx, bool black);
void obj_ssd1675a_cmd_set_RAM_x_address_start_end_position(
        ssd1675a_ctx *ctx,
        uint16_t x_offset,
        uint16_t x_len
);
void obj_ssd1675a_cmd_set_RAM_y_address_start_end_position(
        ssd1675a_ctx *ctx,
        uint16_t y_offset,
        uint16_t y_len
);
void obj_ssd1675a_cmd_auto_write_red_ram(
        ssd1675a_ctx *ctx, 
        uint8_t height, 
        uint8_t width
);
void obj_ssd1675a_cmd_auto_write_bw_ram(
        ssd1675a_ctx *ctx, 
        uint8_t height, 
        uint8_t width
);
void obj_ssd1675a_cmd_set_RAM_x_address_control(ssd1675a_ctx *ctx, uint8_t x_pos);
void obj_ssd1675a_cmd_set_RAM_y_address_control(ssd1675a_ctx *ctx, uint8_t y_pos);
void obj_ssd1675a_cmd_set_analog_block_control(ssd1675a_ctx *ctx);
void obj_ssd1675a_cmd_set_digital_block_control(ssd1675a_ctx *ctx);


/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* OBJ_SSD1675A_H_ */

/************ THIS SECTION IS FOR DOCUMENTATION ONLY !**********************//**
* \addtogroup OBJ_SSD1675A
* \brief object oriented driver for SSD1675A devices
* \{

\details
\li \ref obj_ssd1675a_intro
\li \ref obj_ssd1675a_api_description
\li \ref obj_ssd1675a_data_types
\li \ref obj_ssd1675a_macros
\li \ref obj_ssd1675a_routines
\li \ref obj_ssd1675a_example

# Introduction {#obj_ssd1675a_intro}

This document presents how to use obj_ssd1675a.h API to manage SSD1675A hardware drivers used for EPaper Displays
obj_ssd1675a is an object oriented layer that uses SSD167A device internal registers and pins

# API_Description {#obj_ssd1675a_api_description}

This library contains :

-# reset()
-# init()
-# set_display_bw_ram()
-# set_display_red_ram()
-# set_display_refresh()
-# ...

these routines are described further in this document

# Data_Types {#obj_ssd1675a_data_types}

enums :

-# ssd1675a_refresh_freq
-# ssd1675a_cmd_3c_vs
-# ssd1675a_mcu_interface
-# ssd1675a_ram_sel
-# ssd1675a_vcom_sensing
-# ssd1675a_vci_detection
-# ssd1675a_hv_ready_detection
-# ssd1675a_init_operating_seq
-# ssd1675a_state
-# ssd1675a_x_dimension
-# ssd1675a_y_dimension
-# ssd1675a_driver_output_control
-# ssd1675a_frame_type

function pointer :

-# obj_ssd1675a_delay_us
-# obj_ssd1675a_spi_write
-# obj_ssd1675a_spi_read
-# obj_ssd1675a_gpio_set_state
-# obj_ssd1675a_gpio_get_state

structures :

-# ssd1675a_data_format
-# ssd1675a_ctx

# Macros {#obj_ssd1675a_macros}

RAM data convert macro :

-# OBJ_SSD1675A_RAM_DATA_CONV() convenient macro to write in right logic data RAM to device

device registers + bit mapping macros :

-# OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_FRML()
-# OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_CONV1()
-# OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_FRML()
-# OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH_MIN()
-# OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTNSTPH_MAX()
-# OBJ_SSD1675A_CMD_VCI_DTCT_A_CONV1()
-# OBJ_SSD1675A_CMD_VCI_DTCT_A_FRML()
-# OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_OPTN_NORMAL()
-# OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_OPTN_BPC0()
-# OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_OPTN_IPC()
-# OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_DRTN_FRML()
-# OBJ_SSD1675A_CMD_WRT_LUT_MACRO_1()

ssd1675a_ctx struct init default macro :

-# OBJ_SSD1675A_INIT_DEFAULT() 

set xy dimensions of screen considering ssd1675a_x_dimension and ssd1675a_x_dimension, 
set user defined functions pointers to handle SPI protocol specifications, busy reset data/ncommand GPIO pins and blocking delay

# Routines {#obj_ssd1675a_routines}

configuration routines :

-# obj_ssd1675a_reset()
-# obj_ssd1675a_init()

insert display data to RAM :

-# obj_ssd1675a_set_display_bw_ram()
-# obj_ssd1675a_set_display_red_ram()

refresh display :

-# obj_ssd1675a_set_display_refresh()

direct device internal register setting routine :

-# obj_ssd1675a_cmd_driver_out_control()
-# obj_ssd1675a_cmd_gate_driver_voltage_control()
-# obj_ssd1675a_cmd_source_driver_voltage_control()
-# obj_ssd1675a_cmd_boost_soft_start_control()
-# obj_ssd1675a_cmd_gate_scan_start_position()
-# obj_ssd1675a_cmd_deep_sleep_mode()
-# obj_ssd1675a_cmd_data_entry_mode_setting()
-# obj_ssd1675a_cmd_software_reset()
-# obj_ssd1675a_cmd_hardware_ready_detection()
-# obj_ssd1675a_cmd_VCI_detection()
-# obj_ssd1675a_cmd_temperature_sensor_control()
-# obj_ssd1675a_cmd_temperature_sensor_control_write()
-# obj_ssd1675a_cmd_temperature_sensor_control_read()
-# obj_ssd1675a_cmd_master_activate()
-# obj_ssd1675a_cmd_display_update_control_1()
-# obj_ssd1675a_cmd_display_update_control_2()
-# obj_ssd1675a_cmd_write_ram_black_white()
-# obj_ssd1675a_cmd_write_ram_red()
-# obj_ssd1675a_cmd_read_ram()
-# obj_ssd1675a_cmd_VCOM_sense()
-# obj_ssd1675a_cmd_VCOM_sense_duration()
-# obj_ssd1675a_cmd_program_VCOM_into_OTP()
-# obj_ssd1675a_cmd_write_VCOM_register()
-# obj_ssd1675a_cmd_write_LUT_register()
-# obj_ssd1675a_cmd_set_dummy_line_period()
-# obj_ssd1675a_cmd_set_gate_line_width()
-# obj_ssd1675a_cmd_border_waveform_control()
-# obj_ssd1675a_cmd_set_RAM_x_address_start_end_position()
-# obj_ssd1675a_cmd_set_RAM_y_address_start_end_position()
-# obj_ssd1675a_cmd_auto_write_red_ram()
-# obj_ssd1675a_cmd_auto_write_bw_ram()
-# obj_ssd1675a_cmd_set_RAM_x_address_control()
-# obj_ssd1675a_cmd_set_RAM_y_address_control()
-# obj_ssd1675a_cmd_set_analog_block_control()
-# obj_ssd1675a_cmd_set_digital_block_control()

# Example {#obj_ssd1675a_example}

\code

#include "lo_gpio.h"
#include "lo_spi.h"
#include "obj_ssd1675a.h"

#define DISPLAY_RAM_SIZE (128 * 296 / 8) //byte

uint8_t screen_data[DISPLAY_RAM_SIZE];

static ssd1675a_state spi_write(
        void *handle, 
        uint8_t *data, 
        uint16_t len
) {
    (void)handle;
    //implement SPI protocol transmit code
    return SSD1675A_SUCCESS;
}

static ssd1675a_state spi_read(
        void *handle, 
        uint8_t *data, 
        uint16_t len
) {
    (void)handle;
    //implement SPI protocol receive code
    return SSD1675A_SUCCESS;
}

static bool busy_get(void *handle) {
    (void)handle;
    return false;
    //implement GPIO pin get state code instead of returning only "false" state
}

static void nres_set_state(void *handle, bool state) {
    (void)handle;
    //implement GPIO pin set state code
}

static void d_nc_set_state(void *handle, bool state) {
    (void)handle;
    //implement GPIO pin set state code
}

static void delay_us(void *handle, uint32_t val) {
    (void)handle;
    
    //implement us blocking delay code
}

static void display_reset(ssd1675a_ctx *ctx) {
    
    for(uint16_t i = 0; i < DISPLAY_RAM_SIZE; i++) {
        screen_data[i] = 0x00;
    }

    obj_ssd1675a_set_display_bw_ram(
            ctx, 
            0, 
            0, 
            screen_data, 
            DISPLAY_RAM_SIZE
    );
    obj_ssd1675a_set_display_refresh(ctx);

    while(lo_epd_busy_get(NULL)); //wait BUSY state from display to return low
}

static ssd1675a_ctx ctx = OBJ_SSD1675A_INIT_DEFAULT(
        128, 
        296, 
        spi_write,
        spi_read,
        busy_get,
        nres_set_state,
        d_nc_set_state,
        delay_us,
        NULL                    //if required implement data pointer accessed through handle of all function pointers
);

int main(void) {
    
    //implement low level peripheral init code

    obj_ssd1675a_reset(&ctx);
    obj_ssd1675a_init(&ctx);

    display_reset(&ctx);

    while(1) {

        //write display data into screen_data in pixels (ASCII characters not handled)
    
        obj_ssd1675a_set_display_bw_ram(
                &ctx, 
                0, 
                0, 
                screen_data, 
                DISPLAY_RAM_SIZE
        );
        obj_ssd1675a_set_display_refresh(&ctx);
        while(lo_epd_busy_get(NULL));
    }

}

\endcode

* \} end group obj_ssd1675a ****************************************************/
