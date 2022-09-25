/*!
 * \file s8_lp_reg.c
 * \brief CO2 sensor functions
 * \author Adam LUCAS
 * \date 8 févr. 2021
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "s8_lp_reg.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

static const uint16_t crc_tab16[256] = CRC_ALGO_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn inline static uint16_t modbus_function_select(
*         modbus_reg_t reg_type,
*         bool write
*     );
* \brief Select the function to use for ModBus protocol
* \param[in] INPUT or HOLDING register
* \param[in] true : write, false : read
* \return function word
*/

inline static uint16_t modbus_function_select(modbus_reg_t reg_type, bool write) {
  if(write)
    return MODBUS_FUNC_WRITE_SINGLE_REG;
  else if((reg_type&HOLDING) && !write)
    return MODBUS_FUNC_READ_HOLD_REG;
  else if(!(reg_type&HOLDING) && !write)
    return MODBUS_FUNC_READ_IN_REG;

  return 0;
}

//*****************************************************************************/
/*!
* \fn static uint16_t modbus_crc16(uint8_t *data, uint16_t size);
* \brief computes CRC16 for ModBus
* \param[in] data contents
* \param[in] data length (except CRC16 size !!)
* \return true if acknowledge of the message
*/

//TODO modify algo


static uint16_t modbus_crc16(uint8_t *data, uint16_t size) {

  uint8_t index;
  uint16_t crc = CRC_START_MODBUS;

  while(size--) {

    index = crc ^(uint8_t)*data;
    crc = (crc >> 8)^ crc_tab16[index];

    data++;
  }

  return crc;
}

//*****************************************************************************/
/*!
* \fn static uint8_t modbus_register_read(
        s8_lp_ctx_t *ctx,
        uint8_t _register,
        uint8_t register_num,
        uint8_t *response,
        uint16_t response_len,
        modbus_reg_t reg_type
      )
* \brief ModBus read routine (single or multiple read possible)
* \param[in] ctx instance
* \param[in] register address
* \param[in] register number to read after register address (defined address included)
* \param[out] response data pointer
* \param[in] response data size
* \param[in] INPUT or HOLDING register
* \return 1 if transfert works 0 else
*/

static uint8_t modbus_register_read(
    s8_lp_ctx_t *ctx,
    uint8_t _register,
    uint8_t register_num,
    uint8_t *response,
    uint16_t response_len,
    modbus_reg_t reg_type
) {

  uint8_t query[] = {
      MODBUS_DEVICE_ADDR,
      modbus_function_select(reg_type, false),
      (uint8_t)(_register>>8),
      (uint8_t)_register,
      (uint8_t)(register_num>>8),
      (uint8_t)register_num,
      0x00,
      0x00
  };

  uint16_t crc16 = modbus_crc16(query, sizeof(query) - 2);

  query[MODBUS_QUERY_ERR_CHECK_L] = (uint8_t)crc16;
  query[MODBUS_QUERY_ERR_CHECK_H] = (uint8_t)(crc16>>8);

  ctx->write_reg(ctx->handle, query, sizeof(query));
  ctx->read_reg(ctx->handle, response, response_len);

  if(
      ((uint16_t)
          (
              response[MODBUS_RESP_SIZE(register_num)-2] +
              response[MODBUS_RESP_SIZE(register_num)-1]
          )
      ) == crc16
    )
  return 1;
  else return 0;
}

//*****************************************************************************/
/*!
* \fn static uint8_t modbus_register_write(
        s8_lp_ctx_t *ctx,
        uint8_t _register,
        uint16_t data,
        modbus_reg_t reg_type
      );
* \brief ModBus write routine (single register write)
* \param[in] ctx instance
* \param[in] register address
* \param[in] register value
* \param[in] INPUT or HOLDING register
* \return 1 if transfert works 0 else
*/

static uint8_t modbus_register_write(
    s8_lp_ctx_t *ctx,
    uint8_t _register,
    uint16_t data,
    modbus_reg_t reg_type
) {

  uint8_t query[] = {
      MODBUS_DEVICE_ADDR,
      modbus_function_select(reg_type, true),
      (uint8_t)(_register>>8),
      (uint8_t)_register,
      (uint8_t)(data>>8),
      (uint8_t)data,
      0x00,
      0x00
  };

  uint8_t response[MODBUS_RESP_SIZE(1)]; //single data equals 1 register

  uint16_t crc16 = modbus_crc16(query, sizeof(query) - 2);

  query[MODBUS_QUERY_ERR_CHECK_L] = (uint8_t)crc16;
  query[MODBUS_QUERY_ERR_CHECK_H] = (uint8_t)(crc16>>8);

  ctx->write_reg(ctx->handle, query, sizeof(query));
  ctx->read_reg(ctx->handle, response, sizeof(response));

  if(
      ((uint16_t)
          (
              response[MODBUS_RESP_SIZE(1)-2] +
              response[MODBUS_RESP_SIZE(1)-1]
          )
      ) == crc16
    )
    return 1;
    else return 0;
}

