/*!
 * \file s8_lp_reg.h
 * \brief CO2 sensor s8 lp device header
 * \author Adam LUCAS
 * \date 8 févr. 2021
 */

#ifndef S8_LP_REG_H_
#define S8_LP_REG_H_

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdint.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

//input registers defines

#define S8_LP_METER_STATUS_REG        0x00
#define S8_LP_ALARM_STATUS_REG        0x01
#define S8_LP_OUTPUT_STATUS_REG       0x02
#define S8_LP_SPACE_CO2_REG           0x03
#define S8_LP_PWM_OUTPUT_REG          0x15
#define S8_LP_SENSOR_TYPE_ID_H_REG    0x19
#define S8_LP_SENSOR_TYPE_ID_L_REG    0x1A
#define S8_LP_MEM_MAP_VERSION_REG     0x1B
#define S8_LP_FW_VERSION_REG          0x1C
#define S8_LP_SENSOR_ID_H_REG         0x1D
#define S8_LP_SENSOR_ID_L_REG         0x1E

//meter status register bits

#define S8_LP_METER_STATUS_FATAL_ERR  0
#define S8_LP_METER_STATUS_OFFSET_ERR 1
#define S8_LP_METER_STATUS_ALGO_ERR   2 //wrong config
#define S8_LP_METER_STATUS_OUT_ERR    3 //detected errors during output signals calculation
#define S8_LP_METER_STATUS_DIAG_ERR   4 //need zero calibration
#define S8_LP_METER_STATUS_O_RNG_ERR  5 //overload failures, auto reset
#define S8_LP_METER_STATUS_MEM_ERR    6 //error during memory operations

//output status register bits

#define S8_LP_OUTPUT_STATUS_ALM_STAT  0
#define S8_LP_OUTPUT_STATUS_PWM_STAT  1


//holding registers defines

#define S8_LP_ACK_REG                 0x00
#define S8_LP_SPECIAL_COMM_REG        0x01
#define S8_LP_ABC_PERIOD              0x1F //period in hours

//acknowledgement register bits

#define S8_LP_ACK_BACK_CAL_DONE       5
#define S8_LP_ACK_NITRO_CAL_DONE      6

//special command register commands

#define S8_LP_SPECIAL_COMM_VAL        0x7C
#define S8_LP_SPECIAL_COMM_BACK_CAL   0x06
#define S8_LP_SPECIAL_COMM_ZERO_CAL   0x07

//modbus configs

//query

#define MODBUS_QUERY_SLAVE_ADDR       0
#define MODBUS_QUERY_FUNCTION         1
#define MODBUS_QUERY_START_ADDR_H     2
#define MODBUS_QUERY_START_ADDR_L     3
#define MODBUS_QUERY_NUM_REG_H        4
#define MODBUS_QUERY_NUM_REG_L        5
#define MODBUS_QUERY_ERR_CHECK_L      6 //CRC(low)
#define MODBUS_QUERY_ERR_CHECK_H      7 //CRC(high)

//response

#define MODBUS_RESP_SLAVE_ADDR        0
#define MODBUS_RESP_FUNCTION          1
#define MODBUS_RESP_BYTE_COUNT        2
//data location
#define MODBUS_RESP_ERR_CHECK_L       3 //CRC(low)
#define MODBUS_RESP_ERR_CHECK_H       4 //CRC(high)

#define MODBUS_RESP_SIZE(reg_num)     (reg_num * 2 + 5)

//functions codes

#define MODBUS_FUNC_READ_HOLD_REG     0x03
#define MODBUS_FUNC_READ_IN_REG       0x04
#define MODBUS_FUNC_WRITE_SINGLE_REG  0x06

//other

#define MODBUS_DATA_LENGTH_MAX        40  //bytes ?
#define MODBUS_RESPONCE_TIMEOUT       180 //ms
#define MODBUS_DEVICE_ADDR            0xFE //any address

//Modbus CRC16

#define CRC_POLY_16                   0xA001
#define CRC_START_MODBUS              0xFFFF


