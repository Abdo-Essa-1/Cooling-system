#include <avr\io.h>
#include "timer1.h"
#include "std_types.h"
#include <avr\interrupt.h>

/*to store call back function pointer*/
static volatile void (*g_callBackFunction) (void) = NULL_PTR;




/*Interrupts service routines for both modes*/
ISR(TIMER1_OVF_vect){
	if(g_callBackFunction != NULL_PTR){
		(*g_callBackFunction)();
	}
}

ISR(TIMER1_COMPA_vect){
	if(g_callBackFunction != NULL_PTR){
		(*g_callBackFunction)();
	}
}


/*
 * Description
⮚ Function to initialize the Timer driver
● Inputs: pointer to the configuration structure with type
Timer1_ConfigType.
● Return: None */
void Timer1_init(const Timer1_ConfigType * Configurations){

	// Enable the global interrupts
	sei();

	/*initialize timer*/
	TCNT1 = Configurations -> initial_value;


	/*initialize if normal mode is chosen*/
	if((Configurations -> mode) == MODE_NORMAL){
		/*Overflow interrupt enable*/
		TIMSK |= (1 << TOIE1);

		/*This bits are set to 1 if non-PWM mode is used*/
		TCCR1A = (1 << FOC1A) | (1 << FOC1B);

		/*Set CS12, CS11, CS10 to the selected clock prescaler (3 least bits)*/
		TCCR1B = (Configurations -> prescaler) & 0x07;
	}
	else if((Configurations -> mode) == MODE_COMPARE){

		/*set the compare value to register*/
		ICR1 = Configurations -> compare_value;

		/*Compare mode interrupt*/
		TIMSK |= (1 << OCIE1A);

		/*Set mode to CTC that compare on ICR1*/
		TCCR1A = (1 << FOC1A) | (1 << FOC1B);
		TCCR1B = (1 << WGM12) | (1 << WGM13);

		/*Set Clock Prescaler*/
		TCCR1B = (TCCR1B & 0XF8) | ((Configurations -> prescaler) & 0x07);

	}
}


/*
 * Description
⮚ Function to disable the Timer1.
● Inputs: None
● Return: None */
void Timer1_deInit(void){
	/*Reset all regisers and turn off interrupts of any mode*/
	TCNT1 = 0;
	TCCR1B = 0;
	TCCR1A = 0;
	TIMSK &= ~(1 << TOIE1) & ~(1 << OCIE1A);
}


/*
 * Description
⮚ Function to set the Call Back function address.
● Inputs: pointer to Call Back function.
● Return: None */
void Timer1_setCallBack(void(* a_ptr)(void)){
	g_callBackFunction = a_ptr;
}


/*
 * Description
⮚ Function to initialize Timer 1 for Fast PWM
● Inputs: None.
● Return: None */
void Timer1_Servo_Init()
{
    // Set PD5/OC1A as output
    DDRD |= (1 << PD5);

    // Set Timer1 to Fast PWM mode with ICR1 as TOP
    TCCR1A |= (1 << WGM11);                 // Fast PWM mode, WGM11 = 1
    TCCR1B |= (1 << WGM12) | (1 << WGM13);  // WGM12 = 1, WGM13 = 1

    // Set non-inverting mode for OC1A (PB1)
    TCCR1A |= (1 << COM1A1);                // Clear OC1A on compare match, set at BOTTOM

    // Set prescaler to 8
    TCCR1B |= (1 << CS11);                  // Prescaler = 8

    // Set ICR1 for 20 ms period (50 Hz)
    ICR1 = 2499;							// ICR1 = (1MHz / (50Hz * 8)) - 1
}


/*
 * Description
⮚ Function to Set the angle of the Servo
● Inputs: The angle of the servo.
● Return: None */
void Set_Servo_Angle(uint8 angle)
{
    // Ensure angle is within 0-180 degrees
    if (angle > 180) angle = 180;

    // Calculate pulse width corresponding to the angle
    uint16 pulse_width = ((float)(angle * 125.0) / 180.0) + 125;

    // Set OCR1A for the calculated pulse width
    OCR1A = pulse_width;
}
