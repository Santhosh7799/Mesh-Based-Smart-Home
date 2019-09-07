/*
 * smoke_sensor.h
 *
 *  Created on: Apr 28, 2019
 *      Author: santhosh
 */



#ifndef SRC_GAS_SENSOR_H_
#define SRC_GAS_SENSOR_H_


#include <stdbool.h>
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "i2cspm.h"
#include "I2C.h"
#include "gpio.h"
#include "log.h"

#define GAS_SENSOR_ENABLE_PORT gpioPortD
#define GAS_SENSOR_ENABLE_PIN  11

#define GAS_SENSOR_SLAVE_ADDRESS           0x5A
#define GAS_SENSOR_STATUS_REGISTER         0x00
#define GAS_SENSOR_MEAS_MODE_REGISTER      0x01
#define GAS_SENSOR_ALG_RESULT_DAT_REGISTER 0x02
#define GAS_SENSOR_HW_ID_REGISTER          0x020
#define GAS_SENSOR_HW_VERSION_REGISTER     0x21
#define CCS811_BOOTLOADER_APP_START        0xF4


#define GAS_SENSOR_DRIVEMODE_ENABLE        0x10
#define GAS_SENSOR_CO2_TVOC_BYTES          0X04

extern uint16_t Gas_state;


void Gas_Sensor_enable();
void Gas_Sensor_StatusCheck();
void Gas_Sensor_DriveModeEnable();
void Gas_Sensor_read_data();
void Calculate_eCO2_TVOC_Values();
void Calculate_Gas_state();
void Gas_Sensor_disable();



#endif /* SRC_GAS_SENSOR_H_ */
