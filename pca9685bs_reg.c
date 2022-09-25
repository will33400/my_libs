/*!
 * \file pca9685bs_reg.c
 * \brief PWM generator device functions
 * \author Adam LUCAS
 * \date 17 févr. 2021
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "pca9685bs_reg.h"

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
* \fn void pca9685bs_init(pca9685bs_ctx_t *ctx);
* \brief init device properties
* \param[in] ctx platform dependent parameters
* \return None
*/

void pca9685bs_init(pca9685bs_ctx_t *ctx) {

  //Restart device

  uint8_t parameter_1[] = {
      PCA9685BS_MODE1_REG,
      (1<<PCA9685BS_MODE1_RESTART)
  };

  ctx->i2c_write(ctx->handle, parameter_1, sizeof(parameter_1));

  //Set clock frequency + prescale

  uint8_t MODE1_address = PCA9685BS_MODE1_REG;
  uint8_t MODE1_contents;

  ctx->i2c_read(
      ctx->handle,
      &MODE1_address,
      sizeof(MODE1_address),
      &MODE1_contents,
      sizeof(MODE1_contents)
  );

  uint8_t parameter_2[] = {
      PCA9685BS_MODE1_REG,
      (
          MODE1_contents &
          ~(1<<PCA9685BS_MODE1_RESTART)
      ) | (1<<PCA9685BS_MODE1_SLEEP)
  };

  ctx->i2c_write(ctx->handle, parameter_2, sizeof(parameter_2));

  if(ctx->ext_clock) {
      MODE1_contents |= (1<<PCA9685BS_MODE1_EXTCLK);

      uint8_t temp[] = {
          PCA9685BS_MODE1_REG,
          MODE1_contents
      };

      ctx->i2c_write(ctx->handle, temp, sizeof(temp));
  }

  uint8_t parameter_3[] = {
      PCA9685BS_PRE_SCALE_REG,
      PCA9685BS_PRE_SCALE
      (
          ctx->clock,
          ctx->refresh_rate
      )
  };

  ctx->i2c_write(ctx->handle, parameter_3, sizeof(parameter_3));

  ctx->delay(ctx->handle, 5000); //5ms

  uint8_t parameter_4[] = {
      PCA9685BS_MODE1_REG,
      (
          (
              MODE1_contents &
              ~(1<<PCA9685BS_MODE1_SLEEP)
          ) |
          (1<<PCA9685BS_MODE1_RESTART) |
          (1<<PCA9685BS_MODE1_AI)
      )
  };

  ctx->i2c_write(ctx->handle, parameter_4, sizeof(parameter_4));

  uint8_t MODE2_address = PCA9685BS_MODE2_REG;
  uint8_t MODE2_contents;

  ctx->i2c_read(
      ctx->handle,
      &MODE2_address,
      sizeof(MODE2_address),
      &MODE2_contents,
      sizeof(MODE2_contents)
  );

  uint8_t parameter_5[] = {
      PCA9685BS_MODE2_REG,
      MODE2_contents |= (1<<PCA9685BS_MODE2_OUTDRV)
  };

  ctx->i2c_write(ctx->handle, parameter_5, sizeof(parameter_5));

}

//*****************************************************************************/
/*!
* \fn void pca9685bs_chip_enable(pca9685bs_ctx_t *ctx, bool enable);
* \brief hardware OE pin outputs enable
* \param[in] ctx platform dependent parameters
* \param[in] enable select enable (true) or disable (false)
* \return None
*/

void pca9685bs_chip_enable(pca9685bs_ctx_t *ctx, bool enable) {
  ctx->oe_pin(ctx->handle, !enable);
}

//*****************************************************************************/
/*!
* \fn void pca9685bs_all_on(pca9685bs_ctx_t *ctx);
* \brief software set all pins on
* \param[in] ctx platform dependent parameters
* \return None
*/

void pca9685bs_all_on(pca9685bs_ctx_t *ctx) {
  for(int i = 0; i < 16; i++)
    pca9685bs_set_pin_gpio(ctx, i, true);
}

//*****************************************************************************/
/*!
* \fn void pca9685bs_all_off(pca9685bs_ctx_t *ctx);
* \brief software set all pins off
* \param[in] ctx platform dependent parameters
* \return None
*/

void pca9685bs_all_off(pca9685bs_ctx_t *ctx) {
  for(int i = 0; i < 16; i++)
    pca9685bs_set_pin_gpio(ctx, i, false);
}

//*****************************************************************************/
/*!
* \fn void pca9685bs_prescaler_set(pca9685bs_ctx_t *ctx, uint8_t refresh_rate);
* \brief PRE_SCALE register value set
* \param[in] ctx platform dependent parameters
* \param[in] refresh_rate refresh rate value in Hz
* \return None
*/

