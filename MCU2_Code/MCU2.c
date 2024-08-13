/*
 ============================================================================
 Name        : MCU2.c
 Author      : Abdelrahman Essa
 Date        : 11/8/2024
 Description : Formula Team Task, Micro-controller 2
 ============================================================================
 */

#include "std_types.h"
#include "gpio.h"
#include "adc.h"
#include "motor.h"
#include "uart.h"
#include "lcd.h"
#include "timer1.h"
#include <util/delay.h>

int main()
{
	/********** Initializations **********/
	// ADC Initialization
	ADC_ConfigType ADCinit = {INTERNAL_AVCC, PRESCALAR_128};					// Setting the ADC reference to Internal and the prescalar to 128
	ADC_init(&ADCinit);															// Initializing the Settings to the ADC

	// LCD initialization
	LCD_init();
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Received: ");

	// DC Motor Initialization (Fan)
	DcMotor_Init();																// Initializing the motor pins
	DcMotor_Rotate(MOTOR_STOP, 0);												// Setting the motor initially to stop

	// Buzzer Initialization
	GPIO_setupPinDirection(PORTD_ID, PIN3_ID, PIN_OUTPUT);						// Setting PD3 as o/p Pin (Buzzer)
	GPIO_writePin(PORTD_ID, PIN3_ID, LOGIC_LOW);								// Setting the buzzer initially stop.

	// LEDs Initializations
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);						// Setting PB5 as o/p Pin (Green LED)
	GPIO_setupPinDirection(PORTB_ID, PIN6_ID, PIN_OUTPUT);						// Setting PB6 as o/p Pin (Yellow LED)
	GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_OUTPUT);						// Setting PB7 as o/p Pin (Red LED)
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);								// Setting the Green LED initially OFF.
	GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_LOW);								// Setting the Yellow LED initially OFF.
	GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_LOW);								// Setting the Red LED initially OFF.

	// Creating variable to receive Temperature
	uint8 temp = 0;

	// Creating Machine Speed Variable
	uint16 MoSpeed = 0;

	// UART Initialization
	UART_ConfigType Uconfig = {9600, PARITY_DISABLED, STOP_BIT_1, BIT_MODE_8};
	UART_init(&Uconfig);

	// Servo Initialization
	Timer1_Servo_Init();
	Set_Servo_Angle(0);

	/************** Program **************/
	for(;;)
	{
		// Motor speed change
		MoSpeed = ADC_readChannel(PIN0_ID);										// Storing the potentiometer current reading
		MoSpeed = (uint16)(((float)MoSpeed/1023.0) * 100.0);					// Converting the reading to the percentage of speed to the Machine
		DcMotor_Rotate(MOTOR_CW, MoSpeed);										// Changing the speed of the motor

		// Receiving the temperature
		temp = UART_recieveByte();												// Receiving the temperature from MCU1
		LCD_moveCursor(0, 10);
		LCD_intgerToString(temp);
		_delay_ms(10);
		LCD_displayStringRowColumn(0,10,"   ");
		if(temp == 220)
		{
			LCD_moveCursor(0, 10);
			LCD_intgerToString(temp);
			GPIO_writePin(PORTD_ID, PIN3_ID, LOGIC_HIGH);						// Turning the buzzer ON
			GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);						// Turning the Green LED OFF
			GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_LOW);						// Turning the Yellow LED OFF
			GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_HIGH);						// Turning the Red LED ON
			DcMotor_Rotate(MOTOR_STOP, 0);
			Set_Servo_Angle(90);
			_delay_ms(5000);													// MCU2 Enters abnormal state for 5 seconds
			Set_Servo_Angle(0);
		}
		else if(temp == 200)
		{
			DcMotor_Rotate(MOTOR_STOP, 0);
			temp = UART_recieveByte();											// Receiving the temperature from MCU1
			while(40 < temp)
			{
				temp = UART_recieveByte();										// Receiving the temperature from MCU1
				LCD_moveCursor(0, 10);
				LCD_intgerToString(temp);
			}
		}
		else if(temp <= 20)														// If the temperature is less than 20 degrees
		{
			GPIO_writePin(PORTD_ID, PIN3_ID, LOGIC_LOW);						// Turning the buzzer OFF
			GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);						// Turning the Green LED ON
			GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_LOW);						// Turning the Yellow LED OFF
			GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_LOW);						// Turning the Red LED OFF
		}
		else if(temp <= 40)														// If the temperature is between 20 and 40 degrees
		{
			GPIO_writePin(PORTD_ID, PIN3_ID, LOGIC_LOW);						// Turning the buzzer OFF
			GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);						// Turning the Green LED OFF
			GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_HIGH);						// Turning the Yellow LED ON
			GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_LOW);						// Turning the Red LED OFF
		}
		else if(temp <= 50)														// If the temperature is between 40 and 50 degrees
		{
			GPIO_writePin(PORTD_ID, PIN3_ID, LOGIC_LOW);						// Turning the buzzer OFF
			GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);						// Turning the Green LED OFF
			GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_LOW);						// Turning the Yellow LED OFF
			GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_HIGH);						// Turning the Red LED ON
		}
		else																	// If the temperature is more than 50 degrees
		{
			// emergency state
			GPIO_writePin(PORTD_ID, PIN3_ID, LOGIC_HIGH);						// Turning the buzzer ON
			GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);						// Turning the Green LED OFF
			GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_LOW);						// Turning the Yellow LED OFF
			GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_HIGH);						// Turning the Red LED ON
		}
	}
}
