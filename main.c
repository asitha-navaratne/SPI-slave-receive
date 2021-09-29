#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

void PORT_INIT(void);
void SPI_SLAVE_INIT(void);
char * SPI_RECEIVE_STRING(unsigned char acknowledge,uint8_t datasize);

int main(void){
	DDRA = 0xFF;
	
	PORT_INIT();
	SPI_SLAVE_INIT();
	
	unsigned char acknowledge = 0x41;					///< ACK byte.
	uint8_t datasize = 12;							///< Size of data to be received.
	
	char *receive = SPI_RECEIVE_STRING(acknowledge,datasize);
	
	for(uint8_t i=0;receive[i]!=0;i++){
		PORTA = receive[i];
		_delay_ms(1000);
	}
}

/*!
 *	@brief Initialize ports.
 */

void PORT_INIT(void){
	DDRB &= ~((1<<PB4)|(1<<PB5)|(1<<PB7));					///< Set MOSI, SCK and SS pins to input.
	DDRB |= (1<<PB6);							///< Set MISO pin to output.
}

/*!
 *	@brief Initialize SPI in slave mode.
 */

void SPI_SLAVE_INIT(void){
	SPCR = (1<<SPE);							///< Enable SPI in Slave SPI mode.
}

/*!
 *	@brief Receive a message over SPI.
 *	@param ACK byte (unsigned char) and size of string received (uint8_t).
 *	@return Received data (char *).
 */

char * SPI_RECEIVE_STRING(unsigned char acknowledge,uint8_t datasize){
	char *message = "";
	for(uint8_t i=0;i<datasize;i++){
		SPDR = acknowledge;						///< Load acknowledge byte to SPDR register to shift to master.
		while(!(SPSR & (1<<SPIF)));					///< Wait until whole data byte is received.
		message[i] = SPDR;
	}
	return message;								///< Return the received string.
}
