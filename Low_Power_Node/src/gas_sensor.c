/*
 * smoke_sensor.c
 *
 *  Created on: Apr 28, 2019
 *      Author: santhosh
 */

#ifndef SRC_SMOKE_SENSOR_C_
#define SRC_SMOKE_SENSOR_C_
#include "gas_sensor.h"

uint16_t eCO2_Value;
uint16_t TVOC_Value;
uint16_t Gas_state =0;


void Gas_Sensor_enable()
{
	//GPIO_PinOutSet(GAS_SENSOR_ENABLE_PORT, GAS_SENSOR_ENABLE_PIN);
	gpioI2CSetOn();
	I2C_write(I2C0, GAS_SENSOR_SLAVE_ADDRESS, CCS811_BOOTLOADER_APP_START);
}

void Gas_Sensor_StatusCheck()
{
   I2C_write_read_byte(I2C0, GAS_SENSOR_SLAVE_ADDRESS,GAS_SENSOR_STATUS_REGISTER);
}

void Gas_Sensor_DriveModeEnable()
{
	I2C_write_write(I2C0,GAS_SENSOR_SLAVE_ADDRESS,GAS_SENSOR_MEAS_MODE_REGISTER,GAS_SENSOR_DRIVEMODE_ENABLE);
}

void Gas_Sensor_disable()
{
	gpioI2CSetOff();
	GPIO_PinOutClear(GAS_SENSOR_ENABLE_PORT, GAS_SENSOR_ENABLE_PIN);

}
void Gas_Sensor_read_data()
{
	I2C_write_read_multiple_bytes(I2C0, GAS_SENSOR_SLAVE_ADDRESS,GAS_SENSOR_ALG_RESULT_DAT_REGISTER,GAS_SENSOR_CO2_TVOC_BYTES);
}
void Calculate_eCO2_TVOC_Values()
{
	eCO2_Value =  read_buffer[1]<<8 | read_buffer[0];
	TVOC_Value =  read_buffer[3]<<8 | read_buffer[2];
	LOG_INFO("The eCO2_value obtained is %d\n",eCO2_Value );
	LOG_INFO("The TVOC_Value obtained is %d\n",eCO2_Value );
}

void Calculate_Gas_state()
{
	if(eCO2_Value > 600 | TVOC_Value > 300 )
	{
		Gas_state =1;
	}
	else
	{
		Gas_state =0;
	}
	LOG_INFO("The GAS_STATE obtained is %d\n",Gas_state);
}

#endif /* SRC_SMOKE_SENSOR_C_ */
