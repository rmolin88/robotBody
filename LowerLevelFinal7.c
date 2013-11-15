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

volatile uint16_t sonarReading0;
volatile uint16_t sonarReading1;
volatile uint16_t sonarReading2;

//uint8_t actuatorReady = 1;
uint8_t volatile Serial_Finish = 0;
char volatile Serial_Receive[11];

uint8_t volatile receive = 0;				//receives data from UART interrupt

uint8_t ledCounter = 5;

int main(void)
{
	stdout = &mystdout;
	general_init();
	UART_INIT();
	PWM_Init();
	ADC_Init();
	
	_delay_ms(250);
	Interrupt_Init();
	
	//beep();
	printf("Initialization Complete\n");
	
	for(;;){
		if(Serial_Finish){
			serialDecodify();
			
			actuateMotors();
			//sonarRead();
			//if( (sonarReading0 < 1000) | (sonarReading1 < 1000) | (sonarReading2 < 1000)) sonarReading0 = sonarReading1 = sonarReading2 = 1000;
			//printf("%u%u%u", sonarReading0, sonarReading1, sonarReading2);		//might get an error here because needs interrupts to send data through UART
			ledCounter--;
			if(!ledCounter){
				toggle_leds();
				ledCounter = 5;
			}
		}
	}
}