//*****************************************************************************/
/*!
* \fn void s8_lp_init(s8_lp_ctx_t *ctx);
* \brief init s8 lp device (co2_cal pin pulled to VCC)
* \param[in] ctx instance (initialised with all needed functions pointers)
* \return None
*/

void s8_lp_init(s8_lp_ctx_t *ctx) {
  ctx->co2_cal(ctx->handle, true);
}

//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_meter_status_get(s8_lp_ctx_t *ctx);
* \brief get any error from the MeterStatus register
* \param[in] ctx instance
* \return MeterStatus register contents
*/

uint16_t s8_lp_meter_status_get(s8_lp_ctx_t *ctx) {

  uint8_t response[MODBUS_RESP_SIZE(1)];

  modbus_register_read(
      ctx,
      S8_LP_METER_STATUS_REG,
      1,
      response,
      sizeof(response),
      INPUT
  );

  return (uint16_t)
      (
          (response[MODBUS_RESP_BYTE_COUNT+1]<<8) +
          response[MODBUS_RESP_BYTE_COUNT+2]
      );
}

//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_output_status_get(s8_lp_ctx_t *ctx);
* \brief get alarm and PWM output status from OutputStatus register
* \param[in] ctx instance
* \return OutputStatus contents
*/

uint16_t s8_lp_output_status_get(s8_lp_ctx_t *ctx) {

  uint8_t response[MODBUS_RESP_SIZE(1)];

  modbus_register_read(
      ctx,
      S8_LP_OUTPUT_STATUS_REG,
      1,
      response,
      sizeof(response),
      INPUT
  );

  return (uint16_t)
      (
          (response[MODBUS_RESP_BYTE_COUNT+1]<<8) +
          response[MODBUS_RESP_BYTE_COUNT+2]
      );
}

//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_space_co2_get(s8_lp_ctx_t *ctx);
* \brief get CO2 value (ppm) from SpaceCO2 register
* \param[in] ctx instance
* \return SpaceCO2 contents
*/

uint16_t s8_lp_space_co2_get(s8_lp_ctx_t *ctx) {
  uint8_t response[MODBUS_RESP_SIZE(1)];

  modbus_register_read(
      ctx,
      S8_LP_SPACE_CO2_REG,
      1,
      response,
      sizeof(response),
      INPUT
  );

  return (uint16_t)
      (
          (response[MODBUS_RESP_BYTE_COUNT+1]<<8) +
          response[MODBUS_RESP_BYTE_COUNT+2]
      );
}

//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_pwm_output(s8_lp_ctx_t *ctx);
* \brief get value from 0(min) to 0x3FFF(max) from PWMOutput register
* \param[in] ctx instance
* \return PWMOutput contents
*/

uint16_t s8_lp_pwm_output(s8_lp_ctx_t *ctx) {
  uint8_t response[MODBUS_RESP_SIZE(1)];

  modbus_register_read(
      ctx,
      S8_LP_PWM_OUTPUT_REG,
      1,
      response,
      sizeof(response),
      INPUT
  );

  return (uint16_t)
      (
          (response[MODBUS_RESP_BYTE_COUNT+1]<<8) +
          response[MODBUS_RESP_BYTE_COUNT+2]
      );
}

//*****************************************************************************/
/*!
* \fn void s8_lp_ack_reg_set(s8_lp_ctx_t *ctx, uint16_t val);
* \brief set value to Acknowedgement register
* background / nitrogen calibration performed flags
* \param[in] ctx instance
* \return PWMOutput contents
*/
/*
void s8_lp_ack_reg_set(s8_lp_ctx_t *ctx, uint16_t val) {

}
*/
//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_ack_reg_get(s8_lp_ctx_t *ctx, uint16_t val);
* \brief get value from Acknowedgement register
* background / nitrogen calibration performed flags
* \param[in] ctx instance
* \return Acknowedgement register contents
*/

uint16_t s8_lp_ack_reg_get(s8_lp_ctx_t *ctx) {
  uint8_t response[MODBUS_RESP_SIZE(1)];

  modbus_register_read(
      ctx,
      S8_LP_ACK_REG,
      1,
      response,
      sizeof(response),
      HOLDING
  );

  return (uint16_t)
      (
          (response[MODBUS_RESP_BYTE_COUNT+1]<<8) +
          response[MODBUS_RESP_BYTE_COUNT+2]
      );
}

