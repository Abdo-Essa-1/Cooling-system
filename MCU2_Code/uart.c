#include "uart.h"
#include <avr\io.h>
#include "common_macros.h"


/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_ConfigType * Configurations){

	uint16 ubrr_value = 0; /*to store calculalted baud rate*/

	UCSRA = (1 << U2X); /*Effectively Doubling the transfer rate for Asynch. Operation*/

	UCSRB = (1 << RXEN) | (1 << TXEN); /*Reciever and Transmitter Enable*/

	UCSRC = (1 << URSEL); /*The URSEL must be one when writing the UCSRC*/

	/*Set parity as input in required bits*/
	UCSRC = (UCSRC & 0XCF) | (((Configurations -> parity) << 4));

	/*Set Stop bit configurations*/
	UCSRC = (UCSRC & 0XF7) | (((Configurations -> stop_bit) << 3));

	/*Set Character Size (bit mode)*/
	UCSRC = (UCSRC & 0XF9) | (((Configurations -> bit_mode) << 1));

	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / ((Configurations -> baud_rate) * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;

}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data){
	/*wait till transmit buffer (UDR) is ready to receive new data*/
	while(BIT_IS_CLEAR(UCSRA, UDRE));

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void){
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA, RXC));

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	return UDR;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str){
	int index = 0;
	/*Sending String*/
	while(Str[index] != '\0'){
		UART_sendByte(Str[index]);
		index++;
	}
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str){ // Receive until #
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
