/*
 * HTU21D.c
 *
 * Created: 09.04.2020 16:59:58
 *  Author: Neutron
 */ 
#include "i2c.h"
#include "HTU21D.h"

void StartMeasurement(uint8_t cmd) {
	i2c_start();
	i2c_write((HTDU21D_ADDRESS << 1) | 0);
	i2c_write(cmd);
	i2c_stop();
}


void HTU21D_set_resolution(uint8_t sensorResolution){
uint8_t userRegister;

	i2c_start();
	//Request the user register
	i2c_write((HTDU21D_ADDRESS << 1) | 0); //addr + write
	i2c_write(READ_USER_REG);//write cmd "Read the user register"
	i2c_stop();
	i2c_start();
	i2c_write((HTDU21D_ADDRESS << 1) | 1); //addr + read
	userRegister = i2c_read(0);
	i2c_stop();
	/*----------------------------*/
	userRegister &= 0x7E;                             //clears current resolution bits with 0
	userRegister |= sensorResolution;                 //adds new resolution bits to user register byte
	/*----------------------------*/
	i2c_start();
	i2c_write((HTDU21D_ADDRESS << 1) | 0); //addr + write
	i2c_write(WRITE_USER_REG);//write cmd "Write the user register"
	i2c_write(userRegister);
	i2c_stop();
}

uint16_t HTU21D_readSensor(void){
	uint16_t result;
	i2c_start();
	i2c_write((HTDU21D_ADDRESS << 1) | 1);
	result = (i2c_read(1)) << 8;
	result |= i2c_read(0);
	i2c_stop();
	result &= ~0x0003;   // clear two low bits (status bits)
	return result;
}