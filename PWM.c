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


#define forward 3400		//get this values as close as possible to 1600
#define reverse 3000

#define left 2400
#define right 3600
#define straight 3000

#define PER_54HZ 0x90AD
#define PER_50HZ 40000
#define SINGLE_SLOPE_PWM 3
#define MOTORS_OFF 3200
#define wheelStraight 3000


extern char Serial_Receive[11];

uint16_t servoTemp = 3000;
uint16_t motor;
uint16_t steeringServo;
uint16_t cameraServo;



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
	TCC0_CCA = MOTORS_OFF;							//starting off Motor at 1.5
	TCC0_CCB = wheelStraight;							//starting off Servo at 1.5
	TCC0_CTRLA = TC_CLKSEL_DIV1_gc;				//clock at full speed = 1 | Div/1024 = 7 | Div/64 = 5
	
	TCC1_PER = PER_50HZ;						//fpwm = 54Hz = 18.5 ms | PER = (1/54)/(1/(16e6/64)) = 9259.3 = 0x242B
	TCC1_CTRLB |= TC0_CCBEN_bm + SINGLE_SLOPE_PWM;							//bit[4] cca, ccb enable bit[2:0] single slop bottom to top
	//TCC0_CCA = MOTORS_OFF;							//starting off Motors at 1.5
	TCC1_CCB = 3000;							//starting off Servos at 1.5
	TCC1_CTRLA = TC_CLKSEL_DIV1_gc;				//clock at full speed = 1 | Div/1024 = 7 | Div/64 = 5
	//_delay_ms(1000);
	
}



void actuateMotors(){
		motor = 0;
		steeringServo = 0;
		cameraServo = 0;
		
		for(int i = 0; i<4; i++)
		{
			switch(i)
			{
				case 0:
					motor			+= (Serial_Receive[0] - '0')*1000;
					steeringServo	+= (Serial_Receive[4] - '0')*1000;
					cameraServo		+= (Serial_Receive[8] - '0')*1000;
					break;
				case 1:
					motor			+= (Serial_Receive[1] - '0')*100;
					steeringServo	+= (Serial_Receive[5] - '0')*100;
					cameraServo		+= (Serial_Receive[9] - '0')*100;
					break;
				case 2:
					motor			+= (Serial_Receive[2] - '0')*10;
					steeringServo	+= (Serial_Receive[6] - '0')*10;
					cameraServo		+= (Serial_Receive[10] - '0')*10;
					break;
				case 3:
					motor			+= (Serial_Receive[3] - '0');
					steeringServo	+= (Serial_Receive[7] - '0');
					cameraServo		+= (Serial_Receive[11] - '0');
					break;
			}
		}
		
		if ( (motor > 4000) | (motor < 1800) ) printf("BAD PWM");
		else TCC0_CCA = motor;
		printf("%d motor value\r", motor);
		
		if ( (steeringServo > 3600) | (steeringServo < 2400) ) printf("BAD PWM");
		else TCC0_CCB = steeringServo ;
		printf("%d steeringServo value\r", steeringServo);
		
		//if ( (motor > 3400) | (motor < 2700) ) printf("BAD PWM"); return;
		//TCC1_CCB = cameraServo;
		if(cameraServo > servoTemp){
			for(servoTemp; !(cameraServo == servoTemp); servoTemp++ ){
				TCC1_CCB = servoTemp;
				_delay_us(100);
			}
		}
		else{
			for(servoTemp; !(cameraServo == servoTemp); servoTemp-- ){
				TCC1_CCB = servoTemp;
				_delay_us(100);
			}		
		}
		printf("%d cameraServo value\r", cameraServo);
		
	}

/*//actuatorReady = 0;					
		switch(Serial_Receive[0])
		{
			case 's':
				//motor = MOTORS_OFF;
				TCC0_CCA = MOTORS_OFF;
				break;
			
			case 'f':
				//motor = forward;
				TCC0_CCA = forward;
				break;
			
			case 'r':
				//motor = reverse;
				TCC0_CCA = reverse;
				break;
		}
		if(motor > motorTemp){
			for(motorTemp; !(motor == motorTemp); motorTemp++ ){
				TCC0_CCA = motorTemp;
				_delay_ms(5);
			}
		}
		else{
			for(motorTemp; !(motor == motorTemp); motorTemp-- ){
				TCC0_CCA = motorTemp;
				_delay_ms(5);
			}
		}
		
		switch(Serial_Receive[1]){
			case 'l':
				//servo = left;
				TCC0_CCB = left;
				break;
			
			case 'r':
				//servo = right;
				TCC0_CCB = right;
				break;
			
			case 's':
				//servo = straight;
				TCC0_CCB = straight;
				break;
		}
		if(servo > servoTemp){
			for(servoTemp; !(servo == servoTemp); servoTemp++ ){
				TCC0_CCA = servoTemp;
				_delay_us(200);
			}
		}
		else{
			for(servoTemp; !(servo == servoTemp); servoTemp-- ){
				TCC0_CCA = servoTemp;
				_delay_us(200);
			}		
		}
		*/
		//actuatorReady = 1;
		
		