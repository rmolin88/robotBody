/************************************************************
 * sonar.h
 *
 * Created: 10/16/2013 8:30:02 PM
 *  Author: Ray
 *	Description: This code is going to enable and operate the
 *				sonars
 *************************************************************/ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef SONAR_H_
#define SONAR_H_


void sonarRead();
void ADC_Init();
void Interrupt_Init();


#endif /* SONAR_H_ */