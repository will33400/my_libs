/*!
 * \file obj_ssd1675a.c
 * \brief ssd1675a device functions
 * \author Adam LUCAS
 * \date 06/12/2021
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_ssd1675a.h"

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

static void obj_ssd1675a_send(ssd1675a_ctx *ctx, uint8_t address, uint8_t *data, uint16_t len) {

    if(address != 0) {
        ctx->d_nc_set_state(ctx->handle, false);
        ctx->flag = ctx->spi_write(ctx->handle, &address, 1);
    }
    if(ctx->interface == SSD1675A_SPI_4WIRE) {
        ctx->d_nc_set_state(ctx->handle, true);
    }
    
    ctx->flag = ctx->spi_write(ctx->handle, data, len);
}

static void obj_ssd1675a_send_ram_data(ssd1675a_ctx *ctx, uint8_t *data, uint16_t len) {

    if(ctx->interface == SSD1675A_SPI_4WIRE) {
        ctx->d_nc_set_state(ctx->handle, true);
    }
    
    for(uint16_t i = 0; i < len; i++) {
        uint8_t temp = data[i] ^ 0xff;
        ctx->flag = ctx->spi_write(ctx->handle, &temp, 1);
    }
}

//0x01
void obj_ssd1675a_cmd_driver_out_control(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            (uint8_t)OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_FRML(ctx->y_dimension),//A0
            (uint8_t)(OBJ_SSD1675A_CMD_DRV_OUT_CTRL_A_FRML(ctx->y_dimension)>>8),//A1
            OBJ_SSD1675A_CMD_DRV_OUT_CTRL_B_DEFAULT,
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_DRV_OUT_CTRL, data, sizeof(data));
}

//0x03
void obj_ssd1675a_cmd_gate_driver_voltage_control(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL_A_VGH_DEFAULT,//A
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_GATE_DRV_VLT_CTRL, data, sizeof(data));
}

//0x04
void obj_ssd1675a_cmd_source_driver_voltage_control(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_A_VSH1_P15V,//A
            OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_B_VSH2_P5V,//B
            OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL_C_VSL_N15V,//C
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SRC_DRV_VLT_CTRL, data, sizeof(data));
}

//0x0C
void obj_ssd1675a_cmd_boost_soft_start_control(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_A_PHASE1,//A
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_B_PHASE2,//B
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_C_PHASE3,//C
            OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL_D_DRTN_STT,//D
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_BOOST_SFT_STRT_CTRL, data, sizeof(data));
}

//0x0F
void obj_ssd1675a_cmd_gate_scan_start_position(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            0x27,//(uint8_t)OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS_A_DEFAULT,
            0x01//(uint8_t)(OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS_A_DEFAULT>>8)
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_GATE_SCN_STRT_POS, data, sizeof(data));
}

//0x10
void obj_ssd1675a_cmd_deep_sleep_mode(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_DEEP_SLP_MODE_A_DEFAULT,//A
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_DEEP_SLP_MODE, data, sizeof(data));
}

//0x11
void obj_ssd1675a_cmd_data_entry_mode_setting(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG_A_AM_UPDX | ctx->drv_out_contr,
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_DATA_ENTR_MODE_STNG, data, sizeof(data));
}

//0x12
void obj_ssd1675a_cmd_software_reset(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SW_RESET, NULL, 0);
}

//0x14
void obj_ssd1675a_cmd_hardware_ready_detection(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_HV_RDY_DTCT, NULL, 0);
}

//0x15
void obj_ssd1675a_cmd_VCI_detection(ssd1675a_ctx *ctx, double vlt) {
    uint8_t data[] = {
            (uint8_t)OBJ_SSD1675A_CMD_VCI_DTCT_A_FRML(vlt),
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_VCI_DTCT, data, sizeof(data));
}

//0x18
void obj_ssd1675a_cmd_temperature_sensor_control(ssd1675a_ctx *ctx, bool internal) {
    uint8_t data[] = {
            (uint8_t)internal ? 
                    OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_A_INT_SNSR : 
                    OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_A_EXT_SNSR,
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_TMP_SNSR_CTRL, data, sizeof(data));
}

//0x1A
void obj_ssd1675a_cmd_temperature_sensor_control_write(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            (uint8_t)OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_A_DEFAULT,//A0
            (uint8_t)(OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W_A_DEFAULT>>8),//A1
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_W, data, sizeof(data));
}

//0x1B
void obj_ssd1675a_cmd_temperature_sensor_control_read(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_TMP_SNSR_CTRL_R, NULL, 0);
}

//0x20
void obj_ssd1675a_cmd_master_activate(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_MSTR_ACTIVATE, NULL, 0);
}

//0x21
void obj_ssd1675a_cmd_display_update_control_1(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
        (1<<6)
        /*OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_OPTN_NORMAL(
                OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_BWRAM_SHFT
        ) |
        OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_OPTN_NORMAL(
                OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1_A_REDRAM_SHFT
        )*/,//A
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_1, data, sizeof(data));
}

