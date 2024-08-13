/*
 ============================================================================
 Name        : MCU1.c
 Author      : Abdelrahman Essa
 Date        : 11/8/2024
 Description : Formula Team Task, Micro-controller 1
 ============================================================================
 */

#include "std_types.h"
#include "gpio.h"
#include "adc.h"
#include "motor.h"
#include "uart.h"
#include "lm35_sensor.h"
#include "timer1.h"
#include "lcd.h"
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

// Creating a Temperature variable
uint8 temp = 0;
boolean danger = FALSE;

// creating counter to use in emergency state
uint8 counter = 0;

void t1call()
{
	TCNT1 = 65047;																// Reloading the timer
	if(counter < 14)															// Checking if the counter reaches 14 (7 seconds)
	{
		if(temp < 50)															// Checking if the temperature decreases than 50◦C
		{
			counter = 0;														// Reset the counter
			return;
		}
		++counter;																// Incrementing the counter
	}
	else																		// If the counter reaches 14 (7 seconds)
	{
		counter = 0;
		UART_sendByte(220);														// Sending Abnormal state to MCU2
		eeprom_update_byte((uint8*)0, 'A');										// Initializing the E2PROM to 'A' (Abnormal State)
		wdt_enable(WDTO_15MS);
	}
}

int main()
{
	/********** Initializations **********/
	// ADC Initialization
	ADC_ConfigType ADCinit = {INTERNAL_AVCC, PRESCALAR_128};					// Setting the ADC reference to Internal and the prescalar to 128
	ADC_init(&ADCinit);															// Initializing the Settings to the ADC

	// E2PROM Initialization
	eeprom_update_byte((uint8*)0, 'N');											// Initializing the E2PROM to 'N' (Normal State)

	// LCD initialization
	LCD_init();
	LCD_clearScreen();

	// DC Motor Initialization (Fan)
	DcMotor_Init();																// Initializing the motor pins
	DcMotor_Rotate(MOTOR_STOP, 0);												// Setting the motor initially to stop

	// Push Button Initialization
	GPIO_setupPinDirection(PORTB_ID, PIN0_ID, PIN_INPUT);						// Setting PB0 as i/p Pin
	GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_HIGH);								// Setting the i/p Pin to internal PULL-UP

	// Creating Motor Speed Variable
	uint8 MoSpeed = 0;

	// UART Initialization
	UART_ConfigType Uconfig = {9600, PARITY_DISABLED, STOP_BIT_1, BIT_MODE_8};
	UART_init(&Uconfig);

	// Timer 1 initialization
	sei();
	Timer1_ConfigType T1 = {65047, 0, F_CPU_1024, MODE_NORMAL};
	Timer1_init(&T1);
	Timer1_setCallBack(t1call);

	/************** Program **************/
	for(;;)
	{
		LCD_displayStringRowColumn(0,0,"Counter: ");
		LCD_intgerToString(counter);
		_delay_ms(10);
		LCD_displayStringRowColumn(0,9,"  ");
		temp = LM35_getTemperature();											// Storing the temperature continuously
		UART_sendByte(temp);													// Sending the temperature to MCU2
		if(temp < 20)															// If the temperature is less than 20 degrees
		{
			DcMotor_Rotate(MOTOR_STOP, 0);										// The fan stops.
		}
		else if(temp <= 40)														// If the temperature is between 20 and 40 degrees
		{
			MoSpeed = ((double)temp/20.0 - 1) * 100;
			DcMotor_Rotate(MOTOR_CW, MoSpeed);									// The fan is moved with speed increasing according to increasing temperature
		}
		else if(temp <= 50)														// If the temperature is between 40 and 50 degrees
		{
			danger = !(GPIO_readPin(PORTB_ID, PIN0_ID));
			DcMotor_Rotate(MOTOR_CW, 100);										// The fan is moved with Max speed
			if(danger)															// Checking if the button pressed
			{
				UART_sendByte(200);												// Sending the code which shutting down the Machine
				danger = FALSE;
			}
			else
			{
				// do nothing
			}
		}
		else																	// If the temperature is more than 50 degrees (emergency state)
		{
			DcMotor_Rotate(MOTOR_CW, 100);										// The fan is moved with Max speed
		}
	}
}
