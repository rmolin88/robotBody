/******************************************************************
 * sonar.c
 *
 * Created: 10/16/2013 8:31:11 PM
 *  Author: Ray
 *  Description: This code is going to enable and operate the
 *				sonars
 *****************************************************************/ 


#include "sonar.h"

#define ADC_CH_MUXNEG_INTGND_gc 7

extern volatile uint16_t sonarReading0;												//right sonar	PA.0 blue cable ADCA_CH0
extern volatile uint16_t sonarReading1;												//center sonar	PA.2 yellow cable
extern volatile uint16_t sonarReading2;												//left sonar	PA.1 purple cable ADCA_CH1

void ADC_Init(){
	
	ADCA_CTRLB			|= ADC_CONMODE_bm;										//plus 12-bit resolution right adjusted
	ADCA_REFCTRL		= ADC_REFSEL_VCC_gc;									//Reference Vcc/1.6V
	ADCA_PRESCALER		= ADC_PRESCALER_DIV32_gc;
	
	ADCA_CH0_CTRL		= ADC_CH_INPUTMODE_DIFF_gc;
	ADCA_CH0_MUXCTRL	= ADC_CH_MUXPOS_PIN0_gc + ADC_CH_MUXNEG_INTGND_gc;				//selecting pin adc0 right
	
	ADCA_CH1_CTRL		= ADC_CH_INPUTMODE_DIFF_gc;
	ADCA_CH1_MUXCTRL	= ADC_CH_MUXPOS_PIN1_gc + ADC_CH_MUXNEG_INTGND_gc;				//selecting pin adc1 left
	
	ADCA_CH2_CTRL		= ADC_CH_INPUTMODE_DIFF_gc;
	ADCA_CH2_MUXCTRL	= ADC_CH_MUXPOS_PIN2_gc + ADC_CH_MUXNEG_INTGND_gc;				//selecting pin adc2 center
	
	ADCA_CTRLA			|= ADC_ENABLE_bm;
	
	/*	PC5 for speaker
		PC4 TX daisy chain signal
		PD[0:2] LEDs
		PD[3:5]	Sonars 1,2,3
		
	*/
	
}


void Interrupt_Init(){
	PMIC_CTRL = 1;
	sei();
}

void sonarRead()
{
	sonarReading0 = sonarReading1 = sonarReading2 = 1000;
	
	//READING RIGHT SONAR
	ADCA_CTRLA |= ADC_CH0START_bm + ADC_CH1START_bm + ADC_CH2START_bm;							//signaling a start a conversion
	while( !(ADCA_CH0_INTFLAGS & 1) );						//waiting for conversion to finish
	_delay_us(50);											//waiting a bit more
	sonarReading0 += ADCA_CH0RES;							//recording conversion value
	ADCA_CH0_INTFLAGS = 1;									//clearing the interrupt flag
	
	//READING LEFT SONAR
	while( !(ADCA_CH1_INTFLAGS & 1) );
	_delay_us(50);
	sonarReading2 += ADCA_CH1RES;
	ADCA_CH1_INTFLAGS = 1;
	
	//READING CENTER SONAR
	while( !(ADCA_CH2_INTFLAGS & 1) );						//waiting for conversion to finish
	_delay_us(50);											//waiting a bit more
	sonarReading1 += ADCA_CH2RES;							//recording conversion value
	ADCA_CH2_INTFLAGS = 1;									//clearing the interrupt flag
	
	
	
}