//0x22
void obj_ssd1675a_cmd_display_update_control_2(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            0xC7,//A
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_DSPLY_UPDT_CTRL_2, data, sizeof(data));
}

//0x24
void obj_ssd1675a_cmd_write_ram_black_white(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_WRT_RAM_BW, NULL, 0);
}

//0x26
void obj_ssd1675a_cmd_write_ram_red_write(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_WRT_RAM_RED, NULL, 0);
}

//0x27
void obj_ssd1675a_cmd_read_ram(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_RD_RAM, NULL, 0);
}

//0x28
void obj_ssd1675a_cmd_VCOM_sense(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_VCOM_SENSE, NULL, 0);
}

//0x29
void obj_ssd1675a_cmd_VCOM_sense_duration(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
        OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_DRTN_FRML(
            OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_DRTN
        ) | OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN_A_MODE_NORMAL,
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_VCOM_SENSE_DRTN, data, sizeof(data));
}

//0x2A
void obj_ssd1675a_cmd_program_VCOM_into_OTP(ssd1675a_ctx *ctx) {
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_PRG_VCOM_OTP, NULL, 0);
}

//0x2C
void obj_ssd1675a_cmd_write_VCOM_register(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            0x70,//A modify contrast
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_WRT_VCOM_REG, data, sizeof(data));
}

//0x32
void obj_ssd1675a_cmd_write_LUT_register(
        ssd1675a_ctx *ctx, 
        ssd1675a_cmd_3c_vs black_vs, 
        ssd1675a_cmd_3c_vs white_vs, 
        ssd1675a_cmd_3c_vs red_vs, 
        ssd1675a_cmd_3c_vs vcom_vs, 
        uint8_t tp, 
        uint8_t rp
) {
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_WRT_LUT_MACRO_1(black_vs, white_vs, red_vs, vcom_vs, tp, rp),//A
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_WRT_LUT_REG, data, sizeof(data));
}

//0x3A
void obj_ssd1675a_cmd_set_dummy_line_period(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            (uint8_t)SSD1675A_REFRESH_200HZ,
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SET_DMMY_LINE_PRD, data, sizeof(data));
}

//0x3B
void obj_ssd1675a_cmd_set_gate_line_width(ssd1675a_ctx *ctx) {
    uint8_t data[] = {
            (uint8_t)(SSD1675A_REFRESH_200HZ>>8)
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SET_GATE_LN_WDTH, data, sizeof(data));
}

//0x3C
void obj_ssd1675a_cmd_border_waveform_control(ssd1675a_ctx *ctx, bool black) {
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_FXLVL |
                ((black == true) ? 
                    OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_VSH1 :
                    OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_LVL_VSL)
            | OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL_A_TRS_LUT1,
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_BORDER_WVFRM_CTRL, data, sizeof(data));
}

//0x44
void obj_ssd1675a_cmd_set_RAM_x_address_start_end_position(
        ssd1675a_ctx *ctx,
        uint16_t x_offset,
        uint16_t x_len
) {
    uint8_t data[] = {
            (uint8_t)x_offset,//A
            (uint8_t)((x_offset + x_len - 1)% (OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN_A_MAX + 1)),//B
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_S_P_OPTN, data, sizeof(data));
}

//0x45
void obj_ssd1675a_cmd_set_RAM_y_address_start_end_position(
        ssd1675a_ctx *ctx,
        uint16_t y_offset,
        uint16_t y_len
) {
    uint8_t data[] = {
            (uint8_t)(y_offset % ctx->y_dimension),
            (uint8_t)((y_offset % ctx->y_dimension)>>8),
            (uint8_t)((y_offset + y_len - 1)% ctx->y_dimension),//A
            (uint8_t)((((y_offset + y_len - 1)% ctx->y_dimension)>>8)),//B
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_S_P_OPTN, data, sizeof(data));
}

//0x46
void obj_ssd1675a_cmd_auto_write_red_ram(ssd1675a_ctx *ctx, uint8_t height, uint8_t width) {
    uint8_t data[] = {
        OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN_A_FSV_DEFAULT |
        height |
        width,
    };
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_AUTO_WRT_RED_RAM_RGLR_PTRN, data, sizeof(data));
}

