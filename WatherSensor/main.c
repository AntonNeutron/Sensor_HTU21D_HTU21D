/*
 * WatherSensor.c
 *
 * Created: 08.04.2020 16:33:05
 * Author : Neutron
 */ 
#define F_CPU 8000000UL
// PORTB_MASK ��� ������� I2C ���������� � 0!!!
//           ����     76543210
#define DDRB_MASK 	0b10100000	//��������� �������� ����� x �� ���� ��� �����. 0 � ����� �������� ��� ����. 1 - ����� �������� �� �����.
#define PORTB_MASK 	0b01011111	//���������� ���������� ������ ����� 0 - Hi-Z, 1 - PullUp, ���� ����� 0- ���0, 1- ���1

#define DDRD_MASK 	0b00110000	//��������� �������� ����� x �� ���� ��� �����. 0 � ����� �������� ��� ����. 1 - ����� �������� �� �����.
#define PORTD_MASK 	0b11000100	//���������� ���������� ������ ����� 0 - Hi-Z, 1 - PullUp, ���� ����� 0- ���0, 1- ���1

#include <avr/io.h>
#include "nrf24.h"
#include "HTU21D.h"
#include <avr/sleep.h>
#include <avr/wdt.h> // ����� ������������ ������ � ���������
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
			wdt_enable(WDTO_30MS); // ��������� ������� 1 ���
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
					wdt_enable(WDTO_8S); // ��������� ������� 8 ���
					Cycle = 0; // � ������ �������, ������ ������� ����� 8 ���, ����� ����� ������ ������
					break;
					
		}
		case 75:{Cycle = 0;} // �������� ����� 10 ����� ����� �����
	}
	WDTCR |= (1<<WDIE); // ��������� ���������� �� ��������. ����� ����� �����.
	
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

        //������������� ��������
        wdt_reset(); // ����������
        wdt_enable(WDTO_30MS); // ��������� ������� 1 ���
        WDTCR |= _BV(WDIE); // ��������� ���������� �� ��������. ����� ����� �����.
        sei(); // ��������� ����������
        
        set_sleep_mode(SLEEP_MODE_PWR_DOWN); // ���� ����� - �� �� ������

    while (1) {
		sleep_enable(); // ��������� ���
		
		// turn off brown-out enable in software // �������� ��� Software BOD Disable
		BODCR = (1<< BPDS) | (1<<BPDSE);
		BODCR = (1<< BPDS);
		sleep_cpu(); // �����!
    }
}

