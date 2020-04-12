/*
 * CFile1.c
 *
 * Created: 09.04.2020 10:59:21
 *  Author: Neutron
 */ 

#include "i2c.h"
#include <util/delay.h>

unsigned char i2c_write(uint8_t d)
{
   uint8_t i;
   unsigned char ask;
		
   for(i = 0; i < 8; i++){
      if(d & 0x80) sda_hi() else sda_low();
	  _delay_us(i2c_time);
      scl_hi();
      _delay_us(i2c_time);
      scl_low();
      d = (d << 1);
   }
			
   sda_hi();
   _delay_us(i2c_time);
   scl_hi();
   _delay_us(i2c_time);
   ask = (sdaPIN & (1<<sda)) ? 0:1;
   scl_low();
   //_delay_us(i2c_time);			
   return ask;
}

uint8_t i2c_read(uint8_t ask)
{
   uint8_t byte = 0;
   uint8_t i;
		
   sda_hi();
		
   for(i = 0; i < 8; i++){
      byte = (byte << 1);
      scl_hi();
      _delay_us(i2c_time);
	  if (sdaPIN & (1<<sda)) byte |= 1;
      scl_low();
	 _delay_us(i2c_time);
   }
			
   if(ask) sda_low() else sda_hi();
   _delay_us(i2c_time);
   scl_hi();
   _delay_us(i2c_time);
   scl_low();
	_delay_us(i2c_time);
   sda_hi();
   return byte;
}



void i2c_start(void)  {
 
  sda_low();      // давим SDA и ставим 0
  _delay_us(i2c_time);  // задержка...
  scl_low();      // давим SCL и ставим 0 
  _delay_us(i2c_time);  // задержка...
}

void i2c_restart_cond (void)	{	
	sda_hi(); // отпустить SDA (лог.1)
	_delay_us(i2c_time);
	scl_hi(); // отпустить SCL (лог.1)
	_delay_us(i2c_time);
	
	sda_low(); // притянуть SDA (лог.0)
	_delay_us(i2c_time);
	scl_low(); // притянуть SCL (лог.0)
	_delay_us(i2c_time);
}

// стоп i2c
void i2c_stop(void)  { // Функция стопа/ Возврашает 0 если все нармально. Возвращает 1 если ошибка на SDA. Возвращает 2 если ошибка на SCL. Если 3, ошибка на обеих линиях.
  // unsigned char error;
   scl_low();
  _delay_us(i2c_time);
   sda_low();
   _delay_us(i2c_time);

   scl_hi();
   _delay_us(i2c_time);
   sda_hi();
   _delay_us(i2c_time);
  _delay_us(i2c_time);
  _delay_us(i2c_time);
  /* error=0;
   if(sdapin == 0) error = 2;
   if(sclpin == 0) error |= 1;
   delay_cycles_10();
   return error;*/
}