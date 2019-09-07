/*
 * ambient_light.h
 *
 *  Created on: Apr 20, 2019
 *      Author: santhosh
 */
#include <stdbool.h>
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "i2cspm.h"
#include "I2C.h"
#include "gpio.h"


#ifndef SRC_AMBIENT_LIGHT_H_
#define SRC_AMBIENT_LIGHT_H_

#define AMBIENT_LIGHT_ENABLE_PORT gpioPortA
#define AMBIENT_LIGHT_ENABLE_PIN  2

#define AMBIENT_LIGHT_SLAVE_ADDRESS      0x39
#define AMBIENT_LIGHT_COMMAND_REGISTER   0x80
#define AMBIENT_LIGHT_CONTROL_REGISTER   0x00
#define AMBIENT_LIGHT_TIMING_REGISTER    0x01
#define AMBIENT_LIGHT_DEVICEID_REGISTER  0x0A
#define AMBIENT_LIGHT_CHANNEL0_REGISTER  0x0C
#define AMBIENT_LIGHT_CHANNEL1_REGISTER  0x0E

#define AMBIENT_LIGHT_WORD_ENABLE        0x02
#define AMBIENT_LIGHT_POWERON_ENABLE     0x03


void Ambient_Light_enable();
uint16_t read_channel_data();
double getLuminosityValue(uint16_t Channel0_Data,uint16_t Channel1_Data);
void Ambient_Light_I2C_Init();
void Ambient_Light_disable();
int getbrightnesspercentage(double );




#endif /* SRC_AMBIENT_LIGHT_H_ */
