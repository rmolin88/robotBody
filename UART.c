/***********************************************************************
*	v5.0
*		Changelog:
*			Implementing printf
*
**************************************************************************/




#include "UART.h"

extern char volatile Serial_Receive[11];
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
	
	
	USARTC0_CTRLA = USART_RXCINTLVL0_bm;	//RECEIVED INTERRUPT ENABLE
	USARTC0_CTRLB = USART_RXEN_bm + USART_TXEN_bm;// 0x18;	//receiver and transmitter enabled
	USARTC0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;		//ASYNCHRONOUS MODE
	
	//BSEL = -7 & BSCALE = 150
	USARTC0_BAUDCTRLA =	0x96;
	USARTC0_BAUDCTRLB = 0x90;
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


void serialDecodify(){
	
	//******************************
	//		MOTOR COMMAND
	//******************************
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

	//******************************
	//	STEERING SERVO COMMAND
	//******************************
	do{}while(!Serial_Finish);
	Serial_Receive[4] = receive;
	Serial_Finish = 0;
	
	do{}while(!Serial_Finish);
	Serial_Receive[5] = receive;
	Serial_Finish = 0;
	
	do{}while(!Serial_Finish);
	Serial_Receive[6] = receive;
	Serial_Finish = 0;

	do{}while(!Serial_Finish);
	Serial_Receive[7] = receive;
	Serial_Finish = 0;
	
	//******************************
	//	CAMERA SERVO COMMAND
	//******************************
	do{}while(!Serial_Finish);
	Serial_Receive[8] = receive;
	Serial_Finish = 0;
	
	do{}while(!Serial_Finish);
	Serial_Receive[9] = receive;
	Serial_Finish = 0;
	
	do{}while(!Serial_Finish);
	Serial_Receive[10] = receive;
	Serial_Finish = 0;

	do{}while(!Serial_Finish);
	Serial_Receive[11] = receive;
	Serial_Finish = 0;
	
}



