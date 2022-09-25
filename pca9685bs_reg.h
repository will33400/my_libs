/*!
 * \file pca9685bs_reg.h
 * \brief PWM generator device function header
 * \author Adam LUCAS
 * \date 17 févr. 2021
 */


#ifndef PCA9685BS_REG_H_
#define PCA9685BS_REG_H_

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

// Register definitions

// R/W registers

#define PCA9685BS_MODE1_REG             0x00
#define PCA9685BS_MODE2_REG             0x01

#define PCA9685BS_SUBADR1_REG           0x02
#define PCA9685BS_SUBADR2_REG           0x03
#define PCA9685BS_SUBADR3_REG           0x04

#define PCA9685BS_ALLCALLADR_REG        0x05

#define PCA9685BS_LED0_ON_L_REG         0x06
#define PCA9685BS_LED0_ON_H_REG         0x07
#define PCA9685BS_LED0_OFF_L_REG        0x08
#define PCA9685BS_LED0_OFF_H_REG        0x09

#define PCA9685BS_LED1_ON_L_REG         0x0A
#define PCA9685BS_LED1_ON_H_REG         0x0B
#define PCA9685BS_LED1_OFF_L_REG        0x0C
#define PCA9685BS_LED1_OFF_H_REG        0x0D

#define PCA9685BS_LED2_ON_L_REG         0x0E
#define PCA9685BS_LED2_ON_H_REG         0x0F
#define PCA9685BS_LED2_OFF_L_REG        0x10
#define PCA9685BS_LED2_OFF_H_REG        0x11

#define PCA9685BS_LED3_ON_L_REG         0x12
#define PCA9685BS_LED3_ON_H_REG         0x13
#define PCA9685BS_LED3_OFF_L_REG        0x14
#define PCA9685BS_LED3_OFF_H_REG        0x15

#define PCA9685BS_LED4_ON_L_REG         0x16
#define PCA9685BS_LED4_ON_H_REG         0x17
#define PCA9685BS_LED4_OFF_L_REG        0x18
#define PCA9685BS_LED4_OFF_H_REG        0x19

#define PCA9685BS_LED5_ON_L_REG         0x1A
#define PCA9685BS_LED5_ON_H_REG         0x1B
#define PCA9685BS_LED5_OFF_L_REG        0x1C
#define PCA9685BS_LED5_OFF_H_REG        0x1D

#define PCA9685BS_LED6_ON_L_REG         0x1E
#define PCA9685BS_LED6_ON_H_REG         0x1F
#define PCA9685BS_LED6_OFF_L_REG        0x20
#define PCA9685BS_LED6_OFF_H_REG        0x21

#define PCA9685BS_LED7_ON_L_REG         0x22
#define PCA9685BS_LED7_ON_H_REG         0x23
#define PCA9685BS_LED7_OFF_L_REG        0x24
#define PCA9685BS_LED7_OFF_H_REG        0x25

#define PCA9685BS_LED8_ON_L_REG         0x26
#define PCA9685BS_LED8_ON_H_REG         0x27
#define PCA9685BS_LED8_OFF_L_REG        0x28
#define PCA9685BS_LED8_OFF_H_REG        0x29

#define PCA9685BS_LED9_ON_L_REG         0x2A
#define PCA9685BS_LED9_ON_H_REG         0x2B
#define PCA9685BS_LED9_OFF_L_REG        0x2C
#define PCA9685BS_LED9_OFF_H_REG        0x2D

#define PCA9685BS_LED10_ON_L_REG        0x2E
#define PCA9685BS_LED10_ON_H_REG        0x2F
#define PCA9685BS_LED10_OFF_L_REG       0x30
#define PCA9685BS_LED10_OFF_H_REG       0x31

#define PCA9685BS_LED11_ON_L_REG        0x32
#define PCA9685BS_LED11_ON_H_REG        0x33
#define PCA9685BS_LED11_OFF_L_REG       0x34
#define PCA9685BS_LED11_OFF_H_REG       0x35