void pca9685bs_prescaler_set(pca9685bs_ctx_t *ctx, uint8_t refresh_rate) {
  // switch to SLEEP mode to write PRE_SCALE register

  //read contents of MODE_1 register
  uint8_t parameters_1 = PCA9685BS_MODE1_REG;
  uint8_t MODE_1_REG_content;

  ctx->i2c_read(
      ctx->handle,
      &parameters_1,
      sizeof(parameters_1),
      &MODE_1_REG_content,
      sizeof(MODE_1_REG_content)
  );

  //modify contents of MODE_1 register SLEEP = 1
  uint8_t parameters_2[] = {
      PCA9685BS_MODE1_REG,
      MODE_1_REG_content | (1<<PCA9685BS_MODE1_SLEEP)
  };

  ctx->i2c_write(ctx->handle, parameters_2, sizeof(parameters_2));

  //modify value of PRE_SCALE register
  uint8_t parameters_3[] = {
      PCA9685BS_PRE_SCALE_REG,
      PCA9685BS_PRE_SCALE
      (
          ctx->clock,
          PCA9685BS_PRE_SCALE
          (
              ctx->clock,
              refresh_rate
          )
      )
  };

  ctx->i2c_write(ctx->handle, parameters_3, sizeof(parameters_3));

  //modify contents of MODE_1 register SLEEP = 0
  uint8_t parameters_4[] = {
      PCA9685BS_MODE1_REG,
      MODE_1_REG_content & ~(1<<PCA9685BS_MODE1_SLEEP)
  };

  ctx->i2c_write(ctx->handle, parameters_4, sizeof(parameters_4));

}

//*****************************************************************************/
/*!
* \fn uint16_t pca9685bs_prescaler_get(pca9685bs_ctx_t *ctx);
* \brief get refresh rate from PRE_SCALE register value
* \param[in] ctx platform dependent parameters
* \return refresh rate value in Hz
*/

uint16_t pca9685bs_prescaler_get(pca9685bs_ctx_t *ctx) {

  uint8_t PRE_SCALE_reg = PCA9685BS_PRE_SCALE_REG;
  uint8_t PRE_SCALE_content;

  ctx->i2c_read(
      ctx->handle,
      &PRE_SCALE_reg,
      sizeof(PRE_SCALE_reg),
      &PRE_SCALE_content,
      sizeof(PRE_SCALE_content)
  );

  return PCA9685BS_REFRESH_RATE(ctx->clock, PRE_SCALE_content);
}

//*****************************************************************************/
/*!
* \fn void pca9685bs_chip_restart(pca9685bs_ctx_t *ctx);
* \brief software chip reset
* \param[in] ctx platform dependent parameters
* \return None
*/

void pca9685bs_chip_restart(pca9685bs_ctx_t *ctx) {
  uint8_t register_param = PCA9685BS_MODE1_REG;
  uint8_t MODE1_contents;

  ctx->i2c_read(
      ctx->handle,
      &register_param,
      sizeof(register_param),
      &MODE1_contents,
      sizeof(MODE1_contents)
  );

  uint8_t parameter = MODE1_contents | (1<<PCA9685BS_MODE1_RESTART);

  ctx->i2c_write(ctx->handle, &parameter, sizeof(parameter));

}

//*****************************************************************************/
/*!
* \fn void pca9685bs_set_pin_pwm(
*             pca9685bs_ctx_t *ctx,
*             uint8_t pin_num,
*             uint16_t val
*           );
* \brief generate PWM output
* \param[in] ctx platform dependent parameters
* \param[in] pin_num number of the pin (from 0 to 15)
* \param[in] val analog value (from 0 to 4095)
* \return None
*/

void pca9685bs_set_pin_pwm(pca9685bs_ctx_t *ctx, uint8_t pin_num, uint16_t val) {
  if(val > PCA9685BS_LEDn_MAX_DELAY_VAL)
    return;

  val %= 4096;

  uint8_t parameter[] = {
      (PCA9685BS_LED0_ON_L_REG + (4 * pin_num)),
      0x00,
      0x00,
      (uint8_t)val,
      (uint8_t)(val >> 8),
  };

  ctx->i2c_write(ctx->handle, parameter, sizeof(parameter));

}

//*****************************************************************************/
/*!
* \fn void pca9685bs_set_pin_gpio(
*         pca9685bs_ctx_t *ctx,
*         uint8_t pin_num,
*         bool high
*       );
* \brief generate GPIO output
* \param[in] ctx platform dependent parameters
* \param[in] pin_num number of the pin (from 0 to 15)
* \param[in] high select HIGH (true) or LOW (false)
* \return None
*/

void pca9685bs_set_pin_gpio(pca9685bs_ctx_t *ctx, uint8_t pin_num, bool high) {

  uint8_t parameter[] = {
      (PCA9685BS_LED0_ON_L_REG + (4 * pin_num)),
      0x00,
      0x00,
      0x00,
      0x00,
    };

  if(high) {
    parameter[0] = 0xff;
    parameter[1] = 0x7f;
    parameter[2] = 0x00;
    parameter[3] = 0x00;
  }
  else {
    parameter[0] = 0x00;
    parameter[1] = 0x00;
    parameter[2] = 0xff;
    parameter[3] = 0x7f;
  }

  ctx->i2c_write(ctx->handle, parameter, sizeof(parameter));

}

