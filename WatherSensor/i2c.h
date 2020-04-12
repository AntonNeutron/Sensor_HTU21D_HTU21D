/*
 * i2c.h
 *
 * Created: 09.04.2020 11:10:28
 *  Author: Neutron
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <avr/io.h>

// Обязательно внешние подтягиваюшие сопротивления, PORT для выводов установить в 0!!!
#define sda 		1 // порт для SDA
#define scl 		0 // порт для SCL
#define	sdaDDR		DDRD
#define	sclDDR		DDRD
#define	sdaPIN		PIND
#define	i2c_time	10

#define sda_hi()   {sdaDDR &= ~(1<<sda);}  // переводим в 1 линию SDA для этого переводим его как вход DDR = 0
#define sda_low()  {sdaDDR |= (1<<sda);}  // переводим в 0 линию SDA, переводим в выход DDR=1
#define scl_hi()   {sclDDR &= ~(1<<scl);}  // переводим в 1 линию SCL
#define scl_low()  {sclDDR |= (1<<scl);}  // переводим в 0 линию SCL

unsigned char i2c_write(uint8_t d);
uint8_t i2c_read(uint8_t ask);
void i2c_start(void);
void i2c_restart_cond (void);
void i2c_stop(void);

#endif /* I2C_H_ */