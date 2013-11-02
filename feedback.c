/*
 * feedback.c
 *
 * Created: 10/27/2013 6:40:37 PM
 *  Author: Ray
 */ 

#include "feedback.h"

void feedback_init(){
	PORTE_DIRSET |= PIN1_bm;					//On board LED on
	PORTE_OUTSET |= PIN1_bm;
	
	PORTA_DIRCLR |= PIN0_bm + PIN1_bm + PIN2_bm;											//pin 0 is the blue cable right sonar
	//pin 1 is the purple cable is the left sonar
	//pin 2 is the yellow cable and is the center sonar which does NOT use adc
	
	
	TCC1_CTRLB = 0x11;							//addition of the beeps
	TCC1_CCB = 0x01dc;		
	
	PORTD_DIRSET |= PIN0_bm + PIN1_bm + PIN2_bm +PIN3_bm + PIN4_bm + PIN5_bm;										//all portd pins as output
	PORTD_OUTSET |= PIN0_bm + PIN2_bm + PIN3_bm  + PIN4_bm + PIN5_bm;			//BW high for all sonars					
	
	PORTC_DIRSET |= PIN0_bm + PIN1_bm + PIN3_bm + PIN4_bm + PIN5_bm;
	
	PORTC_OUT ^= PIN4_bm;					//signaling 30us TX high to start daisy chain
	_delay_us(30);
	PORTC_OUT ^= PIN4_bm;
	PORTC_DIR ^= PIN4_bm;
	
	
	
}

void beep(){
	TCC1_CTRLA = 1;								//beeping to signal everything okay
	_delay_ms(250);
	TCC1_CTRLA = 0;
}

void toggle_leds(){
	PORTD_OUT ^= 7;
}

