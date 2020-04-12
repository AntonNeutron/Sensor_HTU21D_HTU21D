/*
 * WatherSensor.c
 *
 * Created: 08.04.2020 16:33:05
 * Author : Neutron
 */ 
#define F_CPU 8000000UL
// PORTB_MASK для выводов I2C установить в 0!!!
//           биты     76543210
#define DDRB_MASK 	0b10100000	//Настройка разрядов порта x на вход или выход. 0 — вывод работает как ВХОД. 1 - вывод работает на ВЫХОД.
#define PORTB_MASK 	0b01011111	//Управление состоянием входов порта 0 - Hi-Z, 1 - PullUp, если выход 0- лог0, 1- лог1

#define DDRD_MASK 	0b00110000	//Настройка разрядов порта x на вход или выход. 0 — вывод работает как ВХОД. 1 - вывод работает на ВЫХОД.
#define PORTD_MASK 	0b11000100	//Управление состоянием входов порта 0 - Hi-Z, 1 - PullUp, если выход 0- лог0, 1- лог1

#include <avr/io.h>
#include "nrf24.h"
#include "HTU21D.h"
#include <avr/sleep.h>
#include <avr/wdt.h> // здесь организована работа с ватчдогом
#include <avr/interrupt.h>

#include <util/delay.h>

/* ------------------------------------------------------------------------- */
uint8_t temp;
uint8_t q = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
/* ------------------------------------------------------------------------- */
volatile uint16_t htu21_temp, htu21_humd;
volatile uint8_t Cycle;

ISR (WDT_OVERFLOW_vect) {
	
	Cycle++;
	switch  (Cycle) {
		case 1: {
			HTU21D_set_resolution(0x80); //HUMIDITY {10 bits}
			StartMeasurement(TRIGGER_HUMD_MEASURE_NOHOLD);
			wdt_enable(WDTO_30MS); // разрешаем ватчдог 1 сек
			break;
		}
		case 2:{
			htu21_humd = HTU21D_readSensor();
			HTU21D_set_resolution(0x81); //TEMP {11 bits}
			StartMeasurement(TRIGGER_TEMP_MEASURE_NOHOLD);
			break;
		}
		case 3:{
			htu21_temp = HTU21D_readSensor();
			break;
		}		
		case 4:{
					wdt_disable();
			        data_array[0] =  ((htu21_humd & 0xFF00) >> 8);
			        data_array[1] = (htu21_humd & 0xFF);
			        data_array[2] =  ((htu21_temp & 0xFF00) >> 8);
			        data_array[3] = (htu21_temp & 0xFF);
			       // data_array[3] = q++;
					nrf24_send(data_array);
				 /* Wait for transmission to end */
					while(nrf24_isSending());
					/* Or you might want to power down after TX */
					nrf24_powerDown();
					wdt_enable(WDTO_8S); // разрешаем ватчдог 8 сек
					Cycle = 0; // в режиме отладки, работа сенсора через 8 сек, потом нужно убрать отсюда
					break;
					
		}
		case 75:{Cycle = 0;} // примерно через 10 минут сброс цикла
	}
	WDTCR |= (1<<WDIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
	
}


int main(void)
{
	PRR = 0x0D; // Power Reduction Register
    DDRB  = DDRB_MASK;
    PORTB = PORTB_MASK;
    DDRD  = DDRD_MASK;
    PORTD = PORTD_MASK;	
	
	
    /* init the software uart */
   // uart_init();

    /* init the xprintf library */
    //xdev_out(uart_put_char);

    /* simple greeting message */
    //xprintf("\r\n> TX device ready\r\n");
    
    /* init hardware pins */
    nrf24_init();
    
    /* Channel #2 , payload length: 4 */
    nrf24_config(2,4);

    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);
	
	//HTU21D_set_resolution(0x80); //HUMIDITY {10 bits}
	//HTU21D_set_resolution(0x01); //HUMIDITY {8 bits}

        //инициализация ватчдога
        wdt_reset(); // сбрасываем
        wdt_enable(WDTO_30MS); // разрешаем ватчдог 1 сек
        WDTCR |= _BV(WDIE); // разрешаем прерывания по ватчдогу. Иначе будет резет.
        sei(); // разрешаем прерывания
        
        set_sleep_mode(SLEEP_MODE_PWR_DOWN); // если спать - то на полную

    while (1) {
		sleep_enable(); // разрешаем сон
		
		// turn off brown-out enable in software // подумать про Software BOD Disable
		BODCR = (1<< BPDS) | (1<<BPDSE);
		BODCR = (1<< BPDS);
		sleep_cpu(); // спать!
    }
}

