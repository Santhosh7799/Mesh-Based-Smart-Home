/*
 * state_machine.c
 *
 *  Created on: 2019¦~2¤ë27¤é
 *      Author: tim01
 */

#include "state_machine_params.h"
#include "em_core.h"
#include "sleep.h"
#include "I2C.h"
#include "ambient_light.h"
#include "log.h"
#include "gatt_db.h"
#include "display.h"
#include "infrastructure.h"
#include "scheduler.h"
#include "gas_sensor.h"
//#include "gecko_native.h"


uint16_t Channel0_Data =0;
uint16_t Channel1_Data =0;
uint8_t status;
double lux_value = 0;
int lux_percentage =0;

#define true	1
#define false	0

#define enable_Log	1
// state machine flag
	volatile STATE_T next_state = STATE_SENSOR_POWEROFF;
	volatile STATE_T current_state = STATE_SENSOR_POWEROFF;
	volatile uint8_t event = NO_EVENT;
	volatile uint8_t connection_flag = BLE_DISCONNECT;
state_stop_flag = 0;
void state_machine()
{
	//LOG_INFO("state_machine in case %d and next state is %d\n",current_state, next_state);
	switch(current_state)
	{
		case STATE_SENSOR_POWEROFF:
			if(event & LETIMER0_UF_FLAG)
			{
				TempSensorSet(true);
				CORE_CRITICAL_SECTION(
						event &= ~(LETIMER0_UF_FLAG);
				);
				Gas_Sensor_enable();
				next_state =STATE_WAITFOR_GAS_SENSOR_ENABLE;

			}
		break;

		case STATE_WAITFOR_GAS_SENSOR_ENABLE:
			if(event & I2C_TRANSFER_COMPLETE)
									{
										CORE_CRITICAL_SECTION(
												event &= ~(I2C_TRANSFER_COMPLETE);
										);
										Gas_Sensor_DriveModeEnable();
										next_state = STATE_WAITFOR_GAS_SENSOR_DRIVE_MODE_ENABLE;
									}
									else if(event & I2C_TRANSFER_ERROR){
										CORE_CRITICAL_SECTION(
												event &= ~(I2C_TRANSFER_ERROR);
										);
										Gas_Sensor_enable();
										next_state = STATE_WAITFOR_GAS_SENSOR_ENABLE;
									}
			break;

	   case STATE_WAITFOR_GAS_SENSOR_DRIVE_MODE_ENABLE:
		   if(event & I2C_TRANSFER_COMPLETE)
		   									{
		   										CORE_CRITICAL_SECTION(
		   												event &= ~(I2C_TRANSFER_COMPLETE);
		   										);
		   										Wait_usec_c(1500000);
		   										next_state = STATE_WAITFOR_GAS_SENSOR_DATAREADY;
		   									}
		   									else if(event & I2C_TRANSFER_ERROR){
		   										CORE_CRITICAL_SECTION(
		   												event &= ~(I2C_TRANSFER_ERROR);
		   										);
		   										Gas_Sensor_DriveModeEnable();
		   										next_state = STATE_WAITFOR_GAS_SENSOR_DRIVE_MODE_ENABLE;
		   									}
		   break;

	   case STATE_WAITFOR_GAS_SENSOR_DATAREADY:
		                                     if(event & LETIMER0_COMP1_FLAG)
                                          	{
		                                    	 	CORE_CRITICAL_SECTION(
		                                    	 	event &= ~(LETIMER0_COMP1_FLAG);
		                                    	 	);
		                                    	 	Gas_Sensor_StatusCheck();
		                                    	 	next_state = STATE_GAS_SENSOR_READVALUE;
                                          	}
		                                    break;
	   case STATE_GAS_SENSOR_READVALUE:
	  		   if(event & I2C_TRANSFER_COMPLETE)
	  		   									{
	  		   										CORE_CRITICAL_SECTION(
	  		   												event &= ~(I2C_TRANSFER_COMPLETE);
	  		   										);
	  		   									status =  read_buffer[0];
	  		   							       LOG_INFO("staus is %d\n",status);
	  		   									Gas_Sensor_read_data();
	  		   									next_state = STATE_GET_GAS_SENSOR_STATE;

	  		   									}
	  		   									else if(event & I2C_TRANSFER_ERROR){
	  		   										CORE_CRITICAL_SECTION(
	  		   												event &= ~(I2C_TRANSFER_ERROR);
	  		   										);
	  		   									    	Gas_Sensor_StatusCheck();
	  		   										next_state = STATE_GAS_SENSOR_READVALUE;
	  		   									}
	  		   break;

	   case STATE_GET_GAS_SENSOR_STATE:
			if(event & I2C_TRANSFER_COMPLETE)
			{
				CORE_CRITICAL_SECTION(
						event &= ~(I2C_TRANSFER_COMPLETE);
				);
				Calculate_eCO2_TVOC_Values();
				Calculate_Gas_state();
				Gas_Sensor_disable();
				displayPrintf(DISPLAY_ROW_TEMPVALUE, "GAS STATE %d",Gas_state);
				gecko_external_signal(GAS_FLAG);
				state_store_LPN(GAS_FLAG);
				next_state = SENSOR_POWEROFF;
			}
				else if(event & I2C_TRANSFER_ERROR){
											CORE_CRITICAL_SECTION(
													event &= ~(I2C_TRANSFER_ERROR);
											);
											Gas_Sensor_read_data();
					                        next_state = STATE_GET_GAS_SENSOR_STATE;
										}
				break;

		case SENSOR_POWEROFF:
				if(event & LETIMER0_UF_FLAG)
				{
					CORE_CRITICAL_SECTION(
							event &= ~(LETIMER0_UF_FLAG);
					);
					Ambient_Light_enable();
					next_state = SENSOR_WAITFORSENSORENABLE;

				}

			break;

		case SENSOR_WAITFORSENSORENABLE:
			if(event & I2C_TRANSFER_COMPLETE)
						{
							CORE_CRITICAL_SECTION(
									event &= ~(I2C_TRANSFER_COMPLETE);
							);

							Wait_usec_c(420000);
							next_state = SENSOR_WAITFORPOWERUP;
						}
						else if(event & I2C_TRANSFER_ERROR){
							CORE_CRITICAL_SECTION(
									event &= ~(I2C_TRANSFER_ERROR);
							);
							Ambient_Light_enable();
							next_state = SENSOR_WAITFORSENSORENABLE;
						}
			break;

		case SENSOR_WAITFORPOWERUP:
			if(event & LETIMER0_COMP1_FLAG)
			{
				CORE_CRITICAL_SECTION(
						event &= ~(LETIMER0_COMP1_FLAG);
				);

				I2C_write_read_word(I2C0,AMBIENT_LIGHT_SLAVE_ADDRESS,AMBIENT_LIGHT_COMMAND_REGISTER | AMBIENT_LIGHT_WORD_ENABLE | AMBIENT_LIGHT_CHANNEL0_REGISTER);
				next_state = I2C_WAITFORWRITECOMPLETE;
			}
			break;
		case I2C_WAITFORWRITECOMPLETE:

			if(event & I2C_TRANSFER_COMPLETE)
			{
				CORE_CRITICAL_SECTION(
						event &= ~(I2C_TRANSFER_COMPLETE);
				);

				Channel0_Data = read_channel_data();
				LOG_INFO("Read Channel0data  %d\n",Channel0_Data);
				I2C_write_read_word(I2C0,AMBIENT_LIGHT_SLAVE_ADDRESS,AMBIENT_LIGHT_COMMAND_REGISTER | AMBIENT_LIGHT_WORD_ENABLE | AMBIENT_LIGHT_CHANNEL1_REGISTER);
				next_state = I2C_WAITFORREADCOMPLETE;
			}
			else if(event & I2C_TRANSFER_ERROR){
				CORE_CRITICAL_SECTION(
						event &= ~(I2C_TRANSFER_ERROR);
				);
				I2C_write_read_word(I2C0,AMBIENT_LIGHT_SLAVE_ADDRESS,AMBIENT_LIGHT_COMMAND_REGISTER | AMBIENT_LIGHT_WORD_ENABLE | AMBIENT_LIGHT_CHANNEL0_REGISTER);
				next_state = I2C_WAITFORWRITECOMPLETE;
			}

			break;
		case I2C_WAITFORREADCOMPLETE:

			if(event & I2C_TRANSFER_COMPLETE)
			{


				CORE_CRITICAL_SECTION(
						event &= ~(I2C_TRANSFER_COMPLETE);
				);
				Channel1_Data = read_channel_data();
				LOG_INFO("Read Channel1data %d\n",Channel1_Data);
			    lux_value = getLuminosityValue(Channel0_Data, Channel1_Data);
			    lux_percentage = getbrightnesspercentage(lux_value);
				LOG_INFO("Read LUX VALUE  %lf\n",lux_value);
				displayPrintf(DISPLAY_ROW_ACTION, "LIGHTVAL %d",lux_percentage);
				Ambient_Light_disable();
				state_store_LPN(LUX_FLAG);
				gecko_external_signal(LUX_FLAG);
				next_state = STATE_SENSOR_POWEROFF;
			}
			else if(event & I2C_TRANSFER_ERROR){
				CORE_CRITICAL_SECTION(
						event &= ~(I2C_TRANSFER_ERROR);
				);
				I2C_write_read_word(I2C0,AMBIENT_LIGHT_SLAVE_ADDRESS,AMBIENT_LIGHT_COMMAND_REGISTER | AMBIENT_LIGHT_WORD_ENABLE | AMBIENT_LIGHT_CHANNEL1_REGISTER);
			    next_state = I2C_WAITFORREADCOMPLETE;
			}
			break;
	}

	if((current_state != next_state) && (state_stop_flag==0)){
		gecko_external_signal(TEMPREAD_FLAG);
		if(enable_Log)LOG_INFO("State Transition:%d to %d",current_state,next_state);
		current_state = next_state;

	}
}