#define PCA9685BS_LED12_ON_L_REG        0x36
#define PCA9685BS_LED12_ON_H_REG        0x37
#define PCA9685BS_LED12_OFF_L_REG       0x38
#define PCA9685BS_LED12_OFF_H_REG       0x39

#define PCA9685BS_LED13_ON_L_REG        0x3A
#define PCA9685BS_LED13_ON_H_REG        0x3B
#define PCA9685BS_LED13_OFF_L_REG       0x3C
#define PCA9685BS_LED13_OFF_H_REG       0x3D

#define PCA9685BS_LED14_ON_L_REG        0x3E
#define PCA9685BS_LED14_ON_H_REG        0x3F
#define PCA9685BS_LED14_OFF_L_REG       0x40
#define PCA9685BS_LED14_OFF_H_REG       0x41

#define PCA9685BS_LED15_ON_L_REG        0x42
#define PCA9685BS_LED15_ON_H_REG        0x43
#define PCA9685BS_LED15_OFF_L_REG       0x44
#define PCA9685BS_LED15_OFF_H_REG       0x45

// R/W/ZERO registers

#define PCA9685BS_ALL_LED_ON_L_REG      0xFA
#define PCA9685BS_ALL_LED_ON_H_REG      0xFB
#define PCA9685BS_ALL_LED_OFF_L_REG     0xFC
#define PCA9685BS_ALL_LED_OFF_H_REG     0xFD
#define PCA9685BS_PRE_SCALE_REG         0xFE
#define PCA9685BS_TEST_MODE_REG         0xFF

// Register definitions end



// MODE1 bits

#define PCA9685BS_MODE1_ALLCALL         0
#define PCA9685BS_MODE1_SUB3            1
#define PCA9685BS_MODE1_SUB2            2
#define PCA9685BS_MODE1_SUB1            3
#define PCA9685BS_MODE1_SLEEP           4
#define PCA9685BS_MODE1_AI              5
#define PCA9685BS_MODE1_EXTCLK          6
#define PCA9685BS_MODE1_RESTART         7

// MODE1 definitions

#define PCA9685BS_MODE1_DEFAULT         (1<<PCA9685BS_MODE1_ALLCALL)

// MODE2 bits

#define PCA9685BS_MODE2_OUTNE0          0
#define PCA9685BS_MODE2_OUTNE1          1
#define PCA9685BS_MODE2_OUTDRV          2
#define PCA9685BS_MODE2_OCH             3
#define PCA9685BS_MODE2_INVRT           4

// MODE2 definitions

#define PCA9685BS_MODE2_DEFAULT         (1<<PCA9685BS_MODE2_OUTDRV)

// SUBADR1 bits
// SUBADR2 bits
// SUBADR3 bits

// LEDn bits

#define PCA9685BS_LEDn_ON_H_FULL_ON     4
#define PCA9685BS_LEDn_OFF_H_FULL_OFF   4



// LEDS defines

#define PCA9685BS_LED_COUNT             16
#define PCA9685BS_LEDn_VAL_SIZE         12 //12 bits
#define PCA9685BS_LEDn_MIN_DELAY_VAL    0
#define PCA9685BS_LEDn_MAX_DELAY_VAL    4095
#define PCA9685BS_LEDn_VAL_MASK         0x0FFF

// ALL_LED bits

#define PCA9685BS_ALL_LED_ON_H_FULL     4 //bit to turn all leds on
#define PCA9685BS_ALL_LED_OFF_H_FULL    4 // // // off

// ALL_LED defines

#define PCA9685BS_ALL_LED_ON_COUNT_SIZE 12 //12 bits
#define PCA9685BS_ALL_LED_OFF_COUNT_SIZE 12 //12 bits
#define PCA9685BS_ALL_LED_ON_DEFAULT_VAL ((4096 / 2) -1)
#define PCA9685BS_ALL_LED_OFF_DEFAULT_VAL (4096 / 2)

