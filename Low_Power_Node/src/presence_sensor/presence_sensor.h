/*
 * presence_detector.h
 *
 *  Created on: 2019�~4��22��
 *      Author: tim01
 */

#ifndef SRC_PRESENCE_SENSOR_PRESENCE_SENSOR_H_
#define SRC_PRESENCE_SENSOR_PRESENCE_SENSOR_H_


#include "em_gpio.h"

//this pin is used for powering the presence_detection_ir_beam
#define PRESENCE_DETECTION_ENABLE_PORT     gpioPortA
#define PRESENCE_DETECTION_ENABLE_PIN      3

//bool presence_state = 0;

void presence_init(void);
void presence_enable(void);
void presence_disable(void);
void enable_presence_interrupt(void);
void disable_presence_interrupt(void);

#endif /* SRC_PRESENCE_DETECTION_H_ */