#define CRC_ALGO_DATA                                                   \
    {                                                                   \
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241, \
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440, \
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40, \
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841, \
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40, \
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41, \
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641, \
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040, \
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240, \
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441, \
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41, \
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840, \
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41, \
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40, \
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640, \
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041, \
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240, \
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441, \
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41, \
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840, \
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41, \
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40, \
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640, \
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041, \
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241, \
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440, \
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40, \
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841, \
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40, \
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41, \
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641, \
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040  \
    }

//delay procedure for calibration

#define S8_LP_400PPM_CAL_DELAY            6 * 1000000  //us
#define S8_LP_0PPM_CAL_DELAY              16 * 1000000 //us
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/*!
 * \enum modbus_reg_t
 * \brief register type (INPUT or HOLDING)
 */

typedef enum {
  INPUT = 0,
  HOLDING = 1,
}modbus_reg_t;

/*!
 * \fn void (*s8_lp_write_ptr)(void *, uint8_t *, uint16_t);
 * \brief UART Transmit function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \param[in] data to transmit buffer
 * \param[in] data length
 * \return None
 */

typedef void (*s8_lp_write_ptr)(void *, uint8_t *, uint16_t);

/*!
 * \fn void (*s8_lp_read_ptr)(void *, uint8_t *, uint16_t);
 * \brief UART Receive function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \param[out] data to receive buffer
 * \param[in] data length
 * \return None
 */

typedef void (*s8_lp_read_ptr)(void *, uint8_t *, uint16_t);

/*!
 * \fn void (*s8_lp_gpio_state_ptr)(void *, bool);
 * \brief GPIO pin change state function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \param[in] pin state (positive logic true : HIGH, false : LOW)
 * \return None
 */

typedef void (*s8_lp_gpio_state_ptr)(void *, bool);

/*!
 * \fn void (*s8_lp_delay)(void *, uint32_t);
 * \brief delay function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \param[in] delay value in µseconds
 * \return None
 */

typedef void (*s8_lp_delay)(void *, uint32_t);

/*!
 * \fn void (*s8_lp_adc)(void *, uint32_t);
 * \brief ADC get value function pointer
 * \param[in] handle data to adapt to pointed function arguments
 * \return ADC value (12 bits)
 */

typedef double (*s8_lp_adc)(void *);

/*!
 * \struct s8_lp_ctx_t
 * \brief function pointers to adapt to lib specifications
 */

typedef struct {
  //!uart

  s8_lp_write_ptr  write_reg;
  s8_lp_read_ptr   read_reg;

  //!gpio MANDATORY : pin is set OUTPUT !!

  s8_lp_gpio_state_ptr co2_cal;

  //!timer delay in µseconds

  s8_lp_delay delay;

  //!adc

  s8_lp_adc co2_vout;

  /** Customizable optional pointer **/
  void *handle;
} s8_lp_ctx_t;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//! functions descriptions : refer to mh_z19b_reg.c

//init function

void s8_lp_init(s8_lp_ctx_t *ctx);

//input registers functions

uint16_t s8_lp_meter_status_get(s8_lp_ctx_t *ctx);
uint16_t s8_lp_output_status_get(s8_lp_ctx_t *ctx);
uint16_t s8_lp_space_co2_get(s8_lp_ctx_t *ctx);
uint16_t s8_lp_pwm_output(s8_lp_ctx_t *ctx);

//holding registers functions

void s8_lp_ack_reg_set(s8_lp_ctx_t *ctx, uint16_t val);
uint16_t s8_lp_ack_reg_get(s8_lp_ctx_t *ctx);
void s8_lp_spe_comm_set(s8_lp_ctx_t *ctx, uint16_t val);
uint16_t s8_lp_spe_comm_get(s8_lp_ctx_t *ctx);
void s8_lp_abc_period_set(s8_lp_ctx_t *ctx, uint16_t val);
uint16_t s8_lp_abc_period_get(s8_lp_ctx_t *ctx);

//calibration / repair functions

void s8_lp_400ppm_cal(s8_lp_ctx_t *ctx);
void s8_lp_0ppm_cal(s8_lp_ctx_t *ctx);

void s8_lp_repair(s8_lp_ctx_t *ctx);

//hardware CO2 acknowledge

uint16_t s8_lp_space_co2_H_get(s8_lp_ctx_t *ctx);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* S8_LP_REG_H_ */
