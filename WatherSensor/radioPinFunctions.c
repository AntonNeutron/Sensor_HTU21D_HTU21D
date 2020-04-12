/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform specific functions in this file ...
* -----------------------------------------------------------------------------
*/

#include <avr/io.h>

#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))

#define PIN_MISO 6
#define PIN_MOSI 5
#define PIN_SCK 7 
#define SPI_PORT PORTB
#define SPI_PIN PINB
#define SPI_DDR DDRB

#define PIN_CE 4
#define PIN_SS 5
#define NRF_PORT PORTD
#define NRF_DDR DDRD

/* ------------------------------------------------------------------------- */
void nrf24_setupPins()
{
    set_bit(NRF_DDR,PIN_CE); // CE output
    set_bit(NRF_DDR,PIN_SS); // CSN output
	
    set_bit(SPI_DDR,PIN_SCK); // SCK output
    set_bit(SPI_DDR,PIN_MOSI); // MOSI output
    clr_bit(SPI_DDR,PIN_MISO); // MISO input
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state) {
        set_bit(NRF_PORT,PIN_CE);
    }  else  {
        clr_bit(NRF_PORT,PIN_CE);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state)
{
    if(state) {
        set_bit(NRF_PORT,PIN_SS);
    } else {
        clr_bit(NRF_PORT,PIN_SS);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state)
{
    if(state) {
        set_bit(SPI_PORT,PIN_SCK);
    } else {
        clr_bit(SPI_PORT,PIN_SCK);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state)
{
    if(state) {
        set_bit(SPI_PORT,PIN_MOSI);
    } else {
        clr_bit(SPI_PORT,PIN_MOSI);
    }
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead()
{
    return check_bit(SPI_PIN,PIN_MISO);
}
/* ------------------------------------------------------------------------- */
