/*
 * ambient_light.c
 *
 *  Created on: Apr 20, 2019
 *      Author: santhosh
 */
#include "ambient_light.h"
#include "log.h"
#include "wait_funct.h"







void Ambient_Light_I2C_Init()
{

		I2CSPM_Init_TypeDef init_ambient_light ={
				I2C0,                       /* Use I2C instance 0 */                       \
			    gpioPortC,                  /* SCL port */                                 \
			    10,                         /* SCL pin */                                  \
			    gpioPortC,                  /* SDA port */                                 \
			    11,                         /* SDA pin */                                  \
			    14,                         /* Location of SCL */                          \
			    16,                         /* Location of SDA */                          \
			    0,                          /* Use currently configured reference clock */ \
			    I2C_FREQ_STANDARD_MAX,      /* Set to standard rate  */                    \
			    i2cClockHLRStandard,        /* Set to use 4:4 low/high duty cycle */       \
			  };
		I2CSPM_Init(&init_ambient_light);

		LOG_INFO("temperature sensor is initialized");
}



void Ambient_Light_enable()
{
	GPIO_PinOutSet(AMBIENT_LIGHT_ENABLE_PORT, AMBIENT_LIGHT_ENABLE_PIN);
	gpioI2CSetOn();
	I2C_write_write(I2C0,AMBIENT_LIGHT_SLAVE_ADDRESS, AMBIENT_LIGHT_COMMAND_REGISTER | AMBIENT_LIGHT_CONTROL_REGISTER,AMBIENT_LIGHT_POWERON_ENABLE );
}

void Ambient_Light_disable()
{
	gpioI2CSetOff();
	GPIO_PinOutClear(AMBIENT_LIGHT_ENABLE_PORT, AMBIENT_LIGHT_ENABLE_PIN);

}

uint16_t read_channel_data()
{
	uint16_t data;
	data =  read_buffer[1]<<8 | read_buffer[0];
	return data;
}


// This function is obtained from the data-sheet to calculate the lux values;
double getLuminosityValue(uint16_t Channel0_Data,uint16_t Channel1_Data)
{
	double ratio, lux = 0;

	ratio = Channel0_Data/Channel0_Data;

	Channel0_Data =  Channel0_Data *16;
	Channel1_Data = Channel1_Data*16;

	if (ratio > 0 && ratio <= 0.50)
	{
		lux = 0.0304*Channel0_Data - 0.062*Channel0_Data*(pow(ratio, 1.4));
	}
	else if (ratio > 0.50 && ratio <= 0.61)
	{
		lux = 0.0224*Channel0_Data - 0.031*Channel1_Data;
	}
	else if (ratio > 0.61 && ratio <= 0.80)
	{
		lux = 0.0128*Channel0_Data - 0.0153*Channel1_Data;
	}
	else if (ratio > 0.80 && ratio <= 1.30)
	{
		lux = 0.00146*Channel0_Data - 0.00112*Channel1_Data;
	}
	else if (ratio > 1.30)
	{
		lux = 0;
	}

	return lux;
}

int getbrightnesspercentage(double lux_value)
{
	int brightness_value;
	if(lux_value >= 2.5)
	{
		brightness_value =100;
	}
	else if(lux_value >= 2 )
		{
			brightness_value = 90;
		}
	else if(lux_value >= 2 )
	   {
				brightness_value = 80;
		}
	else if(lux_value >= 1)
		{
			brightness_value =60;
		}
	else if(lux_value >= 0.8)
		{
			brightness_value =50;
		}
	else if(lux_value >= 0.5 )
		{
			brightness_value =40;
		}
	else if(lux_value >= 0.3)
			{
				brightness_value =20;
			}
	else if(lux_value >= 0.1)
			{
				brightness_value =10;
			}
	 else
			{
				brightness_value =5;
			}
	return brightness_value;
}