//*****************************************************************************/
/*!
* \fn void s8_lp_spe_comm_set(s8_lp_ctx_t *ctx, uint16_t val);
* \brief set value to SpecialCommand register
* background calibration / zero calibration commands
* \param[in] ctx instance
* \param[in] command value
* \return None
*/

void s8_lp_spe_comm_set(s8_lp_ctx_t *ctx, uint16_t val) {

  modbus_register_write(
      ctx,
      S8_LP_SPECIAL_COMM_REG,
      val,
      HOLDING
  );

}

//*****************************************************************************/
/*!
* \fn void s8_lp_spe_comm_set(s8_lp_ctx_t *ctx);
* \brief get value from SpecialCommand register
* background calibration / zero calibration commands
* \param[in] ctx instance
* \return SpecialCommand contents
*/

uint16_t s8_lp_spe_comm_get(s8_lp_ctx_t *ctx) {
  uint8_t response[MODBUS_RESP_SIZE(1)];

  modbus_register_read(
      ctx,
      S8_LP_SPECIAL_COMM_REG,
      1,
      response,
      sizeof(response),
      HOLDING
  );

  return (uint16_t)
      (
          (response[MODBUS_RESP_BYTE_COUNT+1]<<8) +
          response[MODBUS_RESP_BYTE_COUNT+2]
      );
}

//*****************************************************************************/
/*!
* \fn void s8_lp_abc_period_set(s8_lp_ctx_t *ctx, uint16_t val);
* \brief set value to ABCPeriod register in hours
* \param[in] ctx instance
* \param[in] command value
* \return None
*/

void s8_lp_abc_period_set(s8_lp_ctx_t *ctx, uint16_t val) {

  modbus_register_write(
      ctx,
      S8_LP_ABC_PERIOD,
      val,
      HOLDING
  );

}

//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_abc_period_get(s8_lp_ctx_t *ctx);
* \brief set value to ABCPeriod register in hours
* \param[in] ctx instance
* \return ABCPeriod contents
*/

uint16_t s8_lp_abc_period_get(s8_lp_ctx_t *ctx) {
  uint8_t response[MODBUS_RESP_SIZE(1)];

  modbus_register_read(
      ctx,
      S8_LP_ABC_PERIOD,
      1,
      response,
      sizeof(response),
      HOLDING
  );

  return (uint16_t)
      (
          (response[MODBUS_RESP_BYTE_COUNT+1]<<8) +
          response[MODBUS_RESP_BYTE_COUNT+2]
      );
}

//TODO modify doc------------------------------------------------------------

//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_abc_period_get(s8_lp_ctx_t *ctx);
* \brief set value to ABCPeriod register in hours
* \param[in] ctx instance
* \return ABCPeriod contents
*/

void s8_lp_400ppm_cal(s8_lp_ctx_t *ctx) {
  ctx->co2_cal(ctx->handle, false);
  ctx->delay(ctx->handle, S8_LP_400PPM_CAL_DELAY);
  ctx->co2_cal(ctx->handle, true);
}

//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_abc_period_get(s8_lp_ctx_t *ctx);
* \brief set value to ABCPeriod register in hours
* \param[in] ctx instance
* \return ABCPeriod contents
*/

void s8_lp_0ppm_cal(s8_lp_ctx_t *ctx) {
  ctx->co2_cal(ctx->handle, false);
  ctx->delay(ctx->handle, S8_LP_0PPM_CAL_DELAY);
  ctx->co2_cal(ctx->handle, true);
}

//*****************************************************************************/
/*!
* \fn uint16_t s8_lp_abc_period_get(s8_lp_ctx_t *ctx);
* \brief set value to ABCPeriod register in hours
* \param[in] ctx instance
* \return ABCPeriod contents
*/
/*
void s8_lp_repair(s8_lp_ctx_t *ctx) {
  uint16_t meter_status = s8_lp_meter_status_get(ctx);

  switch(meter_status) {
    case S8_LP_METER_STATUS_FATAL_ERR : {

    }
    break;

    case S8_LP_METER_STATUS_OFFSET_ERR : {

    }
    break;

    case S8_LP_METER_STATUS_ALGO_ERR : {

    }
    break;

    case S8_LP_METER_STATUS_OUT_ERR : {

    }
    break;

    case S8_LP_METER_STATUS_DIAG_ERR : {

    }
    break;

    case S8_LP_METER_STATUS_O_RNG_ERR : {

    }
    break;

    case S8_LP_METER_STATUS_MEM_ERR : {

    }
    break;
  }
}*/

uint16_t s8_lp_space_co2_H_get(s8_lp_ctx_t *ctx) {
  return (uint16_t)(ctx->co2_vout(ctx->handle) * 1000);
}