//0x47
void obj_ssd1675a_cmd_auto_write_bw_ram(ssd1675a_ctx *ctx, uint8_t height, uint8_t width) {
    uint8_t data[] = {
        OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN_A_FSV_DEFAULT |
        height |
        width,
    };
    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_AUTO_WRT_BW_RAM_RGLR_PTRN, data, sizeof(data));
}

//0x4E
void obj_ssd1675a_cmd_set_RAM_x_address_control(ssd1675a_ctx *ctx, uint8_t x_pos) {
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_CNTR_A_DEFAULT,//A
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SET_RAM_X_ADDR_CNTR, data, sizeof(data));
}

//0x4F
void obj_ssd1675a_cmd_set_RAM_y_address_control(ssd1675a_ctx *ctx, uint8_t y_pos) {
    uint8_t data[] = {
            (uint8_t)OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_CNTR_A_DEFAULT,//A
            (uint8_t)(OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_CNTR_A_DEFAULT>>8),
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SET_RAM_Y_ADDR_CNTR, data, sizeof(data));
}

//0x74
void obj_ssd1675a_cmd_set_analog_block_control(ssd1675a_ctx *ctx) {
    
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_SET_ANA_BLCK_CTRL_A_DEFAULT,//A
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SET_ANA_BLCK_CTRL, data, sizeof(data));
}

//0x7E
void obj_ssd1675a_cmd_set_digital_block_control(ssd1675a_ctx *ctx) {
    
    uint8_t data[] = {
            OBJ_SSD1675A_CMD_SET_DGTL_BLCK_CTRL_A_DEFAULT,//A
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_SET_DGTL_BLCK_CTRL, data, sizeof(data));
}

//*****************************************************************************/
/*!
* \fn ssd1675a_state obj_ssd1675a_reset(ssd1675a_ctx *ctx);
* \brief device reset routine. MUST BE PERFORMED BEFORE USING obj_ssd1675a_init()
* \param[in] ctx object oriented data pointer
* \return state flag
*/

ssd1675a_state obj_ssd1675a_reset(ssd1675a_ctx *ctx) {

    while (ctx->busy_get_state(ctx->handle));

    ctx->nres_set_state(ctx->handle, false);
    ctx->delay_us(ctx->handle, 50);
    ctx->nres_set_state(ctx->handle, true);

    obj_ssd1675a_cmd_software_reset(ctx);
    ctx->delay_us(ctx->handle, 200);
    
    while(ctx->busy_get_state(ctx->handle));
    return ctx->flag;
}

//*****************************************************************************/
/*!
* \fn ssd1675a_state obj_ssd1675a_init(ssd1675a_ctx *ctx);
* \brief device init routine. MUST BE PERFORMED AFTER USING obj_ssd1675a_reset()
* \param[in] ctx object oriented data pointer
* \return state flag
*/

ssd1675a_state obj_ssd1675a_init(ssd1675a_ctx *ctx) {
    while(ctx->busy_get_state(ctx->handle));

    obj_ssd1675a_cmd_set_analog_block_control(ctx);
    obj_ssd1675a_cmd_set_digital_block_control(ctx);
    obj_ssd1675a_cmd_driver_out_control(ctx);
    obj_ssd1675a_cmd_set_dummy_line_period(ctx);
    obj_ssd1675a_cmd_set_gate_line_width(ctx);
    obj_ssd1675a_cmd_border_waveform_control(ctx, false);
    obj_ssd1675a_cmd_source_driver_voltage_control(ctx);
    obj_ssd1675a_cmd_write_VCOM_register(ctx);
    
    uint8_t data[] = { //TODO modify this data array to change refresh animation
        0x88, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, //lut0
        0x11, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, //lut1
        0x6A, 0x9B, 0x00, 0x00, 0x00, 0x00, 0x00, //lut2
        0x6A, 0x9B, 0x00, 0x00, 0x00, 0x00, 0x00, //lut3
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //lut4
        0x04, 0x04, 0x04, 0x04, 0x08, //tp0 rp0
        0x04, 0x04, 0x04, 0x04, 0x08, //tp0 rp0
        0x00, 0x00, 0x00, 0x00, 0x00, //tp0 rp0
        0x00, 0x00, 0x00, 0x00, 0x00, //tp0 rp0
        0x00, 0x00, 0x00, 0x00, 0x00, //tp0 rp0
        0x00, 0x00, 0x00, 0x00, 0x00, //tp0 rp0
        0x00, 0x00, 0x00, 0x00, 0x00  //tp0 rp0
    };

    obj_ssd1675a_send(ctx, OBJ_SSD1675A_CMD_WRT_LUT_REG, data, sizeof(data));

    return ctx->flag;
}

