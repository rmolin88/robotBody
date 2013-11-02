/******************************
 * LowerLevelFinal6W.c
 *
 * Created: 9/29/2013 1:20:03 AM
 *  Author: Ray
 **********************************/ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "PWM.h"
#include "sonar.h"
#include "feedback.h"


static FILE mystdout = FDEV_SETUP_STREAM(UART_TRANS, NULL, _FDEV_SETUP_WRITE);

volatile uint16_t sonarReading0 = 1000;
volatile uint16_t sonarReading1 = 2000;
volatile uint16_t sonarReading2 = 3000;


uint8_t volatile Serial_Finish = 0;
char actuatorReceived = 0;
char volatile Serial_Receive[5];

uint8_t volatile receive = 0;				//receives data from UART interrupt

uint8_t ledCounter = 5;

int main(void)
{
	stdout = &mystdout;
	UART_INIT();
	PWM_Init();
	feedback_init();
	ADC_Init();
	
	
	_delay_ms(1000);
	Interrupt_Init();
	
	beep();
	printf("Initialization Complete\n");
	
	for(;;)
    {
		ledCounter--;
		if(!ledCounter){
			toggle_leds();
			ledCounter = 5;	
		}
		
		if(Serial_Finish){
			Serial_Receive[0] = receive;
			Serial_Finish = 0;
			
			do{}while(!Serial_Finish);
			Serial_Receive[1] = receive;
			Serial_Finish = 0;
			
			do{}while(!Serial_Finish);
			Serial_Receive[2] = receive;
			Serial_Finish = 0;
			
			do{}while(!Serial_Finish);
			Serial_Receive[3] = receive;
			Serial_Finish = 0;
			
			do{}while(!Serial_Finish);
			Serial_Receive[4] = receive;
			Serial_Finish = 0;
			
			motorPulse();
			servoPulse();
			sonarRead();
			if( (sonarReading0 < 1000) | (sonarReading1 < 1000) | (sonarReading2 < 1000)) sonarReading0 = sonarReading1 = sonarReading2 = 1000;
			printf("%u%u%u", sonarReading0, sonarReading1, sonarReading2);
		}
	}
}