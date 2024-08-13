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
	//sei();

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
