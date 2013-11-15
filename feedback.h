/*
 * feedback.h
 *
 * Created: 10/27/2013 6:40:25 PM
 *  Author: Ray
 */ 
#include <avr/io.h>
#include <util/delay.h>

#ifndef FEEDBACK_H_
#define FEEDBACK_H_


void general_init();
void beep();
void toggle_leds();

#endif /* FEEDBACK_H_ */