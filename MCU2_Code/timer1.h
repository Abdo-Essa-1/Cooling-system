#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/*---------------------Definitions--------------------------------*/

/*to choose the prescaler of the timer clock*/
typedef enum{
	TIMER_STOP, F_CPU_1, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}Timer1_Prescaler;


/*To choose the mode of the timer*/
typedef enum{
	MODE_NORMAL, MODE_COMPARE, FAST_PWM
}Timer1_Mode;


/*Configurations struct to be passed to init function*/
typedef struct{
	uint16 initial_value;
	uint16 compare_value; //used in compare mode only
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
}Timer1_ConfigType;



/*----------------------------------Functions Prototypes--------------------*/

/*
 * Description
⮚ Function to initialize the Timer driver
● Inputs: pointer to the configuration structure with type
Timer1_ConfigType.
● Return: None */
void Timer1_init(const Timer1_ConfigType * Configurations);


/*
 * Description
⮚ Function to disable the Timer1.
● Inputs: None
● Return: None */
void Timer1_deInit(void);


/*
 * Description
⮚ Function to set the Call Back function address.
● Inputs: pointer to Call Back function.
● Return: None */
void Timer1_setCallBack(void(* a_ptr)(void));


/*
 * Description
⮚ Function to initialize Timer 1 for Fast PWM
● Inputs: None.
● Return: None */
void Timer1_Servo_Init();


/*
 * Description
⮚ Function to Set the angle of the Servo
● Inputs: The angle of the servo.
● Return: None */
void Set_Servo_Angle(uint8 angle);

#endif /* TIMER1_H_ */
