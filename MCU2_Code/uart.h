#ifndef UART_H_
#define UART_H_
/*----------------------------------Includes------------------------------*/

#include "std_types.h"

/*---------------------------Definitions---------------------------*/

typedef enum{
	PARITY_DISABLED= 0, PARITY_EVEN = 2, PARITY_ODD = 3
} UART_Parity;

typedef enum{
	STOP_BIT_1 = 0, STOP_BIT_2 = 1
}UART_StopBit;

typedef enum{
	BIT_MODE_5= 0, BIT_MODE_6 = 1,BIT_MODE_7 = 2,BIT_MODE_8 = 3
}UART_CharacterSize;

typedef struct {
	uint32 baud_rate;
	UART_Parity parity;
	UART_StopBit stop_bit;
	UART_CharacterSize bit_mode;
}UART_ConfigType;

/*---------------------------------Functions Definitions-------------------------*/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_ConfigType * Configurations);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #



#endif /* UART_H_ */
