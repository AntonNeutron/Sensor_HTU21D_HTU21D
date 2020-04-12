/*
 * HTU21D.h
 *
 * Created: 09.04.2020 17:03:21
 *  Author: Neutron
 */ 


#ifndef HTU21D_H_
#define HTU21D_H_

#define HTDU21D_ADDRESS				0x40  //Unshifted 7-bit I2C address for the sensor
#define TRIGGER_TEMP_MEASURE_HOLD   0xE3
#define TRIGGER_HUMD_MEASURE_HOLD   0xE5
#define TRIGGER_TEMP_MEASURE_NOHOLD 0xF3
#define TRIGGER_HUMD_MEASURE_NOHOLD 0xF5

#define WRITE_USER_REG              0xE6
#define READ_USER_REG               0xE7
#define SOFT_RESET                  0xFE

#define ERROR_CRC                   998
#define ERROR_TIMEOUT               999


void StartMeasurement(uint8_t cmd);
void HTU21D_set_resolution(uint8_t sensorResolution);
uint16_t HTU21D_readSensor(void);

#endif /* HTU21D_H_ */