#include "motor.h"
#include "gpio.h"
#include "pwm0.h"
#include "common_macros.h"
#include <avr\io.h>


/*• Description
➢ The Function responsible for setup the direction for the two
motor pins through the GPIO driver.
➢ Stop at the DC-Motor at the beginning through the GPIO driver.
• Inputs: None
• Return: None
 * */
void DcMotor_Init(void){
	/*Set Motor direction control pins as output*/
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);


	/*Motor off initially*/
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);


}


/*
 * • Description:
➢ The function responsible for rotate the DC Motor CW/ or A-CW or
stop the motor based on the state input state value.
➢ Send the required duty cycle to the PWM driver based on the
required speed value.
• Inputs:
➢ state: The required DC Motor state, it should be CW or A-CW or stop.
DcMotor_State data type should be declared as enum or uint8.
➢ speed: decimal value for the required motor speed, it should be from
0 → 100. For example, if the input is 50, The motor should rotate with
50% of its maximum speed.
• Return: None
 * */
void DcMotor_Rotate(DcMotor_State state, uint8 speed){

	/*Update state of the motor*/
	if(state == MOTOR_STOP){
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
	}
	else if(state == MOTOR_CW){
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_HIGH);
	}
	else if(state == MOTOR_ACW){
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
	}
	else{
		/*Do Nothing*/
	}

	/*Update Speed*/
	PWM_Timer0_Init(speed);

}
