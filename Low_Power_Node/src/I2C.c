/*
 * I2C.c
 *
 *  Created on: 2019¦~2¤ë27¤é
 *      Author: tim01
 */

#include "em_i2c.h"
#include "I2C.h"
#include "sleep.h"
#include "state_machine_params.h"
#include "log.h"
#include "gpio.h"

// parameters for I2C transfer function
I2C_TransferSeq_TypeDef seq;

uint8_t write_buffer[2]={0,0};
uint8_t read_buffer[4]={0,0,0,0};

// Initialize I2CSPM & seq.device_address
void i2c_Init(void)
{
//	gpioTmpSenSetOff();
	uint16_t device_address = 0x39;
	seq.addr = device_address<<1;
	NVIC_EnableIRQ(I2C0_IRQn);
	gpioI2CSetOn();
}




void I2C_read(I2C_TypeDef *i2c, uint8_t slaveAddr)
{
	//since using 7 -bit address mode, we shift the address
	seq.addr = slaveAddr<<1;

	seq.flags = I2C_FLAG_READ;
	seq.buf[0].data = read_buffer;
	seq.buf[0].len = 1;
	I2C_TransferInit(i2c, &seq);

}

void I2C_write(I2C_TypeDef *i2c, uint8_t slaveAddr, uint8_t reg_addr)
{

	//since using 7 -bit address mode, we shift the address
	seq.addr = slaveAddr<<1;
	seq.flags = I2C_FLAG_WRITE;
	write_buffer[0] =  reg_addr;
	seq.buf[0].data = write_buffer;
	seq.buf[0].len = 1;
	I2C_TransferInit(i2c, &seq);
}

void I2C_write_read_byte(I2C_TypeDef *i2c, uint8_t slaveAddr,uint8_t reg_addr)
{

		//since using 7 -bit address mode, we shift the address
		seq.addr = slaveAddr<<1;

		write_buffer[0] =  reg_addr;
		seq.buf[0].data = write_buffer;
		seq.buf[0].len = 1;

		seq.flags = I2C_FLAG_WRITE_READ;
		seq.buf[1].data = read_buffer;
		seq.buf[1].len = 1;
		I2C_TransferInit(i2c, &seq);
}


void I2C_write_read_word(I2C_TypeDef *i2c, uint8_t slaveAddr,uint8_t reg_addr)
{

		//since using 7 -bit address mode, we shift the address
		seq.addr = slaveAddr<<1;

		write_buffer[0] =  reg_addr;
		seq.buf[0].data = write_buffer;
		seq.buf[0].len = 1;

		seq.flags = I2C_FLAG_WRITE_READ;
		seq.buf[1].data = read_buffer;
		seq.buf[1].len = 2;
		I2C_TransferInit(i2c, &seq);
}

void I2C_write_read_multiple_bytes(I2C_TypeDef *i2c, uint8_t slaveAddr,uint8_t reg_addr,int NumberOfBytes)
{

		//since using 7 -bit address mode, we shift the address
		seq.addr = slaveAddr<<1;

		write_buffer[0] =  reg_addr;
		seq.buf[0].data = write_buffer;
		seq.buf[0].len = 1;

		seq.flags = I2C_FLAG_WRITE_READ;
		seq.buf[1].data = read_buffer;
		seq.buf[1].len = NumberOfBytes;
		I2C_TransferInit(i2c, &seq);
}


void I2C_write_write(I2C_TypeDef *i2c, uint8_t slaveAddr,uint8_t reg_addr, uint8_t value)
{

		//since using 7 -bit address mode, we shift the address
		seq.addr = (uint16_t)((slaveAddr<<1) & ((uint8_t)0xFE));

		write_buffer[0] =  reg_addr;
		seq.buf[0].data = write_buffer;
		seq.buf[0].len = 1;

		seq.flags = I2C_FLAG_WRITE_WRITE;
	    write_buffer[1] = value;
		seq.buf[1].data = write_buffer+1;
		seq.buf[1].len = 1;

		I2C_TransferInit(i2c, &seq);

}


void I2C0_IRQHandler(void)
{
	I2C_TransferReturn_TypeDef ret = I2C_Transfer(I2C0);

	if(ret == i2cTransferDone)
		{
			event |= I2C_TRANSFER_COMPLETE;
		}
	else if(ret != i2cTransferInProgress)
		{
			LOG_ERROR("I2C Error %d",ret);
			event |= I2C_TRANSFER_ERROR;
		}
}