//*****************************************************************************/
/*!
* \fn ssd1675a_state obj_ssd1675a_set_display_bw_ram(
*         ssd1675a_ctx *ctx, 
*         uint16_t x_offset, 
*         uint16_t y_offset, 
*         uint8_t *data,
*         uint16_t data_len,
* );
* \brief device write into black/white ram routine. Use then obj_ssd1675a_set_display_refresh() to display stored data
* \param[in] ctx object oriented data pointer
* \param[in] x_offset first point coordinates x
* \param[in] y_offset first point coordinates y
* \param[in] data data to insert into ram
* \param[in] data_len data length
* \return state flag
*/

ssd1675a_state obj_ssd1675a_set_display_bw_ram(
        ssd1675a_ctx *ctx, 
        uint16_t x_offset, 
        uint16_t y_offset, 
        uint8_t *data,
        uint16_t data_len
) {
    while (ctx->busy_get_state(ctx->handle));

    x_offset %= ctx->x_dimension;
    y_offset %= ctx->y_dimension;

    obj_ssd1675a_cmd_data_entry_mode_setting(ctx);
    obj_ssd1675a_cmd_set_RAM_x_address_start_end_position(
        ctx, x_offset, 16);
    obj_ssd1675a_cmd_set_RAM_y_address_start_end_position(
        ctx, y_offset, 296);
    obj_ssd1675a_cmd_set_RAM_x_address_control(ctx, 0);
    obj_ssd1675a_cmd_set_RAM_y_address_control(ctx, 0);

    obj_ssd1675a_cmd_gate_scan_start_position(ctx);
    obj_ssd1675a_cmd_write_ram_black_white(ctx);
    
    obj_ssd1675a_send_ram_data(ctx, data, data_len);

    return ctx->flag;
}

//*****************************************************************************/
/*!
* \fn ssd1675a_state obj_ssd1675a_set_display_red_ram(
*         ssd1675a_ctx *ctx, 
*         uint16_t x_offset, 
*         uint16_t y_offset, 
*         uint8_t *data,
*         uint16_t data_len
* );
* \brief device write into red ram routine. Use then obj_ssd1675a_set_display_refresh() to display stored data
* \param[in] ctx object oriented data pointer
* \param[in] x_offset first point coordinates x
* \param[in] y_offset first point coordinates y
* \param[in] data data to insert into ram
* \param[in] data_len data length
* \return state flag
*/

ssd1675a_state obj_ssd1675a_set_display_red_ram(
        ssd1675a_ctx *ctx, 
        uint16_t x_offset, 
        uint16_t y_offset, 
        uint8_t *data,
        uint16_t data_len
) {

    while (ctx->busy_get_state(ctx->handle));
    
    x_offset %= ctx->x_dimension;
    y_offset %= ctx->y_dimension;

    obj_ssd1675a_cmd_data_entry_mode_setting(ctx);
    obj_ssd1675a_cmd_set_RAM_x_address_start_end_position(
        ctx, x_offset, 16);
    obj_ssd1675a_cmd_set_RAM_y_address_start_end_position(
        ctx, y_offset, 296/*ctx->y_dimension*/);
    obj_ssd1675a_cmd_set_RAM_x_address_control(ctx, 0);
    obj_ssd1675a_cmd_set_RAM_y_address_control(ctx, 0);

    obj_ssd1675a_cmd_gate_scan_start_position(ctx);
    obj_ssd1675a_cmd_write_ram_red_write(ctx);
    
    obj_ssd1675a_send_ram_data(ctx, data, data_len);

    return ctx->flag;
}

//*****************************************************************************/
/*!
* \fn ssd1675a_state obj_ssd1675a_set_display_refresh(ssd1675a_ctx *ctx);
* \brief device refresh display considering display ram stored data. Must de performed after obj_ssd1675a_set_display_bw_ram() or obj_ssd1675a_set_display_red_ram() routine
* \param[in] ctx object oriented data pointer
* \return state flag
*/

ssd1675a_state obj_ssd1675a_set_display_refresh(ssd1675a_ctx *ctx) {

    while (ctx->busy_get_state(ctx->handle));

    obj_ssd1675a_cmd_display_update_control_1(ctx);
    obj_ssd1675a_cmd_display_update_control_2(ctx); //0xc7
    obj_ssd1675a_cmd_master_activate(ctx);

    return ctx->flag;
}