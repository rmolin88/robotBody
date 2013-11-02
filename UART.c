/***********************************************************************
*	v5.0
*		Changelog:
*			Implementing printf
*
**************************************************************************/




#include "UART.h"


extern uint8_t volatile Serial_Finish;
extern uint8_t volatile receive ;

void UART_INIT()
{
	/*	Function UART_INIT_v1.0
	*
	*	Dependencies:
	*		enable interrupts
	*
	*	UART used:
	*		C0
	*
	*	Pins used: 
	*		PC4
	*
	*	Description:
	*		Initializes the UART Communication
	*		
	*
	*	Notes
	*		USARTC0 MUST BE USED GIVEN THAT IS THE PORT THAT IS CONNECTED TO USB
	*		create char volatile receive = 0;
	*		INITS USARTC0 WITH BSEL OF 144 AND BSCALE OF -6
	*		settings for bray are baud 38400, 8 data bits, none parity, and 1 stop bit
	*
	*
	*/
	
	char BSEL = 150;
	char BSCALE = -7;
	
	//PORTC_OUTSET = 0x04;		//SETTING THIS PIN AS HIGH AND THEN AS OUTPUT
	USARTC0_CTRLA |= USART_RXCINTLVL0_bm;	//RECEIVED INTERRUPT ENABLE
	USARTC0_CTRLB = USART_RXEN_bm + USART_TXEN_bm;// 0x18;	//receiver and transmitter enabled
	USARTC0_CTRLC = USART_CHSIZE_8BIT_gc;		//ASYNCHRONOUS MODE
	
	USARTC0_BAUDCTRLA =	(BSEL & 0xFF);
	USARTC0_BAUDCTRLB = ((BSCALE << 4 & 0xF0 ) | (BSEL >> 8 & 0x0F));
}				


ISR(USARTC0_RXC_vect)
{
	/*	Function UART_TRANS_v2.0
	*
	*	Variable Dependencies:
	*		char volatile receive (global)
	*
	*	UART used:
	*		C0
	*
	*	Description:
	*		Takes a char array from the UART receive
	*		and passes it on to the Key_UART_Recog fucntion
	*/
	receive = USARTC0_DATA;
	USARTC0_STATUS = 0x80;
	Serial_Finish = 1;
}


void UART_TRANS(char trans, FILE *stream)
{
	/*	Function UART_TRANS_v2.0
	*
	*	UART used:
	*		C0
	*
	*	Dependencies: 
	*		Rest of UART functions
	*		If you do not want to send a value make it -1
	*		Add spacing after your string to leave a space after the string and your value
	*
	*
	*	Description:
	*		Takes a char array no bigger than 48 characters
	*		and passes it on to the UART
	*
	*/
	if(trans == '\n') trans = '\r';
	while((USARTC0_STATUS & 0x20) == 0x0);
	USARTC0_DATA = trans;
	
}






