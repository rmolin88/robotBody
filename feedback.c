/*
 * feedback.c
 *
 * Created: 10/27/2013 6:40:37 PM
 *  Author: Ray
 */ 

#include "feedback.h"

void general_init(){
	
	PORTE_DIRSET |= PIN1_bm;					//On board LED on
	PORTE_OUTSET |= PIN1_bm;
	
	
	//***************************************************
	//	PA[0]	-> Right Sonar				(blue)
	//	PA[1]	-> Left Sonar				(yellow)
	//	PA[2]	-> Center Sonar				(purple)
	//**************************************************
	PORTA_DIRCLR |= PIN0_bm + PIN1_bm + PIN2_bm;		
	
	//TCC1_CTRLB = 0x11;							//addition of the beeps
	//TCC1_CCB = 0x01dc;		
	
	
	//******************************************************
	//	PC[0:1] -> Motor & steeringServo	(white)
	//	PC[2:3] -> UART						(white & green)
	//	PC[4]	-> Sonars kick start		(teal)
	//	PC[5]	-> cameraServo				(blue)
	//******************************************************
	PORTC_DIRSET |= PIN0_bm + PIN1_bm + PIN3_bm + PIN4_bm + PIN5_bm;
	PORTC_OUT = PIN3_bm;					//following UART manual instructions
	
	_delay_ms(300);							//giving sonar time to boot up
	
	PORTC_OUT ^= PIN4_bm;					//signaling 30us TX high to start daisy chain
	_delay_us(30);
	PORTC_OUT ^= PIN4_bm;
	PORTC_DIR ^= PIN4_bm;
	
	//*******************************************
	//	PD[0:2] -> Feedback LEDs
	//	PC[3:5] -> Sonars high signal
	//*****************************************
	PORTD_DIRSET |= PIN0_bm + PIN1_bm + PIN2_bm +PIN3_bm + PIN4_bm + PIN5_bm;										//all portd pins as output
	PORTD_OUTSET |= PIN0_bm + PIN2_bm + PIN3_bm  + PIN4_bm + PIN5_bm;												//BW high for all sonars and two leds
}

void beep(){
	TCC1_CTRLA = 1;								//beeping to signal everything okay
	_delay_ms(250);
	TCC1_CTRLA = 0;
}

void toggle_leds(){
	PORTD_OUT ^= 7;
}

