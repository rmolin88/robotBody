/**************************************************************************
 UART initializations
 ****************************************************************************/ 



#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>




//void Interrupt_Init(void);
//void Key_UART_Recog(void);
void UART_TRANS(char trans, FILE *stream);
void UART_INIT(void);




#endif


