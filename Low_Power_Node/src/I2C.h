/*
 * I2C.h
 *
 *  Created on: 2019¦
 *      Author: santhosh
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_



void I2C_read(I2C_TypeDef *i2c, uint8_t slaveAddr);
void I2C_write(I2C_TypeDef *i2c, uint8_t slaveAddr, uint8_t reg_addr);
void I2C_write_read_byte(I2C_TypeDef *i2c, uint8_t slaveAddr,uint8_t reg_addr);
void I2C_write_read_word(I2C_TypeDef *i2c, uint8_t slaveAddr,uint8_t reg_addr);
void I2C_write_read_multiple_bytes(I2C_TypeDef *i2c, uint8_t slaveAddr,uint8_t reg_addr,int NumberOfBytes);
void I2C_write_write(I2C_TypeDef *i2c, uint8_t slaveAddr,uint8_t reg_addr, uint8_t value);


// parameters for I2C transfer function

extern uint8_t write_buffer[2];
extern uint8_t read_buffer[4];


#endif /* SRC_I2C_H_ */