// software reset command

#define PCA9685BS_SWRST_ADDR            0x00
#define PCA9685BS_SWRST_COMMAND         0x06



// Timing defines

#define PCA9685BS_PWM_REFRESH_MIN       24UL
#define PCA9685BS_PWM_REFRESH_MAX       1526UL
#define PCA9685BS_INTERNAL_CLOCK        25000000UL
#define PCA9685BS_REFRESH_RATE(clock, prescale)                               \
                                            (clock / (4096 *(prescale + 1)))
#define PCA9685BS_PRE_SCALE(clock, refresh_rate)                              \
                                            (clock / (4096 * refresh_rate)- 1)

// PRE_SCALER 1 config

#define PCA9685BS_PRE_SCALE_VAL         PCA9685BS_PRE_SCALE                   \
                                          (                                   \
                                            PCA9685BS_INTERNAL_CLOCK,         \
                                            60UL                              \
                                          )


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \fn void (*pca9685bs_i2c_write)(void *, uint8_t *, uint16_t);
 * \brief I2C Write function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \param[in] data to Write buffer
 * \param[in] data length
 * \return None
 */

typedef void (*pca9685bs_i2c_write)(void *, uint8_t *, uint16_t);

/*!
 * \fn void (*pca9685bs_i2c_read)(
 *            void *,
 *            uint8_t *,
 *            uint16_t,
 *            uint8_t *,
 *            uint16_t
 *          );
 * \brief I2C WriteRead or Read at a register location function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \param[in] data to Write buffer
 * \param[in] data to Write length
 * \param[out] data to Read buffer
 * \param[in] data to Read length
 * \return None
 */

typedef void (*pca9685bs_i2c_read)(void *, uint8_t *, uint16_t, uint8_t *, uint16_t);

/*!
 * \fn void (*pca9685bs_delay)(void *, uint32_t);
 * \brief delay function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \param[in] delay value in µseconds
 * \return None
 */

typedef void (*pca9685bs_delay)(void *, uint32_t);

/*!
 * \fn void (*pca9685bs_gpio_state_set)(void *, bool);
 * \brief GPIO pin change state function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \param[in] pin state (positive logic true : HIGH, false : LOW)
 * \return None
 */

typedef void (*pca9685bs_gpio_state_set)(void *, bool);

/*!
 * \struct pca9685bs_ctx_t
 * \brief object oriented functions + handle data
 */

typedef struct {
  //!i2c
  pca9685bs_i2c_write i2c_write;
  pca9685bs_i2c_read i2c_read;

  //!delay
  pca9685bs_delay delay;

  //!gpio MANDATORY : pin set as OUTPUT !!
  pca9685bs_gpio_state_set oe_pin;

  //!parameters variables
  uint32_t clock;
  bool ext_clock;

  uint8_t refresh_rate;

  //! handle
  void *handle;
}pca9685bs_ctx_t;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//! functions descriptions : refer to pca9685bs_reg.c

void pca9685bs_init(pca9685bs_ctx_t *ctx);

void pca9685bs_chip_enable(pca9685bs_ctx_t *ctx, bool enable);
void pca9685bs_all_on(pca9685bs_ctx_t *ctx);
void pca9685bs_all_off(pca9685bs_ctx_t *ctx);
void pca9685bs_prescaler_set(pca9685bs_ctx_t *ctx, uint8_t refresh_rate);
uint16_t pca9685bs_prescaler_get(pca9685bs_ctx_t *ctx);
void pca9685bs_chip_restart(pca9685bs_ctx_t *ctx);
void pca9685bs_set_pin_pwm(pca9685bs_ctx_t *ctx, uint8_t pin_num, uint16_t val);
void pca9685bs_set_pin_gpio(pca9685bs_ctx_t *ctx, uint8_t pin_num, bool high);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* PCA9685BS_REG_H_ */
