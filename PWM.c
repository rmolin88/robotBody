/***********************************************************************
*	v2.0
*		Changelog:
*				I am going to change the Frequency of the PWM to 54 Hz
*			for both the servo and the motor with new include values
*				Also I am going to make them both in the same Timer
*			just different CC's
*				Programming for the new board also
*					ATxmega128A4U
*
**************************************************************************/


#include "PWM.h"
#include <util/delay.h>


extern char actuatorReceived;
extern char Serial_Receive[5];

volatile uint16_t motorTemp = 3200;
volatile uint16_t servoTemp = 3000;
uint16_t motor = 0;
uint16_t servo = 0;


#define PER_54HZ 0x90AD  
#define SINGLE_SLOPE_PWM 3
#define MOTORS_OFF 3200
#define wheelStraight 3000

void PWM_Init()
{
	/*	Function PWM_Pulse_v2.0
	*
	*	Dependencies: 
	*		PWM_Init()
	*		input of two variables the first one 
	*		actuator 0 = motor
	*		actuator 1 = servo
	*		Second one value of PWM pulse
	*
	*	Description:
	*		sends pulse to desired actuator
	*		protects servo from pulse higher than 10%
	*
	*	Equation to calculate PER register value 
	*		(Value_s)/(1/(fclk/clk_div)) = PER 
	*
	*/
				//setting PC1 & PC0 as output this means we are going to be working with TCC0
												//0C0B = PC1 --- SERVO & OC0A = PC0 --- MOTOR
	//SERVO/Motor SET UP
	TCC0_PER = PER_54HZ;						//fpwm = 54Hz = 18.5 ms | PER = (1/54)/(1/(16e6/64)) = 9259.3 = 0x242B
	TCC0_CTRLB |= TC0_CCAEN_bm + TC0_CCBEN_bm + SINGLE_SLOPE_PWM;							//bit[4] cca, ccb enable bit[2:0] single slop bottom to top
	TCC0_CCA = MOTORS_OFF;							//starting off Motors at 1.5
	TCC0_CCB = wheelStraight;							//starting off Servos at 1.5
	TCC0_CTRLA = TC_CLKSEL_DIV1_gc;				//clock at full speed = 1 | Div/1024 = 7 | Div/64 = 5
	//_delay_ms(1000);
	
	
	
// 	for(int i = 0x960; i<0x0BB8; i++)
// 	{
// 		TCC0_CCB = i;
// 		_delay_ms(10);
// 	}
// 	for(int i = 0x0BB8; i>0x960; i--)
// 	{
// 		TCC0_CCB = i;
// 		_delay_ms(10);
// 	}
	
}



void motorPulse()
{
	/*	Function PWM_Pulse_v2.0
	*
	*	Dependencies: 
	*		PWM_Init()
	*		
	*	Description:
	*		sends pulse to desired actuator
	*		protects servo from pulse higher than 10%
	* 
	*	Duty Cycle Calcs:
	*		Use the PWM Calculator Doc
	*
	*	About servos
	*		never give it a bigger than duty cycle than 10%
	*		include for this
	*		1ms pulse to turn left
	*		2ms pulse to turn right
	*		1.5ms pulse to center
	*
	*	About motors
	*		1.2ms Full Reverse
	*		1.35 Revers
	*		1.5ms Neutral
	*		1.65 Forward
	*		1.8ms Full Forward
	*
	*	//SERVO DEFINES
	*	#define Hard_Left 0x1f
	*	#define Left 0x27
	*	#define Straight 0x2e
	*	#define Right 0x36
	*	#define Hard_Right 0x3e
	*
	*	//MOTOR DEFINES
	*	#define Full_Reverse 0x25
	*	#define Reverse 0x2a
	*	#define Neutral 0x2e
	*	#define Forward 0x33
	*	#define Full_Forward 0x38
	*
	*	#define Motor 0
	*	#define Servo 1
	*	
	*/
	actuatorReceived = Serial_Receive[0];
	//printf("%c", actuatorReceived);
	switch(actuatorReceived)
	{
		case 's':
			motor = 3200;
			break;
		case 'f':
			if(motorTemp == 3000)
			{
				motor = 3200;
				break;
			}
			motor = 3400;
			break;
		case 'r':
			if(motorTemp == 3400)
			{
				motor = 3200;
				break;
			}
			motor = 3000;
			break;
	}
	//printf("motor value %u\r", motor);
	if (motorTemp == motor)return;
	if(motor > motorTemp)
	{
		while(!(motorTemp == motor))										//slowly getting the motor pulse high
		{
			motorTemp += 1;
			if( (motorTemp > 3400) | (motorTemp < 2700) ) return;				//preventing the motor values to ramp up or down
			TCC0_CCA = motorTemp;
			_delay_us(10);
		}
	}
	else
	{
		while(!(motorTemp == motor))										//slowly getting the motor pulse low
		{
			motorTemp -= 1;
			if( (motorTemp > 3400) | (motorTemp < 2700) ) return;
			TCC0_CCA = motorTemp;
			_delay_us(10);
		}
	}
	if (motorTemp == 3200) _delay_ms(1);								//if order is to stop motors give it a bit more rest so they stop
	//printf("motorTemp value %u\r", motorTemp);
}

void servoPulse()
{
	
	for(int i = 0; i<4; i++)
	{
		switch(i)
		{
			case 0:
			servo += (Serial_Receive[1] - '0')*1000;
			break;
			case 1:
			servo += (Serial_Receive[2] - '0')*100;
			break;
			case 2:
			servo += (Serial_Receive[3] - '0')*10;
			break;
			case 3:
			servo += (Serial_Receive[4] - '0');
			break;
		}
	}
	//printf("%d", servo);
	
	
	if (servoTemp == servo)return;
	if(servo > servoTemp)
	{
		while(!(servoTemp == servo))										//slowly getting the motor pulse high
		{
			servoTemp += 1;
			if( (servoTemp > 3600) | (servoTemp < 2400) ) return;				//preventing the motor values to ramp up or down
			TCC0_CCB = servoTemp;
			_delay_us(10);
		}
	}
	else
	{	
		while(!(servoTemp == servo))										//slowly getting the motor pulse low
		{
			servoTemp -= 1;
			if( (servoTemp > 3400) | (servoTemp < 2700) ) return;
			TCC0_CCB = servoTemp;
			_delay_us(10);
		}
	}
	//printf("servoTemp value %u\r", servoTemp);
}